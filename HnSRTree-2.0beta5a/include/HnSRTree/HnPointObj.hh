/*
 * HnPointObj.hh
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
 * $Id: HnPointObj.hh,v 1.7 2000/12/15 10:01:12 katayama Exp $
 */

#ifndef _HnPointObj_hh
#define _HnPointObj_hh

#include "HnSRTree/HnObject.hh"
#include "HnSRTree/HnString.hh"

/*{
#include "HnSRTree/HnBlockStream.hh"
}*/

class HnPointObj: public HnObject {
private:
    int dimension;
    double *coords;

public:
    HnPointObj(int dimension) {
	this->dimension = dimension;
	this->coords = (double *)HnMalloc(sizeof(double) *  dimension);
	memset(this->coords, 0, sizeof(double) * dimension);
    }
    HnPointObj(int dimension, HnBlockStream &blockStream) {
	this->dimension = dimension;
	this->coords = (double *)HnMalloc(sizeof(double) *  dimension);
	blockStream.readCharArray((char *)this->coords,
				  sizeof(double) * dimension);
    }
    HnPointObj(const HnPoint &point) {
	dimension = point.getObject()->dimension;
	coords = (double *)HnMalloc(sizeof(double) * dimension);
	memcpy(coords, point.getObject()->coords, sizeof(double) * dimension);
    }
    ~HnPointObj(void) {
	HnFree(coords, sizeof(double) * dimension);
    }

    /* attributes */
    int getDimension(void) const {
	return dimension;
    }
    double &getCoordAt(int index) const {
#ifdef DEBUGGING
	if ( index < 0 || index >= dimension ) {
	    HnAbort("index is out of bounds.");
	}
#endif
	return coords[index];
    }
    void setCoordAt(double coord, int index) {
#ifdef DEBUGGING
	if ( index < 0 || index >= dimension ) {
	    HnAbort("index is out of bounds.");
	}
#endif
	coords[index] = coord;
    }
    double *getCoords(void) const {
	return coords;
    }

    /* HnBlockStream */
    void writeTo(HnBlockStream &blockStream) const {
	blockStream.writeCharArray((char *)coords, sizeof(double) * dimension);
    }
    static int getSizeInBlockStream(int dimension) {
	return sizeof(double) * dimension;
    }

    /* utilities */
    HnBool equals(const HnPoint &point) const;
    HnString toString(void) const;

    double getDistance(const HnPoint &point) const;
    double getSquareDistance(const HnPoint &point) const;
    double getUpperBoundDistance(const HnPoint &point) const;
    double getLowerBoundDistance(const HnPoint &point) const;
};

#endif /* _HnPointObj_hh */
