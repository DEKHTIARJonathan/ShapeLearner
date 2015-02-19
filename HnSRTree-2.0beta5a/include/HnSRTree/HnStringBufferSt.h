/*
 * HnStringBufferSt.h
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
 * $Id: HnStringBufferSt.h,v 1.3 2002/09/10 11:56:53 katayama Exp $
 */

#ifndef _HnStringBufferSt_h
#define _HnStringBufferSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

/*
 * Structure
 */

typedef struct {
    int allocatedSize;
    int length;
    char *buffer;
} HnStringBufferSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnStringBufferSt *HnStringBufferSt_allocate(void);
_HNSRTIMP void HnStringBufferSt_free(HnStringBufferSt *strbuf);

_HNSRTIMP void HnStringBufferSt_appendChars(HnStringBufferSt *strbuf,
					    const char *s, int n);
_HNSRTIMP void HnStringBufferSt_appendString(HnStringBufferSt *strbuf,
					     const char *str);
_HNSRTIMP void HnStringBufferSt_appendChar(HnStringBufferSt *strbuf, char c);

#ifdef __cplusplus
};
#endif

#define HnClass HnStringBuffer
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnStringBufferSt_h */
