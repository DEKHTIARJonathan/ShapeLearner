/*
 * HnSphereObj.cc
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
 * 08/22/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSphereObj.cc,v 1.7 2002/09/11 04:01:27 katayama Exp $
 */

#include <math.h>
#include "HnSRTree/HnSphere.hh"
#include "HnSRTree/HnSphereObj.hh"
#include "HnSRTree/HnRect.hh"

HnBool
HnSphereObj::equals(const HnSphere &sphere) const
{
    return (center.equals(sphere.getCenter()) && 
	    radius == sphere.getRadius());
}

HnBool
HnSphereObj::includes(const HnPoint &point) const
{
    /*
     * NOTE:
     *	    The order of subtraction, i.e., ``point.getDistance(center)''
     *	    or ``center.getDistance(point)'', should be consistent with
     *	    the order that is used at the computation of the radius.
     */

    if ( point.getDistance(center) <= radius ) {
	return HnTRUE;
    }
    else {
	return HnFALSE;
    }
}

HnBool
HnSphereObj::overlaps(const HnSphere &sphere) const
{
    if ( center.getDistance(sphere.getCenter()) <=
	 radius + sphere.getRadius() ) {
	return HnTRUE;
    }
    else {
	return HnFALSE;
    }
}

HnString
HnSphereObj::toString(void) const
{
    HnString string;
    char buffer[256];

    string = (HnString)"HnSphere[center=" + center.toString();

    sprintf(buffer, ",radius=%g", radius);
    string += buffer;

    string += " ]";

    return string;
}

double
HnSphereObj::getVolume(void) const
{
#ifdef _MSC_VER
    int dimension = center.getDimension();
    double pi, g;

    pi = atan(1.0) * 4.0;

    /* g = gamma((double)dimension / 2 + 1) */
    if ( (dimension % 2) == 0 ) {
	int i, n;

	n = dimension / 2 + 1;
	g = 1;
	for ( i=2; i<n; i++ ) {
	    g *= i;
	}
    }
    else {
	double x;

	x = (double)dimension / 2 + 1;
	g = 1;
	while ( x > 1 ) {
	    g *= (x - 1);
	    x --;
	}
	g *= sqrt(pi);
    }

    return pow(pi, (double)dimension / 2) * pow(radius, dimension) / g;
#else
    int dimension = center.getDimension();
    double lg, g;
    extern int signgam;

    lg = lgamma((double)dimension / 2 + 1);
    g = (double)signgam * exp(lg);

    return pow(M_PI, (double)dimension / 2) * pow(radius, dimension) / g;
#endif
}

double
HnSphereObj::getMinDistance(const HnPoint &point) const
{
    double distance = point.getDistance(center);

    if ( distance < radius ) {
	return 0;
    }
    else {
	return distance - radius;
    }
}

double
HnSphereObj::getMaxDistance(const HnPoint &point) const
{
    return point.getDistance(center) + radius;
}

double
HnSphereObj::getLowerBoundMinDistance(const HnPoint &point) const
{
    double distance = point.getLowerBoundDistance(center);

    if ( distance < radius ) {
	return 0;
    }
    else {
	return distance - radius;
    }
}

double
HnSphereObj::getUpperBoundMaxDistance(const HnPoint &point) const
{
    return point.getUpperBoundDistance(center) + radius;
}
