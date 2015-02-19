/*
 * HnSRTreeClusterObj.hh
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
 * 08/24/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeClusterObj.hh,v 1.3 2000/06/10 11:19:50 katayama Exp $
 */

#ifndef _HnSRTreeClusterObj_hh
#define _HnSRTreeClusterObj_hh

#include "HnSRTree/HnObject.hh"

/*{
#include "HnSRTree/HnPoint.hh"
#include "HnSRTree/HnSphere.hh"
#include "HnSRTree/HnRect.hh"
}*/

class HnSRTreeClusterObj: public HnObject {
private:
    HnPoint centroid;
    int weight;
    HnSphere sphere;
    HnRect rect;

public:
    HnSRTreeClusterObj(const HnPoint &centroid, int weight,
		       const HnSphere &sphere,
		       const HnRect &rect);

    HnBool equals(const HnSRTreeCluster &cluster) const;

    int getDimension(void) const {
	return centroid.getDimension();
    }

    const HnPoint &getCentroid(void) const {
	return centroid;
    }
    int getWeight(void) const {
	return weight;
    }
    const HnSphere &getSphere(void) const {
	return sphere;
    }
    const HnRect &getRect(void) const {
	return rect;
    }

    HnString toString(void) const;
};

#endif /* _HnSRTreeClusterObj_hh */
