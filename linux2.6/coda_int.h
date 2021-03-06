#ifndef _CODA_INT_H_
#define _CODA_INT_H_

extern struct file_system_type coda_fs_type;

void coda_destroy_inodecache(void);
int coda_init_inodecache(void);

int coda_fsync(struct file *coda_file, struct dentry *coda_dentry,
	       int datasync);

void coda_sysctl_init(void);
void coda_sysctl_clean(void);

#endif /* _CODA_INT_H_ */
