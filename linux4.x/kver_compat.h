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
#error "missing compatibility glue"
#endif

#endif /* _KVER_COMPAT_H_ */
