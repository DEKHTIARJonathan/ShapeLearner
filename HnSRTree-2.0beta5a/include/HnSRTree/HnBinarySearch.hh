/*
 * HnBinarySearch.hh
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
 * 02/12/99 katayama@rd.nacsis.ac.jp
 * $Id: HnBinarySearch.hh,v 1.2 2000/06/10 11:18:17 katayama Exp $
 */

#ifndef _HnBinarySearch_hh
#define _HnBinarySearch_hh

#include "HnSRTree/HnBool.h"

class HnBinarySearch {
protected:
    virtual int getNumElements(void) = 0;
    virtual int compareToElementAt(int i) = 0;

    /*
     * NOTE:
     *	    The number returned in the argument ``index_return'' is the
     *	    position of the compared element. The elements can be kept
     *	    sorted when the compared element is inserted at that position.
     *
     *	    When the argument ``leftMost'' is true, the leftmost
     *	    position will be returned in the argument ``index_return''.
     *	    The position indicates the leftmost element among such
     *	    elements that are equal to the compared element.
     *
     *	    When the argument ``leftMost'' is false, the rightmost
     *	    position will be returned. The position indicates the
     *	    element next to the rightmost element among those equal to
     *	    the compared element.
     */
    void searchElements(HnBool *found_return, int *index_return) {
	searchElements(HnTRUE, found_return, index_return);
    }
    void searchElements(HnBool leftMost,
			HnBool *found_return, int *index_return);

public:
    HnBinarySearch(void) {}
    virtual ~HnBinarySearch(void) {}
};

#endif /* _HnBinarySearch_hh */
