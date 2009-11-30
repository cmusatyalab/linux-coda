/*
 * Super block/filesystem wide operations
 *
 * Copyright (C) 1996 Peter J. Braam <braam@maths.ox.ac.uk> and 
 * Michael Callahan <callahan@maths.ox.ac.uk> 
 * 
 * Rewritten for Linux 2.1.  Peter Braam <braam@cs.cmu.edu>
 * Copyright (C) Carnegie Mellon University
 */

#include <linux/module.h>
#include <linux/smp_lock.h>
#include <linux/file.h>
#include <linux/vfs.h>
#include <linux/fs.h>
#include <linux/namei.h>

#include <linux/coda.h>
#include <linux/coda_linux.h>
#include <linux/coda_psdev.h>
#include <linux/coda_cache.h>

#include "compat.h"

/* VFS super_block ops */
static void coda_clear_inode(struct inode *);
static void coda_put_super(struct super_block *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18) /* 2.6.18-rc1 */
static int coda_statfs(struct super_block *sb, struct kstatfs *buf);
#else
/* git commit 726c334223180e3c0197cc980a432681370d4baf */
static int coda_statfs(struct dentry *dentry, struct kstatfs *buf);
#endif

static struct kmem_cache * coda_inode_cachep;

static struct inode *coda_alloc_inode(struct super_block *sb)
{
	struct coda_inode_info *ei;
	ei = (struct coda_inode_info *)kmem_cache_alloc(coda_inode_cachep, SLAB_KERNEL);
	if (!ei)
		return NULL;
	memset(&ei->c_fid, 0, sizeof(struct CodaFid));
	ei->c_flags = 0;
	ei->c_uid = 0;
	ei->c_cached_perm = 0;
	return &ei->vfs_inode;
}

static void coda_destroy_inode(struct inode *inode)
{
	kmem_cache_free(coda_inode_cachep, ITOC(inode));
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 24) /* 2.6.24-rc1 */
static void init_once(void * foo, struct kmem_cache * cachep, unsigned long flags)
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 27) /* 2.6.27-rc1 */
/* git commit 4ba9b9d0ba0a49d91fa6417c7510ee36f48cf957 remove useless ctor */
static void init_once(struct kmem_cache * cachep, void * foo)
#else
/* git commit 51cc50685a4275c6a02653670af9f108a64e01cf drop kmem cache argument from constructor */
static void init_once(void * foo)
#endif
{
	struct coda_inode_info *ei = (struct coda_inode_info *) foo;

#ifdef SLAB_CTOR_VERIFY
/* 50953fe9e00ebbeffa032a565ab2f08312d51a87  slab allocators: Remove SLAB_DEBUG_INITIAL flag */
/* a35afb830f8d71ec211531aeb9a621b09a2efb39  Remove SLAB_CTOR_CONSTRUCTOR */
	if ((flags & (SLAB_CTOR_VERIFY|SLAB_CTOR_CONSTRUCTOR)) ==
	    SLAB_CTOR_CONSTRUCTOR)
#endif
		inode_init_once(&ei->vfs_inode);
}

int coda_init_inodecache(void)
{
	coda_inode_cachep = kmem_cache_create("coda_inode_cache",
				sizeof(struct coda_inode_info),
				0, SLAB_RECLAIM_ACCOUNT|SLAB_MEM_SPREAD,
				init_once);
	if (coda_inode_cachep == NULL)
		return -ENOMEM;
	return 0;
}

void coda_destroy_inodecache(void)
{
	kmem_cache_destroy(coda_inode_cachep);
}

static int coda_remount(struct super_block *sb, int *flags, char *data)
{
	*flags |= MS_NOATIME;
	return 0;
}

/* exported operations */
static struct super_operations coda_super_operations =
{
	.alloc_inode	= coda_alloc_inode,
	.destroy_inode	= coda_destroy_inode,
	.clear_inode	= coda_clear_inode,
	.put_super	= coda_put_super,
	.statfs		= coda_statfs,
	.remount_fs	= coda_remount,
};

static int coda_fill_super(struct super_block *sb, void *data, int silent)
{
        struct inode *root = NULL; 
	struct venus_comm *vc = (struct venus_comm *)data;
	struct CodaFid fid;
        int error;

	if (!vc->vc_inuse) {
		printk("coda: CFS device not open, venus dead?\n");
		return -EINVAL;
	}

	if ( vc->vc_sb ) {
		printk("coda: device already in use\n");
		return -EBUSY;
	}

	vc->vc_sb = sb;

	sb->s_fs_info = vc;
	sb->s_flags |= MS_NOATIME;
	sb->s_blocksize = 1024;	/* XXXXX  what do we put here?? */
	sb->s_blocksize_bits = 10;
	sb->s_magic = CODA_SUPER_MAGIC;
	sb->s_op = &coda_super_operations;

	/* get root fid from Venus: this needs the root inode */
	error = venus_rootfid(sb, &fid);
	if ( error ) {
	        printk("coda_read_super: coda_get_rootfid failed with %d\n",
		       error);
		goto error;
	}
	printk("coda_read_super: rootfid is %s\n", coda_f2s(&fid));
	
	/* make root inode */
        error = coda_cnode_make(&root, &fid, sb);
        if ( error || !root ) {
	    printk("Failure of coda_cnode_make for root: error %d\n", error);
	    goto error;
	} 

	printk("coda_read_super: rootinode is %ld dev %s\n", 
	       root->i_ino, root->i_sb->s_id);
	sb->s_root = d_alloc_root(root);
	if (!sb->s_root)
		goto error;
        return 0;

 error:
	if (root)
                iput(root);
	if(vc)
		vc->vc_sb = NULL;		

        return -EINVAL;
}

static void coda_put_super(struct super_block *sb)
{
	coda_sbp(sb)->vc_sb = NULL;
	sb->s_fs_info = NULL;
	printk("Coda: Bye bye.\n");
}

static void coda_clear_inode(struct inode *inode)
{
	coda_cache_clear_inode(inode);
}

int coda_getattr(struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat)
{
	int err = coda_revalidate_inode(dentry);
	if (!err)
		generic_fillattr(dentry->d_inode, stat);
	return err;
}

int coda_setattr(struct dentry *de, struct iattr *iattr)
{
	struct inode *inode = de->d_inode;
	struct coda_vattr vattr;
	int error;

	lock_kernel();
	
	memset(&vattr, 0, sizeof(vattr)); 

	inode->i_ctime = CURRENT_TIME_SEC;
	coda_iattr_to_vattr(iattr, &vattr);
	vattr.va_type = C_VNON; /* cannot set type */

	/* Venus is responsible for truncating the container-file!!! */
	error = venus_setattr(inode->i_sb, coda_i2f(inode), &vattr);

	if ( !error ) {
	        coda_vattr_to_iattr(inode, &vattr); 
		coda_cache_clear_inode(inode);
	}

	unlock_kernel();

	return error;
}

struct inode_operations coda_file_inode_operations = {
	.permission	= coda_permission,
	.getattr	= coda_getattr,
	.setattr	= coda_setattr,
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18) /* 2.6.18-rc1 */
static int coda_statfs(struct super_block *sb, struct kstatfs *buf)
{
#else
/* git commit 726c334223180e3c0197cc980a432681370d4baf */
static int coda_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	struct super_block *sb = dentry->d_sb;
#endif
	int error;

	lock_kernel();

	error = venus_statfs(sb, buf);

	unlock_kernel();

	if (error) {
		/* fake something like AFS does */
		buf->f_blocks = 9000000;
		buf->f_bfree  = 9000000;
		buf->f_bavail = 9000000;
		buf->f_files  = 9000000;
		buf->f_ffree  = 9000000;
	}

	/* and fill in the rest */
	buf->f_type = CODA_SUPER_MAGIC;
	buf->f_bsize = 1024;
	buf->f_namelen = CODA_MAXNAMLEN;

	return 0;
}

static struct venus_comm *get_device(struct inode *inode)
{
	int idx;

	if (!inode)
		return NULL;

	if (!S_ISCHR(inode->i_mode) || imajor(inode) != CODA_PSDEV_MAJOR) {
		printk("coda_get_sb: Bad file\n");
		return NULL;
	}

	idx = iminor(inode);
	if (idx < 0 || idx >= MAX_CODADEVS) {
		printk("coda_get_sb: Bad minor number\n");
		return NULL;
	}

	return &coda_comms[idx];
}

/* init_coda: used by filesystems.c to register coda */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18) /* 2.6.18-rc1 */
static struct super_block *coda_get_sb(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
#else
/* git commit 454e2398be9b9fa30433fccc548db34d19aa9958 add vfsmount to get_sb */
static int coda_get_sb(struct file_system_type *fs_type, int flags,
		       const char *dev_name, void *data, struct vfsmount *mnt)
#endif
{
	struct coda_mount_data *md;
	struct nameidata nd;
	struct file *file;
	struct venus_comm *vc = NULL;
	int err;

	err = path_lookup(dev_name, LOOKUP_FOLLOW, &nd);
	if (!err) {
		vc = get_device(nd_path_dentry(nd)->d_inode);
		nd_path_release(&nd);
	}
	if (vc)
		goto mount;

	md = (struct coda_mount_data *)data;
	if (!md || md->version != CODA_MOUNT_VERSION) {
		printk("coda: Bad mount data\n");
		goto mount;
	}

	file = fget(md->fd);
	if (file) {
		vc = get_device(file->f_dentry->d_inode);
		fput(file);
	}

mount:
	if (!vc)
		vc = &coda_comms[0];

	return get_sb_nodev(fs_type, flags, vc, coda_fill_super, mnt);
}

struct file_system_type coda_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "coda",
	.get_sb		= coda_get_sb,
	.kill_sb	= kill_anon_super,
	.fs_flags	= FS_BINARY_MOUNTDATA,
};

