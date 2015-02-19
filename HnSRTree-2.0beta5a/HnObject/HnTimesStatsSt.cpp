/*
 * HnTimesStatsSt.c
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
 * $Id: HnTimesStatsSt.c,v 1.2 2000/06/10 11:12:45 katayama Exp $
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnTimesStatsSt.h"

HnTimesStatsSt *
HnTimesStatsSt_allocate(void)
{
    HnTimesStatsSt *sp;

    sp = (HnTimesStatsSt *)HnMalloc(sizeof(HnTimesStatsSt));

    sp->realTime = HnStatisticsSt_allocate();
    sp->userTime = HnStatisticsSt_allocate();
    sp->systemTime = HnStatisticsSt_allocate();
    sp->cpuTime = HnStatisticsSt_allocate();

    return sp;
}

void
HnTimesStatsSt_free(HnTimesStatsSt *sp)
{
    HnStatisticsSt_free(sp->realTime);
    HnStatisticsSt_free(sp->userTime);
    HnStatisticsSt_free(sp->systemTime);
    HnStatisticsSt_free(sp->cpuTime);

    HnFree(sp, sizeof(HnTimesStatsSt));
}

void
HnTimesStatsSt_addSample(HnTimesStatsSt *sp, const HnTimesSt *tp)
{
    HnStatisticsSt_addSample(sp->realTime, HnTimesSt_getRealTime(tp));
    HnStatisticsSt_addSample(sp->userTime, HnTimesSt_getUserTime(tp));
    HnStatisticsSt_addSample(sp->systemTime, HnTimesSt_getSystemTime(tp));
    HnStatisticsSt_addSample(sp->cpuTime, HnTimesSt_getCPUTime(tp));
}

void
HnTimesStatsSt_print(const HnTimesStatsSt *sp)
{
    printf("HnTimesStatsSt[\n");

    printf("    real  : ");
    HnStatisticsSt_print(sp->realTime);

    printf("    cpu   : ");
    HnStatisticsSt_print(sp->cpuTime);

    printf("    user  : ");
    HnStatisticsSt_print(sp->userTime);

    printf("    system: ");
    HnStatisticsSt_print(sp->systemTime);

    printf("]\n");
}

#define HnClass HnTimesStats
#include "HnSRTree/HnClassArraySt.c"
