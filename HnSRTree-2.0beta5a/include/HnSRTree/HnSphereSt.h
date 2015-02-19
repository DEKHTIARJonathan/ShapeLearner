/*
 * HnSphereSt.h
 * Copyright (C) 2002 Norio Katayama
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
 * 09/12/2002 Norio KATAYAMA
 * $Id: HnSphereSt.h,v 1.2 2002/09/13 18:31:35 katayama Exp $
 */

#ifndef _HnSphereSt_h
#define _HnSphereSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnPointSt.h"

/*
 * Structure
 */

typedef struct {
    HnPointSt *center;
    double radius;
} HnSphereSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnSphereSt *HnSphereSt_allocate(int dimension);
_HNSRTIMP void HnSphereSt_free(HnSphereSt *sphere);

#ifdef __cplusplus
};
#endif

#define HnClass HnSphere
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnSphereSt_h */
