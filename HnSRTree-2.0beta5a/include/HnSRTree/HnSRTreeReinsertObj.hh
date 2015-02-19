/*
 * HnSRTreeReinsertObj.hh
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
 * 08/23/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeReinsertObj.hh,v 1.6 2002/09/13 05:08:48 katayama Exp $
 */

#ifndef _HnSRTreeReinsertObj_hh
#define _HnSRTreeReinsertObj_hh

#include "HnSRTree/HnObject.hh"

/*{
#ifndef _MSC_VER
#include <unistd.h>
#include <sys/types.h>
#endif
#include "HnSRTree/HnPoint.hh"
#include "HnSRTree/HnDataItem.hh"
}*/

class HnSRTreeReinsertObj: public HnObject {
private:
    HnSRTreeReinsert::Type type;

    /* for points */
    HnPoint point;
    HnDataItem dataItem;

    /* for blocks */
    long offset;
    int level;
public:
/*{
    enum Type { POINT, BLOCK };
}*/
    HnSRTreeReinsertObj(const HnPoint &point, const HnDataItem &dataItem) {
	this->type = HnSRTreeReinsert::POINT;
	this->point = point;
	this->dataItem = dataItem;
    }
    HnSRTreeReinsertObj(long offset, int level) {
	this->type = HnSRTreeReinsert::BLOCK;
	this->offset = offset;
	this->level = level;
    }
    HnSRTreeReinsert::Type getType(void) const {
	return type;
    }
    const HnPoint &getPoint(void) const {
#ifdef DEBUGGING
	if ( type != HnSRTreeReinsert::POINT ) {
	    HnAbort("not a point.");
	}
#endif
	return point;
    }
    const HnDataItem &getDataItem(void) const {
#ifdef DEBUGGING
	if ( type != HnSRTreeReinsert::POINT ) {
	    HnAbort("not a point.");
	}
#endif
	return dataItem;
    }
    long getOffset(void) const {
#ifdef DEBUGGING
	if ( type != HnSRTreeReinsert::BLOCK ) {
	    HnAbort("not a block.");
	}
#endif
	return offset;
    }
    int getLevel(void) const {
#ifdef DEBUGGING
	if ( type != HnSRTreeReinsert::BLOCK ) {
	    HnAbort("not a block.");
	}
#endif
	return level;
    }
};

#endif /* _HnSRTreeReinsertObj_hh */
