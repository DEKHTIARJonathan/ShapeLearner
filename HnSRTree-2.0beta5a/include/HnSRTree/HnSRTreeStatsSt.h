/*
 * HnSRTreeStatsSt.h
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
 * 08/01/1999 Norio KATAYAMA
 * $Id: HnSRTreeStatsSt.h,v 1.3 2002/09/11 02:42:27 katayama Exp $
 */

#ifndef _HnSRTreeStatsSt_h
#define _HnSRTreeStatsSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnStatisticsSt.h"
#include "HnSRTree/HnSRTreeProfileSt.h"

/*
 * Structure
 */

typedef struct {
    HnStatisticsSt *numBlockReadWrites;
    HnStatisticsSt *numBlockReads;
    HnStatisticsSt *numBlockWrites;

    HnStatisticsSt *numSuperBlockReads;
    HnStatisticsSt *numSuperBlockWrites;
    HnStatisticsSt *numNodeBlockReads;
    HnStatisticsSt *numNodeBlockWrites;
    HnStatisticsSt *numLeafBlockReads;
    HnStatisticsSt *numLeafBlockWrites;

    HnStatisticsSt *numVisitedNodes;
    HnStatisticsSt *numVisitedLeaves;
    HnStatisticsSt *numComparedNodeEntries;
    HnStatisticsSt *numComparedLeafEntries;

    HnStatisticsSt *numEqualDistances;
    HnStatisticsSt *numFartherSpheres;
    HnStatisticsSt *numFartherRects;
} HnSRTreeStatsSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnSRTreeStatsSt *HnSRTreeStatsSt_allocate(void);
_HNSRTIMP void HnSRTreeStatsSt_free(HnSRTreeStatsSt *sp);

_HNSRTIMP void HnSRTreeStatsSt_addSample(HnSRTreeStatsSt *sp,
					 const HnSRTreeProfileSt *pp);
_HNSRTIMP void HnSRTreeStatsSt_print(const HnSRTreeStatsSt *sp);

#ifdef __cplusplus
};
#endif

#define HnClass HnSRTreeStats
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnSRTreeStatsSt_h */
