/*
 * HnRectSt.c
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
 * $Id: HnRectSt.c,v 1.3 2000/06/10 11:19:20 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnRectSt.h"

HnRectSt *
HnRectSt_allocate(int dimension)
{
    HnRectSt *rect;
    int i;

    rect = (HnRectSt *)HnMalloc(sizeof(HnRectSt));

    rect->dimension = dimension;
    rect->ranges = (HnRangeSt *)HnMalloc(sizeof(HnRangeSt) * dimension);

    for ( i=0; i<dimension; i++ ) {
	rect->ranges[i].min = 0;
	rect->ranges[i].max = 0;
    }

    return rect;
}

void
HnRectSt_free(HnRectSt *rect)
{
    HnFree(rect->ranges, sizeof(HnRangeSt) * rect->dimension);

    rect->ranges = NULL;
    rect->dimension = 0;

    HnFree(rect, sizeof(HnRectSt));
}
    
#define HnClass	HnRect
#include "HnSRTree/HnClassArraySt.c"
