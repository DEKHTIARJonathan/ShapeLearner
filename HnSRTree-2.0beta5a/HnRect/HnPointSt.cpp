/*
 * HnPointSt.c
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
 * 07/27/1999 Norio KATAYAMA
 * $Id: HnPointSt.c,v 1.3 2000/06/10 11:19:16 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "HnSRTree/HnAbort.h"
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnPointSt.h"

#define BUFFER_SIZE	1024

HnPointSt *
HnPointSt_allocate(int dimension)
{
    HnPointSt *point;
    int i;

    point = (HnPointSt *)HnMalloc(sizeof(HnPointSt));

    point->dimension = dimension;
    point->coords = (double *)HnMalloc(sizeof(double) * dimension);

    for ( i=0; i<dimension; i++ ) {
	point->coords[i] = 0;
    }

    return point;
}

void
HnPointSt_free(HnPointSt *point)
{
    HnFree(point->coords, sizeof(double) * point->dimension);

    point->coords = NULL;
    point->dimension = 0;

    HnFree(point, sizeof(HnPointSt));
}
    
HnBool
HnPointSt_equals(const HnPointSt *point1, const HnPointSt *point2)
{
    if ( point1->dimension == point2->dimension ) {
	int i;

	for ( i=0; i<point1->dimension; i++ ) {
	    if ( point1->coords[i] != point2->coords[i] ) {
		return HnFALSE;
	    }
	}

	return HnTRUE;
    }
    else {
	return HnFALSE;
    }
}

double
HnPointSt_getDistance(const HnPointSt *point1, const HnPointSt *point2)
{
    int i;
    double sum, diff;

    if ( point1->dimension != point2->dimension ) {
	HnAbort("mismatch in dimensions.");
    }

    sum = 0;
    for ( i=0; i<point1->dimension; i++ ) {
	diff = point1->coords[i] - point2->coords[i];
	sum += diff * diff;
    }

    return sqrt(sum);
}

const char *
HnPointSt_toString(const HnPointSt *point)
{
    static char buffer[BUFFER_SIZE];
    int i;
    char *bp;

    bp = buffer;

    strcpy(bp, "HnPointSt[");
    bp += strlen(bp);

    for ( i=0; i<point->dimension; i++ ) {
	if ( i != 0 ) {
	    *bp++ = ',';
	}
	sprintf(bp, "%0.6g", point->coords[i]);
	bp += strlen(bp);
    }

    strcpy(bp, "]");

    return buffer;
}

#define HnClass	HnPoint
#include "HnSRTree/HnClassArraySt.c"
