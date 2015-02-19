/*
 * HnStatisticsSt.h
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
 * $Id: HnStatisticsSt.h,v 1.3 2002/09/10 11:54:12 katayama Exp $
 */

#ifndef _HnStatisticsSt_h
#define _HnStatisticsSt_h

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
    double sum;
    double sum2;
    double min;
    double max;
    int count;
} HnStatisticsSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnStatisticsSt *HnStatisticsSt_allocate(void);
_HNSRTIMP void HnStatisticsSt_free(HnStatisticsSt *sp);

_HNSRTIMP void HnStatisticsSt_addSample(HnStatisticsSt *sp, double sample);
_HNSRTIMP void HnStatisticsSt_print(const HnStatisticsSt *sp);

_HNSRTIMP double HnStatisticsSt_getSum(const HnStatisticsSt *sp);
_HNSRTIMP double HnStatisticsSt_getAvg(const HnStatisticsSt *sp);
_HNSRTIMP double HnStatisticsSt_getVar(const HnStatisticsSt *sp);
_HNSRTIMP double HnStatisticsSt_getStd(const HnStatisticsSt *sp);
_HNSRTIMP double HnStatisticsSt_getMin(const HnStatisticsSt *sp);
_HNSRTIMP double HnStatisticsSt_getMax(const HnStatisticsSt *sp);
_HNSRTIMP int HnStatisticsSt_getNumSamples(const HnStatisticsSt *sp);

#ifdef __cplusplus
};
#endif

#define HnClass HnStatistics
#include "HnSRTree/HnClassArraySt.h"

#endif /* _HnStatisticsSt_h */
