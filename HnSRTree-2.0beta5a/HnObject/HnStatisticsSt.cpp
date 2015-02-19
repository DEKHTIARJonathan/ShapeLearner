/*
 * HnStatisticsSt.c
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
 * $Id: HnStatisticsSt.c,v 1.2 2000/06/10 11:12:43 katayama Exp $
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnStatisticsSt.h"

HnStatisticsSt *
HnStatisticsSt_allocate(void)
{
    HnStatisticsSt *sp;

    sp = (HnStatisticsSt *)HnMalloc(sizeof(HnStatisticsSt));

    sp->sum = 0;
    sp->sum2 = 0;
    sp->min = 0;
    sp->max = 0;
    sp->count = 0;

    return sp;
}

void
HnStatisticsSt_free(HnStatisticsSt *sp)
{
    HnFree(sp, sizeof(HnStatisticsSt));
}

void
HnStatisticsSt_addSample(HnStatisticsSt *sp, double sample)
{
    sp->sum += sample;
    sp->sum2 += sample * sample;
    if ( sp->count == 0 || sample < sp->min ) {
	sp->min = sample;
    }
    if ( sp->count == 0 || sample > sp->max ) {
	sp->max = sample;
    }
    sp->count ++;
}

void
HnStatisticsSt_print(const HnStatisticsSt *sp)
{
    printf("avg=%g, min=%g, max=%g, std=%g, cnt=%d\n",
	   HnStatisticsSt_getAvg(sp),
	   HnStatisticsSt_getMin(sp),
	   HnStatisticsSt_getMax(sp),
	   HnStatisticsSt_getStd(sp),
	   HnStatisticsSt_getNumSamples(sp));
}

double
HnStatisticsSt_getSum(const HnStatisticsSt *sp)
{
    return sp->sum;
}

double
HnStatisticsSt_getAvg(const HnStatisticsSt *sp)
{
    return sp->sum / sp->count;
}

double
HnStatisticsSt_getVar(const HnStatisticsSt *sp)
{
    double avg = HnStatisticsSt_getAvg(sp);
    return sp->sum2 / sp->count - avg * avg;
}

double
HnStatisticsSt_getStd(const HnStatisticsSt *sp)
{
    return sqrt(HnStatisticsSt_getVar(sp));
}

double
HnStatisticsSt_getMin(const HnStatisticsSt *sp)
{
    return sp->min;
}

double
HnStatisticsSt_getMax(const HnStatisticsSt *sp)
{
    return sp->max;
}

int
HnStatisticsSt_getNumSamples(const HnStatisticsSt *sp)
{
    return sp->count;
}

#define HnClass HnStatistics
#include "HnSRTree/HnClassArraySt.c"
