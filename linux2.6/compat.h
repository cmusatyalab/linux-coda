#ifndef _COMPAT_H_
#define _COMPAT_H_

#include <linux/version.h>

#ifndef CURRENT_TIME_SEC
#define CURRENT_TIME_SEC CURRENT_TIME
#endif

/* GregKH has been having fun. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 13) /* 2.6.13 */
#define CLASS struct class_simple
#else
#define CLASS struct class
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 13) /* 2.6.13 */
#define class_create(a,b)		class_simple_create(a,b)
#define class_destroy(a)		class_simple_destroy(a)
#define device_create(a,b,c,d,e)	class_simple_device_add(a,c,NULL,d,e)
#define device_destroy(a,b)		class_simple_device_remove(b)
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 15) /* 2.6.15-rc1 */
#define device_create(a,b,c,d,e)	class_device_create(a,c,NULL,d,e)
#define device_destroy(a,b)		class_device_destroy(a,b)
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 25) /* 2.6.25-rc1 */
#define device_create(a,b,c,d,e)	class_device_create(a,b,c,NULL,d,e)
#define device_destroy(a,b)		class_device_destroy(a,b)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16) /* 2.6.16-rc1 */
#define du_child d_child
#define i_mutex i_sem
#define mutex_lock(x) down(x)
#define mutex_unlock(x) up(x)
#else
#define du_child d_u.d_child
#endif

/* git commit 4b6a9316fab51af611dc8671f296734089f6a22a add SLAB_MEM_SPREAD */
#ifndef SLAB_MEM_SPREAD
#define SLAB_MEM_SPREAD 0
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18) /* 2.6.18-rc1 */
/* git commit 454e2398be9b9fa30433fccc548db34d19aa9958 add vfsmount to get_sb */
#define get_sb_nodev(a,b,c,d,e) get_sb_nodev(a,b,c,d)
/* git commit ff23eca3e8f613034e0d20ff86f6a89b62f5a14e */
#include <linux/devfs_fs_kernel.h>
#else
/* git commit 95dc112a5770dc670a1b45a3d9ee346fdd2b2697 remove devfs_mk_dir */
/* git commit 7c69ef79741910883d5543caafa06aca3ebadbd1 remove devfs_mk_cdev */
/* git commit 8ab5e4c15b53e147c08031a959d9f776823dbe73 remove devfs_remove */
#define devfs_mk_dir(a)
#define devfs_mk_cdev(a,b,c,d) 0
#define devfs_remove(a...)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19) /* 2.6.19-rc1 */
/* git commit 9a53c3a783c2fa9b969628e65695c11c3e51e673 add drop_nlink */
/* git commit d8c76e6f45c111c32a4b3e50a2adc9210737b0d8 add inc_nlink */
#define drop_nlink(i) (i)->i_nlink--
#define inc_nlink(i)  (i)->i_nlink++
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20) /* 2.6.20-rc1 */
/* git commit e94b1766097d53e6f3ccfb36c8baa562ffeda3fc remove SLAB_KERNEL */
#define SLAB_KERNEL GFP_KERNEL
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 21) /* 2.6.21-rc1 */
/* git commit 0b4d414714f0d2f922d39424b0c5c82ad900a381 remove insert_at_head */
#define register_sysctl_table(fs_table) register_sysctl_table(fs_table,0)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 23) /* 2.6.23-rc1 */
/* git commit 20c2df83d25c6a95affe6157a4c9cac4cf5ffaac - Remove slab dtors */
#define kmem_cache_create(a,b,c,d,e) kmem_cache_create(a,b,c,d,e,NULL)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 24) /* 2.6.24-rc1 */
/* git commit a47afb0f9d794d525a372c8d69902147cc88222a pid namespaces API */
#define task_pgrp_nr(a)	process_group(a)
#endif

#endif /* _COMPAT_H_ */

