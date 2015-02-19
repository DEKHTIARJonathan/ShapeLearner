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
 * $Id: HnBinarySearch.cc,v 1.2 2000/06/10 11:18:17 katayama Exp $
 */

#include "HnSRTree/HnAbort.h"
#include "HnSRTree/HnBinarySearch.hh"

void
HnBinarySearch::searchElements(HnBool leftMost,
			       HnBool *found_return, int *index_return)
{
    int count = getNumElements();
    int left, right, mid, sign;

    if ( count == 0 ) {
	*found_return = HnFALSE;
	*index_return = 0;
	return;
    }

    left = 0;
    right = count - 1;

    for ( ;; ) {
	if ( left > right ) {
	    HnAbort("impossible situation.");
	}

	if ( left == right ) {
	    if ( (sign = compareToElementAt(left)) < 0 ) {
		*found_return = HnFALSE;
		*index_return = left;
		return;
	    }
	    else if ( sign == 0 ) {
		*found_return = HnTRUE;
		if ( leftMost ) {
		    /* finds the leftmost position among the same elements */
		    *index_return = left;
		}
		else {
		    /* finds the rightmost position among the same elements */
		    *index_return = right + 1;
		}
		return;
	    }
	    else {
		*found_return = HnFALSE;
		*index_return = right + 1;
		return;
	    }
	}
	else {
	    if ( leftMost ) {
		mid = left + (right - left) / 2;
	    }
	    else {
		mid = left + ((right - left) + 1) / 2;
	    }

	    if ( (sign = compareToElementAt(mid)) < 0 ) {
		if ( left == mid ) {
		    *found_return = HnFALSE;
		    *index_return = left;
		    return;
		}
		else {
		    right = mid - 1;
		}
	    }
	    else if ( sign == 0 ) {
		if ( leftMost ) {
		    /* finds the leftmost position among the same elements */
		    right = mid;
		}
		else {
		    /* finds the rightmost position among the same elements */
		    left = mid;
		}
	    }
	    else {
		if ( mid == right ) {
		    *found_return = HnFALSE;
		    *index_return = right + 1;
		    return;
		}
		else {
		    left = mid + 1;
		}
	    }
	}
    }
}
