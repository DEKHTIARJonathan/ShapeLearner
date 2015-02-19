/*
 * HnStatisticsObj.c
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
 * $Id: HnStatisticsObj.cc,v 1.3 2000/06/10 11:11:56 katayama Exp $
 */

#include "HnSRTree/HnStatistics.hh"
#include "HnSRTree/HnStatisticsObj.hh"

HnStatisticsObj::HnStatisticsObj(void)
{
    sum = 0;
    sum2 = 0;
    min = 0;
    max = 0;
    count = 0;
}

void
HnStatisticsObj::addSample(double sample)
{
    sum += sample;
    sum2 += sample * sample;
    if ( count == 0 || sample < min ) {
	min = sample;
    }
    if ( count == 0 || sample > max) {
	max = sample;
    }
    count ++;
}

void
HnStatisticsObj::print(void) const
{
    printf("avg=%g, min=%g, max=%g, std=%g, cnt=%d\n",
	   getAvg(), getMin(), getMax(), getStd(), getNumSamples());
}

double
HnStatisticsObj::getAvg(void) const
{
    return sum / count;
}

double
HnStatisticsObj::getVar(void) const
{
    double avg = getAvg();
    return sum2 / count - avg * avg;
}

double
HnStatisticsObj::getStd(void) const
{
    return sqrt(getVar());
}
