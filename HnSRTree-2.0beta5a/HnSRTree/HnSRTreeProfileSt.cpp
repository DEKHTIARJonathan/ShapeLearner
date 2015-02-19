/*
 * HnSRTreeProfileSt.c
 * Copyright (C) 1998 Norio Katayama
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
 * 10/14/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeProfileSt.c,v 1.3 2000/06/10 11:20:03 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnSRTreeProfileSt.h"

HnSRTreeProfileSt *
HnSRTreeProfileSt_allocate(void)
{
    HnSRTreeProfileSt *profile;

    profile = (HnSRTreeProfileSt *)HnMalloc(sizeof(HnSRTreeProfileSt));

    HnSRTreeProfileSt_reset(profile);

    return profile;
}

void
HnSRTreeProfileSt_free(HnSRTreeProfileSt *profile)
{
    HnFree(profile, sizeof(HnSRTreeProfileSt));
}

void
HnSRTreeProfileSt_reset(HnSRTreeProfileSt *profile)
{
    /* block I/O */
    profile->numSuperBlockReads = 0;
    profile->numSuperBlockWrites = 0;
    profile->numNodeBlockReads = 0;
    profile->numNodeBlockWrites = 0;
    profile->numLeafBlockReads = 0;
    profile->numLeafBlockWrites = 0;

    /* split */
    profile->numNodeSplits = 0;
    profile->numLeafSplits = 0;

    /* search */
    profile->numVisitedNodes = 0;
    profile->numVisitedLeaves = 0;
    profile->numComparedNodeEntries = 0;
    profile->numComparedLeafEntries = 0;

    /* nearest neighbor search */
    profile->numEqualDistances = 0;
    profile->numFartherSpheres = 0;
    profile->numFartherRects = 0;
}

void
HnSRTreeProfileSt_print(const HnSRTreeProfileSt *pp)
{
    int numBlockReads, numBlockWrites;

    numBlockReads = (pp->numSuperBlockReads + 
                     pp->numNodeBlockReads + pp->numLeafBlockReads);
    numBlockWrites = (pp->numSuperBlockWrites +
                      pp->numNodeBlockWrites + pp->numLeafBlockWrites);

    printf("HnSRTreeProfileSt[\n");

    printf("    numBlockReadWrites    : %d\n", numBlockReads + numBlockWrites);
    printf("    numBlockReads         : %d\n", numBlockReads);
    printf("    numBlockWrites        : %d\n", numBlockWrites);

    printf("    numSuperBlockReads    : %d\n", pp->numSuperBlockReads);
    printf("    numSuperBlockWrites   : %d\n", pp->numSuperBlockWrites);
    printf("    numNodeBlockReads     : %d\n", pp->numNodeBlockReads);
    printf("    numNodeBlockWrites    : %d\n", pp->numNodeBlockWrites);
    printf("    numLeafBlockReads     : %d\n", pp->numLeafBlockReads);
    printf("    numLeafBlockWrites    : %d\n", pp->numLeafBlockWrites);

    printf("    numVisitedNodes       : %d\n", pp->numVisitedNodes);
    printf("    numVisitedLeaves      : %d\n", pp->numVisitedLeaves);
    printf("    numComparedNodeEntries: %d\n", pp->numComparedNodeEntries);
    printf("    numComparedLeafEntries: %d\n", pp->numComparedLeafEntries);

    printf("    numEqualDistances     : %d\n", pp->numEqualDistances);
    printf("    numFartherSpheres     : %d\n", pp->numFartherSpheres);
    printf("    numFartherRects       : %d\n", pp->numFartherRects);

    printf("]\n");
}
