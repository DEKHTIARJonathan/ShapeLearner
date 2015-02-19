/*
 * HnSRTreeFileObj.hh
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
 * 03/20/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeFileObj.hh,v 1.15 2002/09/13 17:21:25 katayama Exp $
 */

#ifndef _HnSRTreeFileObj_hh
#define _HnSRTreeFileObj_hh

#include "HnSRTree/HnObject.hh"
#include "HnSRTree/HnStatistics.hh"
#include "HnSRTree/HnBlockFile.hh"
#include "HnSRTree/HnSRTreeInfo.hh"
#include "HnSRTree/HnSRTreeBlock.hh"
#include "HnSRTree/HnSRTreeNode.hh"
#include "HnSRTree/HnSRTreeLeaf.hh"
#include "HnSRTree/HnSRTreeStack.hh"
#include "HnSRTree/HnSRTreeReinsert.hh"
#include "HnSRTree/HnSRTreeNeighbor.hh"
#include "HnSRTree/HnFTlongArray.hh"

/*{
#include "HnSRTree/HnPoint.hh"
#include "HnSRTree/HnDataItem.hh"
#include "HnSRTree/HnProperties.hh"
#include "HnSRTree/HnSphere.hh"
#include "HnSRTree/HnRect.hh"
#include "HnSRTree/HnSRTreeProto.h"
#include "HnSRTree/HnSRTreeProfileSt.h"
}*/

class HnSRTreeQueryRegion {
public:
    virtual HnBool overlaps(const HnSRTreeCluster &cluster) const = 0;
    virtual HnBool includes(const HnPoint &point) const = 0;
    virtual ~HnSRTreeQueryRegion(void) {};
};

class HnSRTreeFileObj: public HnObject {
private:
    /* split flag */
    enum SplitFlag { LEFT, RIGHT, SPLIT_NONE };

    /* reinsert flag */
    enum ReinsertFlag { REINSERT, STAY, REINSERT_NONE };

    struct REntry {
	HnSRTreeBlock::Type type;
	HnSRTreeNode node;
	HnSRTreeLeaf leaf;
	int level;

	REntry() {
	    type = HnSRTreeBlock::FREE;
	    node = HnSRTreeNode::null;
	    leaf = HnSRTreeLeaf::null;
	    level = -1;
	}
	REntry(const REntry &entry) {
	    type = entry.type;
	    node = entry.node;
	    leaf = entry.leaf;
	    level = entry.level;
	}
	REntry &operator=(const REntry &entry) {
	    type = entry.type;
	    node = entry.node;
	    leaf = entry.leaf;
	    level = entry.level;
	    return *this;
	}
    };

private:
    HnBlockFile blockFile;

    HnSRTreeInfo info;

    struct context_s {
	HnSRTreeStack stack;
	HnSRTreeQueryRegion *queryRegion;
    } context;

    HnSRTreeReinsertVector reinsertList;
    HnFTlongVector reinsertedBlocks;

    HnSRTreeProfileSt *profile;

    /* initialize */
    void initialize(void) {
	context.queryRegion = NULL;
	profile = HnSRTreeProfileSt_allocate();
    }
    void dispose(void) {
	HnSRTreeProfileSt_free(profile);
    }

    /* properties */
    HnProperties getOverriddenProperties(const HnProperties &properties) const;
    void parseProperties(const HnProperties &properties,
			 int dimension,
			 int *blockSize_return,
			 int *splitFactor_return,
			 int *reinsertFactor_return,
			 HnSRTreeInfo::StaticAlgorithm *staticAlgorithm_return,
			 HnSRTreeInfo::NonLeafFloatType *
			 nonLeafFloatType_return,
			 HnSRTreeInfo::NeighborAlgorithm *
			 neighborAlgorithm_return);

    static HnSRTreeInfo::StaticAlgorithm
        stringToStaticAlgorithm(const HnString &value);
    static HnString
        staticAlgorithmToString(HnSRTreeInfo::StaticAlgorithm value);

    static HnSRTreeInfo::NonLeafFloatType
        stringToNonLeafFloatType(const HnString &value);
    static HnString
        nonLeafFloatTypeToString(HnSRTreeInfo::NonLeafFloatType value);

    static HnSRTreeInfo::NeighborAlgorithm
        stringToNeighborAlgorithm(const HnString &value);
    static HnString
        neighborAlgorithmToString(HnSRTreeInfo::NeighborAlgorithm value);

    /* super block */
    void writeSuperBlock(void);
    void readSuperBlock(void);

    /* block */
    HnSRTreeBlock readBlock(long offset);
    void writeBlock(const HnSRTreeBlock &block);
    long allocateBlock(void);
    void releaseBlock(long offset);

    /* leaf */
    void writeLeaf(const HnSRTreeLeaf &leaf);
    HnSRTreeLeaf allocateLeaf(void);

    /* node */
    void writeNode(const HnSRTreeNode &node);
    HnSRTreeNode allocateNode(void);

    /* store */
    void insertPoint(const HnPoint &point, const HnDataItem &dataItem);
    void insertBlock(long offset, int level);
    HnSRTreeStack chooseLeaf(const HnPoint &point);
    int chooseSubtree(const HnSRTreeNode &node, const HnPoint &point);
    HnSRTreeStack chooseNode(const HnPoint &centroid, int level);
    void updateCluster(HnSRTreeStack stack);

    void reinsertLeaf(HnSRTreeStack &stack,
		      const HnPoint &newPoint, const HnDataItem &newDataItem);
    void selectPoints(const HnPointArray &points, ReinsertFlag **flags_return);
    void reinsertNode(HnSRTreeStack &stack,
		      const HnSRTreeCluster &newCluster, long newOffset);
    void selectClusters(const HnSRTreeClusterArray &clusters,
			ReinsertFlag **flags_return);

    void splitLeaf(HnSRTreeStack &stack,
		   const HnPoint &newPoint, const HnDataItem &newDataItem);
    void splitNode(HnSRTreeStack &stack,
		   const HnSRTreeCluster &newCluster, long newOffset);

    void updateNode(HnSRTreeStack &stack,
		    const HnSRTreeCluster &leftCluster, long leftOffset,
		    const HnSRTreeCluster &rightCluster, long rightOffset);
    void extendTree(const HnSRTreeCluster &leftCluster, long leftOffset,
		    const HnSRTreeCluster &rightCluster, long rightOffset);

    /* split */
    void splitPoints(const HnPointArray &points, SplitFlag **flags_return);
    void splitClusters(const HnSRTreeClusterArray &clusters,
		       SplitFlag **flags_return);

    /* construction */
    void searchPoint(const HnPointVector &points, int offset, int count,
		     double keyCoord, int axis, int order,
		     HnBool *found_return, int *index_return);
    void searchCluster(const HnSRTreeClusterVector &clusters,
		       int offset, int count,
		       double keyCoord, int axis, int order,
		       HnBool *found_return, int *index_return);
    void selectPoint(HnPointVector &points, HnDataItemVector &dataItems,
		     int offset, int count, int axis, int index);
    void selectCluster(HnSRTreeClusterVector &clusters,
		       HnFTlongVector &offsets,
		       int offset, int count, int axis, int index);
    void sortPoints(HnPointVector &points, HnDataItemVector &dataItems,
		    int offset, int count, int sortDimension);
    void sortClusters(HnSRTreeClusterVector &clusters, HnFTlongVector &offsets,
		      int offset, int count, int sortDimension);
    int getMaxVarianceAxis(HnPointVector &points, int offset, int count);
    int getMaxVarianceAxis(HnSRTreeClusterVector &clusters,
			   int offset, int count);

    /* construction (VAM) */
    void constructTree_VAM(HnPointVector &points, HnDataItemVector &dataItems);
    REntry CreateVAMSRTree(HnPointVector &points, HnDataItemVector &dataItems);
    int BuildVAMSRTree(HnPointVector &points, HnDataItemVector &dataItems,
		       int start, int end, REntry *entries, int level);
    REntry CreateRNodeBucket(HnPointVector &points,
			     HnDataItemVector &dataItems,
			     int start, int end);
    REntry CreateRNodeFromEntries(REntry *entries, int count);

    int SplitDataset(HnPointVector &points, HnDataItemVector &dataItems,
		     int start, int end, int size, int cscap, int gscap);
    int SplitDataset_VAM(HnPointVector &points, HnDataItemVector &dataItems,
			 int start, int end, int size, int cscap, int gscap);

    void SelectOnDimension_VAM(HnPointVector &points,
			       HnDataItemVector &dataItems,
			       int split_dim, int start, int end, int lo_size);

    int FindMaxVarianceDimension(HnPointVector &points, int start, int end);


    /* remove */
    HnSRTreeStack searchPoint(const HnPoint &point,
			      const HnDataItem &dataItem);

    /* sequential access */
    void getFirst(HnSRTreeQueryRegion *queryRegion,
		  HnPoint *point_return, HnDataItem *dataItem_return);

    /* neighbor search */
    double getMinDistance(const HnPoint &point,
			  const HnSRTreeCluster &cluster);
    double getMaxDistance(const HnPoint &point,
			  const HnSRTreeCluster &cluster);

    /* neighbor search (DepthFirst) */
    void getNeighborsByDepthFirst(const HnPoint &queryPoint, int maxCount,
				  HnPointVector *points_return,
				  HnDataItemVector *dataItems_return);
    void chooseNeighbors(long offset,
			 const HnPoint &queryPoint, int maxCount,
			 HnSRTreeNeighborVector &neighbors);
    void chooseNeighborsInNode(const HnSRTreeBlock &block,
			       const HnPoint &queryPoint, int maxCount,
			       HnSRTreeNeighborVector &neighbors);
    void chooseNeighborsInLeaf(const HnSRTreeBlock &block,
			       const HnPoint &queryPoint, int maxCount,
			       HnSRTreeNeighborVector &neighbors);

    /* neighbor search (BreadthFirst) */
    void getNeighborsByBreadthFirst(const HnPoint &queryPoint, int maxCount,
				    HnPointVector *points_return,
				    HnDataItemVector *dataItems_return);
    void insertNeighbors(HnSRTreeNeighborVector &neighbors,
			 int &numPointsInVector,
			 const HnPoint &queryPoint, int maxCount,
			 long offset);
    void insertNeighbor(HnSRTreeNeighborVector &neighbors,
			const HnSRTreeNeighbor &newNeighbor);

    /* colored neighbor search */
    double getDistance(const HnPointVector &queryPoints,
		       const HnPoint &point);
    double getMinDistance(const HnPointVector &queryPoints,
			  const HnSRTreeCluster &cluster);
    double getMaxDistance(const HnPointVector &queryPoints,
			  const HnSRTreeCluster &cluster);
    void searchDistanceVector(const HnSRTreeNeighborVector &distanceVector,
			      const HnSRTreeNeighbor &neighbor,
			      HnSRTreeCompColorsFunc *compColorsFunc,
			      HnBool *found_return, int *index_return);
    void searchColorVector(const HnSRTreeNeighborVector &colorVector,
			   const HnSRTreeNeighbor &neighbor,
			   HnSRTreeCompColorsFunc *compColorsFunc,
			   HnBool *found_return, int *index_return);

    /* colored neighbor search (DepthFirst) */
    void getColoredNeighborsByDepthFirst(const HnPointVector &queryPoints,
					 int maxCount,
					 HnPointVector *points_return,
					 HnDataItemVector *dataItems_return,
					 HnSRTreeCompColorsFunc *
					 compColorsFunc);
    void chooseColoredNeighbors(long offset,
				const HnPointVector &queryPoints, int maxCount,
				HnSRTreeNeighborVector &distanceVector,
				HnSRTreeNeighborVector &colorVector,
				HnSRTreeCompColorsFunc *compColorsFunc);
    void chooseColoredNeighborsInNode(const HnSRTreeBlock &block,
				      const HnPointVector &queryPoints,
				      int maxCount,
				      HnSRTreeNeighborVector &distanceVector,
				      HnSRTreeNeighborVector &colorVector,
				      HnSRTreeCompColorsFunc *compColorsFunc);
    void chooseColoredNeighborsInLeaf(const HnSRTreeBlock &block,
				      const HnPointVector &queryPoints,
				      int maxCount,
				      HnSRTreeNeighborVector &distanceVector,
				      HnSRTreeNeighborVector &colorVector,
				      HnSRTreeCompColorsFunc *
				      compColorsFunc);

    /* colored neighbor search (BreadthFirst) */
    void getColoredNeighborsByBreadthFirst(const HnPointVector &queryPoints,
					   int maxCount,
					   HnPointVector *points_return,
					   HnDataItemVector *dataItems_return,
					   HnSRTreeCompColorsFunc *
					   compColorsFunc);
    void insertColoredNeighbors(HnSRTreeNeighborVector &distanceVector,
				HnSRTreeNeighborVector &colorVector,
				int &numPointsInVector,
				const HnPointVector &queryPoints, int maxCount,
				long offset,
				HnSRTreeCompColorsFunc *compColorsFunc);
    void insertColoredNeighbor(HnSRTreeNeighborVector &distanceVector,
			       HnSRTreeNeighborVector &colorVector,
			       int &numPointsInVector,
			       const HnSRTreeNeighbor &newNeighbor,
			       HnSRTreeCompColorsFunc *compColorsFunc);

    /* check */
    int checkBlock(long offset, const HnSRTreeNode &parent, int childIndex);
    HnPointVector checkInclusion(long offset);
    void describeExclusion(long offset, const HnPoint &center);

    /* print */
    void printNode(const HnSRTreeNode &node);
    void printLeaf(const HnSRTreeLeaf &leaf);
    void measureClusters(long offset, int level,
			 HnStatistics nodeSphereRadius,
			 HnStatistics nodeSphereVolume,
			 HnStatistics nodeRectDiagonal,
			 HnStatistics nodeRectVolume,
			 HnStatistics leafSphereRadius,
			 HnStatistics leafSphereVolume,
			 HnStatistics leafRectDiagonal,
			 HnStatistics leafRectVolume);

public:
    HnSRTreeFileObj(const char *path, int dimension, int dataItemSize,
		    const HnProperties &properites = HnProperties::null);
    HnSRTreeFileObj(const char *path,
		    int dimension, int dataItemSize, 
		    HnPointVector &points, HnDataItemVector &dataItems,
		    const HnProperties &properties = HnProperties::null);
    HnSRTreeFileObj(const char *path, const char *mode);
    ~HnSRTreeFileObj(void);

    int getDimension(void) const {
	return info.getDimension();
    }
    int getDataItemSize(void) const {
	return info.getDataItemSize();
    }
    int getHeight(void) const {
	return info.getHeight();
    }
    int getBlockSize(void) const {
	return info.getBlockSize();
    }

    void store(const HnPoint &point, const HnDataItem &dataItem);
    void remove(const HnPoint &point, const HnDataItem &dataItem);
    void close(void);

    void getFirst(HnPoint *point_return, HnDataItem *dataItem_return);
    void getFirst(const HnRect &rect,
		  HnPoint *point_return, HnDataItem *dataItem_return);
    void getFirst(const HnSphere &sphere,
		  HnPoint *point_return, HnDataItem *dataItem_return);
    void getNext(HnPoint *point_return, HnDataItem *dataItem_return);
    void getInRect(const HnRect &rect,
		   HnPointVector *points_return,
		   HnDataItemVector *dataItems_return);
    void getInSphere(const HnSphere &sphere,
		     HnPointVector *points_return,
		     HnDataItemVector *dataItems_return);

    void getNeighbors(const HnPoint &point, int maxCount,
		      HnPointVector *points_return,
		      HnDataItemVector *dataItems_return);
    void getColoredNeighbors(const HnPointVector &queryPoints, int maxCount,
			     HnPointVector *points_return,
			     HnDataItemVector *dataItems_return);
    void getColoredNeighbors(const HnPointVector &queryPoints, int maxCount,
			     HnPointVector *points_return,
			     HnDataItemVector *dataItems_return,
			     HnSRTreeCompColorsFunc *compColorsFunc);

    void check(void);
    void print(HnBool verbose);

    void resetProfile(void) {
	HnSRTreeProfileSt_reset(profile);
    }
    void copyProfileInto(HnSRTreeProfileSt *profile) {
	*profile = *this->profile;
    }

    HnBool dumpToFile(const char *fileName);
    HnBool dumpToFileStream(FILE *fp);

    /* properties */
    HnProperties getDefaultProperties(void) const;
    HnProperties getProperties(void) const;
    void setProperties(const HnProperties &properties);

private:
    static HnBool debug;
public:
    static void setDebug(HnBool debug) {
	HnSRTreeFileObj::debug = debug;
    }
};

#endif /* _HnSRTreeFileObj_hh */
