#ifndef _COMPAT_H_
#define _COMPAT_H_

#include <linux/version.h>

#ifndef CURRENT_TIME_SEC
#define CURRENT_TIME_SEC CURRENT_TIME
#endif

/* GregKH has been having fun. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 13) /* 2.6.13 */
#define class_create(a,b)		 class_simple_create(a,b)
#define class_device_create(a,b,c,d,e,f) class_simple_device_add(a,c,d,e,f)
#define class_device_destroy(a,b)	 class_simple_device_remove(b)
#define class_destroy(a)		 class_simple_destroy(a)
#define CLASS struct class_simple
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 15) /* 2.6.15-rc1 */
#define class_device_create(a,b,c,d,e,f) class_device_create(a,c,d,e,f)
#define CLASS struct class
#else
#define CLASS struct class
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16) /* 2.6.16-rc1 */
#define du_child d_child
#define i_mutex i_sem
#define mutex_lock(x) down(x)
#define mutex_unlock(x) up(x)
#else
#define du_child d_u.d_child
#endif

#endif /* _COMPAT_H_ */
