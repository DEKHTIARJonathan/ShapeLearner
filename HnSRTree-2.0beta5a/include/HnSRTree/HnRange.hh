/*
 * HnRange.hh
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
 * $Id: HnRange.hh,v 1.6 2000/06/10 11:19:17 katayama Exp $
 */

#ifndef _HnRange_hh
#define _HnRange_hh

#include "HnSRTree/HnAbort.h"
#include "HnSRTree/HnString.hh"

class HnRange {
public:
    static const HnRange null;

private:
    double minValue;
    double maxValue;

public:
    HnRange(void) {
	minValue =  1;
	maxValue = -1;
    }
    HnBool isNull(void) const {
	return (minValue > maxValue);
    }

    void set(double minValue, double maxValue) {
#ifdef DEBUGGING
	if ( minValue > maxValue ) {
	    HnAbort("HnRange: min(%g) is greater than max(%g).",
		    minValue, maxValue);
	}
#endif
	this->minValue = minValue;
	this->maxValue = maxValue;
    }

    /* attributes */
    double getMin(void) const {
#ifdef DEBUGGING
	if ( isNull() ) {
	    HnAbort("invalid operation for ``null''.");
	}
#endif
	return minValue;
    }
    double getMax(void) const {
#ifdef DEBUGGING
	if ( isNull() ) {
	    HnAbort("invalid operation for ``null''.");
	}
#endif
	return maxValue;
    }

    /* utilities */
    double getCenter(void) const {
#ifdef DEBUGGING
	if ( isNull() ) {
	    HnAbort("invalid operation for ``null''.");
	}
#endif
	return (maxValue + minValue) / 2;
    }
    double getWidth(void) const {
#ifdef DEBUGGING
	if ( isNull() ) {
	    HnAbort("invalid operation for ``null''.");
	}
#endif
	return maxValue - minValue;
    }

    /* equals */
    HnBool equals(const HnRange &range) const;
    HnBool operator==(const HnRange &range) const {
	return equals(range);
    }
    HnBool operator!=(const HnRange &range) const {
	return !operator==(range);
    }

    /* overlaps and includes */
    HnBool overlaps(const HnRange &range) const {
#ifdef DEBUGGING
	if ( isNull() ) {
	    HnAbort("invalid operation for ``null''.");
	}
	if ( range.isNull() ) {
	    HnAbort("``range'' is null.");
	}
#endif
	if ( range.getMax() < getMin() || range.getMin() > getMax() ) {
	    return HnFALSE;
	}
	else {
	    return HnTRUE;
	}
    }
    HnBool includes(const HnRange &range) const {
#ifdef DEBUGGING
	if ( isNull() ) {
	    HnAbort("invalid operation for ``null''.");
	}
	if ( range.isNull() ) {
	    HnAbort("``range'' is null.");
	}
#endif
	if ( range.getMin() < getMin() || range.getMax() > getMax() ) {
	    return HnFALSE;
	}
	else {
	    return HnTRUE;
	}
    }
    HnBool includes(double x) const {
#ifdef DEBUGGING
	if ( isNull() ) {
	    HnAbort("invalid operation for ``null''.");
	}
#endif
	if ( x < getMin() || x > getMax() ) {
	    return HnFALSE;
	}
	else {
	    return HnTRUE;
	}
    }

    HnRange getOverlappingRange(const HnRange &range) const;
    HnRange getBoundingRange(const HnRange &range) const;

    HnString toString(void) const;
};

#define HnClass HnRange
#include "HnSRTree/HnClassArray.hh"

#endif /* _HnRange_hh */
