/*
 * HnSphereObj.hh
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
 * $Id: HnSphereObj.hh,v 1.5 2000/12/15 09:57:07 katayama Exp $
 */

#ifndef _HnSphereObj_hh
#define _HnSphereObj_hh

#include "HnSRTree/HnObject.hh"
#include "HnSRTree/HnString.hh"

/*{
#include "HnSRTree/HnPoint.hh"
}*/

class HnSphereObj: public HnObject {
private:
    HnPoint center;
    double radius;

public:
    HnSphereObj(const HnPoint &center, double radius) {
	this->center = center;
	this->radius = radius;
    }
    const HnPoint &getCenter(void) const {
	return center;
    }
    double getRadius(void) const {
	return radius;
    }
    int getDimension(void) const {
	return center.getDimension();
    }

    HnBool equals(const HnSphere &sphere) const;
    HnString toString(void) const;

    /* utilities */
    HnBool includes(const HnPoint &point) const;
    HnBool overlaps(const HnSphere &sphere) const;
    double getVolume(void) const;

    double getMinDistance(const HnPoint &point) const;
    double getMaxDistance(const HnPoint &point) const;

    double getLowerBoundMinDistance(const HnPoint &point) const;
    double getUpperBoundMaxDistance(const HnPoint &point) const;
};

#endif /* _HnSphereObj_hh */
