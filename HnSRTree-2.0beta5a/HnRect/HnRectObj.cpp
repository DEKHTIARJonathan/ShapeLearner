/*
 * HnRectObj.cc
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
 * $Id: HnRectObj.cc,v 1.7 2000/12/15 09:58:37 katayama Exp $
 */

#include <math.h>
#include "HnSRTree/HnRect.hh"
#include "HnSRTree/HnRectObj.hh"

#include "HnSRTree/HnFTdoubleArray.hh"
#include "HnSRTree/HnFPControl.h"

double
HnRectObj::getMaxWidth(void) const
{
    double max, width;
    int i;

    max = getRangeAt(0).getWidth();

    for ( i=1; i<dimension; i++ ) {
	width = getRangeAt(i).getWidth();

	if ( width > max ) {
	    max = width;
	}
    }

    return max;
}

double
HnRectObj::getMargin(void) const
{
    int i, numEdges;
    double sum;

    /* the number of edges for each axis */
    numEdges = 1 << (dimension - 1);

    sum = 0;
    for ( i=0; i<dimension; i++ ) {
	sum += getRangeAt(i).getWidth();
    }

    return sum * numEdges;
}

HnPoint
HnRectObj::getCenterPoint(void) const
{
    HnPoint point = new_HnPoint(dimension);
    int i;

    for ( i=0; i<dimension; i++ ) {
	double min = ranges[i].getMin();
	double max = ranges[i].getMax();

	point.setCoordAt((min + max) / 2, i);
    }

    return point;
}

HnBool
HnRectObj::equals(const HnRect &rect) const
{
    int i;

#ifdef DEBUGGING
    if ( rect.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    for ( i=0; i<dimension; i++ ) {
	if ( !getRangeAt(i).equals(rect.getRangeAt(i)) ) {
	    return HnFALSE;
	}
    }

    return HnTRUE;
}

HnBool
HnRectObj::overlaps(const HnRect &rect) const
{
    int i;

#ifdef DEBUGGING
    if ( rect.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    for ( i=0; i<dimension; i++ ) {
	if ( !getRangeAt(i).overlaps(rect.getRangeAt(i)) ) {
	    return HnFALSE;
	}
    }

    return HnTRUE;
}

HnBool
HnRectObj::overlaps(const HnSphere &sphere) const
{
#ifdef DEBUGGING
    if ( sphere.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif
    /*
     * NOTE:
     *	    Both the rectangle and the sphere are assumed to be closed,
     *	    i.e., their borders are assumed to be inclusive.
     */
    return getMinDistance(sphere.getCenter()) <= sphere.getRadius();
}

HnBool
HnRectObj::includes(const HnRect &rect) const
{
    int i;

#ifdef DEBUGGING
    if ( rect.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    for ( i=0; i<dimension; i++ ) {
	if ( !getRangeAt(i).includes(rect.getRangeAt(i)) ) {
	    return HnFALSE;
	}
    }

    return HnTRUE;
}

HnBool
HnRectObj::includes(const HnPoint &point) const
{
    int i;

#ifdef DEBUGGING
    if ( point.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    for ( i=0; i<dimension; i++ ) {
	if ( !getRangeAt(i).includes(point.getCoordAt(i)) ) {
	    return HnFALSE;
	}
    }

    return HnTRUE;
}

HnRect
HnRectObj::getOverlappingRect(const HnRect &rect) const
{
    int i;
    HnRange overlappingRange;
    HnRect overlappingRect;

#ifdef DEBUGGING
    if ( rect.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    overlappingRect = new_HnRect(dimension);

    for ( i=0; i<dimension; i++ ) {
	overlappingRange =
	    getRangeAt(i).getOverlappingRange(rect.getRangeAt(i));

	if ( overlappingRange == HnRange::null ) {
	    return HnRect::null;
	}

	overlappingRect.setRangeAt(overlappingRange, i);
    }

    return overlappingRect;
}

HnRect
HnRectObj::getBoundingRect(const HnRect &rect) const
{
    int i;
    HnRange boundingRange;
    HnRect boundingRect;

#ifdef DEBUGGING
    if ( rect.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    boundingRect = new_HnRect(dimension);

    for ( i=0; i<dimension; i++ ) {
	boundingRange = getRangeAt(i).getBoundingRange(rect.getRangeAt(i));

	boundingRect.setRangeAt(boundingRange, i);
    }

    return boundingRect;
}

double
HnRectObj::getDiagonal(void) const
{
    int i;
    double sum = 0;

    for ( i=0; i<dimension; i++ ) {
	double width = getRangeAt(i).getWidth();

	sum += width * width;
    }

    return sqrt(sum);
}

double
HnRectObj::getVolume(void) const
{
    int i;
    double volume = 1;

    for ( i=0; i<dimension; i++ ) {
	volume *= getRangeAt(i).getWidth();
    }

    return volume;
}

HnPoint
HnRectObj::getNearestPoint(const HnPoint &point) const
{
    HnPoint nearestPoint = new_HnPoint(dimension);
    int axis;

#ifdef DEBUGGING
    if ( point.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    /* find nearest point */
    for ( axis=0; axis<dimension; axis++ ) {
	double min = getRangeAt(axis).getMin();
	double max = getRangeAt(axis).getMax();
	double x = point.getCoordAt(axis);

	if ( x < min ) {
	    nearestPoint.setCoordAt(min, axis);
	}
	else if ( x < max ) {
	    nearestPoint.setCoordAt(x, axis);
	}
	else {
	    nearestPoint.setCoordAt(max, axis);
	}
    }

    return nearestPoint;
}

HnPoint
HnRectObj::getFarthestPoint(const HnPoint &point) const
{
    HnPoint farthestPoint = new_HnPoint(dimension);
    int axis;

#ifdef DEBUGGING
    if ( point.getDimension() != dimension ) {
	HnAbort("mismatch in dimensions.");
    }
#endif

    /* find farthest point */
    for ( axis=0; axis<dimension; axis++ ) {
	double min = getRangeAt(axis).getMin();
	double max = getRangeAt(axis).getMax();
	double x = point.getCoordAt(axis);

	if ( x < (min + max) / 2 ) {
	    farthestPoint.setCoordAt(max, axis);
	}
	else {
	    farthestPoint.setCoordAt(min, axis);
	}
    }

    return farthestPoint;
}

double
HnRectObj::getMinDistance(const HnPoint &point) const
{
    return getMinDistance(point, NULL);
}

double
HnRectObj::getMinDistance(const HnPoint &point,
			  HnPoint *nearestPoint_return) const
{
    HnPoint nearestPoint = getNearestPoint(point);

    /* set nearestPoint_return if necessary */
    if ( nearestPoint_return != NULL ) {
	*nearestPoint_return = nearestPoint;
    }

    return nearestPoint.getDistance(point);
}

double
HnRectObj::getMaxDistance(const HnPoint &point) const
{
    return getMaxDistance(point, NULL);
}

double
HnRectObj::getMaxDistance(const HnPoint &point,
			  HnPoint *farthestPoint_return) const
{
    HnPoint farthestPoint = getFarthestPoint(point);

    /* set farthestPoint_return if necessary */
    if ( farthestPoint_return != NULL ) {
	*farthestPoint_return = farthestPoint;
    }

    return farthestPoint.getDistance(point);
}

double
HnRectObj::getLowerBoundMinDistance(const HnPoint &point) const
{
    return getLowerBoundMinDistance(point, NULL);
}

double
HnRectObj::getLowerBoundMinDistance(const HnPoint &point,
				    HnPoint *nearestPoint_return) const
{
    HnPoint nearestPoint = getNearestPoint(point);

    /* set nearestPoint_return if necessary */
    if ( nearestPoint_return != NULL ) {
	*nearestPoint_return = nearestPoint;
    }

    return nearestPoint.getLowerBoundDistance(point);
}

double
HnRectObj::getUpperBoundMaxDistance(const HnPoint &point) const
{
    return getUpperBoundMaxDistance(point, NULL);
}

double
HnRectObj::getUpperBoundMaxDistance(const HnPoint &point,
				    HnPoint *farthestPoint_return) const
{
    HnPoint farthestPoint = getFarthestPoint(point);

    /* set farthestPoint_return if necessary */
    if ( farthestPoint_return != NULL ) {
	*farthestPoint_return = farthestPoint;
    }

    return farthestPoint.getUpperBoundDistance(point);
}

HnString
HnRectObj::toString(void) const
{
    HnString string;
    int i;

    string = "HnRect[";

    for ( i=0; i<dimension; i++ ) {
	if ( i != 0 ) {
	    string += ",";
	}
	string += ranges[i].toString();
    }

    string += "]";

    return string;
}

/*
 * Class Methods
 */

HnRect
HnRectObj::getBoundingRect(const HnPointArray &points)
{
    int dimension;
    HnRect boundingRect;
    int i, axis;
    double min, max;

    if ( points.length == 0 ) {
	return HnRect::null;
    }

    dimension = points[0].getDimension();
    boundingRect = new_HnRect(dimension);

    for ( axis=0; axis<dimension; axis++ ) {
	min = max = points[0].getCoordAt(axis);

	for ( i=1; i<points.length; i++ ) {
	    double coord = points[i].getCoordAt(axis);

	    if ( coord < min ) {
		min = coord;
	    }
	    if ( coord > max ) {
		max = coord;
	    }
	}

	boundingRect.setRangeAt(min, max, axis);
    }

    return boundingRect;
}

HnRect
HnRectObj::getBoundingRect(const HnSphereArray &spheres)
{
    int dimension;
    int i, axis;
    HnFTdoubleArray minCoords, maxCoords;
    HnRect boundingRect;

    if ( spheres.length == 0 ) {
	return HnRect::null;
    }

    dimension = spheres[0].getDimension();
    minCoords = new_HnFTdoubleArray(dimension);
    maxCoords = new_HnFTdoubleArray(dimension);

    for ( i=0; i<spheres.length; i++ ) {
	HnPoint center = spheres[i].getCenter();
	double radius = spheres[i].getRadius();

	for ( axis=0; axis<dimension; axis++ ) {
	    double coord = center.getCoordAt(axis);
	    double lowerCoord, upperCoord;
	    int flag;

	    flag = HnFPControl_setRound(HnFPControl_ROUND_MINUS);

	    lowerCoord = coord - radius;

	    HnFPControl_setRound(HnFPControl_ROUND_PLUS);

	    upperCoord = coord + radius;

	    HnFPControl_setRound(flag);

	    if ( i == 0 || lowerCoord < minCoords[axis] ) {
		minCoords[axis] = lowerCoord;
	    }
	    if ( i == 0 || upperCoord > maxCoords[axis] ) {
		maxCoords[axis] = upperCoord;
	    }
	}
    }

    boundingRect = new_HnRect(dimension);
    for ( axis=0; axis<dimension; axis++ ) {
	boundingRect.setRangeAt(minCoords[axis], maxCoords[axis], axis);
    }

    return boundingRect;
}

HnRect
HnRectObj::getBoundingRect(const HnRectArray &rects)
{
    HnRect rect;
    int i;

    if ( rects.length == 0 ) {
	return HnRect::null;
    }

    rect = rects[0];

    for ( i=1; i<rects.length; i++ ) {
	rect = rect.getBoundingRect(rects[i]);
    }

    return rect;
}
