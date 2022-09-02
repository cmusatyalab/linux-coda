// SPDX-License-Identifier: GPL-2.0
/*
 * Symlink inode operations for Coda filesystem
 * Original version: (C) 1996 P. Braam and M. Callahan
 * Rewritten for Linux 2.1. (C) 1997 Carnegie Mellon University
 * 
 * Carnegie Mellon encourages users to contribute improvements to
 * the Coda project. Contact Peter Braam (coda@cs.cmu.edu).
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/pagemap.h>

#include "coda.h"
#include "coda_psdev.h"
#include "coda_linux.h"
#include "kver_compat.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 19, 0)
static int coda_symlink_filler(struct file *file, struct page *page)
{
	struct inode *inode = page->mapping->host;
#define read_folio readpage
#else
static int coda_symlink_filler(struct file *file, struct folio *folio)
{
	struct page *page = &folio->page;
	struct inode *inode = folio->mapping->host;
#endif
	int error;
	struct coda_inode_info *cii;
	unsigned int len = PAGE_SIZE;
	char *p = _coda_symlink_kmap(page);

	cii = ITOC(inode);

	error = venus_readlink(inode->i_sb, &cii->c_fid, p, &len);
	if (error)
		goto fail;
	SetPageUptodate(page);
	_coda_symlink_kunmap(page);
	unlock_page(page);
	return 0;

fail:
	SetPageError(page);
	_coda_symlink_kunmap(page);
	unlock_page(page);
	return error;
}

const struct address_space_operations coda_symlink_aops = {
	.read_folio	= coda_symlink_filler,
};
