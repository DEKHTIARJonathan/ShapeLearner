/*
 * HnRangeSt.c
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
 * $Id: HnRangeSt.c,v 1.2 2002/09/10 12:10:06 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnRangeSt.h"

HnRangeSt *
HnRangeSt_allocate(void)
{
    HnRangeSt *range;

    range = (HnRangeSt *)HnMalloc(sizeof(HnRangeSt));

    range->min = 0;
    range->max = 0;

    return range;
}

void
HnRangeSt_free(HnRangeSt *range)
{
    range->min = 0;
    range->max = 0;

    HnFree(range, sizeof(HnRangeSt));
}
    
#define HnClass	HnRange
#include "HnSRTree/HnClassArraySt.c"
