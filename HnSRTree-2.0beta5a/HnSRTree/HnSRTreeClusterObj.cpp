/*
 * HnSRTreeClusterObj.cc
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
 * $Id: HnSRTreeClusterObj.cc,v 1.3 2000/06/10 11:19:49 katayama Exp $
 */

#include "HnSRTree/HnSRTreeCluster.hh"
#include "HnSRTree/HnSRTreeClusterObj.hh"

HnSRTreeClusterObj::HnSRTreeClusterObj(const HnPoint &centroid, int weight,
				       const HnSphere &sphere,
				       const HnRect &rect)
{
    if ( centroid.getDimension() != sphere.getDimension() ||
	 centroid.getDimension() != rect.getDimension() ) {
	HnAbort("mismatch in dimensions.");
    }

    this->centroid = centroid;
    this->weight = weight;
    this->sphere = sphere;
    this->rect = rect;
}

HnBool
HnSRTreeClusterObj::equals(const HnSRTreeCluster &cluster) const
{
    return (centroid.equals(cluster.getCentroid()) &&
	    weight == cluster.getWeight() &&
	    sphere.equals(cluster.getSphere()) &&
	    rect.equals(cluster.getRect()));
}

HnString
HnSRTreeClusterObj::toString(void) const
{
    HnString string;

    string = "[centroid=";
    string += centroid.toString();

    string += ",weight=";
    string += HnString::valueOf(weight);

    string += ",sphere=";
    string += sphere.toString();

    string += ",rect=";
    string += rect.toString();

    string += "]";

    return string;
}
