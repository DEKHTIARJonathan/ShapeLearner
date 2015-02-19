/*
 * HnStringSt.h
 * Copyright (C) 1997 Norio Katayama
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
 * $Id: HnStringSt.h,v 1.3 2002/09/10 11:44:44 katayama Exp $
 */

#ifndef _HnStringSt_h
#define _HnStringSt_h

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
    int allocatedSize;
    int length;
    char *buffer;
} HnStringSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnStringSt *HnStringSt_allocate(const char *s);
_HNSRTIMP HnStringSt *HnStringSt_allocateN(const char *s, int n);
_HNSRTIMP void HnStringSt_free(HnStringSt *string);

_HNSRTIMP HnBool HnStringSt_equals(const HnStringSt *string1,
				   const HnStringSt *string2);

#ifdef __cplusplus
};
#endif

#define HnClass HnString
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnStringSt_h */
