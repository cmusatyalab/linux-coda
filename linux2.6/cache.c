/*
 * Cache operations for Coda.
 * For Linux 2.1: (C) 1997 Carnegie Mellon University
 * For Linux 2.3: (C) 2000 Carnegie Mellon University
 *
 * Carnegie Mellon encourages users of this code to contribute improvements
 * to the Coda project http://www.coda.cs.cmu.edu/ <coda@cs.cmu.edu>.
 */

#include <linux/fs.h>

#include <linux/coda_linux.h>
#include <linux/coda_psdev.h>

static atomic_t permission_epoch = ATOMIC_INIT(0);

/* replace or extend an acl cache hit */
void coda_cache_enter(struct inode *inode, int mask)
{
	struct coda_inode_info *cii = ITOC(inode);

	cii->c_cached_epoch = atomic_read(&permission_epoch);
	if (cii->c_uid != current->fsuid) {
                cii->c_uid = current->fsuid;
                cii->c_cached_perm = mask;
        } else
                cii->c_cached_perm |= mask;
}

/* remove cached acl from an inode */
void coda_cache_clear_inode(struct inode *inode)
{
	struct coda_inode_info *cii = ITOC(inode);
	cii->c_cached_epoch = atomic_read(&permission_epoch) - 1;
}

/* remove all acl caches */
void coda_cache_clear_all(struct super_block *sb)
{
	atomic_inc(&permission_epoch);
}


/* check if the mask has been matched against the acl already */
int coda_cache_check(struct inode *inode, int mask)
{
	struct coda_inode_info *cii = ITOC(inode);
        int hit;
	
        hit = (mask & cii->c_cached_perm) == mask &&
		cii->c_uid == current->fsuid &&
		cii->c_cached_epoch == atomic_read(&permission_epoch);

        return hit;
}


/* Purging dentries and children */
/* The following routine drops dentries which are not in use.  */
int __d_detach(struct dentry *de)
{
	spin_lock(&de->d_lock);

	if (!d_unhashed(de))
		__d_drop(de);

	if (!de->d_inode) {
		spin_unlock(&de->d_lock);
		return 0;
	}

	list_del_init(&de->d_child);
	de->d_parent = de;
	de->d_flags |= DCACHE_DISCONNECTED;
	de->d_flags &= ~DCACHE_UNHASHED;
	hlist_add_head(&de->d_hash, &de->d_sb->s_anon);

	spin_unlock(&de->d_lock);
	return 1;
}

void coda_flag_inode_children(struct inode *inode)
{
	struct list_head *child;
	struct dentry *alias_de, *de;

	if (!inode)
		return;

	alias_de = d_find_alias(inode);
	if (!alias_de)
		return;

	spin_lock(&dcache_lock);
restart:
	list_for_each(child, &alias_de->d_subdirs)
	{
		de = list_entry(child, struct dentry, d_child);
		if (__d_detach(de))
			 goto restart;
	}
	spin_unlock(&dcache_lock);

	shrink_dcache_parent(alias_de);
	dput(alias_de);
}

