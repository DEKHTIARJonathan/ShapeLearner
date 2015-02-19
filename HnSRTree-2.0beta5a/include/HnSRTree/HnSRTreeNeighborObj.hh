/*
 * HnSRTreeNeighborObj.hh
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
 * 07/04/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeNeighborObj.hh,v 1.5 2000/06/10 11:20:01 katayama Exp $
 */

#ifndef _HnSRTreeNeighborObj_hh
#define _HnSRTreeNeighborObj_hh

#include "HnSRTree/HnObject.hh"

/*{
#include "HnSRTree/HnPoint.hh"
#include "HnSRTree/HnDataItem.hh"
}*/

class HnSRTreeNeighborObj: public HnObject {
private:
    /*
     * NOTE:
     *	    The metrics for the distance may be min distance,
     *	    max distance, center point distance, or others,
     *	    as long as it is less than or equal to the min distance
     *	    and greater than or equal to the max distance.
     */

    enum Type { POINT, BLOCK } type;

    /* point */
    HnPoint point;
    HnDataItem dataItem;

    /* block */
    long offset;

    double distance;

public:
    HnSRTreeNeighborObj(const HnPoint &point, const HnDataItem &dataItem,
			double distance) {
	this->type = POINT;

	this->point = point;
	this->dataItem = dataItem;

	this->offset = -1;

	this->distance = distance;
    }
    HnSRTreeNeighborObj(long offset, double distance) {
	this->type = BLOCK;

	this->point = HnPoint::null;
	this->dataItem = HnDataItem::null;

	this->offset = offset;

	this->distance = distance;
    }

    HnPoint getPoint(void) const {
#ifdef DEBUGGING
	if ( type != POINT ) {
	    HnAbort("this neighbor is not a point.");
	}
#endif
	return point;
    }
    HnDataItem getDataItem(void) const {
#ifdef DEBUGGING
	if ( type != POINT ) {
	    HnAbort("this neighbor is not a point.");
	}
#endif
	return dataItem;
    }
    long getOffset(void) const {
#ifdef DEBUGGING
	if ( type != BLOCK ) {
	    HnAbort("this neighbor is not a block.");
	}
#endif
	return offset;
    }
    double getDistance(void) const {
	return distance;
    }

    HnBool isPoint(void) const {
	if ( type == POINT ) {
	    return HnTRUE;
	}
	else {
	    return HnFALSE;
	}
    }

    int compareTo(const HnSRTreeNeighbor &neighbor) const;
    HnString toString(void) const;
};

#endif /* _HnSRTreeNeighborObj_hh */
