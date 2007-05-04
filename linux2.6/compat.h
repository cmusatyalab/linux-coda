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
#define class_device_create(a,b,c,d,e,f) class_simple_device_add(a,c,d,e,f)
#define class_device_destroy(a,b)       class_simple_device_remove(b)
#define class_destroy(a)		class_simple_destroy(a)
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 15) /* 2.6.15-rc1 */
#define class_device_create(a,b,c,d,e,f) class_device_create(a,c,d,e,f)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16) /* 2.6.16-rc1 */
#define du_child d_child
#define i_mutex i_sem
#define mutex_lock(x) down(x)
#define mutex_unlock(x) up(x)
#else
#define du_child d_u.d_child
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18) /* 2.6.18-rc1 */
/* git commit 454e2398be9b9fa30433fccc548db34d19aa9958 add vfsmount to get_sb */
#define get_sb_nodev(a,b,c,d,e) get_sb_nodev(a,b,c,d)
/* git commit ff23eca3e8f613034e0d20ff86f6a89b62f5a14e */
#include <linux/devfs_fs_kernel.h>
int coda_flush(struct file *coda_file);
#else
/* git commit 95dc112a5770dc670a1b45a3d9ee346fdd2b2697 remove devfs_mk_dir */
/* git commit 7c69ef79741910883d5543caafa06aca3ebadbd1 remove devfs_mk_cdev */
/* git commit 8ab5e4c15b53e147c08031a959d9f776823dbe73 remove devfs_remove */
#define devfs_mk_dir(a)
#define devfs_mk_cdev(a,b,c,d) 0
#define devfs_remove(a...)
/* git commit 75e1fcc0b18df0a65ab113198e9dc0e98999a08c add POSIX lock owner */
int coda_flush(struct file *coda_file, fl_owner_t id);
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

#endif /* _COMPAT_H_ */
