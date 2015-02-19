/*
 * HnPointSt.h
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
 * 07/27/1999 katayama@rd.nacsis.ac.jp
 * $Id: HnPointSt.h,v 1.3 2002/09/13 03:46:35 katayama Exp $
 */

#ifndef _HnPointSt_h
#define _HnPointSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnBool.h"

/*
 * Structure
 */

typedef struct {
    int dimension;
    double *coords;
} HnPointSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnPointSt *HnPointSt_allocate(int dimension);
_HNSRTIMP void HnPointSt_free(HnPointSt *point);

_HNSRTIMP HnBool HnPointSt_equals(const HnPointSt *point1,
				  const HnPointSt *point2);
_HNSRTIMP double HnPointSt_getDistance(const HnPointSt *point1,
				       const HnPointSt *point2);
_HNSRTIMP const char *HnPointSt_toString(const HnPointSt *point);

#ifdef __cplusplus
};
#endif

#define HnClass HnPoint
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnPointSt_h */
