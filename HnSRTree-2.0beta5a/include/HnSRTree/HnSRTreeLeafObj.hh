/*
 * HnSRTreeLeafObj.hh
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
 * 03/21/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeLeafObj.hh,v 1.6 2002/09/13 03:54:18 katayama Exp $
 */

#ifndef _HnSRTreeLeafObj_hh
#define _HnSRTreeLeafObj_hh

#include "HnSRTree/HnObject.hh"
#include "HnSRTree/HnString.hh"
/*{
#include "HnSRTree/HnDataItem.hh"
#include "HnSRTree/HnPoint.hh"
#include "HnSRTree/HnSphere.hh"
#include "HnSRTree/HnRect.hh"
#include "HnSRTree/HnSRTreeInfo.hh"
#include "HnSRTree/HnSRTreeBlock.hh"
#include "HnSRTree/HnSRTreeCluster.hh"
#include "HnSRTree/HnSRTreeNode.hh"
#ifndef _MSC_VER
#include <unistd.h>
#endif
}*/

class HnSRTreeLeafObj: public HnObject {
private:
    HnSRTreeInfo info;

    int maxCount;
    long offset;

    HnPointVector points;
    HnDataItemVector dataItems;

private:
    void initialize(const HnSRTreeInfo &info, long offset);

    /* entry */
    static void writeEntryTo(const HnSRTreeInfo &info,
			     const HnPoint &point, const HnDataItem &dataItem,
			     HnBlockStream &blockStream);
    static void readEntryFrom(const HnSRTreeInfo &info,
			      HnBlockStream &blockStream,
			      HnPoint *point_return,
			      HnDataItem *dataItem_return);
    static int getEntrySize(const HnSRTreeInfo &info);

    int getWeight(void) const;
    HnSphere getBoundingSphere(const HnPoint &center) const;
    HnRect getBoundingRect(void) const;

public:
    HnSRTreeLeafObj(const HnSRTreeInfo &info, long offset);
    HnSRTreeLeafObj(const HnSRTreeInfo &info, const HnSRTreeBlock &block);

    int getDimension(void) const {
	return info.getDimension();
    }
    int getDataItemSize(void) const {
	return info.getDataItemSize();
    }
    int getBlockSize(void) const {
	return info.getBlockSize();
    }
    long getOffset(void) const {
	return offset;
    }
    int getCount(void) const {
	return points.size();
    }
    int getContentSize(void) const;
    HnBool isFull(void) const {
	return getCount() == maxCount;
    }

    void addElement(const HnPoint &point, const HnDataItem &dataItem) {
#ifdef DEBUGGING
	if ( getCount() == maxCount ) {
	    HnAbort("no more entry can be added.");
	}
	if ( point.getDimension() != info.getDimension() ) {
	    HnAbort("mismatch in dimensions.");
	}
	if ( dataItem.length() > info.getDataItemSize() ) {
	    HnAbort("data item size (%d) exceeds the limit (%d).",
		    dataItem.length(), info.getDataItemSize());
	}
#endif
	points.addElement(point);
	dataItems.addElement(dataItem);
    }

    void removeElementAt(int index) {
	points.removeElementAt(index);
	dataItems.removeElementAt(index);
    }

    HnPoint getCentroid(void) const;
    HnSRTreeCluster getCluster(void) const;

    HnPoint &getPointAt(int index) const {
	return points[index];
    }
    HnDataItem &getDataItemAt(int index) const {
	return dataItems[index];
    }

    HnSRTreeBlock toBlock(void) const;

    HnString toString(void) const;

    static int getMaxCount(const HnSRTreeInfo &info);
};

#endif /* _HnSRTreeLeafObj_hh */
