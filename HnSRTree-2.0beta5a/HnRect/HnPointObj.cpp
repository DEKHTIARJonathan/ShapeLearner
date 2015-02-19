/*
 * HnPointObj.cc
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
 * 06/29/96 katayama@rd.nacsis.ac.jp
 * $Id: HnPointObj.cc,v 1.7 2000/12/15 10:01:12 katayama Exp $
 */

#include "HnSRTree/HnPoint.hh"
#include "HnSRTree/HnPointObj.hh"
#include "HnSRTree/HnFPControl.h"
#include <math.h>

HnBool
HnPointObj::equals(const HnPoint &point) const
{
    /*
     * NOTE:
     *	    It is inadequate to compare points by memcmp(),
     *	    since the same double values do not necessarily have
     *	    the same binary value, e.g., 0 (zero) and -0 (minus zero).
     */

    int i;

#ifdef DEBUGGING
    if ( point.getObject()->dimension != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    for ( i=0; i<dimension; i++ ) {
	if ( point.getObject()->coords[i] != coords[i] ) {
	    return HnFALSE;
	}
    }

    return HnTRUE;
}

HnString
HnPointObj::toString(void) const
{
    HnString string;
    int i;

    string = "HnPoint[";

    for ( i=0; i<dimension; i++ ) {
	if ( i != 0 ) {
	    string += ",";
	}
	string += HnString::valueOf(getCoordAt(i));
    }

    string += "]";

    return string;
}

double
HnPointObj::getDistance(const HnPoint &point) const
{
    return sqrt(getSquareDistance(point));
}

double
HnPointObj::getSquareDistance(const HnPoint &point) const
{
    int i;
    double dif, sum;

#ifdef DEBUGGING
    if ( point.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    sum = 0;
    for ( i=0; i<dimension; i++ ) {
	dif = getCoordAt(i) - point.getCoordAt(i);
	sum += dif * dif;
    }

    return sum;
}

double
HnPointObj::getUpperBoundDistance(const HnPoint &point) const
{
    int flag;
    double distance;

    flag = HnFPControl_setRound(HnFPControl_ROUND_PLUS);

    distance = getDistance(point);

    HnFPControl_setRound(flag);

    return distance;
}

double
HnPointObj::getLowerBoundDistance(const HnPoint &point) const
{
    int flag;
    double distance;

    flag = HnFPControl_setRound(HnFPControl_ROUND_MINUS);

    distance = getDistance(point);

    HnFPControl_setRound(flag);

    return distance;
}
