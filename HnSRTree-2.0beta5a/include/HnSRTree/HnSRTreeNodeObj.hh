/*
 * HnSRTreeNodeObj.hh
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
 * $Id: HnSRTreeNodeObj.hh,v 1.6 2002/09/13 05:07:37 katayama Exp $
 */

#ifndef _HnSRTreeNodeObj_hh
#define _HnSRTreeNodeObj_hh

#include "HnSRTree/HnObject.hh"
/*{
#include "HnSRTree/HnFTlongArray.hh"
#include "HnSRTree/HnSRTreeInfo.hh"
#include "HnSRTree/HnSRTreeBlock.hh"
#include "HnSRTree/HnSRTreeCluster.hh"
#ifndef _MSC_VER
#include <unistd.h>
#endif
}*/

class HnSRTreeNodeObj: public HnObject {
private:
    HnSRTreeInfo info;

    int maxCount;
    long offset;

    HnSRTreeClusterVector clusters;
    HnFTlongVector offsets;

private:
    void initialize(const HnSRTreeInfo &info, long offset);

    static int getHeaderSize(const HnSRTreeInfo &info);

    /* entry */
    static void writeEntryTo(const HnSRTreeInfo &info,
			     const HnSRTreeCluster &cluster, long offset,
			     HnBlockStream &blockStream);
    static void readEntryFrom(const HnSRTreeInfo &info,
			      HnBlockStream &blockStream,
			      HnSRTreeCluster *cluster_return,
			      long *offset_return);
    static int getEntrySize(const HnSRTreeInfo &info);

    /* cluster */
    static void writeClusterTo(const HnSRTreeInfo &info,
			       const HnSRTreeCluster &cluster,
			       HnBlockStream &blockStream);
    static HnSRTreeCluster readClusterFrom(const HnSRTreeInfo &info,
					   HnBlockStream &blockStream);
    static int getClusterSize(const HnSRTreeInfo &info);

    int getWeight(void) const;
    HnSphere getBoundingSphere(const HnPoint &center) const;
    HnRect getBoundingRect(void) const;

public:
    HnSRTreeNodeObj(const HnSRTreeInfo &info, long offset);
    HnSRTreeNodeObj(const HnSRTreeInfo &info, const HnSRTreeBlock &block);

    int getDimension(void) const {
	return info.getDimension();
    }
    int getBlockSize(void) const {
	return info.getBlockSize();
    }
    long getOffset(void) const {
	return offset;
    }
    int getCount(void) const {
	return clusters.size();
    }
    int getContentSize(void) const;
    HnBool isFull(void) const {
	return getCount() == maxCount;
    }

    void addElement(const HnSRTreeCluster &cluster, long offset) {
#ifdef DEBUGGING
	if ( getCount() == maxCount ) {
	    HnAbort("no more entry can be added.");
	}
	if ( cluster.getDimension() != info.getDimension() ) {
	    HnAbort("mismatch in dimensions.");
	}
#endif
	clusters.addElement(cluster);
	offsets.addElement(offset);
    }
    void removeElementAt(int index) {
	clusters.removeElementAt(index);
	offsets.removeElementAt(index);
    }
    void setElementAt(const HnSRTreeCluster &cluster, long offset,
		      int index) {
	clusters.setElementAt(cluster, index);
	offsets.setElementAt(offset, index);
    }
    void setClusterAt(const HnSRTreeCluster &cluster, int index) {
	clusters.setElementAt(cluster, index);
    }

    HnPoint getCentroid(void) const;
    HnSRTreeCluster getCluster(void) const;

    HnSRTreeCluster &getClusterAt(int index) const {
	return clusters[index];
    }
    long getOffsetAt(int index) const {
	return offsets[index];
    }

    HnSRTreeBlock toBlock(void) const;

    HnString toString(void) const;

    static int getMaxCount(const HnSRTreeInfo &info);
};

#endif /* _HnSRTreeNodeObj_hh */
