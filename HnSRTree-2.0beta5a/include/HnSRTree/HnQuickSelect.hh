/*
 * HnQuickSelect.hh
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
 * $Id: HnQuickSelect.hh,v 1.2 2000/06/10 11:18:31 katayama Exp $
 */

#ifndef _HnQuickSelect_hh
#define _HnQuickSelect_hh

#include "HnSRTree/HnBool.h"

class HnQuickSelect {
protected:
    HnBool debug;
    virtual int getNumElements(void) = 0;
    virtual int compareElementsAt(int i, int j) = 0;
    virtual void swapElementsAt(int i, int j) = 0;

    void selectElement(int index);

public:
    HnQuickSelect(void) { debug = HnFALSE; }
    virtual ~HnQuickSelect(void) {}
};

#endif /* _HnQuickSelect_hh */
