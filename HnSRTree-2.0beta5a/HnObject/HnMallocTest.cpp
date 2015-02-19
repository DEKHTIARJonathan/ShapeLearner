/*
 * HnMallocTest.cc
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
 * $Id: HnMallocTest.cc,v 1.2 2000/06/10 11:18:25 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnTimes.hh"

#define ITERATION	100000
#define MAGNITUDE	5

int
main(void)
{
    HnTimes start, end, interval;
    void *ptr;
    int i, j;

    start = new_HnTimes();
    end = new_HnTimes();

    printf("**** malloc() ****\n");

    for ( i=0; i<MAGNITUDE; i++ ) {
	size_t size = sizeof(HnPointer) * (1 << i);

	start.setCurrentTimes();

	for ( j=0; j<ITERATION; j++ ) {
	    ptr = malloc(size);
	    free(ptr);
	}

	end.setCurrentTimes();

	interval = end.subtract(start);

	printf("Size: %d, CPU time: %g usec\n",
	       size, interval.getCPUTime() / ITERATION * 1e+6);
    }

    printf("**** HnMalloc() ****\n");

    for ( i=0; i<MAGNITUDE; i++ ) {
	size_t size = sizeof(HnPointer) * (1 << i);

	start.setCurrentTimes();

	for ( j=0; j<ITERATION; j++ ) {
	    ptr = HnMalloc(size);
	    HnFree(ptr, size);
	}

	end.setCurrentTimes();

	interval = end.subtract(start);

	printf("Size: %d, CPU time: %g usec\n",
	       size, interval.getCPUTime() / ITERATION * 1e+6);
    }

    printf("**** realloc() ****\n");

    for ( i=0; i<MAGNITUDE; i++ ) {
	size_t size = sizeof(HnPointer *) * (1 << i);

	start.setCurrentTimes();

	for ( j=0; j<ITERATION; j++ ) {
	    ptr = malloc(size);
	    ptr = realloc(ptr, size * 2);
	    free(ptr);
	}

	end.setCurrentTimes();

	interval = end.subtract(start);

	printf("Size: %d, CPU time: %g usec\n",
	       size, interval.getCPUTime() / ITERATION * 1e+6);
    }

    printf("**** HnRealloc() ****\n");

    for ( i=0; i<MAGNITUDE; i++ ) {
	size_t size = sizeof(HnPointer *) * (1 << i);

	start.setCurrentTimes();

	for ( j=0; j<ITERATION; j++ ) {
	    ptr = HnMalloc(size);
	    ptr = HnRealloc(ptr, size, size * 2);
	    HnFree(ptr, size * 2);
	}

	end.setCurrentTimes();

	interval = end.subtract(start);

	printf("Size: %d, CPU time: %g usec\n",
	       size, interval.getCPUTime() / ITERATION * 1e+6);
    }

    printf("**** memcpy() ****\n");

    for ( i=0; i<MAGNITUDE; i++ ) {
	size_t size = sizeof(HnPointer) * (1 << i);
	void *src = malloc(size);
	void *dst = malloc(size);

	start.setCurrentTimes();

	for ( j=0; j<ITERATION; j++ ) {
	    memcpy(dst, src, size);
	}

	end.setCurrentTimes();

	interval = end.subtract(start);

	free(src);
	free(dst);

	printf("Size: %d, CPU time: %g usec\n",
	       size, interval.getCPUTime() / ITERATION * 1e+6);
    }

    return 0;
}

