/*
 * File operations for Coda.
 * Original version: (C) 1996 Peter Braam 
 * Rewritten for Linux 2.1: (C) 1997 Carnegie Mellon University
 *
 * Carnegie Mellon encourages users of this code to contribute improvements
 * to the Coda project. Contact Peter Braam <coda@cs.cmu.edu>.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/smp_lock.h>
#include <linux/string.h>
#include <asm/uaccess.h>

#include <linux/coda.h>
#include <linux/coda_linux.h>
#include <linux/coda_fs_i.h>
#include <linux/coda_psdev.h>
#include <linux/coda_proc.h>

#include "compat.h"

/* if CODA_STORE fails with EOPNOTSUPP, venus clearly doesn't support
 * CODA_STORE/CODA_RELEASE and we fall back on using the CODA_CLOSE upcall */
static int use_coda_close;

static ssize_t
coda_file_read(struct file *coda_file, char __user *buf, size_t count, loff_t *ppos)
{
	struct coda_file_info *cfi;
	struct file *host_file;

	cfi = CODA_FTOC(coda_file);
	BUG_ON(!cfi || cfi->cfi_magic != CODA_MAGIC);
	host_file = cfi->cfi_container;

	if (!host_file->f_op || !host_file->f_op->read)
		return -EINVAL;

	return host_file->f_op->read(host_file, buf, count, ppos);
}

static ssize_t
coda_file_sendfile(struct file *coda_file, loff_t *ppos, size_t count,
		   read_actor_t actor, void *target)
{
	struct coda_file_info *cfi;
	struct file *host_file;

	cfi = CODA_FTOC(coda_file);
	BUG_ON(!cfi || cfi->cfi_magic != CODA_MAGIC);
	host_file = cfi->cfi_container;

	if (!host_file->f_op || !host_file->f_op->sendfile)
		return -EINVAL;

	return host_file->f_op->sendfile(host_file, ppos, count, actor, target);
}

static ssize_t
coda_file_write(struct file *coda_file, const char __user *buf, size_t count, loff_t *ppos)
{
	struct inode *host_inode, *coda_inode = coda_file->f_dentry->d_inode;
	struct coda_file_info *cfi;
	struct file *host_file;
	ssize_t ret;

	cfi = CODA_FTOC(coda_file);
	BUG_ON(!cfi || cfi->cfi_magic != CODA_MAGIC);
	host_file = cfi->cfi_container;

	if (!host_file->f_op || !host_file->f_op->write)
		return -EINVAL;

	host_inode = host_file->f_dentry->d_inode;
	down(&coda_inode->i_sem);

	ret = host_file->f_op->write(host_file, buf, count, ppos);

	coda_inode->i_size = host_inode->i_size;
	coda_inode->i_blocks = (coda_inode->i_size + 511) >> 9;
	coda_inode->i_mtime = coda_inode->i_ctime = CURRENT_TIME_SEC;
	up(&coda_inode->i_sem);

	return ret;
}

static int
coda_file_mmap(struct file *coda_file, struct vm_area_struct *vma)
{
	struct coda_file_info *cfi;
	struct file *host_file;
	struct inode *coda_inode;
	int err;

	cfi = CODA_FTOC(coda_file);
	BUG_ON(!cfi || cfi->cfi_magic != CODA_MAGIC);
	host_file = cfi->cfi_container;

	if (!host_file->f_op || !host_file->f_op->mmap)
		return -ENODEV;

	coda_inode = coda_file->f_dentry->d_inode;

	/* we can't use mmap if we were not able to setup the redirection
	 * during open */
	if (coda_file->f_mapping == &coda_inode->i_data)
		return -EBUSY;

	err = host_file->f_op->mmap(host_file, vma);
	if (err)
		return err;

	/* the mmap operation on the host_file might have changed the mappings.
	 * (especially filesystems that looked at how Coda used to set up the
	 * redirections) We could try to fix it up and hope we were the only
	 * opener or use BUG_ON() to track down any offenders. */
	BUG_ON(coda_file->f_mapping != host_file->f_mapping);

	return 0;
}

int coda_open(struct inode *coda_inode, struct file *coda_file)
{
	struct file *host_file = NULL;
	int error;
	unsigned short flags = coda_file->f_flags & (~O_EXCL);
	unsigned short coda_flags = coda_flags_to_cflags(flags);
	struct coda_file_info *cfi;
	struct coda_inode_info *cii;

	lock_kernel();
	coda_vfs_stat.open++;

	cfi = kmalloc(sizeof(struct coda_file_info), GFP_KERNEL);
	if (!cfi) {
		unlock_kernel();
		return -ENOMEM;
	}

	error = venus_open(coda_inode->i_sb, coda_i2f(coda_inode), coda_flags,
			   &host_file); 
	if (!host_file)
		error = -EIO;

	if (error) {
		kfree(cfi);
		if (host_file)
			fput(host_file);
		unlock_kernel();
		return error;
	}

	host_file->f_flags |= coda_file->f_flags & (O_APPEND | O_SYNC);

	cfi->cfi_magic = CODA_MAGIC;
	cfi->cfi_container = host_file;

	BUG_ON(coda_file->private_data != NULL);
	coda_file->private_data = cfi;

	/* if we haven't redirected the mmap redirection yet, do it here. */
	if (coda_inode->i_mapping == &coda_inode->i_data) {
		coda_inode->i_mapping = host_file->f_mapping;
		coda_file->f_mapping = host_file->f_mapping;
	}

	/* keep track of the number of file handles that could map the
	 * container file */
	if (coda_file->f_mapping == host_file->f_mapping) {
		cii = ITOC(coda_inode);
		cii->c_mapcount++;
	} else {
		/* container file changed on a previously opened inode. Reset
		 * the mapping as we can not allow mmap to succeed as it would
		 * see the old file through mmap, but the new one through
		 * read/write. */
		coda_file->f_mapping = &coda_inode->i_data;
	}

	unlock_kernel();
	return 0;
}

int coda_flush(struct file *coda_file)
{
	unsigned short flags = coda_file->f_flags & ~O_EXCL;
	unsigned short coda_flags = coda_flags_to_cflags(flags);
	struct coda_file_info *cfi;
	struct inode *coda_inode;
	int err = 0, fcnt;

	lock_kernel();

	coda_vfs_stat.flush++;

	/* last close semantics */
	fcnt = file_count(coda_file);
	if (fcnt > 1)
		goto out;

	/* No need to make an upcall when we have not made any modifications
	 * to the file */
	if ((coda_file->f_flags & O_ACCMODE) == O_RDONLY)
		goto out;

	if (use_coda_close)
		goto out;

	cfi = CODA_FTOC(coda_file);
	BUG_ON(!cfi || cfi->cfi_magic != CODA_MAGIC);

	coda_inode = coda_file->f_dentry->d_inode;

	err = venus_store(coda_inode->i_sb, coda_i2f(coda_inode), coda_flags,
			  coda_file->f_uid);

	if (err == -EOPNOTSUPP) {
		use_coda_close = 1;
		err = 0;
	}

out:
	unlock_kernel();
	return err;
}

int coda_release(struct inode *coda_inode, struct file *coda_file)
{
	unsigned short flags = (coda_file->f_flags) & (~O_EXCL);
	unsigned short coda_flags = coda_flags_to_cflags(flags);
	struct coda_file_info *cfi;
	struct coda_inode_info *cii;
	int err = 0;

	lock_kernel();
	coda_vfs_stat.release++;
 
	if (!use_coda_close) {
		err = venus_release(coda_inode->i_sb, coda_i2f(coda_inode),
				    coda_flags);
		if (err == -EOPNOTSUPP) {
			use_coda_close = 1;
			err = 0;
		}
	}

	cfi = CODA_FTOC(coda_file);
	BUG_ON(!cfi || cfi->cfi_magic != CODA_MAGIC);

	if (use_coda_close)
		err = venus_close(coda_inode->i_sb, coda_i2f(coda_inode),
				  coda_flags, coda_file->f_uid);

	/* did we redirect the mapping for this file? */
	if (coda_file->f_mapping != &coda_inode->i_data) {
		cii = ITOC(coda_inode);
		if (--cii->c_mapcount == 0)
			coda_inode->i_mapping = &coda_inode->i_data;
	}

	fput(cfi->cfi_container);
	kfree(coda_file->private_data);
	coda_file->private_data = NULL;

	unlock_kernel();
	return err;
}

int coda_fsync(struct file *coda_file, struct dentry *coda_dentry, int datasync)
{
	struct file *host_file;
	struct dentry *host_dentry;
	struct inode *host_inode, *coda_inode = coda_dentry->d_inode;
	struct coda_file_info *cfi;
	int err = 0;

	if (!(S_ISREG(coda_inode->i_mode) || S_ISDIR(coda_inode->i_mode) ||
	      S_ISLNK(coda_inode->i_mode)))
		return -EINVAL;

	cfi = CODA_FTOC(coda_file);
	BUG_ON(!cfi || cfi->cfi_magic != CODA_MAGIC);
	host_file = cfi->cfi_container;

	coda_vfs_stat.fsync++;

	if (host_file->f_op && host_file->f_op->fsync) {
		host_dentry = host_file->f_dentry;
		host_inode = host_dentry->d_inode;
		/* we don't need to call down(&host_inode->i_sem) as sys_fsync
		 * already locked coda_file->f_mapping->host->i_sem. */
		err = host_file->f_op->fsync(host_file, host_dentry, datasync);
	}

	if ( !err && !datasync ) {
		lock_kernel();
		err = venus_fsync(coda_inode->i_sb, coda_i2f(coda_inode));
		unlock_kernel();
	}

	return err;
}

struct file_operations coda_file_operations = {
	.llseek		= generic_file_llseek,
	.read		= coda_file_read,
	.write		= coda_file_write,
	.mmap		= coda_file_mmap,
	.open		= coda_open,
	.flush		= coda_flush,
	.release	= coda_release,
	.fsync		= coda_fsync,
	.sendfile	= coda_file_sendfile,
};

