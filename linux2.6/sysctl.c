/*
 * Sysctl operations for Coda filesystem
 * Original version: (C) 1996 P. Braam and M. Callahan
 * Rewritten for Linux 2.1. (C) 1997 Carnegie Mellon University
 * 
 * Carnegie Mellon encourages users to contribute improvements to
 * the Coda project. Contact Peter Braam (coda@cs.cmu.edu).
 * 
 * CODA operation statistics
 * (c) March, 1998 Zhanyong Wan <zhanyong.wan@yale.edu>
 *
 */

#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/module.h>

#include <linux/coda.h>
#include <linux/coda_linux.h>
#include <linux/coda_psdev.h>
#include <linux/coda_proc.h>

#include "compat.h"

static struct ctl_table_header *fs_table_header;

/* fd6065b4fdcb64c43e400278ebd0cb56989871c3  sysctl: C99 convert coda ctl_tables and remove binary sysctls */
#ifdef CTL_UNNUMBERED
#define FS_CODA		 CTL_UNNUMBERED
#define CODA_TIMEOUT	 CTL_UNNUMBERED
#define CODA_HARD	 CTL_UNNUMBERED
#define CODA_VFS	 CTL_UNNUMBERED
#define CODA_CACHE_INV   CTL_UNNUMBERED
#define CODA_FAKE_STATFS CTL_UNNUMBERED
#else
#define FS_CODA         1       /* Coda file system */
#define CODA_TIMEOUT    3       /* timeout on upcalls to become intrble */
#define CODA_HARD       5       /* mount type "hard" or "soft" */
#define CODA_VFS 	 6       /* vfs statistics */
#define CODA_CACHE_INV 	 9       /* cache invalidation statistics */
#define CODA_FAKE_STATFS 10	 /* don't query venus for actual cache usage */
#endif

struct coda_vfs_stats		coda_vfs_stat;
static struct coda_cache_inv_stats	coda_cache_inv_stat;

static void reset_coda_vfs_stats( void )
{
	memset( &coda_vfs_stat, 0, sizeof( coda_vfs_stat ) );
}

static void reset_coda_cache_inv_stats( void )
{
	memset( &coda_cache_inv_stat, 0, sizeof( coda_cache_inv_stat ) );
}

static int do_reset_coda_vfs_stats( ctl_table * table, int write,
				    struct file * filp, void __user * buffer,
				    size_t * lenp, loff_t * ppos )
{
	if ( write ) {
		reset_coda_vfs_stats();

		*ppos += *lenp;
	} else {
		*lenp = 0;
	}

	return 0;
}

static int do_reset_coda_cache_inv_stats( ctl_table * table, int write,
					  struct file * filp,
					  void __user * buffer,
					  size_t * lenp, loff_t * ppos )
{
	if ( write ) {
		reset_coda_cache_inv_stats();

		*ppos += *lenp;
	} else {
		*lenp = 0;
	}
  
	return 0;
}

static int coda_vfs_stats_get_info( char * buffer, char ** start,
				    off_t offset, int length)
{
	int len=0;
	off_t begin;
	struct coda_vfs_stats * ps = & coda_vfs_stat;
  
  /* this works as long as we are below 1024 characters! */
	len += sprintf( buffer,
			"Coda VFS statistics\n"
			"===================\n\n"
			"File Operations:\n"
			"\topen\t\t%9d\n"
			"\tflush\t\t%9d\n"
			"\trelease\t\t%9d\n"
			"\tfsync\t\t%9d\n\n"
			"Dir Operations:\n"
			"\treaddir\t\t%9d\n\n"
			"Inode Operations\n"
			"\tcreate\t\t%9d\n"
			"\tlookup\t\t%9d\n"
			"\tlink\t\t%9d\n"
			"\tunlink\t\t%9d\n"
			"\tsymlink\t\t%9d\n"
			"\tmkdir\t\t%9d\n"
			"\trmdir\t\t%9d\n"
			"\trename\t\t%9d\n"
			"\tpermission\t%9d\n",

			/* file operations */
			ps->open,
			ps->flush,
			ps->release,
			ps->fsync,

			/* dir operations */
			ps->readdir,
		  
			/* inode operations */
			ps->create,
			ps->lookup,
			ps->link,
			ps->unlink,
			ps->symlink,
			ps->mkdir,
			ps->rmdir,
			ps->rename,
			ps->permission); 

	begin = offset;
	*start = buffer + begin;
	len -= begin;

	if ( len > length )
		len = length;
	if ( len < 0 )
		len = 0;

	return len;
}

static int coda_cache_inv_stats_get_info( char * buffer, char ** start,
					  off_t offset, int length)
{
	int len=0;
	off_t begin;
	struct coda_cache_inv_stats * ps = & coda_cache_inv_stat;
  
	/* this works as long as we are below 1024 characters! */
	len += sprintf( buffer,
			"Coda cache invalidation statistics\n"
			"==================================\n\n"
			"flush\t\t%9d\n"
			"purge user\t%9d\n"
			"zap_dir\t\t%9d\n"
			"zap_file\t%9d\n"
			"zap_vnode\t%9d\n"
			"purge_fid\t%9d\n"
			"replace\t\t%9d\n",
			ps->flush,
			ps->purge_user,
			ps->zap_dir,
			ps->zap_file,
			ps->zap_vnode,
			ps->purge_fid,
			ps->replace );
  
	begin = offset;
	*start = buffer + begin;
	len -= begin;

	if ( len > length )
		len = length;
	if ( len < 0 )
		len = 0;

	return len;
}

static ctl_table coda_table[] = {
	{
		.ctl_name = CODA_TIMEOUT,
		.procname = "timeout",
		.data     = &coda_timeout,
		.maxlen   = sizeof(int),
		.mode     = 0644,
		.proc_handler = &proc_dointvec
	},
	{
		.ctl_name = CODA_HARD,
		.procname = "hard",
		.data     = &coda_hard,
		.maxlen   = sizeof(int),
		.mode     = 0644,
		.proc_handler = &proc_dointvec
	},
	{
		.ctl_name = CODA_VFS,
		.procname = "vfs_stats",
		.mode     = 0644,
		.proc_handler = &do_reset_coda_vfs_stats
	},
	{
		.ctl_name = CODA_CACHE_INV,
		.procname = "cache_inv_stats",
		.mode     = 0644,
		.proc_handler = &do_reset_coda_cache_inv_stats
	},
	{
		.ctl_name = CODA_FAKE_STATFS,
		.procname = "fake_statfs",
		.data     = &coda_fake_statfs,
		.maxlen   = sizeof(int),
		.mode     = 0600,
		.proc_handler = &proc_dointvec
	},
	{}
};

static ctl_table fs_table[] = {
	{
		.ctl_name = FS_CODA,
		.procname = "coda",
		.mode	  = 0555,
		.child    = coda_table
	},
	{}
};


#ifdef CONFIG_PROC_FS

/*
 target directory structure:
   /proc/fs  (see linux/fs/proc/root.c)
   /proc/fs/coda
   /proc/fs/coda/{vfs_stats,

*/

static struct proc_dir_entry* proc_fs_coda;

#endif

#define coda_proc_create(name,get_info) \
	create_proc_info_entry(name, 0, proc_fs_coda, get_info)

void coda_sysctl_init(void)
{
	reset_coda_vfs_stats();
	reset_coda_cache_inv_stats();

#ifdef CONFIG_PROC_FS
	proc_fs_coda = proc_mkdir("coda", proc_root_fs);
	if (proc_fs_coda) {
		proc_fs_coda->owner = THIS_MODULE;
		coda_proc_create("vfs_stats", coda_vfs_stats_get_info);
		coda_proc_create("cache_inv_stats", coda_cache_inv_stats_get_info);
	}
#endif

#ifdef CONFIG_SYSCTL
	if ( !fs_table_header )
		fs_table_header = compat_register_sysctl_table(fs_table);
#endif
}

void coda_sysctl_clean(void) 
{

#ifdef CONFIG_SYSCTL
	if ( fs_table_header ) {
		unregister_sysctl_table(fs_table_header);
		fs_table_header = NULL;
	}
#endif

#ifdef CONFIG_PROC_FS
        remove_proc_entry("cache_inv_stats", proc_fs_coda);
        remove_proc_entry("vfs_stats", proc_fs_coda);
	remove_proc_entry("coda", proc_root_fs);
#endif 
}
