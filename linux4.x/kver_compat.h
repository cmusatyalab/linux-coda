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
#error "missing compatibility glue"
#endif

#endif /* _KVER_COMPAT_H_ */
