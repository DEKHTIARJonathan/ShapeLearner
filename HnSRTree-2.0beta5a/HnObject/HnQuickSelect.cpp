/*
 * HnQuickSelect.cc
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
 * Jun.21,1999 Norio KATAYAMA
 * $Id: HnQuickSelect.cc,v 1.2 2000/06/10 11:18:30 katayama Exp $
 */

#include "HnSRTree/HnAbort.h"
#include "HnSRTree/HnQuickSelect.hh"

void
HnQuickSelect::selectElement(int index)
{
    int beginIndex, endIndex;

    beginIndex = 0;
    endIndex = getNumElements();

    for ( ;; ) {
	int leftIndex, rightIndex;

	leftIndex = beginIndex;
	rightIndex = endIndex;

	/* move a pivot to the beginning */
	if ( beginIndex != index ) {
	    swapElementsAt(beginIndex, index);
	}

	for ( ;; ) {
	    do {
		leftIndex ++;
	    } while ( leftIndex < endIndex &&
		      compareElementsAt(leftIndex, beginIndex) <= 0 );
	    do {
		rightIndex --;
	    } while ( rightIndex > beginIndex &&
		      compareElementsAt(beginIndex, rightIndex) < 0 );

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

	if ( leftIndex - 1 != beginIndex ) {
	    swapElementsAt(beginIndex, leftIndex - 1);
	}

	if ( debug ) {
	    int i;
	    int pivotIndex = leftIndex - 1;

	    for ( i=beginIndex; i<leftIndex - 1; i++ ) {
		if ( compareElementsAt(i, pivotIndex) > 0 ) {
		    HnAbort("left pivot is failed.");
		}
	    }
	    for ( i=leftIndex; i<endIndex; i++ ) {
		if ( compareElementsAt(i, pivotIndex) <= 0 ) {
		    HnAbort("right pivot is failed.");
		}
	    }
	}

	if ( leftIndex <= index ) {
	    beginIndex = leftIndex;
	}
	else if ( index < leftIndex - 1 ) {
	    endIndex = leftIndex - 1;
	}
	else {
	    break;
	}
    }
}
