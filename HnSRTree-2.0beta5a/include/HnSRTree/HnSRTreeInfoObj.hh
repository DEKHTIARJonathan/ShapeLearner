/*
 * HnSRTreeInfoObj.hh
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
 * 02/05/99 Norio KATAYAMA
 * $Id: HnSRTreeInfoObj.hh,v 1.6 2000/12/15 12:41:42 katayama Exp $
 */

#ifndef _HnSRTreeInfoObj_hh
#define _HnSRTreeInfoObj_hh

#include "HnSRTree/HnObject.hh"

/*{
#include "HnSRTree/HnBlockStream.hh"
}*/

class HnSRTreeInfoObj: public HnObject {
private:
    int dimension;
    int dataItemSize;

    long fileSize;
    long freeOffset;
    long rootOffset;
    int height;

    int blockSize;
    int splitFactor;
    int reinsertFactor;
    HnSRTreeInfo::StaticAlgorithm staticAlgorithm;
    HnSRTreeInfo::NonLeafFloatType nonLeafFloatType;

    /* volatile attributes */
    HnSRTreeInfo::NeighborAlgorithm neighborAlgorithm;

private:
    HnSRTreeInfoObj(void) {}

public:
/*{
    enum StaticAlgorithm {
	NONSTATIC       = 0,
        VAM_ORIGINAL    = 1,
	VAM_CORRECTED   = 2
    };
    enum NonLeafFloatType {
        NON_LEAF_SINGLE = 0,
	NON_LEAF_DOUBLE = 1
    };
    enum NeighborAlgorithm {
        DEPTH_FIRST     = 0,
        BREADTH_FIRST   = 1
    };
}*/
    HnSRTreeInfoObj(HnBlockStream blockStream);
    HnSRTreeInfoObj(int dimension, int dataItemSize,
		    long fileSize, long freeOffset, long rootOffset,
		    int height,
		    int blockSize, int splitFactor, int reinsertFactor,
		    HnSRTreeInfo::StaticAlgorithm staticAlgorithm,
		    HnSRTreeInfo::NonLeafFloatType nonLeafFloatType,
		    HnSRTreeInfo::NeighborAlgorithm neighborAlgorithm) {
	this->dimension = dimension;
	this->dataItemSize = dataItemSize;

	this->fileSize = fileSize;
	this->freeOffset = freeOffset;
	this->rootOffset = rootOffset;
	this->height = height;

	this->blockSize = blockSize;
	this->splitFactor = splitFactor;
	this->reinsertFactor = reinsertFactor;
	this->staticAlgorithm = staticAlgorithm;
	this->nonLeafFloatType = nonLeafFloatType;

	this->neighborAlgorithm = neighborAlgorithm;
    }

    /* get attributes */
    int getDimension(void) const {
	return dimension;
    }
    int getDataItemSize(void) const {
	return dataItemSize;
    }

    long getFileSize(void) const {
	return fileSize;
    }
    long getFreeOffset(void) const {
	return freeOffset;
    }
    long getRootOffset(void) const {
	return rootOffset;
    }
    int getHeight(void) const {
	return height;
    }

    int getBlockSize(void) const {
	return blockSize;
    }
    int getSplitFactor(void) const {
	return splitFactor;
    }
    int getReinsertFactor(void) const {
	return reinsertFactor;
    }
    HnSRTreeInfo::StaticAlgorithm getStaticAlgorithm(void) const {
	return staticAlgorithm;
    }
    HnSRTreeInfo::NonLeafFloatType getNonLeafFloatType(void) const {
	return nonLeafFloatType;
    }

    HnSRTreeInfo::NeighborAlgorithm getNeighborAlgorithm(void) const {
	return neighborAlgorithm;
    }

    /* set attributes */
    void setFileSize(long fileSize) {
	this->fileSize = fileSize;
    }
    void setFreeOffset(long freeOffset) {
	this->freeOffset = freeOffset;
    }
    void setRootOffset(long rootOffset) {
	this->rootOffset = rootOffset;
    }
    void setHeight(int height) {
	this->height = height;
    }

    void setNeighborAlgorithm(HnSRTreeInfo::NeighborAlgorithm
			      neighborAlgorithm) {
	this->neighborAlgorithm = neighborAlgorithm;
    }

    void writeTo(HnBlockStream blockStream) const;
    HnBlockStream toBlockStream(void) const;
    static int getBlockStreamSize(void);
};

#endif /* _HnSRTreeInfoObj_hh */
