/*
 * Fixups for kernel version differences to allow for out of tree builds
 * of the Coda kernel module.
 */
#ifndef _KVER_COMPAT_H_
#define _KVER_COMPAT_H_

#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 11, 0)
// commit 1527f926fd04490f648c42f42b45218a04754f87
// Author: Christian König <christian.koenig@amd.com>
// Date:   Fri Oct 9 15:08:55 2020 +0200
//
//     mm: mmap: fix fput in error path v2
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0)
// commit ce6595a28a15c874aee374757dcd08f537d7b24d
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Sun Jul 14 16:42:44 2019 -0400
//
//     kill the last users of user_{path,lpath,path_dir}()
// commit 22b139691f9eb8b9d0bfd7341fa7436cb7a9491d
// Author: Deepa Dinamani <deepa.kernel@gmail.com>
// Date:   Tue Jul 30 08:22:29 2019 -0700
//
//     fs: Fill in max and min timestamps in superblock
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 3, 0)
// commit a9fba24c6ac9b66c09dfc2a0e845ecace187e89c
// Author: Pedro Cuadra <pjcuadra@gmail.com>
// Date:   Tue Jul 16 16:29:13 2019 -0700
//
//     coda: add hinting support for partial file caching
// commit 5bb44810f47a00b608ed2cb9f892ae7ce37b02bd
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Tue Jul 16 16:29:09 2019 -0700
//
//     coda: ftoc validity check integration
// commit 7f6118ce95d2673f8c936dd47d9bbeb60f4d16ad
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Tue Jul 16 16:29:06 2019 -0700
//
//     coda: remove sb test in coda_fid_to_inode()
// commit 6975259ae30e05068e87ab35dfc4caefb47cffe2
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Tue Jul 16 16:29:03 2019 -0700
//
//     coda: remove sysctl object from module when unused
// commit f94845284abedf80b8d9ab37eafe0d8f737543e8
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Tue Jul 16 16:29:00 2019 -0700
//
//     coda: add __init to init_coda_psdev()
// commit 50e9a6efb0fa6ee54b2b8a311a75fc6ae2d6a0ec
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Tue Jul 16 16:28:57 2019 -0700
//
//     coda: use SIZE() for stat
// commit 79a0d65e77ed5e9081492848a1b936d19cdfbb0f
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Tue Jul 16 16:28:54 2019 -0700
//
//     coda: destroy mutex in put_super()
// commit 6dc280ebeed2c96a2fb933103dafe655a922b9c1
// Author: Jan Harkes <jaharkes@cs.cmu.edu>
// Date:   Tue Jul 16 16:28:51 2019 -0700
//
//     coda: remove uapi/linux/coda_psdev.h
// commit 8fc8b9df831387e0d02c1d0f5bb53d327e0d477a
// Author: David Howells <dhowells@redhat.com>
// Date:   Tue Jul 16 16:28:47 2019 -0700
//
//     coda: move internal defs out of include/linux/ [ver #2]
// commit b6a18c60080fcff0921e81991fec049394fb04e9
// Author: Jan Harkes <jaharkes@cs.cmu.edu>
// Date:   Tue Jul 16 16:28:44 2019 -0700
//
//     coda: bump module version
// commit 936dae4525322fb46f6f7bb407c5663c83e5d353
// Author: Dan Carpenter <dan.carpenter@oracle.com>
// Date:   Tue Jul 16 16:28:41 2019 -0700
//
//     coda: get rid of CODA_FREE()
// commit 4dc48193d748f0f0fbe37105a905466ff3a6ad50
// Author: Dan Carpenter <dan.carpenter@oracle.com>
// Date:   Tue Jul 16 16:28:38 2019 -0700
//
//     coda: get rid of CODA_ALLOC()
// commit 5e7c31dfe74703f428220384b2863525957cc160
// Author: Jan Harkes <jaharkes@cs.cmu.edu>
// Date:   Tue Jul 16 16:28:35 2019 -0700
//
//     coda: change Coda's user api to use 64-bit time_t in timespec
// commit 6ced9aa7b56baeb241a715df4539e60d5e3118e2
// Author: Arnd Bergmann <arnd@arndb.de>
// Date:   Tue Jul 16 16:28:32 2019 -0700
//
//     coda: stop using 'struct timespec' in user API
// commit 850622136ff2a1296b2c8aa5280a74a6a3aabe4e
// Author: Colin Ian King <colin.king@canonical.com>
// Date:   Tue Jul 16 16:28:29 2019 -0700
//
//     coda: clean up indentation, replace spaces with tab
// commit 9a05671dd8d85cbce6cf1a00727d5919c1e56192
// Author: Jan Harkes <jaharkes@cs.cmu.edu>
// Date:   Tue Jul 16 16:28:23 2019 -0700
//
//     coda: don't try to print names that were considered too long
// commit 6e51f8aa76b67d0a6eb168fd41a81e8478ae07a9
// Author: Jan Harkes <jaharkes@cs.cmu.edu>
// Date:   Tue Jul 16 16:28:16 2019 -0700
//
//     coda: potential buffer overflow in coda_psdev_write()
// commit 02551c23bcd85f0c68a8259c7b953d49d44f86af
// Author: Zhouyang Jia <jiazhouyang09@gmail.com>
// Date:   Tue Jul 16 16:28:13 2019 -0700
//
//     coda: add error handling for fget
// commit 7fa0a1da3dadfd9216df7745a1331fdaa0940d1c
// Author: Jan Harkes <jaharkes@cs.cmu.edu>
// Date:   Tue Jul 16 16:28:04 2019 -0700
//
//     coda: pass the host file in vma->vm_file on mmap
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 2, 0)
// commit d984892bd7e8f3a10ab4436f3916a84c3ee69606
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Mon Apr 15 22:28:35 2019 -0400
//
//     coda: switch to ->free_inode()
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0)
// commit 95582b00838837fc07e042979320caf917ce3fe6
// Author: Deepa Dinamani <deepa.kernel@gmail.com>
// Date:   Tue May 8 19:36:02 2018 -0700
//
//     vfs: change inode times to use struct timespec64
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 16, 0)
// commit a9a08845e9acbd224e4ee466f5c1275ed50054e8
// Author: Linus Torvalds <torvalds@linux-foundation.org>
// Date:   Sun Feb 11 14:34:03 2018 -0800
//
//     vfs: do bulk POLL* -> EPOLL* replacement
#define EPOLLOUT POLLOUT
#define EPOLLIN POLLIN
#define EPOLLWRNORM POLLWRNORM
#define EPOLLRDNORM POLLRDNORM
// commit cfe39442ab8ce9670b4ddd04291b8cddb9cb1129
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Thu Feb 1 12:14:57 2018 -0500
//
//     use linux/poll.h instead of asm/poll.h
// commit 168fe32a072a4b8dc81a3aebf0e5e588d38e2955
// commit 076ccb76e1a6cf0aa5371132efdd502a11e806f1
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Mon Jul 3 01:02:18 2017 -0400
//
//     fs: annotate ->poll() instances
typedef unsigned int __poll_t;
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0)
// commit 1751e8a6cb935e555fcdbcb9ab4f0446e322ca3e
// Author: Linus Torvalds <torvalds@linux-foundation.org>
// Date:   Mon Nov 27 13:05:09 2017 -0800
//
//     Rename superblock flags (MS_xyz -> SB_xyz)
#define SB_NOATIME MS_NOATIME
// commit d337b66a4c52c7b04eec661d86c2ef6e168965a2
// Author: Jan Harkes <jaharkes@cs.cmu.edu>
// Date:   Wed Sep 27 15:52:12 2017 -0400
//
//     coda: fix 'kernel memory exposure attempt' in fsync
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
// commit b24413180f5600bcb3bb70fbed5cf186b60864bd
// Author: Greg Kroah-Hartman <gregkh@linuxfoundation.org>
// Date:   Wed Nov 1 15:07:57 2017 +0100
//
//     License cleanup: add SPDX GPL-2.0 license identifier to files with no license
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
// commit 752ade68cbd81d0321dfecc188f655a945551b25
// Author: Michal Hocko <mhocko@suse.com>
// Date:   Mon May 8 15:57:27 2017 -0700
//
//     treewide: use kv[mz]alloc* rather than opencoded variants
static inline void *kvmalloc(size_t size, gfp_t flags)
{
    if (size < PAGE_SIZE)
	return kzalloc(size, flags);
    else
	return vzalloc(size);
}
#define kvzalloc kvmalloc
// commit a5695a79088653c73c92ae8d48658cbc49f31884
// Author: Jan Kara <jack@suse.cz>
// Date:   Wed Apr 12 12:24:38 2017 +0200
//
//     coda: Convert to separately allocated bdi
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
static inline int _super_setup_bdi(
    struct super_block *sb, struct venus_comm *vc)
{
    int error = bdi_setup_and_register(&vc->bdi, "coda");
    sb->s_bdi = &vc->bdi;
    return error;
}
#endif
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
#include "coda_fs_i.h"
static inline ssize_t _coda_file_splice_read(
        struct file *coda_file, loff_t *ppos,
        struct pipe_inode_info *pipe, size_t count,
        unsigned int flags)
{
       ssize_t (*splice_read)(struct file *, loff_t *,
                              struct pipe_inode_info *, size_t, unsigned int);
       struct coda_file_info *cfi;
       struct file *host_file;

       cfi = coda_ftoc(coda_file);
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
int coda_rename(struct inode *old_dir, struct dentry *old_dentry,
		struct inode *new_dir, struct dentry *new_dentry,
		unsigned int flags);
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
#define SLAB_ACCOUNT 0
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
// commit e36cb0b89ce20b4f8786a57e8a6bc8476f577650
// Author: David Howells <dhowells@redhat.com>
// Date:   Thu Jan 29 12:02:35 2015 +0000
//
//     VFS: (Scripted) Convert S_ISLNK/DIR/REG(dentry->d_inode) to d_is_*(dentry)
// commit b625032b10222c4406979c7604189f2bef29c5d0
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Tue Feb 17 13:45:25 2015 -0800
//
//     fs/coda/dir.c: forward declaration clean-up
// commit b4caecd48005fbed3949dde6c1cb233142fd69e9
// Author: Christoph Hellwig <hch@lst.de>
// Date:   Wed Jan 14 10:42:32 2015 +0100
//
//     fs: introduce f_op->mmap_capabilities for nommu mmap support
static inline int _super_setup_bdi(
    struct super_block *sb, struct venus_comm *vc)
{
    int error = bdi_setup_and_register(&vc->bdi, "coda", BDI_CAP_MAP_COPY);
    sb->s_bdi = &vc->bdi;
    return error;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 19, 0)
// commit 93fe74b2e2b5d266d630f0c3f8287efcbe6ecd10
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Thu Dec 11 13:19:03 2014 -0500
//
//     coda_venus_readdir(): use file_inode()
// commit 946e51f2bf37f1656916eb75bd0742ba33983c28
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Sun Oct 26 19:19:16 2014 -0400
//
//     move d_rcu from overlapping d_child to overlapping d_alias
// commit a7400222e3eb7d5ce3820d2234905bbeafabd171
// Author: Al Viro <viro@zeniv.linux.org.uk>
// Date:   Tue Oct 21 15:20:42 2014 -0400
//
//     new helper: is_root_inode()
static inline bool is_root_inode(struct inode *inode)
{
    return inode == inode->i_sb->s_root->d_inode;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 17, 0)
// commit 834b46c37a2900bc90b5f1c5a11815be5a025445
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Fri Aug 8 14:20:33 2014 -0700
//
//     fs/coda: use linux/uaccess.h
// 361a3bf00582469877f8d18ff20f1efa6b781274
// Author: John Stultz <john.stultz@linaro.org>
// Date:   Wed Jul 16 21:03:58 2014 +0000
//
//     time64: Add time64.h header and define struct timespec64
#if __BITS_PER_LONG == 64
#define timespec64 timespec
#else
typedef __s64 time64_t;
struct timespec64 {
	time64_t	tv_sec;
	long		tv_nsec;
};
#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 16, 0)
#error "missing compatibility glue"
// commit a88bbbeef69daa4a9eebe656bd53ad8f8e657646
// Author: Joe Perches <joe@perches.com>
// Date:   Fri Jun 6 14:38:00 2014 -0700
//
//     coda: convert use of typedef ctl_table to struct ctl_table
// commit 6d6bd94f4d83d70cdff67d0bf2a64ef6878216e7
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Fri Jun 6 14:36:20 2014 -0700
//
//     fs/coda: use __func__
// commit f38cfb2564f5fead53eebd9617258ee0376b1906
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Fri Jun 6 14:36:19 2014 -0700
//
//     fs/coda: logging prefix uniformization
// commit d9b4b3195a06e646c357f6108a424569b1a920d9
// Author: Fabian Frederick <fabf@skynet.be>
// Date:   Fri Jun 6 14:36:18 2014 -0700
//
//     fs/coda: replace printk by pr_foo()
#endif

#endif /* _KVER_COMPAT_H_ */
