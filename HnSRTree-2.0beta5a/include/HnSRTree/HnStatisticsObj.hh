/*
 * HnStatisticsObj.hh
 * Copyright (C) 1998 Norio Katayama
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
 * 08/28/98 katayama@rd.nacsis.ac.jp
 * $Id: HnStatisticsObj.hh,v 1.3 2000/06/10 11:11:26 katayama Exp $
 */

#ifndef _HnStatisticsObj_hh
#define _HnStatisticsObj_hh

#include <math.h>
#include "HnSRTree/HnObject.hh"

class HnStatisticsObj: public HnObject {
private:
    double sum;
    double sum2;
    double min;
    double max;
    int count;

public:
    HnStatisticsObj(void);
    void addSample(double sample);
    void print(void) const;

    double getSum(void) const {
	return sum;
    }
    double getAvg(void) const;
    double getVar(void) const;
    double getStd(void) const;
    double getMin(void) const {
	return min;
    }
    double getMax(void) const {
	return max;
    }
    int getNumSamples(void) const {
	return count;
    }
};

#endif /* _HnStatisticsObj_h */
