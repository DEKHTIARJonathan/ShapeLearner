/*
 * HnRectSt.h
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
 * $Id: HnRectSt.h,v 1.4 2002/09/10 12:11:15 katayama Exp $
 */

#ifndef _HnRectSt_h
#define _HnRectSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnRangeSt.h"

/*
 * Structure
 */

typedef struct {
    int dimension;
    HnRangeSt *ranges;
} HnRectSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnRectSt *HnRectSt_allocate(int dimension);
_HNSRTIMP void HnRectSt_free(HnRectSt *rect);

#ifdef __cplusplus
};
#endif

#define HnClass HnRect
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnRectSt_h */
