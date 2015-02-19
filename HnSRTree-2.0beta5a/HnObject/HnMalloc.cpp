/*
 * HnMalloc.c
 * Copyright (C) 1999 Norio Katayama
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA
 *
 * $Id: HnMalloc.c,v 1.2 2000/06/10 11:14:04 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HnSRTree/HnAbort.h"
#include "HnSRTree/HnMalloc.h"

#define MAX_CACHED_BLOCK_SIZE	512
#define MAX_CACHE_SIZE		(1024 * 1024)

typedef struct {
    size_t blockSize;
    void *block;
    int numBlocks;
    int maxNumBlocks;
} CacheEntry;

static CacheEntry *cache = NULL;
static CacheEntry **cacheIndex = NULL;

static int minBitShift = 0;
static size_t minBlockSize = 0;
static int maxBitShift = 0;
static size_t maxBlockSize = 0;
static int numLevels;

static void
initialize(void)
{
    size_t blockSize, size;
    int bitShift;
    int cacheSizePerLevel, level;

    blockSize = 1;
    bitShift = 0;

    while ( blockSize < sizeof(void *) ) {
	blockSize <<= 1;
	bitShift ++;
    }
    minBitShift = bitShift;
    minBlockSize = (1 << minBitShift);

    while ( blockSize < MAX_CACHED_BLOCK_SIZE ) {
	blockSize <<= 1;
	bitShift ++;
    }
    maxBitShift = bitShift;
    maxBlockSize = (1 << maxBitShift);

    numLevels = (maxBitShift - minBitShift) + 1;
    cacheSizePerLevel = MAX_CACHE_SIZE / numLevels;

    if ( (cache = (CacheEntry *)
	  malloc(sizeof(CacheEntry) * numLevels)) == NULL ) {
	perror("malloc");
	exit(1);
    }
    for ( level=0; level<numLevels; level++ ) {
	cache[level].blockSize = (1 << (minBitShift + level));
	cache[level].block = NULL;
	cache[level].numBlocks = 0;
	cache[level].maxNumBlocks = cacheSizePerLevel / cache[level].blockSize;
    }

    if ( (cacheIndex = (CacheEntry **)
	  malloc(sizeof(CacheEntry *) * (maxBlockSize + 1))) == NULL ) {
	perror("malloc");
	exit(1);
    }

    for ( size=0; size<=cache[0].blockSize; size++ ) {
	cacheIndex[size] = &cache[0];
    }

    for ( level=1; level<numLevels; level++ ) {
	for ( size = cache[level-1].blockSize + 1;
	      size <= cache[level].blockSize;
	      size ++ ) {
	    cacheIndex[size] = &cache[level];
	}
    }
}

void *
HnMalloc(size_t size)
{
    void *ptr;

    if ( cache == NULL ) {
	initialize();
    }

    if ( size <= maxBlockSize ) {
	CacheEntry *entry = cacheIndex[size];
	
	if ( (ptr = entry->block) != NULL ) {
	    entry->block = *((void **)ptr);
	    entry->numBlocks --;
	}
	else {
	    ptr = malloc(entry->blockSize);
	}
    }
    else {
	ptr = malloc(size);
    }

    return ptr;
}

void
HnFree(void *ptr, size_t size)
{
    if ( cache == NULL ) {
	HnAbort("HnMalloc() is not called yet.");
    }

    if ( size <= maxBlockSize ) {
	CacheEntry *entry = cacheIndex[size];

	if ( entry->numBlocks < entry->maxNumBlocks ) {
	    *((void **)ptr) = entry->block;
	    entry->block = ptr;
	    entry->numBlocks ++;
	}
	else {
	    free(ptr);
	}
    }
    else {
	free(ptr);
    }
}

void *
HnRealloc(void *oldPtr, size_t oldSize, size_t newSize)
{
    void *newPtr;

    if ( oldPtr == NULL ) {
	if ( oldSize != 0 ) {
	    HnAbort("``oldSize'' must be zero when ``oldPtr'' is NULL: %d.",
		    oldSize);
	}
	return HnMalloc(newSize);
    }

    if ( newSize > maxBlockSize ) {
	if ( oldSize <= maxBlockSize ) {
	    /*
	     * (oldSize <= maxBlockSize) && (newSize > maxBlockSize)
	     */
	    newPtr = malloc(newSize);
	    memcpy(newPtr, oldPtr, oldSize);
	    HnFree(oldPtr, oldSize);
	}
	else {
	    /*
	     * (oldSize > maxBlockSize) && (newSize > maxBlockSize)
	     */
	    newPtr = realloc(oldPtr, newSize);
	}
    }
    else {
	if ( oldSize <= maxBlockSize ) {
	    /*
	     * (oldSize <= maxBlockSize) && (newSize <= maxBlockSize)
	     */
	    if ( cacheIndex[oldSize]->blockSize ==
		 cacheIndex[newSize]->blockSize ) {
		/* same level */
		newPtr = oldPtr;
	    }
	    else {
		/* different levels */
		newPtr = HnMalloc(newSize);
		memcpy(newPtr, oldPtr, oldSize);
		HnFree(oldPtr, oldSize);
	    }
	}
	else {
	    /*
	     * (oldSize > maxBlockSize) && (newSize <= maxBlockSize)
	     */
	    newPtr = HnMalloc(newSize);
	    memcpy(newPtr, oldPtr, oldSize);
	    free(oldPtr);
	}
    }

    return newPtr;
}
