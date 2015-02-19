/*
 * HnTimesSt.h
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
 * $Id: HnTimesSt.h,v 1.3 2002/09/11 03:44:37 katayama Exp $
 */

#ifndef _HnTimesSt_h
#define _HnTimesSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#ifdef _MSC_VER
#include <windows.h>	/* for struct timeval */
#else
#include <sys/time.h>
#endif

/*
 * Structure
 */

typedef struct {
    struct timeval rtime;
    struct timeval utime;
    struct timeval stime;
} HnTimesSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnTimesSt *HnTimesSt_allocate(void);
_HNSRTIMP void HnTimesSt_free(HnTimesSt *tp);

_HNSRTIMP double HnTimesSt_getRealTime(const HnTimesSt *tp);
_HNSRTIMP double HnTimesSt_getUserTime(const HnTimesSt *tp);
_HNSRTIMP double HnTimesSt_getSystemTime(const HnTimesSt *tp);
_HNSRTIMP double HnTimesSt_getCPUTime(const HnTimesSt *tp);

_HNSRTIMP void HnTimesSt_setCurrentTimes(HnTimesSt *tp);
_HNSRTIMP void HnTimesSt_add(HnTimesSt *tp3,
			     const HnTimesSt *tp1, const HnTimesSt *tp2);
_HNSRTIMP void HnTimesSt_subtract(HnTimesSt *tp3,
				  const HnTimesSt *tp1, const HnTimesSt *tp2);
_HNSRTIMP void HnTimesSt_print(const HnTimesSt *tp);

#ifdef __cplusplus
};
#endif

#define HnClass HnTimes
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnTimesSt_h */
