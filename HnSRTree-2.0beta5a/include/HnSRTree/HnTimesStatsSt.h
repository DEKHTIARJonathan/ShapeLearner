/*
 * HnTimesStatsSt.h
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
 * $Id: HnTimesStatsSt.h,v 1.3 2002/09/10 11:58:50 katayama Exp $
 */

#ifndef _HnTimesStatsSt_h
#define _HnTimesStatsSt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnStatisticsSt.h"
#include "HnSRTree/HnTimesSt.h"

/*
 * Structure
 */

typedef struct {
    HnStatisticsSt *realTime;
    HnStatisticsSt *userTime;
    HnStatisticsSt *systemTime;
    HnStatisticsSt *cpuTime;
} HnTimesStatsSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnTimesStatsSt *HnTimesStatsSt_allocate(void);
_HNSRTIMP void HnTimesStatsSt_free(HnTimesStatsSt *timesStats);

_HNSRTIMP void HnTimesStatsSt_addSample(HnTimesStatsSt *timesStats,
					const HnTimesSt *ts);
_HNSRTIMP void HnTimesStatsSt_print(const HnTimesStatsSt *timesStats);

#ifdef __cplusplus
};
#endif

#define HnClass HnTimesStats
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnTimesStatsSt_h */
