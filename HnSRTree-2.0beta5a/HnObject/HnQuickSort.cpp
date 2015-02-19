/*
 * HnQuickSort.cc
 * Copyright (C) 1999 Norio Katayama
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
 * 11/25/95 katayama@rd.nacsis.ac.jp
 * $Id: HnQuickSort.cc,v 1.2 2000/06/10 11:18:32 katayama Exp $
 */

#include "HnSRTree/HnAbort.h"
#include "HnSRTree/HnQuickSort.hh"

void
HnQuickSort::sortElements(void)
{
    sortElements(0, getNumElements());
}

void
HnQuickSort::sortElements(int offset, int count)
{
    if ( count < 16 ) {
	int i, j;

	for ( i=offset; i<offset + count; i++ ) {
	    for ( j=i+1; j<offset + count; j++ ) {
		if ( compareElementsAt(i, j) > 0 ) {
		    swapElementsAt(i, j);
		}
	    }
	}
    }
    else {
	int pivotIndex;
	int leftIndex, rightIndex;

	pivotIndex = offset + count / 2;

	/* move a pivot to the beginning */
	if ( pivotIndex != offset ) {
	    swapElementsAt(offset, pivotIndex);
	}

	leftIndex = offset;
	rightIndex = offset + count;

	for ( ;; ) {
	    do {
		leftIndex ++;
	    } while ( leftIndex < offset + count &&
		      compareElementsAt(leftIndex, offset) <= 0 );
	    do {
		rightIndex --;
	    } while ( rightIndex > offset && 
		      compareElementsAt(offset, rightIndex) < 0);

	    if ( leftIndex > rightIndex ) {
		break;
	    }

	    if ( leftIndex == rightIndex ) {
		HnAbort("impossible situation.");
	    }

	    swapElementsAt(leftIndex, rightIndex);
	}

	if ( leftIndex != rightIndex + 1 ) {
	    HnAbort("impossible situation.");
	}

	if ( offset != leftIndex - 1 ) {
	    swapElementsAt(offset, leftIndex - 1);
	}

	if ( debug ) {
	    int i;
	    int pivotIndex = leftIndex - 1;

	    for ( i=offset; i<leftIndex - 1; i++ ) {
		if ( compareElementsAt(i, pivotIndex) > 0 ) {
		    HnAbort("left pivot is failed.");
		}
	    }
	    for ( i=leftIndex; i<offset + count; i++ ) {
		if ( compareElementsAt(i, pivotIndex) <= 0 ) {
		    HnAbort("right pivot is failed.");
		}
	    }
	}

	sortElements(offset, leftIndex - 1 - offset);
	sortElements(leftIndex, offset + count - leftIndex);
    }
}
