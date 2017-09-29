/*
 * Fixups for kernel version differences to allow for out of tree builds
 * of the Coda kernel module.
 */
#ifndef _KVER_COMPAT_H_
#define _KVER_COMPAT_H_

#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
// commit bdd1d2d3d251c65b74ac4493e08db18971c09240
// Author: Christoph Hellwig <hch@lst.de>
// Date:   Fri Sep 1 17:39:13 2017 +0200
//
//     fs: fix kernel_read prototype
#define kernel_read(a,buf,c,off) kernel_read(a,*(off),(void *)buf,c)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 13, 0)
// commit abbb65899aecfc97bda64b6816d1e501754cfe1f
// Author: Christoph Hellwig <hch@lst.de>
// Date:   Sat May 27 11:16:52 2017 +0300
//
//     fs: implement vfs_iter_write using do_iter_write
#define vfs_iter_write(a,b,c,d) vfs_iter_write(a,b,c)
// commit 18e9710ee59ce3bd2a2512ddcd3f7ceebe8b8d17
// Author: Christoph Hellwig <hch@lst.de>
// Date:   Sat May 27 11:16:51 2017 +0300
//
//     fs: implement vfs_iter_read using do_iter_read
#define vfs_iter_read(a,b,c,d) vfs_iter_read(a,b,c)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 12, 0)
// commit a5695a79088653c73c92ae8d48658cbc49f31884
// Author: Jan Kara <jack@suse.cz>
// Date:   Wed Apr 12 12:24:38 2017 +0200
//
//     coda: Convert to separately allocated bdi
static inline int _super_setup_bdi(
    struct super_block *sb, struct venus_comm *vc)
{
    int error = bdi_setup_and_register(&vc->bdi, "coda");
    sb->s_bdi = &vc->bdi;
    return error;
}
#define super_setup_bdi(sb) _super_setup_bdi(sb, vc)
#else
#define bdi_destroy(a)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0)
// commit a528d35e8bfcc521d7cb70aaf03e1bd296c8493f
// Author: David Howells <dhowells@redhat.com>
// Date:   Tue Jan 31 16:46:22 2017 +0000
//
//     statx: Add a system call to make enhanced file info available
static inline int _coda_getattr(
    struct vfsmount *m, struct dentry *d, struct kstat *s)
{
    struct path p = {
        .mnt = m,
        .dentry = d
    };
    return coda_getattr(&p, s, 0, 0);
}
// commit 174cd4b1e5fbd0d74c68cf3a74f5bd4923485512
// Author: Ingo Molnar <mingo@kernel.org>
// Date:   Thu Feb 2 19:15:33 2017 +0100
//
//     sched/headers: Prepare to move signal wakeup & sigpending methods from <linux/sched.h> into <linux/sched/signal.h>
// commit 3f07c0144132e4f59d88055ac8ff3e691a5fa2b8
// Author: Ingo Molnar <mingo@kernel.org>
// Date:   Wed Feb 8 18:51:30 2017 +0100
//
//     sched/headers: Prepare for new header dependencies before moving code to <linux/sched/signal.h>
#include <linux/sched.h>
// commit f74ac01520c9f6d89bbc3c6931a72f757b742f86
// Author: Miklos Szeredi <mszeredi@redhat.com>
// Date:   Mon Feb 20 16:51:23 2017 +0100
//
//     mm: use helper for calling f_op->mmap()
#define call_mmap(file,vma) file->f_op->mmap(file,vma)
#else
#define _coda_getattr coda_getattr
#include <linux/sched/signal.h>
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 10, 0)
// commit dfeef68862edd7d4bafe68ef7aeb5f658ef24bb5
// Author: Miklos Szeredi <mszeredi@redhat.com>
// Date:   Fri Dec 9 16:45:04 2016 +0100
//
//     vfs: remove ".readlink = generic_readlink" assignments
#define _coda_readlink generic_readlink
#else
#define _coda_readlink NULL
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0)
// commit 82c156f853840645604acd7c2cebcb75ed1b6652
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Thu Sep 22 23:35:42 2016 -0400
//
//     switch generic_file_splice_read() to use of ->read_iter()
static inline ssize_t _coda_file_splice_read(
        struct file *coda_file, loff_t *ppos,
        struct pipe_inode_info *pipe, size_t count,
        unsigned int flags)
{
       ssize_t (*splice_read)(struct file *, loff_t *,
                              struct pipe_inode_info *, size_t, unsigned int);
       struct coda_file_info *cfi;
       struct file *host_file;

       cfi = CODA_FTOC(coda_file);
       BUG_ON(!cfi || cfi->cfi_magic != CODA_MAGIC);
       host_file = cfi->cfi_container;

       splice_read = host_file->f_op->splice_read;
       if (!splice_read)
               splice_read = default_file_splice_read;

       return splice_read(host_file, ppos, pipe, count, flags);
}
// commit 02027d42c3f747945f19111d3da2092ed2148ac8
// Author: Deepa Dinamani <deepa.kernel@gmail.com>
// Date:   Wed Sep 14 07:48:05 2016 -0700
//
//     fs: Replace CURRENT_TIME_SEC with current_time() for inode timestamps
#define current_time(i) CURRENT_TIME_SEC
// commit 2773bf00aeb9bf39e022463272a61dd0ec9f55f4
// Author: Miklos Szeredi <mszeredi@redhat.com>
// Date:   Tue Sep 27 11:03:58 2016 +0200
//
//     fs: rename "rename2" i_op to "rename"
// commit 1cd66c93ba8cdb873258f58ae6a817b28a02bcc3
// Author: Miklos Szeredi <mszeredi@redhat.com>
// Date:   Tue Sep 27 11:03:58 2016 +0200
//
//     fs: make remaining filesystems use .rename2
static inline int _coda_rename(
        struct inode *oi, struct dentry *od,
        struct inode *ni, struct dentry *nd)
{
    return coda_rename(oi, od, ni, nd, 0);
}
#else
#define _coda_file_splice_read generic_file_splice_read
#define _coda_rename coda_rename
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 8, 0)
// commit 84c60b1388249a0167d5fe8160f84e66a1221ba8
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Fri May 27 22:40:31 2016 -0400
//
//     drop redundant ->owner initializations
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 7, 0)
// commit 6192269444ebfbfb42e23c7a6a93c76ffe4b5e51
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Wed Apr 20 23:08:32 2016 -0400
//
//     introduce a parallel variant of ->iterate()
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 5, 0)
// commit 1d5cfdb076288df5eb95545a547a39905e95c930
// Author: Tetsuo Handa <penguin-kernel@i-love.sakura.ne.jp>
// Date:   Fri Jan 22 15:11:02 2016 -0800
//
//     tree wide: use kvfree() than conditional kfree()/vfree()
// commit 5955102c9984fa081b2d570cfac75c97eecf8f3b
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Fri Jan 22 15:40:57 2016 -0500
//
//     wrappers for ->i_mutex access
#define inode_lock(i)   mutex_lock(&i->i_mutex)
#define inode_unlock(i) mutex_unlock(&i->i_mutex)
// commit 5d097056c9a017a3b720849efb5432f37acabbac
// Author: Vladimir Davydov <vdavydov@virtuozzo.com>
// Date:   Thu Jan 14 15:18:21 2016 -0800
//
//     kmemcg: account certain kmem allocations to memcg
// commit fceef393a538134f03b778c5d2519e670269342f
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Tue Dec 29 15:58:39 2015 -0500
//
//     switch ->get_link() to delayed_call, kill ->put_link()
// commit 6b2553918d8b4e6de9853fd6315bec7271a2e592
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Tue Nov 17 10:20:54 2015 -0500
//
//     replace ->follow_link() with new method that could stay in RCU mode
// commit 21fc61c73c3903c4c312d0802da01ec2b323d174
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Tue Nov 17 01:07:57 2015 -0500
//
//     don't put symlink bodies in pagecache into highmem
#define inode_nohighmem(i)
#define _coda_symlink_kmap(p)   kmap(p)
#define _coda_symlink_kunmap(p) kunmap(p)
#else
#define _coda_symlink_kmap(p)   page_address(p)
#define _coda_symlink_kunmap(p)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 4, 0)
// commit 3725e9dd5bef376ab87d41e40c437a6b2a20eb59
// Author: Jan Harkes <jaharkes@cs.cmu.edu>
// Date:   Wed Sep 9 15:38:01 2015 -0700
//
//     fs/coda: fix readlink buffer overflow
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 2, 0)
// commit db6172c41194576ba2a27e64fa2a5576d11d6eb9
// Author: Rasmus Villemoes <linux@rasmusvillemoes.dk>
// Date:   Thu Mar 19 12:28:04 2015 +0100
//
//     fs: cleanup slight list_entry abuse
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
// commit 2b0143b5c986be1ce8408b3aadc4709e0a94429d
// Author: David Howells <dhowells@redhat.com>
// Date:   Tue Mar 17 22:25:59 2015 +0000
//
//     VFS: normal filesystems (and lustre): d_inode() annotations
#define d_inode(de)  (de->d_inode)
#define d_really_is_positive(de) ((de) != NULL)
#define d_really_is_negative(de) ((de) == NULL)
// commit 5d5d568975307877e9195f5305f4240e506a2807
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Fri Apr 3 15:41:18 2015 -0400
//
//     make new_sync_{read,write}() static
// commit c12c49e70218ed1bde28fc8182ac274889d15b9d
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Fri Apr 3 10:58:11 2015 -0400
//
//     coda: switch to ->read_iter/->write_iter
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
#error "missing compatibility glue"
#endif

#endif /* _KVER_COMPAT_H_ */
