/*
 * HnRectObj.hh
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
 * $Id: HnRectObj.hh,v 1.7 2000/12/15 10:00:35 katayama Exp $
 */

#ifndef _HnRectObj_hh
#define _HnRectObj_hh

#include "HnSRTree/HnObject.hh"
#include "HnSRTree/HnString.hh"

/*{
#include "HnSRTree/HnBlockStream.hh"
#include "HnSRTree/HnPoint.hh"
#include "HnSRTree/HnRange.hh"
#include "HnSRTree/HnSphere.hh"
class HnRectArray;
}*/

class HnRectObj: public HnObject {
private:
    int dimension;
    HnRange *ranges;

public:
    HnRectObj(int dimension) {
	this->dimension = dimension;
	this->ranges = (HnRange *)HnMalloc(sizeof(HnRange) * dimension);
	memset(this->ranges, 0, sizeof(HnRange) * dimension);
    }
    HnRectObj(int dimension, HnBlockStream &blockStream) {
	this->dimension = dimension;
	this->ranges = (HnRange *)HnMalloc(sizeof(HnRange) * dimension);
	blockStream.readCharArray((char *)this->ranges,
				  sizeof(HnRange) * dimension);
    }
    HnRectObj(const HnRect &rect) {
	dimension = rect.getObject()->dimension;
	ranges = (HnRange *)HnMalloc(sizeof(HnRange) * dimension);
	memcpy(this->ranges, rect.getObject()->ranges,
	       sizeof(HnRange) * dimension);
    }
    ~HnRectObj(void) {
	HnFree(ranges, sizeof(HnRange) * dimension);
    }

    /* attributes */
    int getDimension(void) const {
	return dimension;
    }
    const HnRange &getRangeAt(int index) const {
	return ranges[index];
    }
    void setRangeAt(double min, double max, int index) {
	ranges[index].set(min, max);
    }
    void setRangeAt(const HnRange &range, int index) {
	ranges[index] = range;
    }
    HnRange *getRanges(void) const {
	return ranges;
    }

    /* HnBlockStream */
    void writeTo(HnBlockStream &blockStream) const {
	blockStream.writeCharArray((char *)ranges,
				   sizeof(HnRange) * dimension);
    }
    static int getSizeInBlockStream(int dimension) {
	return sizeof(HnRange) * dimension;
    }

    /* utilities */
    HnBool equals(const HnRect &rect) const;
    HnBool overlaps(const HnRect &rect) const;
    HnBool overlaps(const HnSphere &sphere) const;
    HnBool includes(const HnRect &rect) const;
    HnBool includes(const HnPoint &point) const;

    double getMaxWidth(void) const;
    double getMargin(void) const;
    HnPoint getCenterPoint(void) const;

    HnRect getOverlappingRect(const HnRect &rect) const;
    HnRect getBoundingRect(const HnRect &rect) const;

    double getDiagonal(void) const;
    double getVolume(void) const;

    HnPoint getNearestPoint(const HnPoint &point) const;
    HnPoint getFarthestPoint(const HnPoint &point) const;

    double getMinDistance(const HnPoint &point) const;
    double getMinDistance(const HnPoint &point,
			  HnPoint *nearestPoint_return) const;
    double getMaxDistance(const HnPoint &point) const;
    double getMaxDistance(const HnPoint &point,
			  HnPoint *farthestPoint_return) const;

    double getLowerBoundMinDistance(const HnPoint &point) const;
    double getLowerBoundMinDistance(const HnPoint &point,
				    HnPoint *nearestPoint_return) const;
    double getUpperBoundMaxDistance(const HnPoint &point) const;
    double getUpperBoundMaxDistance(const HnPoint &point,
				    HnPoint *farthestPoint_return) const;

    HnString toString(void) const;

    /* class methods */
    static HnRect getBoundingRect(const HnPointArray &points);
    static HnRect getBoundingRect(const HnSphereArray &spheres);
    static HnRect getBoundingRect(const HnRectArray &rects);
};

#endif /* _HnRectObj_hh */
