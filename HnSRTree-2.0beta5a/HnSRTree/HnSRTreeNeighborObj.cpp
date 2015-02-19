/*
 * HnSRTreeNeighborObj.cc
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
 * $Id: HnSRTreeNeighborObj.cc,v 1.5 2000/06/10 11:20:00 katayama Exp $
 */

#include "HnSRTree/HnSRTreeNeighbor.hh"
#include "HnSRTree/HnSRTreeNeighborObj.hh"

#include "HnSRTree/HnStringBuffer.hh"

int
HnSRTreeNeighborObj::compareTo(const HnSRTreeNeighbor &neighbor) const
{
    double distance1, distance2;
    HnBool isPoint1, isPoint2;
    long offset1, offset2;
    int length1, length2;

    /* distance */
    distance1 = getDistance();
    distance2 = neighbor.getDistance();

    if ( distance1 != distance2 ) {
	if ( distance1 < distance2 ) {
	    return -1;
	}
	else {
	    return 1;
	}
    }

    /* point or block */
    isPoint1 = isPoint();
    isPoint2 = neighbor.isPoint();

    if ( isPoint1 != isPoint2 ) {
	if ( !isPoint1 && isPoint2 ) {
	    return -1;
	}
	else {
	    return 1;
	}
    }

    if ( !isPoint1 ) {
	/* offset */
	offset1 = getOffset();
	offset2 = neighbor.getOffset();

        if ( offset1 < offset2 ) {
            return -1;
	}
        if ( offset1 > offset2 ) {
            return 1;
	}
        return 0;
    }
    else {
	/* dataItem */
	length1 = getDataItem().length();
	length2 = neighbor.getDataItem().length();

	if ( length1 != length2 ) {
	    if ( length1 < length2 ) {
		return -1;
	    }
            else {
		return 1;
	    }
	}

        return memcmp(getDataItem().toCharArray(),
		      neighbor.getDataItem().toCharArray(),
                      length1);
    }
}

HnString
HnSRTreeNeighborObj::toString(void) const
{
    HnStringBuffer sb = new_HnStringBuffer();

    switch (type) {
    case POINT:
	sb += "HnSRTreeNeighbor[";
	sb += "point=";
	sb += point.toString();
	sb += ",distance=";
	sb += HnString::valueOf(distance);
	sb += "]";
	break;
    case BLOCK:
	sb += "HnSRTreeNeighbor[";
	sb += "offset=";
	sb += HnString::valueOf(offset);
	sb += ",distance=";
	sb += HnString::valueOf(distance);
	sb += "]";
	break;
    default:
	HnAbort("unexpected value for ``type'': %d.", type);
    }

    return sb.toString();
}
