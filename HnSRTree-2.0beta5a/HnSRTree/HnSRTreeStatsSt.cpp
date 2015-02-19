/*
 * HnSRTreeStatsSt.c
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
 * $Id: HnSRTreeStatsSt.c,v 1.2 2000/06/10 11:20:08 katayama Exp $
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnSRTreeStatsSt.h"

HnSRTreeStatsSt *
HnSRTreeStatsSt_allocate(void)
{
    HnSRTreeStatsSt *sp;

    sp = (HnSRTreeStatsSt *)HnMalloc(sizeof(HnSRTreeStatsSt));

    sp->numBlockReadWrites = HnStatisticsSt_allocate();
    sp->numBlockReads = HnStatisticsSt_allocate();
    sp->numBlockWrites = HnStatisticsSt_allocate();

    sp->numSuperBlockReads = HnStatisticsSt_allocate();
    sp->numSuperBlockWrites = HnStatisticsSt_allocate();
    sp->numNodeBlockReads = HnStatisticsSt_allocate();
    sp->numNodeBlockWrites = HnStatisticsSt_allocate();
    sp->numLeafBlockReads = HnStatisticsSt_allocate();
    sp->numLeafBlockWrites = HnStatisticsSt_allocate();

    sp->numVisitedNodes = HnStatisticsSt_allocate();
    sp->numVisitedLeaves = HnStatisticsSt_allocate();
    sp->numComparedNodeEntries = HnStatisticsSt_allocate();
    sp->numComparedLeafEntries = HnStatisticsSt_allocate();

    sp->numEqualDistances = HnStatisticsSt_allocate();
    sp->numFartherSpheres = HnStatisticsSt_allocate();
    sp->numFartherRects = HnStatisticsSt_allocate();

    return sp;
}

void
HnSRTreeStatsSt_free(HnSRTreeStatsSt *sp)
{
    HnStatisticsSt_free(sp->numBlockReadWrites);
    HnStatisticsSt_free(sp->numBlockReads);
    HnStatisticsSt_free(sp->numBlockWrites);

    HnStatisticsSt_free(sp->numSuperBlockReads);
    HnStatisticsSt_free(sp->numSuperBlockWrites);
    HnStatisticsSt_free(sp->numNodeBlockReads);
    HnStatisticsSt_free(sp->numNodeBlockWrites);
    HnStatisticsSt_free(sp->numLeafBlockReads);
    HnStatisticsSt_free(sp->numLeafBlockWrites);

    HnStatisticsSt_free(sp->numVisitedNodes);
    HnStatisticsSt_free(sp->numVisitedLeaves);
    HnStatisticsSt_free(sp->numComparedNodeEntries);
    HnStatisticsSt_free(sp->numComparedLeafEntries);

    HnStatisticsSt_free(sp->numEqualDistances);
    HnStatisticsSt_free(sp->numFartherSpheres);
    HnStatisticsSt_free(sp->numFartherRects);

    HnFree(sp, sizeof(HnSRTreeStatsSt));
}

void
HnSRTreeStatsSt_addSample(HnSRTreeStatsSt *sp, const HnSRTreeProfileSt *pp)
{
    int numBlockReads, numBlockWrites;

    numBlockReads = (pp->numSuperBlockReads + 
		     pp->numNodeBlockReads + pp->numLeafBlockReads);
    numBlockWrites = (pp->numSuperBlockWrites +
                      pp->numNodeBlockWrites + pp->numLeafBlockWrites);

    HnStatisticsSt_addSample(sp->numBlockReadWrites,
			     numBlockReads + numBlockWrites);
    HnStatisticsSt_addSample(sp->numBlockReads, numBlockReads);
    HnStatisticsSt_addSample(sp->numBlockWrites, numBlockWrites);

    HnStatisticsSt_addSample(sp->numSuperBlockReads, pp->numSuperBlockReads);
    HnStatisticsSt_addSample(sp->numSuperBlockWrites, pp->numSuperBlockWrites);
    HnStatisticsSt_addSample(sp->numNodeBlockReads, pp->numNodeBlockReads);
    HnStatisticsSt_addSample(sp->numNodeBlockWrites, pp->numNodeBlockWrites);
    HnStatisticsSt_addSample(sp->numLeafBlockReads, pp->numLeafBlockReads);
    HnStatisticsSt_addSample(sp->numLeafBlockWrites, pp->numLeafBlockWrites);

    HnStatisticsSt_addSample(sp->numVisitedNodes, pp->numVisitedNodes);
    HnStatisticsSt_addSample(sp->numVisitedLeaves, pp->numVisitedLeaves);
    HnStatisticsSt_addSample(sp->numComparedNodeEntries,
			     pp->numComparedNodeEntries);
    HnStatisticsSt_addSample(sp->numComparedLeafEntries,
			     pp->numComparedLeafEntries);

    HnStatisticsSt_addSample(sp->numEqualDistances, pp->numEqualDistances);
    HnStatisticsSt_addSample(sp->numFartherSpheres, pp->numFartherSpheres);
    HnStatisticsSt_addSample(sp->numFartherRects, pp->numFartherRects);
}

void
HnSRTreeStatsSt_print(const HnSRTreeStatsSt *sp)
{
    printf("HnSRTreeStatsSt[\n");

    printf("    numBlockReadWrites    : ");
    HnStatisticsSt_print(sp->numBlockReadWrites);

    printf("    numBlockReads         : ");
    HnStatisticsSt_print(sp->numBlockReads);

    printf("    numBlockWrites        : ");
    HnStatisticsSt_print(sp->numBlockWrites);

    printf("    numSuperBlockReads    : ");
    HnStatisticsSt_print(sp->numSuperBlockReads);

    printf("    numSuperBlockWrites   : ");
    HnStatisticsSt_print(sp->numSuperBlockWrites);

    printf("    numNodeBlockReads     : ");
    HnStatisticsSt_print(sp->numNodeBlockReads);

    printf("    numNodeBlockWrites    : ");
    HnStatisticsSt_print(sp->numNodeBlockWrites);

    printf("    numLeafBlockReads     : ");
    HnStatisticsSt_print(sp->numLeafBlockReads);

    printf("    numLeafBlockWrites    : ");
    HnStatisticsSt_print(sp->numLeafBlockWrites);

    printf("    numVisitedNodes       : ");
    HnStatisticsSt_print(sp->numVisitedNodes);

    printf("    numVisitedLeaves      : ");
    HnStatisticsSt_print(sp->numVisitedLeaves);

    printf("    numComparedNodeEntries: ");
    HnStatisticsSt_print(sp->numComparedNodeEntries);

    printf("    numComparedLeafEntries: ");
    HnStatisticsSt_print(sp->numComparedLeafEntries);

    printf("    numEqualDistances     : ");
    HnStatisticsSt_print(sp->numEqualDistances);

    printf("    numFartherSpheres     : ");
    HnStatisticsSt_print(sp->numFartherSpheres);

    printf("    numFartherRects       : ");
    HnStatisticsSt_print(sp->numFartherRects);

    printf("]\n");
}

#define HnClass HnSRTreeStats
#include "HnSRTree/HnClassArraySt.c"
