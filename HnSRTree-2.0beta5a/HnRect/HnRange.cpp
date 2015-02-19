/*
 * HnRange.cc
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
 * 08/27/96 katayama@rd.nacsis.ac.jp
 * $Id: HnRange.cc,v 1.6 2000/06/10 11:19:16 katayama Exp $
 */

#include "HnSRTree/HnRange.hh"

const HnRange HnRange::null;

HnBool
HnRange::equals(const HnRange &range) const
{
    /*
     * NOTE:
     *	    It is inadequate to compare ranges by memcmp(),
     *	    since the same double values do not necessarily have
     *	    the same binary value, e.g., 0 (zero) and -0 (minus zero).
     */
    if ( isNull() || range.isNull() ) {
	if ( isNull() && range.isNull() ) {
	    return HnTRUE;
	}
	else {
	    return HnFALSE;
	}
    }

    if ( getMin() == range.getMin() && getMax() == range.getMax() ) {
	return HnTRUE;
    }
    else {
	return HnFALSE;
    }
}

HnRange
HnRange::getOverlappingRange(const HnRange &range) const
{
    double minValue, maxValue;
    HnRange overlappingRange;
#ifdef DEBUGGING
    if ( isNull() ) {
	HnAbort("invalid operation for ``null''.");
    }
    if ( range.isNull() ) {
	HnAbort("``range'' is null.");
    }
#endif
    /* separated */
    if ( range.getMax() < getMin() ) {
	return HnRange::null;
    }
    if ( range.getMin() > getMax() ) {
	return HnRange::null;
    }

    /* overlapped */
    if ( getMin() > range.getMin() ) {
	minValue = getMin();
    }
    else {
	minValue = range.getMin();
    }

    if ( getMax() < range.getMax() ) {
	maxValue = getMax();
    }
    else {
	maxValue = range.getMax();
    }

    overlappingRange.set(minValue, maxValue);
    return overlappingRange;
}

HnRange
HnRange::getBoundingRange(const HnRange &range) const
{
    double minValue, maxValue;
    HnRange boundingRange;
#ifdef DEBUGGING
    if ( isNull() ) {
	HnAbort("invalid operation for ``null''.");
    }
    if ( range.isNull() ) {
	HnAbort("``range'' is null.");
    }
#endif
    /* get the minimum value */
    if ( getMin() < range.getMin() ) {
	minValue = getMin();
    }
    else {
	minValue = range.getMin();
    }

    /* get the maximum value */
    if ( getMax() > range.getMax() ) {
	maxValue = getMax();
    }
    else {
	maxValue = range.getMax();
    }

    boundingRange.set(minValue, maxValue);
    return boundingRange;
}

HnString
HnRange::toString(void) const
{
    if ( isNull() ) {
	return "HnRange::null";
    }
    else {
	char buffer[256];

	sprintf(buffer, "[%g, %g]", getMin(), getMax());

	return buffer;
    }
}

#define HnClass HnRange
#include "HnSRTree/HnClassArray.cpp"
