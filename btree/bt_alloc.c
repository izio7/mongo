/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2008-2010 WiredTiger, Inc.
 *	All rights reserved.
 *
 * $Id$
 */

#include "wt_internal.h"

static void __wt_cache_extend(WT_TOC *, u_int32_t *, u_int32_t);

/*
 * __wt_cache_alloc --
 *	Alloc a chunk of space from the underlying cache.
 */
int
__wt_cache_alloc(WT_TOC *toc, u_int32_t *addrp, u_int32_t size)
{
	WT_CACHE *cache;
	IDB *idb;

	cache = toc->env->ienv->cache;
	idb = toc->db->idb;

	__wt_cache_extend(toc, addrp, size);

	WT_STAT_INCR(cache->stats, CACHE_ALLOC);
	WT_STAT_INCR(idb->stats, DB_CACHE_ALLOC);

	return (0);
}

/*
 * __wt_cache_free --
 *	Free a chunk of space to the underlying cache.
 */
int
__wt_cache_free(WT_TOC *toc, u_int32_t addr, u_int32_t size)
{
	WT_CACHE *cache;
	IDB *idb;

	cache = toc->env->ienv->cache;
	idb = toc->db->idb;

	/*
	 * If we don't yet have a free page or the free page can't hold more
	 * entries, get another free page and update the root page.
	 */
	WT_STAT_INCR(cache->stats, CACHE_FREE);
	WT_STAT_INCR(idb->stats, DB_CACHE_FREE);
	return (0);
}

/*
 * __wt_cache_extend --
 *	Extend the file to allocate space.
 */
static void
__wt_cache_extend(WT_TOC *toc, u_int32_t *addrp, u_int32_t size)
{
	DB *db;
	WT_FH *fh;

	db = toc->db;
	fh = db->idb->fh;

	/* Extend the file. */
	*addrp = WT_OFF_TO_ADDR(db, fh->file_size);
	fh->file_size += size;
}
