/*
 * HnPropertiesSt.h
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
 * $Id: HnPropertiesSt.h,v 1.3 2002/09/13 03:41:37 katayama Exp $
 */

#ifndef _HnPropertiesSt_h
#define _HnPropertiesSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnStringSt.h"

/*
 * Structure
 */

typedef struct {
    HnStringVectorSt *keys;
    HnStringVectorSt *values;
} HnPropertiesSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnPropertiesSt *HnPropertiesSt_allocate();
_HNSRTIMP void HnPropertiesSt_free(HnPropertiesSt *properties);

_HNSRTIMP void HnPropertiesSt_removeAllProperties(HnPropertiesSt *properties);
_HNSRTIMP void HnPropertiesSt_setProperty(HnPropertiesSt *properties,
					  const char *key, const char *value);
_HNSRTIMP const char *
HnPropertiesSt_getProperty(const HnPropertiesSt *properties, const char *key);
_HNSRTIMP void HnPropertiesSt_load(HnPropertiesSt *properties,
				   const char *buffer);
_HNSRTIMP void HnPropertiesSt_print(const HnPropertiesSt *properties);

#ifdef __cplusplus
};
#endif

#define HnClass HnProperties
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnPropertiesSt_h */
