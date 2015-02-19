/*
 * HnSRTreeProfileSt.h
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
 * $Id: HnSRTreeProfileSt.h,v 1.3 2002/09/11 02:40:42 katayama Exp $
 */

#ifndef _HnSRTreeProfileSt_h
#define _HnSRTreeProfileSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /* block I/O */
    int numSuperBlockReads;
    int numSuperBlockWrites;
    int numNodeBlockReads;
    int numNodeBlockWrites;
    int numLeafBlockReads;
    int numLeafBlockWrites;

    /* split */
    int numNodeSplits;
    int numLeafSplits;

    /* search */
    int numVisitedNodes;
    int numVisitedLeaves;
    int numComparedNodeEntries;
    int numComparedLeafEntries;

    /* nearest neighbor search */
    int numEqualDistances;
    int numFartherSpheres;
    int numFartherRects;
} HnSRTreeProfileSt;

_HNSRTIMP HnSRTreeProfileSt *HnSRTreeProfileSt_allocate(void);
_HNSRTIMP void HnSRTreeProfileSt_free(HnSRTreeProfileSt *profile);

_HNSRTIMP void HnSRTreeProfileSt_reset(HnSRTreeProfileSt *profile);
_HNSRTIMP void HnSRTreeProfileSt_print(const HnSRTreeProfileSt *pp);

#ifdef __cplusplus
};
#endif

#endif /* _HnSRTreeProfileSt_h */
