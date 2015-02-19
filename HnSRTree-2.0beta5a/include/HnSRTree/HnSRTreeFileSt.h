/*
 * HnSRTreeFileSt.h
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
 * $Id: HnSRTreeFileSt.h,v 1.6 2002/09/13 11:51:51 katayama Exp $
 */

#ifndef _HnSRTreeFileSt_h
#define _HnSRTreeFileSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include <stdio.h>

#include "HnSRTree/HnAbort.h"
#include "HnSRTree/HnPointSt.h"
#include "HnSRTree/HnDataItemSt.h"
#include "HnSRTree/HnSphereSt.h"
#include "HnSRTree/HnRectSt.h"
#include "HnSRTree/HnPropertiesSt.h"

#include "HnSRTree/HnSRTreeProto.h"
#include "HnSRTree/HnSRTreeProfileSt.h"

/*
 * Structure
 */

typedef struct {
    void *opaque;
} HnSRTreeFileSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnSRTreeFileSt *
HnSRTreeFileSt_create(const char *path, int dimension, int dataItemSize, 
		      const HnPropertiesSt *properties);
_HNSRTIMP HnSRTreeFileSt *
HnSRTreeFileSt_open(const char *path, const char *mode);
_HNSRTIMP HnSRTreeFileSt *
HnSRTreeFileSt_build(const char *path, int dimension, int dataItemSize,
		     const HnPointVectorSt *points,
		     const HnDataItemVectorSt *dataItems,
		     const HnPropertiesSt *properties);
_HNSRTIMP void HnSRTreeFileSt_close(HnSRTreeFileSt *file);

_HNSRTIMP void HnSRTreeFileSt_store(HnSRTreeFileSt *file,
				    const HnPointSt *point,
				    const HnDataItemSt *dataItem);
_HNSRTIMP void HnSRTreeFileSt_remove(HnSRTreeFileSt *file,
				     const HnPointSt *point,
				     const HnDataItemSt *dataItem);

_HNSRTIMP void HnSRTreeFileSt_getFirstInRect(HnSRTreeFileSt *file,
					     const HnRectSt *queryRect,
					     HnPointSt **point_return,
					     HnDataItemSt **dataItem_return);
_HNSRTIMP void HnSRTreeFileSt_getFirstInSphere(HnSRTreeFileSt *file,
					       const HnSphereSt *querySphere,
					       HnPointSt **point_return,
					       HnDataItemSt **dataItem_return);
_HNSRTIMP void HnSRTreeFileSt_getNext(HnSRTreeFileSt *file,
				      HnPointSt **point_return,
				      HnDataItemSt **dataItem_return);
/* for the backward compatibility */
#define HnSRTreeFileSt_getFirst(f,q,p,d) HnSRTreeFileSt_getFirstInRect(f,q,p,d)

_HNSRTIMP void HnSRTreeFileSt_getInRect(HnSRTreeFileSt *file,
					const HnRectSt *queryRect,
					HnPointVectorSt **points_return,
					HnDataItemVectorSt **dataItems_return);
_HNSRTIMP void
HnSRTreeFileSt_getInSphere(HnSRTreeFileSt *file,
			   const HnSphereSt *querySphere,
			   HnPointVectorSt **points_return,
			   HnDataItemVectorSt **dataItems_return);

_HNSRTIMP void
HnSRTreeFileSt_getNeighbors(HnSRTreeFileSt *file,
			    const HnPointSt *queryPoint, int maxCount,
			    HnPointVectorSt **points_return,
			    HnDataItemVectorSt **dataItems_return);
_HNSRTIMP void
HnSRTreeFileSt_getColoredNeighbors(HnSRTreeFileSt *file,
				   const HnPointVectorSt *queryPoints,
				   int maxCount,
				   HnPointVectorSt **points_return,
				   HnDataItemVectorSt **dataItems_return,
				   HnSRTreeCompColorsFunc *compColorsFunc);

_HNSRTIMP int HnSRTreeFileSt_getDimension(HnSRTreeFileSt *file);
_HNSRTIMP int HnSRTreeFileSt_getBlockSize(HnSRTreeFileSt *file);
_HNSRTIMP int HnSRTreeFileSt_getDataItemSize(HnSRTreeFileSt *file);
_HNSRTIMP int HnSRTreeFileSt_getHeight(HnSRTreeFileSt *file);

_HNSRTIMP void HnSRTreeFileSt_check(HnSRTreeFileSt *file);
_HNSRTIMP void HnSRTreeFileSt_print(HnSRTreeFileSt *file,
					   HnBool verbose);

_HNSRTIMP void HnSRTreeFileSt_resetProfile(HnSRTreeFileSt *file);
_HNSRTIMP void HnSRTreeFileSt_copyProfileInto(HnSRTreeFileSt *file,
					      HnSRTreeProfileSt *profile);

_HNSRTIMP HnBool HnSRTreeFileSt_dumpToFile(HnSRTreeFileSt *file,
					   const char *fileName);
_HNSRTIMP HnBool HnSRTreeFileSt_dumpToFileStream(HnSRTreeFileSt *file,
						 FILE *fp);

_HNSRTIMP void HnSRTreeFileSt_setProperties(HnSRTreeFileSt *file,
					    const HnPropertiesSt *properties);
_HNSRTIMP void HnSRTreeFileSt_getProperties(HnSRTreeFileSt *file,
					    HnPropertiesSt *properties_return);

_HNSRTIMP void HnSRTreeFileSt_setDebug(HnBool debug);

#ifdef __cplusplus
};
#endif

#endif /* _HnSRTreeFileSt_h */
