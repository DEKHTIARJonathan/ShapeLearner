/*
 * HnQuickSort.hh
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
 * 02/02/99 katayama@rd.nacsis.ac.jp
 * $Id: HnQuickSort.hh,v 1.2 2000/06/10 11:18:32 katayama Exp $
 */

#ifndef _HnQuickSort_hh
#define _HnQuickSort_hh

#include "HnSRTree/HnBool.h"

class HnQuickSort {
private:
    void sortElements(int offset, int count);

protected:
    HnBool debug;
    virtual int getNumElements(void) = 0;
    virtual int compareElementsAt(int i, int j) = 0;
    virtual void swapElementsAt(int i, int j) = 0;

    void sortElements(void);

public:
    HnQuickSort(void) { debug = HnFALSE; }
    virtual ~HnQuickSort(void) {}
};

#endif /* _HnQuickSort_hh */
