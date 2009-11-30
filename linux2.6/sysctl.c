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
#include <linux/module.h>

#include <linux/coda_linux.h>
#include <linux/coda_psdev.h>

#include "compat.h"

#ifdef CONFIG_SYSCTL
static struct ctl_table_header *fs_table_header;
#endif

/* fd6065b4fdcb64c43e400278ebd0cb56989871c3  sysctl: C99 convert coda ctl_tables and remove binary sysctls */
#ifdef CTL_UNNUMBERED
#define FS_CODA		 CTL_UNNUMBERED
#define CODA_TIMEOUT	 CTL_UNNUMBERED
#define CODA_HARD	 CTL_UNNUMBERED
#define CODA_FAKE_STATFS CTL_UNNUMBERED
#else
#define FS_CODA         1       /* Coda file system */
#define CODA_TIMEOUT    3       /* timeout on upcalls to become intrble */
#define CODA_HARD       5       /* mount type "hard" or "soft" */
#define CODA_FAKE_STATFS 10	 /* don't query venus for actual cache usage */
#endif

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
		.ctl_name = CODA_FAKE_STATFS,
		.procname = "fake_statfs",
		.data     = &coda_fake_statfs,
		.maxlen   = sizeof(int),
		.mode     = 0600,
		.proc_handler = &proc_dointvec
	},
	{}
};

#ifdef CONFIG_SYSCTL
static ctl_table fs_table[] = {
	{
		.ctl_name = FS_CODA,
		.procname = "coda",
		.mode	  = 0555,
		.child    = coda_table
	},
	{}
};
#endif

void coda_sysctl_init(void)
{
#ifdef CONFIG_SYSCTL
	if ( !fs_table_header )
		fs_table_header = register_sysctl_table(fs_table);
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
}
