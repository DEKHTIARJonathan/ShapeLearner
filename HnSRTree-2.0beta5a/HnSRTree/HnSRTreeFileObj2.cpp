/*
 * HnSRTreeFileObj2.cc (static construction methods)
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
 * 03/20/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeFileObj2.cc,v 1.4 2002/09/13 03:50:54 katayama Exp $
 */

#include <math.h>
#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#include <sys/types.h>
#endif
#include "HnSRTree/HnSRTreeFile.hh"
#include "HnSRTree/HnSRTreeFileObj.hh"
#include "HnSRTree/HnBinarySearch.hh"
#include "HnSRTree/HnQuickSelect.hh"
#include "HnSRTree/HnQuickSort.hh"
#include "HnSRTree/HnStatistics.hh"

/*
 * Construction (VAM)
 */

void
HnSRTreeFileObj::constructTree_VAM(HnPointVector &points,
				   HnDataItemVector &dataItems)
{
    REntry entry;

    entry = CreateVAMSRTree(points, dataItems);

    if ( entry.type == HnSRTreeBlock::NODE ) {
	info.setRootOffset(entry.node.getOffset());
	info.setHeight(entry.level + 1);

	writeSuperBlock();
    }
    else if ( entry.type == HnSRTreeBlock::LEAF ) {
	info.setRootOffset(entry.leaf.getOffset());
	info.setHeight(entry.level + 1);

	writeSuperBlock();
    }
    else {
	HnAbort("unexpected block type (%d).", entry.type);
    }
}

HnSRTreeFileObj::REntry
HnSRTreeFileObj::CreateVAMSRTree(HnPointVector &points,
				 HnDataItemVector &dataItems)
{
    REntry out_entry, *entries;
    int INTERNAL_FANOUT, BUCKET_SIZE, MAX_TREE_LEVELS;
    int start, end;

    INTERNAL_FANOUT = HnSRTreeNode::getMaxCount(info);
    BUCKET_SIZE = HnSRTreeLeaf::getMaxCount(info);
    start = 0;
    end = points.size();

    if ( points.size() <= BUCKET_SIZE ) {
	MAX_TREE_LEVELS = 1;
    }
    else {
	MAX_TREE_LEVELS = (int)ceil(log((double)points.size()/BUCKET_SIZE)/
				    log((double)INTERNAL_FANOUT));
    }

    entries = new REntry[INTERNAL_FANOUT * MAX_TREE_LEVELS];

    if ( debug ) {
	fprintf(stderr, "The length of entries[] = %d\n",
		INTERNAL_FANOUT * MAX_TREE_LEVELS);
    }

    BuildVAMSRTree(points, dataItems, start, end, entries, -1);
    out_entry = entries[0];

    delete[] entries;

    return out_entry;
}

int
HnSRTreeFileObj::BuildVAMSRTree(HnPointVector &points,
				HnDataItemVector &dataItems,
				int start, int end,
				REntry *entries, int level)
{
    int size, child_level, cscap, lo_size, lo_entries, hi_entries, out_entries;
    int BUCKET_SIZE = HnSRTreeLeaf::getMaxCount(info);
    int INTERNAL_FANOUT = HnSRTreeNode::getMaxCount(info);
    double LOG_INTERNAL_FANOUT = log((double)INTERNAL_FANOUT);
    int gscap;	/* grandchild subtree capacity */

    if ( debug ) {
	fprintf(stderr, "BuildVAMSRTree: starting ... \n");
	fprintf(stderr, "                BUCKET_SIZE=%d, INTERNAL_FANOUT=%d\n",
		BUCKET_SIZE, INTERNAL_FANOUT);
	fprintf(stderr, "                LOG_INTERNAL_FANOUT=%g\n",
		LOG_INTERNAL_FANOUT);
	fprintf(stderr, "                start=%d, end=%d, level=%d\n",
		start, end, level);
    }

    size = end - start;

    if ( level >= 0 ) {
	if ( size < BUCKET_SIZE / 2 ) {
	    HnAbort("BulidVAMSRTree: invalid size (%d).", size);
	}
    }

    if ( size <= BUCKET_SIZE ) {
	entries[0] = CreateRNodeBucket(points, dataItems, start, end);
	return 1;
    }
    if ( size <= 2 * BUCKET_SIZE ) {
	child_level = 0;
	cscap = 1;
	gscap = 0;
    }
    else {
	if ( info.getStaticAlgorithm() == HnSRTreeInfo::VAM_ORIGINAL ) {
	    child_level = (int)((log(double(size)/double(2*BUCKET_SIZE)))/
				double(LOG_INTERNAL_FANOUT));
	}
	else {
#if 0
	    child_level = (int)ceil(log(ceil((double)size/BUCKET_SIZE))/
				    LOG_INTERNAL_FANOUT) - 1;
#else
	    /* circumvent round-off error */
	    int numNodes = (int)ceil((double)size/BUCKET_SIZE);
	    int count = 1;
	    int exp = 0;
	    while ( count < numNodes ) {
		count = INTERNAL_FANOUT * count;
		exp ++;
	    }
	    child_level = exp - 1;
#endif
	}
	cscap = (int)(BUCKET_SIZE * pow((double)INTERNAL_FANOUT, child_level));
	gscap = cscap / INTERNAL_FANOUT;
    }

    if ( debug ) {
	fprintf(stderr, "BuildVAMSRTree: "
		"size=%d, child_level=%d, cscap=%d, gscap=%d\n",
		size, child_level, cscap, gscap);
    }

    lo_size = SplitDataset(points, dataItems, start, end, size, cscap, gscap);
    lo_entries = BuildVAMSRTree(points, dataItems,
				start, start + lo_size,
				entries, child_level);
    hi_entries = BuildVAMSRTree(points, dataItems,
				start + lo_size, end,
				entries + lo_entries, child_level);
    out_entries = lo_entries + hi_entries;

    if ( debug ) {
	fprintf(stderr,
		"BuildVAMSRTree: "
		"lo_size=%d, lo_entries=%d, hi_entries=%d, out_entries=%d\n",
		lo_size, lo_entries, hi_entries, out_entries);
    }

    if ( level == -1 || child_level < level ) {
	if ( debug ) {
	    /*
	     * verify that the minimum number of nodes are used.
	     */
	    int i;

	    if ( end < points.size() ) {
		/* left side */
		for ( i=0; i<out_entries; i++ ) {
		    if ( child_level == 0 ) {
			/* leaf */
			if ( entries[i].leaf.getCount() < BUCKET_SIZE ) {
			    HnAbort("a leaf is not fully utilized.");
			}
		    }
		    else {
			/* node */
			if ( entries[i].node.getCount() < INTERNAL_FANOUT ) {
			    HnAbort("a node is not fully utilized.");
			}
		    }
		}
	    }
	    else {
		/* right side */
		for ( i=0; i<out_entries-2; i++ ) {
		    if ( child_level == 0 ) {
			/* leaf */
			if ( entries[i].leaf.getCount() < BUCKET_SIZE ) {
			    HnAbort("a leaf is not fully utilized.");
			}
		    }
		    else {
			/* node */
			if ( entries[i].node.getCount() < INTERNAL_FANOUT ) {
			    HnAbort("a node is not fully utilized.");
			}
		    }
		}
		for ( i=out_entries-2; i<out_entries; i++ ) {
		    if ( child_level == 0 ) {
			/* leaf */
			if ( entries[i].leaf.getCount() < BUCKET_SIZE/2 ) {
			    HnAbort("a leaf is not fully utilized.");
			}
		    }
		    else {
			/* node */
			if ( entries[i].node.getCount() < INTERNAL_FANOUT/2 ) {
			    HnAbort("a node is not fully utilized.");
			}
		    }
		}
	    }   
	}

	entries[0] = CreateRNodeFromEntries(entries, out_entries);
	out_entries = 1;
    }

    if ( debug ) {
	fprintf(stderr, "BuildVAMSRTree: returning ... \n");
    }

    return out_entries;
}

HnSRTreeFileObj::REntry
HnSRTreeFileObj::CreateRNodeBucket(HnPointVector &points,
				   HnDataItemVector &dataItems,
				   int start, int end)
{
    HnSRTreeLeaf leaf;
    int i;
    REntry entry;

    if ( debug ) {
	fprintf(stderr,
		"CreateRNodeBucket: allocating a leaf (start=%d, end=%d)\n",
		start, end);
    }

    leaf = allocateLeaf();

    for ( i=start; i<end; i++ ) {
	leaf.addElement(points[i], dataItems[i]);
    }

    writeLeaf(leaf);

    entry.type = HnSRTreeBlock::LEAF;
    entry.leaf = leaf;
    entry.node = HnSRTreeNode::null;
    entry.level = 0;

    return entry;
}

HnSRTreeFileObj::REntry
HnSRTreeFileObj::CreateRNodeFromEntries(REntry *entries, int count)
{
    HnSRTreeNode node;
    int i;
    REntry entry;

    if ( debug ) {
	fprintf(stderr,
		"CreateRNodeFromEntries: allocating a node (count=%d)\n",
		count);
    }

    node = allocateNode();

    /* confirm the balance of the tree */
    for ( i=1; i<count; i++ ) {
	if ( entries[i].level != entries[0].level ) {
	    int j;

	    for ( j=0; j<count; j++ ) {
		fprintf(stderr, "entries[%d].level=%d\n",
			j, entries[j].level);
	    }

	    HnAbort("subtree is not balanced.");
	}
    }

    for ( i=0; i<count; i++ ) {
	switch ( entries[i].type ) {
	case HnSRTreeBlock::NODE:
	    node.addElement(entries[i].node.getCluster(),
			    entries[i].node.getOffset());
	    break;
	case HnSRTreeBlock::LEAF:
	    node.addElement(entries[i].leaf.getCluster(),
			    entries[i].leaf.getOffset());
	    break;
	default:
	    HnAbort("unexpected block type (%d).", entries[i].type);
	}
    }

    writeNode(node);

    entry.type = HnSRTreeBlock::NODE;
    entry.leaf = HnSRTreeLeaf::null;
    entry.node = node;
    entry.level = entries[0].level + 1;

    return entry;
}

int
HnSRTreeFileObj::SplitDataset(HnPointVector &points,
			      HnDataItemVector &dataItems,
			      int start, int end, int size,
			      int cscap, int gscap)
{
    switch ( info.getStaticAlgorithm() ) {
    case HnSRTreeInfo::VAM_ORIGINAL:
    case HnSRTreeInfo::VAM_CORRECTED:
	return SplitDataset_VAM(points, dataItems,
				start, end, size, cscap, gscap);
    default:
	HnAbort("unexpected value for ``staticAlgorithm'' (%d).",
		info.getStaticAlgorithm());
    }
}

int
HnSRTreeFileObj::SplitDataset_VAM(HnPointVector &points,
				  HnDataItemVector &dataItems,
				  int start, int end, int size,
				  int cscap, int gscap)
{
    int lo_size, split_dim;

    if ( info.getStaticAlgorithm() == HnSRTreeInfo::VAM_ORIGINAL ) {
	lo_size = cscap * (size / (2 * cscap));
    }
    else {
	if ( gscap != 0 && size <= 2 * cscap ) {
	    /* split the dataset by the grandchild subtree capacity */
	    lo_size = gscap * (int)floor(ceil((double)size/gscap)/2);
	}
	else {
	    /* split the dataset by the child subtree capacity */
	    lo_size = cscap * (int)floor(ceil((double)size/cscap)/2);
	}
    }

    split_dim = FindMaxVarianceDimension(points, start, end);
    SelectOnDimension_VAM(points, dataItems, split_dim, start, end, lo_size);

    if ( debug ) {
	fprintf(stderr,
		"SplitDataset: size=%d, cscap=%d, gscap=%d, lo_size=%d.\n",
		size, cscap, gscap, lo_size);
    }

    return lo_size;
}

void
HnSRTreeFileObj::SelectOnDimension_VAM(HnPointVector &points,
				       HnDataItemVector &dataItems,
				       int split_dim,
				       int start, int end, int lo_size)
{
    if ( debug ) {
	fprintf(stderr, "SelectOnDimension: "
		"split_dim=%d, start=%d, end=%d, lo_size=%d.\n",
		split_dim, start, end, lo_size);
    }

    selectPoint(points, dataItems, start, end - start, split_dim, lo_size);
}

int
HnSRTreeFileObj::FindMaxVarianceDimension(HnPointVector &points,
					  int start, int end)
{
    return getMaxVarianceAxis(points, start, end - start);
}

/*
 * searchPoint()
 */

class HnSRTreeSearchPoint: public HnBinarySearch {
private:
    HnPointVector points;
    int offset;
    int count;
    double keyCoord;
    int axis;
    int order;

private:
    int getNumElements(void) {
	return count;
    }
    int compareToElementAt(int i) {
	double coord = points[offset + i].getCoordAt(axis);

	if ( order > 0 ) {
	    if ( keyCoord == coord) {
		return 0;
	    }
	    else if ( keyCoord < coord ) {
		return -1;
	    }
	    else {
		return 1;
	    }
	}
	else {
	    if ( keyCoord == coord ) {
		return 0;
	    }
	    else if ( keyCoord < coord ) {
		return 1;
	    }
	    else {
		return -1;
	    }
	}
    }   

    HnSRTreeSearchPoint(const HnPointVector &points, int offset, int count,
			double keyCoord, int axis, int order) {
	this->points = points;
	this->offset = offset;
	this->count = count;
	this->keyCoord = keyCoord;
	this->axis = axis;
	this->order = order;
    }

public:
    static void search(const HnPointVector &points, int offset, int count,
		       double keyCoord, int axis, int order,
		       HnBool *found_return, int *index_return) {
	HnSRTreeSearchPoint searcher(points, offset, count,
				     keyCoord, axis, order);
	searcher.searchElements(found_return, index_return);
    }
};

void
HnSRTreeFileObj::searchPoint(const HnPointVector &points,
			     int offset, int count,
			     double keyCoord, int axis, int order,
			     HnBool *found_return, int *index_return)
{
    HnSRTreeSearchPoint::search(points, offset, count, keyCoord, axis, order,
				found_return, index_return);
}

/*
 * searchCluster()
 */

class HnSRTreeSearchCluster: public HnBinarySearch {
private:
    HnSRTreeClusterVector clusters;
    int offset;
    int count;
    double keyCoord;
    int axis;
    int order;

private:
    int getNumElements(void) {
	return count;
    }
    int compareToElementAt(int i) {
	double coord =
	    clusters[offset + i].getRect().getRangeAt(axis).getCenter();

	if ( order > 0 ) {
	    if ( keyCoord == coord) {
		return 0;
	    }
	    else if ( keyCoord < coord ) {
		return -1;
	    }
	    else {
		return 1;
	    }
	}
	else {
	    if ( keyCoord == coord ) {
		return 0;
	    }
	    else if ( keyCoord < coord ) {
		return 1;
	    }
	    else {
		return -1;
	    }
	}
    }   

    HnSRTreeSearchCluster(const HnSRTreeClusterVector &clusters,
			  int offset, int count,
			  double keyCoord, int axis, int order) {
	this->clusters = clusters;
	this->offset = offset;
	this->count = count;
	this->keyCoord = keyCoord;
	this->axis = axis;
	this->order = order;
    }

public:
    static void search(const HnSRTreeClusterVector &clusters,
		       int offset, int count,
		       double keyCoord, int axis, int order,
		       HnBool *found_return, int *index_return) {
	HnSRTreeSearchCluster searcher(clusters, offset, count,
				       keyCoord, axis, order);
	searcher.searchElements(found_return, index_return);
    }
};

void
HnSRTreeFileObj::searchCluster(const HnSRTreeClusterVector &clusters,
			       int offset, int count,
			       double keyCoord, int axis, int order,
			       HnBool *found_return, int *index_return)
{
    HnSRTreeSearchCluster::search(clusters, offset, count,
				  keyCoord, axis, order,
				  found_return, index_return);
}

/*
 * selectPoint()
 */

class HnSRTreeSelectPoint: public HnQuickSelect {
private:
    HnPointVector points;
    HnDataItemVector dataItems;
    int offset;
    int count;
    int axis;

private:
    HnSRTreeSelectPoint(HnPointVector &points, HnDataItemVector &dataItems,
			int offset, int count, int axis) {
	this->points = points;
	this->dataItems = dataItems;
	this->offset = offset;
	this->count = count;
	this->axis = axis;
    }
    int getNumElements(void) {
	return count;
    }
    int compareElementsAt(int i, int j) {
	double coord1 = points[offset + i].getCoordAt(axis);
	double coord2 = points[offset + j].getCoordAt(axis);

	if ( coord1 > coord2 ) {
	    return 1;
	}
	else if ( coord1 < coord2 ) {
	    return -1;
	}
	else {
	    return 0;
	}
    }
    void swapElementsAt(int i, int j) {
	points.swapElementsAt(offset + i, offset + j);
	dataItems.swapElementsAt(offset + i, offset + j);
    }

public:
    static void select(HnPointVector &points, HnDataItemVector &dataItems,
		       int offset, int count, int axis, int index) {
	HnSRTreeSelectPoint selecter(points, dataItems,
				     offset, count, axis);
	selecter.selectElement(index);
    }
};

void
HnSRTreeFileObj::selectPoint(HnPointVector &points,
			     HnDataItemVector &dataItems,
			     int offset, int count,
			     int axis, int index)
{
    HnSRTreeSelectPoint::select(points, dataItems, offset, count, axis, index);
}

/*
 * selectCluster()
 */

class HnSRTreeSelectCluster: public HnQuickSelect {
private:
    HnSRTreeClusterVector clusters;
    HnFTlongVector offsets;
    int offset;
    int count;
    int axis;

private:
    HnSRTreeSelectCluster(HnSRTreeClusterVector &clusters,
			  HnFTlongVector &offsets,
			  int offset, int count, int axis) {
	this->clusters = clusters;
	this->offsets = offsets;
	this->offset = offset;
	this->count = count;
	this->axis = axis;
    }
    int getNumElements(void) {
	return count;
    }
    int compareElementsAt(int i, int j) {
	double coord1 =
	    clusters[offset + i].getRect().getRangeAt(axis).getCenter();
	double coord2 =
	    clusters[offset + j].getRect().getRangeAt(axis).getCenter();

	if ( coord1 > coord2 ) {
	    return 1;
	}
	else if ( coord1 < coord2 ) {
	    return -1;
	}
	else {
	    return 0;
	}
    }
    void swapElementsAt(int i, int j) {
	clusters.swapElementsAt(offset + i, offset + j);
	offsets.swapElementsAt(offset + i, offset + j);
    }

public:
    static void select(HnSRTreeClusterVector &clusters,
		       HnFTlongVector &offsets,
		       int offset, int count, int axis, int index) {
	HnSRTreeSelectCluster selecter(clusters, offsets, offset, count, axis);
	selecter.selectElement(index);
    }
};

void
HnSRTreeFileObj::selectCluster(HnSRTreeClusterVector &clusters,
			       HnFTlongVector &offsets,
			       int offset, int count, int axis, int index)
{
    HnSRTreeSelectCluster::select(clusters, offsets,
				  offset, count, axis, index);
}

/*
 * sortPoints()
 */

class HnSRTreeSortPoints: public HnQuickSort {
private:
    HnPointVector points;
    HnDataItemVector dataItems;
    int offset;
    int count;
    int sortDimension;

private:
    HnSRTreeSortPoints(HnPointVector &points, HnDataItemVector &dataItems,
		       int offset, int count, int sortDimension) {
	this->points = points;
	this->dataItems = dataItems;
	this->offset = offset;
	this->count = count;
	this->sortDimension = sortDimension;
    }
    int getNumElements(void) {
	return count;
    }
    int compareElementsAt(int i, int j) {
	double coord1 = points[offset + i].getCoordAt(sortDimension);
	double coord2 = points[offset + j].getCoordAt(sortDimension);

	if ( coord1 > coord2 ) {
	    return 1;
	}
	else if ( coord1 < coord2 ) {
	    return -1;
	}
	else {
	    return 0;
	}
    }
    void swapElementsAt(int i, int j) {
	points.swapElementsAt(offset + i, offset + j);
	dataItems.swapElementsAt(offset + i, offset + j);
    }

public:
    static void sort(HnPointVector &points, HnDataItemVector &dataItems,
		     int offset, int count, int sortDimension) {
	HnSRTreeSortPoints sorter(points, dataItems,
				  offset, count, sortDimension);
	sorter.sortElements();
    }
};

void
HnSRTreeFileObj::sortPoints(HnPointVector &points, HnDataItemVector &dataItems,
			    int offset, int count, int sortDimension)
{
    HnSRTreeSortPoints::sort(points, dataItems,
			     offset, count, sortDimension);
}

/*
 * sortClusters()
 */

class HnSRTreeSortClusters: public HnQuickSort {
private:
    HnSRTreeClusterVector clusters;
    HnFTlongVector offsets;
    int offset;
    int count;
    int sortDimension;

private:
    HnSRTreeSortClusters(HnSRTreeClusterVector &clusters,
			 HnFTlongVector &offsets,
			 int offset, int count, int sortDimension) {
	this->clusters = clusters;
	this->offsets = offsets;
	this->offset = offset;
	this->count = count;
	this->sortDimension = sortDimension;
    }
    int getNumElements(void) {
	return count;
    }
    int compareElementsAt(int i, int j) {
	double coord1 = clusters[offset + i]
	    .getRect().getRangeAt(sortDimension).getCenter();
	double coord2 = clusters[offset + j]
	    .getRect().getRangeAt(sortDimension).getCenter();

	if ( coord1 > coord2 ) {
	    return 1;
	}
	else if ( coord1 < coord2 ) {
	    return -1;
	}
	else {
	    return 0;
	}
    }
    void swapElementsAt(int i, int j) {
	clusters.swapElementsAt(offset + i, offset + j);
	offsets.swapElementsAt(offset + i, offset + j);
    }

public:
    static void sort(HnSRTreeClusterVector &clusters, HnFTlongVector &offsets,
		     int offset, int count, int sortDimension) {
	HnSRTreeSortClusters sorter(clusters, offsets,
				    offset, count, sortDimension);
	sorter.sortElements();
    }
};

void
HnSRTreeFileObj::sortClusters(HnSRTreeClusterVector &clusters,
			      HnFTlongVector &offsets,
			      int offset, int count, int sortDimension)
{
    HnSRTreeSortClusters::sort(clusters, offsets,
			       offset, count, sortDimension);
}

/*
 * getMaxVarianceAxis()
 */

int
HnSRTreeFileObj::getMaxVarianceAxis(HnPointVector &points,
				    int offset, int count)
{
    int dimension = info.getDimension();
    int axis;
    double *sum = new double[dimension];
    double *sum2 = new double[dimension];
    int i;
    struct {
	int axis;
	double var;
    } max;

    for ( axis=0; axis<dimension; axis++ ) {
	sum[axis] = 0;
	sum2[axis] = 0;
    }

    for ( i=offset; i<offset + count; i++ ) {
	for ( axis=0; axis<dimension; axis++ ) {
	    double x = points[i].getCoordAt(axis);

	    sum[axis] += x;
	    sum2[axis] += x * x;
	}
    }

    max.axis = -1;
    max.var = 0;

    for ( axis=0; axis<dimension; axis++ ) {
	double mean = sum[axis] / count;
	double var = sum2[axis] / count - mean * mean;

	if ( axis == 0 ) {
	    max.axis = 0;
	    max.var = var;
	}
	else {
	    if ( var > max.var ) {
		max.axis = axis;
		max.var = var;
	    }
	}
    }

    delete[] sum;
    delete[] sum2;

    return max.axis;
}

int
HnSRTreeFileObj::getMaxVarianceAxis(HnSRTreeClusterVector &clusters,
				    int offset, int count)
{
    int dimension = info.getDimension();
    int axis;
    double *sum = new double[dimension];
    double *sum2 = new double[dimension];
    int totalWeight;
    int i;
    struct {
	int axis;
	double var;
    } max;

    for ( axis=0; axis<dimension; axis++ ) {
	sum[axis] = 0;
	sum2[axis] = 0;
    }
    totalWeight = 0;

    for ( i=offset; i<offset + count; i++ ) {
#if 0
	int weight = clusters[i].getWeight();

	for ( axis=0; axis<dimension; axis++ ) {
	    double coord = clusters[i].getCentroid().getCoordAt(axis);

	    /*
	     * NOTE:
	     *      In the calculation of the variance,
	     *	    it is assumed that all points are located
	     *      at the centroid of the cluster.
	     */

	    sum[axis] += coord * weight;
	    sum2[axis] += coord * coord * weight;
	}

	totalWeight += weight;
#else
	for ( axis=0; axis<dimension; axis++ ) {
	    double coord = clusters[i].getRect().getRangeAt(axis).getCenter();

	    sum[axis] += coord;
	    sum2[axis] += coord * coord;
	}

	totalWeight ++;
#endif
    }

    max.axis = -1;
    max.var = 0;

    for ( axis=0; axis<dimension; axis++ ) {
	double mean = sum[axis] / totalWeight;
	double var = sum2[axis] / totalWeight - mean * mean;

	if ( axis == 0 ) {
	    max.axis = 0;
	    max.var = var;
	}
	else {
	    if ( var > max.var ) {
		max.axis = axis;
		max.var = var;
	    }
	}
    }

    delete[] sum;
    delete[] sum2;

    return max.axis;
}

