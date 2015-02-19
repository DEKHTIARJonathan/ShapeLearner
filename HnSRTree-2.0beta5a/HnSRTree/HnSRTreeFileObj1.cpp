/*
 * HnSRTreeFileObj1.cc (dynamic construction methods)
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
 * $Id: HnSRTreeFileObj1.cc,v 1.5 2002/09/13 03:50:54 katayama Exp $
 */

#include <math.h>
#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#include <sys/types.h>
#endif
#include "HnSRTree/HnSRTreeFile.hh"
#include "HnSRTree/HnSRTreeFileObj.hh"
#include "HnSRTree/HnStatistics.hh"

/*
 * Store
 */

void
HnSRTreeFileObj::store(const HnPoint &point, const HnDataItem &dataItem)
{
    if ( point.getDimension() != getDimension() ) {
	HnAbort("mismatch in the dimensions (point: %d, file: %d)",
		point.getDimension(), getDimension());
    }
    if ( dataItem.length() > info.getDataItemSize() ) {
	HnAbort("The size of data item (%d) exceeds the limit (%d).",
		dataItem.length(), info.getDataItemSize());
    }

    reinsertList = new_HnSRTreeReinsertVector();
    reinsertedBlocks = new_HnFTlongVector();

    reinsertList.addElement(new_HnSRTreeReinsert(point, dataItem));

    while ( reinsertList.size() != 0 ) {
	if ( reinsertList[0].getType() == HnSRTreeReinsert::POINT ) {
	    HnPoint point = reinsertList[0].getPoint();
	    HnDataItem dataItem = reinsertList[0].getDataItem();

	    reinsertList.removeElementAt(0);
	    insertPoint(point, dataItem);
	}
	else {
	    long offset = reinsertList[0].getOffset();
	    int level = reinsertList[0].getLevel();

	    reinsertList.removeElementAt(0);
	    insertBlock(offset, level);
	}
    }

    if ( debug ) {
	check();
    }
}

void
HnSRTreeFileObj::insertPoint(const HnPoint &point, const HnDataItem &dataItem)
{
    HnSRTreeStack stack;
    HnSRTreeLeaf leaf;

    stack = chooseLeaf(point);

    leaf = stack.getTopLeaf();

    if ( !leaf.isFull() ) {
	leaf.addElement(point, dataItem);
	writeLeaf(leaf);
	updateCluster(stack);
    }
    else {
	int reinsertCount =
	    (leaf.getCount() + 1) * info.getReinsertFactor() / 100;

	if ( stack.getDepth() == 1 ) {
	    /* leaf is the root node */
	    splitLeaf(stack, point, dataItem);
	}
	else if ( reinsertCount <= 0 ) {
	    /* no entry needs to be reinserted */
	    splitLeaf(stack, point, dataItem);
	}
	else {
	    /* reinsert some of entries */
	    int index = -1;

	    if ( (index = reinsertedBlocks.indexOf(leaf.getOffset())) < 0 ) {
		reinsertedBlocks.addElement(leaf.getOffset());
		reinsertLeaf(stack, point, dataItem);
	    }
	    else {
		reinsertedBlocks.removeElementAt(index);
		splitLeaf(stack, point, dataItem);
	    }
	}
    }
}

void
HnSRTreeFileObj::insertBlock(long offset, int level)
{
    HnSRTreeBlock block = readBlock(offset);
    HnSRTreeCluster cluster;
    HnSRTreeStack stack;
    HnSRTreeNode parent;

    if ( block.isNode() ) {
	HnSRTreeNode node = new_HnSRTreeNode(info, block);
	cluster = node.getCluster();
    }
    else if ( block.isLeaf() ) {
	HnSRTreeLeaf leaf = new_HnSRTreeLeaf(info, block);
	cluster = leaf.getCluster();
    }
    else {
	HnAbort("unexpected block type.");
    }

    stack = chooseNode(cluster.getCentroid(), level);

    parent = stack.getTopNode();

    if ( !parent.isFull() ) {
	parent.addElement(cluster, offset);
	writeNode(parent);
	updateCluster(stack);
    }
    else {
	int reinsertCount =
	    (parent.getCount() + 1) * info.getReinsertFactor() / 100;

	if ( stack.getDepth() == 1 ) {
	    /* parent is the root node */
	    splitNode(stack, cluster, offset);
	}
	else if ( reinsertCount <= 0 ) {
	    /* no entry needs to be reinserted */
	    splitNode(stack, cluster, offset);
	}	    
	else {
	    /* reinsert some of entries */
	    int index = -1;

	    if ( (index = reinsertedBlocks.indexOf(parent.getOffset())) < 0 ) {
		reinsertedBlocks.addElement(parent.getOffset());
		reinsertNode(stack, cluster, offset);
	    }
	    else {
		reinsertedBlocks.removeElementAt(index);
		splitNode(stack, cluster, offset);
	    }
	}
    }
}

HnSRTreeStack
HnSRTreeFileObj::chooseLeaf(const HnPoint &point)
{
    HnSRTreeStack stack = new_HnSRTreeStack();
    HnSRTreeBlock block;
    HnSRTreeNode node;
    HnSRTreeLeaf leaf;
    int index;

    /*
     * NOTE:
     *	    The stack is used for keeping the trace of the access path.
     *	    Cursors indicates which entry is used at each node.
     */

    block = readBlock(info.getRootOffset());

    while ( block.isNode() ) {
	node = new_HnSRTreeNode(info, block);
	index = chooseSubtree(node, point);

	stack.push(node, index);
	block = readBlock(node.getOffsetAt(index));
    }

    leaf = new_HnSRTreeLeaf(info, block);
    stack.push(leaf, -1);

    return stack;
}

int
HnSRTreeFileObj::chooseSubtree(const HnSRTreeNode &node, const HnPoint &point)
{
    struct {
	int index;
	double distance;
    } min, cur;

    min.index = -1;
    min.distance = -1;

    for ( cur.index=0; cur.index<node.getCount(); cur.index++ ) {
	HnSRTreeCluster cluster = node.getClusterAt(cur.index);

	cur.distance = point.getDistance(cluster.getCentroid());

	if ( min.index == -1 ) {
	    min = cur;
	}
	else {
	    if ( cur.distance < min.distance ) {
		min = cur;
	    }
	}
    }

    return min.index;
}

HnSRTreeStack
HnSRTreeFileObj::chooseNode(const HnPoint &centroid, int level)
{
    HnSRTreeStack stack = new_HnSRTreeStack();
    HnSRTreeBlock block;
    HnSRTreeNode node;
    int index;

    /*
     * NOTE:
     *	    The stack is used for keeping the trace of the access path.
     *	    Cursors indicates which entry is used at each node.
     */

    block = readBlock(info.getRootOffset());

    while ( stack.getDepth() != info.getHeight() - level ) {
	node = new_HnSRTreeNode(info, block);
	index = chooseSubtree(node, centroid);

	stack.push(node, index);
	block = readBlock(node.getOffsetAt(index));
    }

    return stack;
}

void
HnSRTreeFileObj::updateCluster(HnSRTreeStack stack)
{
    HnSRTreeCluster cluster;
    HnSRTreeNode parent;
    int cursor;

    if ( stack.getDepth() == 0 ) {
	HnAbort("stack is empty.");
    }

    if ( stack.isTopNode() ) {
	HnSRTreeNode node = stack.getTopNode();
	cluster = node.getCluster();
    }
    else {
	HnSRTreeLeaf leaf = stack.getTopLeaf();
	cluster = leaf.getCluster();
    }
    stack.pop();

    while ( stack.getDepth() > 0 ) {
	parent = stack.getTopNode();
	cursor = stack.getCursor();

	parent.setClusterAt(cluster, cursor);

	cluster = parent.getCluster();

	writeNode(parent);
	stack.pop();
    }
}

/*
 * Reinsert
 */

struct HnSRTreeReinsertionEntry {
    int index;
    double distance;
};

static int
HnSRTreeCompareReinsertionEntries(const void *ptr1, const void *ptr2)
{
    HnSRTreeReinsertionEntry *entry1 = (HnSRTreeReinsertionEntry *)ptr1;
    HnSRTreeReinsertionEntry *entry2 = (HnSRTreeReinsertionEntry *)ptr2;

    if ( entry1->distance == entry2->distance ) {
	return 0;
    }
    else {
	/* entries are compared for descending order */
	if ( entry1->distance < entry2->distance ) {
	    return 1;
	}
	else {
	    return -1;
	}
    }
}

/*
 * Reinsert (leaf)
 */

void
HnSRTreeFileObj::reinsertLeaf(HnSRTreeStack &stack,
			      const HnPoint &newPoint,
			      const HnDataItem &newDataItem)
{
    HnSRTreeLeaf leaf, newLeaf;
    HnPointArray points;
    int i;
    ReinsertFlag *flags;

    leaf = stack.getTopLeaf();

    /* put points into an array */
    points = new_HnPointArray(leaf.getCount() + 1);
    for ( i=0; i<leaf.getCount(); i++ ) {
	points[i] = leaf.getPointAt(i);
    }
    points[i] = newPoint;

    selectPoints(points, &flags);

    newLeaf = new_HnSRTreeLeaf(info, leaf.getOffset());

    for ( i=0; i<leaf.getCount(); i++ ) {
	HnPoint point = leaf.getPointAt(i);
	HnDataItem dataItem = leaf.getDataItemAt(i);

	switch ( flags[i] ) {
	case STAY:
	    newLeaf.addElement(point, dataItem);
	    break;
	case REINSERT:
	    reinsertList.addElement(new_HnSRTreeReinsert(point, dataItem));
	    break;
	default:
	    HnAbort("reinsert flag is incorrectly assigned.");
	}
    }
    switch ( flags[i] ) {
    case STAY:
	newLeaf.addElement(newPoint, newDataItem);
	break;
    case REINSERT:
	reinsertList.addElement(new_HnSRTreeReinsert(newPoint, newDataItem));
	break;
    default:
	HnAbort("reinsert flag is incorrectly assigned.");
    }

    writeLeaf(newLeaf);

    /* replace leaf with newLeaf in the stack */
    stack.pop();
    stack.push(newLeaf);

    updateCluster(stack);
}

void
HnSRTreeFileObj::selectPoints(const HnPointArray &points,
			      ReinsertFlag **flags_return)
{
    static ReinsertFlag *flags = NULL;
    int numPoints = points.length;
    int dimension = info.getDimension();
    int reinsertCount = numPoints * info.getReinsertFactor() / 100;
    HnPoint centroid;
    int i, axis;
    HnSRTreeReinsertionEntry *entries;

    /* initialize flags */
    if ( flags != NULL ) {
	delete[] flags;
    }
    flags = new ReinsertFlag[numPoints];

    for ( i=0; i<numPoints; i++ ) {
	flags[i] = REINSERT_NONE;
    }

    /* calculate centroid */
    centroid = new_HnPoint(dimension);

    for ( axis=0; axis<dimension; axis++ ) {
	double sum = 0;

	for ( i=0; i<numPoints; i++ ) {
	    sum += points[i].getCoordAt(axis);
	}

	centroid.setCoordAt(sum / numPoints, axis);
    }

    /* initialize entries */
    entries = (HnSRTreeReinsertionEntry *)
	HnMalloc(sizeof(HnSRTreeReinsertionEntry) * numPoints);

    for ( i=0; i<numPoints; i++ ) {
	entries[i].index = i;
	entries[i].distance = points[i].getDistance(centroid);
    }

    /* sort points by distance in descent order */
    qsort(entries, numPoints, sizeof(HnSRTreeReinsertionEntry),
	  HnSRTreeCompareReinsertionEntries);

    /* make reinsert flags */
    i=0;
    while ( i<reinsertCount ) {
	flags[entries[i].index] = REINSERT;
	i++;
    }
    while ( i<numPoints ) {
	flags[entries[i].index] = STAY;
	i++;
    }

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::selectPoints:\n");
	fprintf(stderr, "    REINSERT\n");
	for ( i=0; i<numPoints; i++ ) {
	    if ( flags[i] == REINSERT )
		fprintf(stderr,
			"    points[%d].getDistance(centroid) = %g\n",
			i, points[i].getDistance(centroid));
	}
	fprintf(stderr, "    STAY\n");
	for ( i=0; i<numPoints; i++ ) {
	    if ( flags[i] == STAY )
		fprintf(stderr,
			"    points[%d].getDistance(centroid) = %g\n",
			i, points[i].getDistance(centroid));
	}
    }

    *flags_return = flags;

    HnFree(entries, sizeof(HnSRTreeReinsertionEntry) * numPoints);
}

/*
 * Reinsert (node)
 */

void
HnSRTreeFileObj::reinsertNode(HnSRTreeStack &stack,
			      const HnSRTreeCluster &newCluster,
			      long newOffset)
{
    HnSRTreeNode node, newNode;
    HnSRTreeClusterArray clusters;
    int i, level;
    ReinsertFlag *flags;

    node = stack.getTopNode();
    level = info.getHeight() - stack.getDepth();

    /* put clusters into an array */
    clusters = new_HnSRTreeClusterArray(node.getCount() + 1);
    for ( i=0; i<node.getCount(); i++ ) {
	clusters[i] = node.getClusterAt(i);
    }
    clusters[i] = newCluster;

    /* select clusters */
    selectClusters(clusters, &flags);

    newNode = new_HnSRTreeNode(info, node.getOffset());

    for ( i=0; i<node.getCount(); i++ ) {
	HnSRTreeCluster cluster = node.getClusterAt(i);
	long offset = node.getOffsetAt(i);

	switch ( flags[i] ) {
	case STAY:
	    newNode.addElement(cluster, offset);
	    break;
	case REINSERT:
	    reinsertList.addElement(new_HnSRTreeReinsert(offset, level));
	    break;
	default:
	    HnAbort("reinsert flag is incorrectly assigned.");
	}
    }
    switch ( flags[i] ) {
    case STAY:
	newNode.addElement(newCluster, newOffset);
	break;
    case REINSERT:
	reinsertList.addElement(new_HnSRTreeReinsert(newOffset, level));
	break;
    default:
	HnAbort("reinsert flag is incorrectly assigned.");
    }

    writeNode(newNode);

    /* replace node with newNode in the stack */
    stack.pop();
    stack.push(newNode);

    updateCluster(stack);
}

void
HnSRTreeFileObj::selectClusters(const HnSRTreeClusterArray &clusters,
				ReinsertFlag **flags_return)
{
    static ReinsertFlag *flags = NULL;
    int numClusters = clusters.length;
    int dimension = info.getDimension();
    int reinsertCount = numClusters * info.getReinsertFactor() / 100;
    HnPoint centroid;
    int i, axis;
    HnSRTreeReinsertionEntry *entries;

    /* initialize flags */
    if ( flags != NULL ) {
	delete[] flags;
    }
    flags = new ReinsertFlag[numClusters];

    for ( i=0; i<numClusters; i++ ) {
	flags[i] = REINSERT_NONE;
    }

    /* calculate centroid */
    centroid = new_HnPoint(dimension);

    for ( axis=0; axis<dimension; axis++ ) {
	double sum = 0;
	int total = 0;

	for ( i=0; i<numClusters; i++ ) {
	    double coord = clusters[i].getCentroid().getCoordAt(axis);
	    int weight = clusters[i].getWeight();

	    sum += coord * weight;
	    total += weight;
	}

	centroid.setCoordAt(sum / total, axis);
    }

    /* initialize entries */
    entries = (HnSRTreeReinsertionEntry *)
	HnMalloc(sizeof(HnSRTreeReinsertionEntry) * numClusters);

    for ( i=0; i<numClusters; i++ ) {
	entries[i].index = i;
	entries[i].distance = clusters[i].getCentroid().getDistance(centroid);
    }

    /* sort clusters by distance in descent order */
    qsort(entries, numClusters, sizeof(HnSRTreeReinsertionEntry),
	  HnSRTreeCompareReinsertionEntries);

    /* make reinsert flags */
    i=0;
    while ( i<reinsertCount ) {
	flags[entries[i].index] = REINSERT;
	i++;
    }
    while ( i<numClusters ) {
	flags[entries[i].index] = STAY;
	i++;
    }

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::selectClusters:\n");
	fprintf(stderr, "    REINSERT\n");
	for ( i=0; i<numClusters; i++ ) {
	    if ( flags[i] == REINSERT ) {
		fprintf(stderr,
			"    clusters[%d].getCentroid()."
			"getDistance(centroid) = %g\n",
			i, clusters[i].getCentroid().getDistance(centroid));
	    }
	}
	fprintf(stderr, "    STAY\n");
	for ( i=0; i<numClusters; i++ ) {
	    if ( flags[i] == STAY ) {
		fprintf(stderr,
			"    clusters[%d].getCentroid()."
			"getDistance(centroid) = %g\n",
			i, clusters[i].getCentroid().getDistance(centroid));
	    }
	}
    }

    *flags_return = flags;

    HnFree(entries, sizeof(HnSRTreeReinsertionEntry) * numClusters);
}

/*
 * Split
 */

void
HnSRTreeFileObj::splitLeaf(HnSRTreeStack &stack,
			   const HnPoint &newPoint,
			   const HnDataItem &newDataItem)
{
    HnSRTreeLeaf leaf, left, right;
    int i;
    HnPointArray points;
    SplitFlag *flags;

    leaf = stack.getTopLeaf();

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::splitLeaf: leaf = %s\n",
		(char *)leaf.toString());
    }

    /* put points into an array */
    points = new_HnPointArray(leaf.getCount() + 1);
    for ( i=0; i<leaf.getCount(); i++ ) {
	points[i] = leaf.getPointAt(i);
    }
    points[i] = newPoint;

    /* split points */
    splitPoints(points, &flags);

    /* put points into the left and right leaves */
    left = new_HnSRTreeLeaf(info, leaf.getOffset());
    right = allocateLeaf();

    for ( i=0; i<leaf.getCount(); i++ ) {
	switch ( flags[i] ) {
	case LEFT:
	    left.addElement(leaf.getPointAt(i), leaf.getDataItemAt(i));
	    break;
	case RIGHT:
	    right.addElement(leaf.getPointAt(i), leaf.getDataItemAt(i));
	    break;
	default:
	    HnAbort("split flag is incorrectly assigned.");
	}
    }
    switch ( flags[i] ) {
    case LEFT:
	left.addElement(newPoint, newDataItem);
	break;
    case RIGHT:
	right.addElement(newPoint, newDataItem);
	break;
    default:
	HnAbort("split flag is incorrectly assigned.");
    }

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::splitLeaf: left  = %s\n",
		(char *)left.toString());
	fprintf(stderr, "HnSRTreeFileObj::splitLeaf: right = %s\n",
		(char *)right.toString());
    }

    writeLeaf(left);
    writeLeaf(right);

    stack.pop();

    updateNode(stack,
	       left.getCluster(), left.getOffset(),
	       right.getCluster(), right.getOffset());

    profile->numLeafSplits ++;
}

void
HnSRTreeFileObj::splitNode(HnSRTreeStack &stack,
			   const HnSRTreeCluster &newCluster, long newOffset)
{
    HnSRTreeNode node, left, right;
    int i;
    HnSRTreeClusterArray clusters;
    SplitFlag *flags;

    node = stack.getTopNode();

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::splitNode: node = %s\n",
		(char *)node.toString());
    }

    /* put clusters into an array */
    clusters = new_HnSRTreeClusterArray(node.getCount() + 1);
    for ( i=0; i<node.getCount(); i++ ) {
	clusters[i] = node.getClusterAt(i);
    }
    clusters[i] = newCluster;

    /* split clusters */
    splitClusters(clusters, &flags);

    /* put clusters into the left and right nodes */
    left = new_HnSRTreeNode(info, node.getOffset());
    right = allocateNode();

    for ( i=0; i<node.getCount(); i++ ) {
	switch ( flags[i] ) {
	case LEFT:
	    left.addElement(node.getClusterAt(i), node.getOffsetAt(i));
	    break;
	case RIGHT:
	    right.addElement(node.getClusterAt(i), node.getOffsetAt(i));
	    break;
	default:
	    HnAbort("split flag is incorrectly assigned.");
	}
    }
    switch ( flags[i] ) {
    case LEFT:
	left.addElement(newCluster, newOffset);
	break;
    case RIGHT:
	right.addElement(newCluster, newOffset);
	break;
    default:
	HnAbort("split flag is incorrectly assigned.");
    }

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::splitNode: left  = %s\n",
		(char *)left.toString());
	fprintf(stderr, "HnSRTreeFileObj::splitNode: right = %s\n",
		(char *)right.toString());
    }

    writeNode(left);
    writeNode(right);

    stack.pop();

    updateNode(stack,
	       left.getCluster(), left.getOffset(),
	       right.getCluster(), right.getOffset());

    profile->numNodeSplits ++;
}

void
HnSRTreeFileObj::updateNode(HnSRTreeStack &stack,
			    const HnSRTreeCluster &leftCluster,
			    long leftOffset,
			    const HnSRTreeCluster &rightCluster,
			    long rightOffset)
{
    HnSRTreeNode node;
    int cursor;

    if ( stack.getDepth() == 0 ) {
	extendTree(leftCluster, leftOffset, rightCluster, rightOffset);
    }
    else {
	node = stack.getTopNode();
	cursor = stack.getCursor();

	node.setElementAt(leftCluster, leftOffset, cursor);

	if ( node.isFull() ) {
	    int reinsertCount =
		(node.getCount() + 1) * info.getReinsertFactor() / 100;

	    if ( stack.getDepth() == 1 ) {
		/* the parent is the root node */
		splitNode(stack, rightCluster, rightOffset);
	    }
	    else if ( reinsertCount <= 0 ) {
		/* no entry needs to be reinserted */
		splitNode(stack, rightCluster, rightOffset);
	    }
	    else {
		/* reinsert some of entries */
		int index = -1;

		if ( (index =
		      reinsertedBlocks.indexOf(node.getOffset())) < 0 ) {
		    reinsertedBlocks.addElement(node.getOffset());
		    reinsertNode(stack, rightCluster, rightOffset);
		}
		else {
		    reinsertedBlocks.removeElementAt(index);
		    splitNode(stack, rightCluster, rightOffset);
		}
	    }
	}
	else {
	    node.addElement(rightCluster, rightOffset);

	    writeNode(node);
	    updateCluster(stack);
	}
    }
}

void
HnSRTreeFileObj::extendTree(const HnSRTreeCluster &leftCluster,
			    long leftOffset,
			    const HnSRTreeCluster &rightCluster,
			    long rightOffset)
{
    HnSRTreeNode node;

    node = allocateNode();
    node.addElement(leftCluster, leftOffset);
    node.addElement(rightCluster, rightOffset);
    writeNode(node);

    info.setRootOffset(node.getOffset());
    info.setHeight(info.getHeight() + 1);
    writeSuperBlock();
}

/*
 * Split
 */

struct HnSRTreeAxisSortEntry {
    int index;
    double coord;
};

static int
HnSRTreeCompareAxisSortEntries(const void *ptr1, const void *ptr2)
{
    HnSRTreeAxisSortEntry *entry1 = (HnSRTreeAxisSortEntry *)ptr1;
    HnSRTreeAxisSortEntry *entry2 = (HnSRTreeAxisSortEntry *)ptr2;

    if ( entry1->coord == entry2->coord ) {
	return 0;
    }
    else {
	if ( entry1->coord < entry2->coord ) {
	    return -1;
	}
	else {
	    return 1;
	}
    }
}

/*
 * Split (leaf)
 */

void
HnSRTreeFileObj::splitPoints(const HnPointArray &points,
			     SplitFlag **flags_return)
{
    static SplitFlag *flags = NULL;
    int numPoints = points.length;
    int dimension = info.getDimension();
    int minCount, maxCount;
    struct {
	int axis;
	double var;
    } max;
    struct {
	int count;
	double leftVar, rightVar;
    } min;
    int axis, count;
    int i;
    HnSRTreeAxisSortEntry *entries;

    /* initialize minCount and maxCount */
    if ( (minCount = numPoints * info.getSplitFactor() / 100) == 0 ) {
	minCount = 1;
    }
    maxCount = numPoints - minCount;

    /* initialize flags */
    if ( flags != NULL ) {
	delete[] flags;
    }
    flags = new SplitFlag[numPoints];

    for ( i=0; i<numPoints; i++ ) {
	flags[i] = SPLIT_NONE;
    }

    max.axis = -1;
    max.var = -1;

    /* calculate variance */
    for ( axis=0; axis<dimension; axis++ ) {
	double avg, var;
	double sum = 0;
	double sum2 = 0;

	for ( i=0; i<numPoints; i++ ) {
	    double coord = points[i].getCoordAt(axis);
	    sum += coord;
	    sum2 += coord * coord;
	}

	avg = sum / numPoints;
	var = sum2 / numPoints - avg * avg;

	if ( max.axis == -1 ) {
	    max.axis = axis;
	    max.var = var;
	}
	else {
	    if ( var > max.var ) {
		max.axis = axis;
		max.var = var;
	    }
	}
    }

    /* sort points along the max variance axis */
    entries = (HnSRTreeAxisSortEntry *)
	HnMalloc(sizeof(HnSRTreeAxisSortEntry) * numPoints);

    for ( i=0; i<numPoints; i++ ) {
	entries[i].index = i;
	entries[i].coord = points[i].getCoordAt(max.axis);
    }

    qsort(entries, numPoints, sizeof(HnSRTreeAxisSortEntry),
	  HnSRTreeCompareAxisSortEntries);

    /* choose split point */
    min.count = -1;
    min.leftVar = 0;
    min.rightVar = 0;

    for ( count=minCount; count<=maxCount; count++ ) {
	double leftVar, rightVar;

	/* calculate the sum of variances on the left side */
	leftVar = 0;

	for ( axis=0; axis<dimension; axis++ ) {
	    double sum, sum2, avg, var;

	    sum = 0;
	    sum2 = 0;

	    for ( i=0; i<count; i++ ) {
		double coord = points[entries[i].index].getCoordAt(axis);
		sum += coord;
		sum2 += coord * coord;
	    }

	    avg = sum / count;
	    var = sum2 / count - avg * avg;

	    leftVar += var;
	}

	/* calculate the sum of variances on the right side */
	rightVar = 0;

	for ( axis=0; axis<dimension; axis++ ) {
	    double sum, sum2, avg, var;

	    sum = 0;
	    sum2 = 0;

	    for ( i=count; i<numPoints; i++ ) {
		double coord = points[entries[i].index].getCoordAt(axis);
		sum += coord;
		sum2 += coord * coord;
	    }
	    avg = sum / (numPoints - count);
	    var = sum2 / (numPoints - count) - avg * avg;

	    rightVar += var;
	}

	if ( min.count < 0 ) {
	    min.count = count;
	    min.leftVar = leftVar;
	    min.rightVar = rightVar;
	}
	else {
	    if ( leftVar + rightVar < min.leftVar + min.rightVar ) {
		min.count = count;
		min.leftVar = leftVar;
		min.rightVar = rightVar;
	    }
	}
    }

    /* make split flags */
    i=0;
    while ( i<min.count ) {
	flags[entries[i].index] = LEFT;
	i++;
    }
    while ( i<numPoints ) {
	flags[entries[i].index] = RIGHT;
	i++;
    }

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::splitPoints\n");
	fprintf(stderr, "    LEFT\n");
	for ( i=0; i<numPoints; i++ ) {
	    if ( flags[i] == LEFT )
		fprintf(stderr,
			"    points[%d].getCoordAt(%d) = %g\n",
			i, max.axis, points[i].getCoordAt(max.axis));
	}
	fprintf(stderr, "    RIGHT\n");
	for ( i=0; i<numPoints; i++ ) {
	    if ( flags[i] == RIGHT )
		fprintf(stderr,
			"    points[%d].getCoordAt(%d) = %g\n",
			i, max.axis, points[i].getCoordAt(max.axis));
	}
    }

    *flags_return = flags;

    HnFree(entries, sizeof(HnSRTreeAxisSortEntry) * numPoints);
}

/*
 * Split (node)
 */

void
HnSRTreeFileObj::splitClusters(const HnSRTreeClusterArray &clusters,
			       SplitFlag **flags_return)
{
    static SplitFlag *flags = NULL;
    int numClusters = clusters.length;
    int dimension = info.getDimension();
    int minCount, maxCount;
    struct {
	int axis;
	double var;
    } max;
    struct {
	int count;
	double leftVar, rightVar;
    } min;
    int axis, count;
    int i;
    HnSRTreeAxisSortEntry *entries;

    /* initialize minCount and maxCount */
    if ( (minCount = numClusters * info.getSplitFactor() / 100) == 0 ) {
	minCount = 1;
    }
    maxCount = numClusters - minCount;

    /* initialize flags */
    if ( flags != NULL ) {
	delete[] flags;
    }
    flags = new SplitFlag[numClusters];

    for ( i=0; i<numClusters; i++ ) {
	flags[i] = SPLIT_NONE;
    }

    max.axis = -1;
    max.var = -1;

    /* calculate variance */
    for ( axis=0; axis<dimension; axis++ ) {
	double avg, var;
	double sum = 0;
	double sum2 = 0;
	int total = 0;

	for ( i=0; i<numClusters; i++ ) {
	    double coord = clusters[i].getCentroid().getCoordAt(axis);

	    /*
	     * NOTE:
	     *      In the calculation of the variance,
	     *	    it is assumed that all points are located
             *      at the centroid of the cluster.
	     */

	    sum += coord * clusters[i].getWeight();
	    sum2 += coord * coord * clusters[i].getWeight();
	    total += clusters[i].getWeight();
	}

	avg = sum / total;
	var = sum2 / total - avg * avg;

	if ( max.axis == -1 || var > max.var ) {
	    max.axis = axis;
	    max.var = var;
	}
    }

    /* sort clusters along the max variance axis */
    entries = (HnSRTreeAxisSortEntry *)
	HnMalloc(sizeof(HnSRTreeAxisSortEntry) * numClusters);

    for ( i=0; i<numClusters; i++ ) {
	entries[i].index = i;
	entries[i].coord = clusters[i].getCentroid().getCoordAt(max.axis);
    }

    qsort(entries, numClusters, sizeof(HnSRTreeAxisSortEntry),
	  HnSRTreeCompareAxisSortEntries);

    /* choose split point */
    min.count = -1;
    min.leftVar = 0;
    min.rightVar = 0;

    for ( count=minCount; count<=maxCount; count++ ) {
	double leftVar, rightVar;

	/* calculate the sum of variances on the left side */
	leftVar = 0;

	for ( axis=0; axis<dimension; axis++ ) {
	    double sum, sum2, avg, var;
	    int total;

	    /*
	     * NOTE:
	     *      In the calculation of the variance,
	     *	    it is assumed that all points are located
	     *      at the centroid of the cluster.
	     */

	    sum = 0;
	    sum2 = 0;
	    total = 0;

	    for ( i=0; i<count; i++ ) {
		HnSRTreeCluster &cluster = clusters[entries[i].index];
		double coord = cluster.getCentroid().getCoordAt(axis);
		int weight = cluster.getWeight();

		sum += coord * weight;
		sum2 += coord * coord * weight;
		total += weight;
	    }

	    avg = sum / total;
	    var = sum2 / total - avg * avg;

	    leftVar += var;
	}

	/* calculate the sum of variances on the right side */
	rightVar = 0;

	for ( axis=0; axis<dimension; axis++ ) {
	    double sum, sum2, avg, var;
	    int total;

	    /*
	     * NOTE:
	     *	    In the calculation of the variance,
	     *	    it is assumed that all points are located
	     *	    at the centroid of the cluster.
	     */

	    sum = 0;
	    sum2 = 0;
	    total = 0;

	    for ( i=count; i<numClusters; i++ ) {
		HnSRTreeCluster &cluster = clusters[entries[i].index];
		double coord = cluster.getCentroid().getCoordAt(axis);
		int weight = cluster.getWeight();

		sum += coord * weight;
		sum2 += coord * coord * weight;
		total += weight;
	    }

	    avg = sum / total;
	    var = sum2 / total - avg * avg;

	    rightVar += var;
	}

	if ( min.count < 0 ) {
	    min.count = count;
	    min.leftVar = leftVar;
	    min.rightVar = rightVar;
	}
	else {
	    if ( leftVar + rightVar < min.leftVar + min.rightVar ) {
		min.count = count;
		min.leftVar = leftVar;
		min.rightVar = rightVar;
	    }
	}
    }

    /* make split flags */
    i=0;
    while ( i<min.count ) {
	flags[entries[i].index] = LEFT;
	i++;
    }
    while ( i<numClusters ) {
	flags[entries[i].index] = RIGHT;
	i++;
    }

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::selectClusters:\n");
	fprintf(stderr, "    LEFT\n");
	for ( i=0; i<numClusters; i++ ) {
	    if ( flags[i] == LEFT ) {
		fprintf(stderr,
			"    clusters[%d].getCentroid().getCoordAt(%d) = %g\n",
			i, max.axis,
			clusters[i].getCentroid().getCoordAt(max.axis));
	    }
	}
	fprintf(stderr, "    RIGHT\n");
	for ( i=0; i<numClusters; i++ ) {
	    if ( flags[i] == RIGHT ) {
		fprintf(stderr,
			"    clusters[%d].getCentroid().getCoordAt(%d) = %g\n",
			i, max.axis,
			clusters[i].getCentroid().getCoordAt(max.axis));
	    }
	}
    }

    *flags_return = flags;

    HnFree(entries, sizeof(HnSRTreeAxisSortEntry) * numClusters);
}

/*
 * Remove
 */

void
HnSRTreeFileObj::remove(const HnPoint &point, const HnDataItem &dataItem)
{
    HnSRTreeStack stack;
    HnSRTreeLeaf leaf;
    HnSRTreeNode node;
    int cursor, maxCount, minCount;
    int level;
    HnBool underflow;
    HnSRTreeCluster cluster;
    int i;

    /* search point */
    if ( (stack = searchPoint(point, dataItem)) == HnSRTreeStack::null )
	HnAbort("the given point/dataItem pair is not found in the tree.");

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFile::remove: point is found.\n");
    }

    /* remove point */
    leaf = stack.getTopLeaf();
    cursor = stack.getCursor();
    stack.pop();

    leaf.removeElementAt(cursor);
	
    if ( stack.getDepth() == 0 ) {
	writeLeaf(leaf);

	if ( debug ) {
	    fprintf(stderr, "HnSRTreeFile::remove: removal is finished.\n");
	}

	return;
    }

    /* condense tree */
    reinsertList = new_HnSRTreeReinsertVector();
    reinsertedBlocks = new_HnFTlongVector();

    maxCount = HnSRTreeLeaf::getMaxCount(info);
    minCount = maxCount * info.getSplitFactor() / 100;
    underflow = (leaf.getCount() < minCount);

    if ( underflow ) {
	if ( debug ) {
	    fprintf(stderr, "HnSRTreeFile::remove: "
		    "underflow (count: %d, minCount: %d) "
		    "occurred in the leaf (offset: 0x%08X).\n",
		    leaf.getCount(), minCount,
		    (int)leaf.getOffset());
	}

	for ( i=0; i<leaf.getCount(); i++ ) {
	    HnPoint leafPoint = leaf.getPointAt(i);
	    HnDataItem leafDataItem = leaf.getDataItemAt(i);

	    reinsertList.addElement(new_HnSRTreeReinsert(leafPoint,
							 leafDataItem));
	}
	releaseBlock(leaf.getOffset());
    }
    else {
	writeLeaf(leaf);
	cluster = leaf.getCluster();
    }

    /* intermediate nodes */
    while ( stack.getDepth() > 1 ) {
	node = stack.getTopNode();
	cursor = stack.getCursor();
	level = info.getHeight() - stack.getDepth();
	stack.pop();

	if ( underflow ) {
	    node.removeElementAt(cursor);

	    maxCount = HnSRTreeNode::getMaxCount(info);
	    minCount = maxCount * info.getSplitFactor() / 100;
	    underflow = (node.getCount() < minCount);

	    if ( underflow ) {
		if ( debug ) {
		    fprintf(stderr,
			    "HnSRTreeFile::remove: "
			    "underflow (count: %d, minCount: %d) "
			    "occurred in the node "
			    "(offset: 0x%08X, level: %d).\n",
			    node.getCount(), minCount,
			    (int)node.getOffset(), level);
		}

		for ( i=0; i<node.getCount(); i++ ) {
		    long offset = node.getOffsetAt(i);
		    HnSRTreeReinsert reinsert;

		    reinsert = new_HnSRTreeReinsert(offset, level);
		    reinsertList.addElement(reinsert);
		}
		releaseBlock(node.getOffset());
	    }
	    else {
		writeNode(node);
		cluster = node.getCluster();
	    }
	}
	else {
	    node.setClusterAt(cluster, cursor);

	    writeNode(node);
	    cluster = node.getCluster();
	}
    }
		
    /* root node */
    node = stack.getTopNode();
    cursor = stack.getCursor();
    level = info.getHeight() - stack.getDepth();
    stack.pop();

    if ( underflow ) {
	node.removeElementAt(cursor);

	if ( node.getCount() == 1 ) {
	    if ( debug ) {
		fprintf(stderr, "HnSRTreeFile::remove: tree is shrunken.\n");
	    }

	    releaseBlock(node.getOffset());
	    info.setRootOffset(node.getOffsetAt(0));
	    info.setHeight(info.getHeight() - 1);

	    writeSuperBlock();
	}
	else {
	    writeNode(node);
	}
    }
    else {
	node.setClusterAt(cluster, cursor);

	writeNode(node);
    }

    /* reinsert orphaned entries */
    while ( reinsertList.size() != 0 ) {
	if ( reinsertList[0].getType() == HnSRTreeReinsert::POINT ) {
	    HnPoint point = reinsertList[0].getPoint();
	    HnDataItem dataItem = reinsertList[0].getDataItem();

	    if ( debug ) {
		fprintf(stderr,
			"HnSRTreeFile::remove: reinserting point %s.\n",
			(char *)point.toString());
	    }

	    reinsertList.removeElementAt(0);
	    insertPoint(point, dataItem);
	}
	else {
	    long offset = reinsertList[0].getOffset();
	    int level = reinsertList[0].getLevel();

	    if ( debug ) {
		fprintf(stderr, "HnSRTreeFile::remove: "
			"reinserting block 0x%08X at the level %d.\n",
			(int)offset, level);
	    }

	    reinsertList.removeElementAt(0);
	    insertBlock(offset, level);
	}
    }

    if ( debug ) {
	fprintf(stderr,	"HnSRTreeFile::remove: removal is finished.\n");
    }

    if ( debug ) {
	check();
    }
}

HnSRTreeStack
HnSRTreeFileObj::searchPoint(const HnPoint &point, const HnDataItem &dataItem)
{
    HnSRTreeStack stack;
    HnSRTreeBlock block;
    HnSRTreeNode node;
    HnSRTreeLeaf leaf;
    HnBool found;

    stack = new_HnSRTreeStack();

    block = readBlock(info.getRootOffset());
    if ( block.isNode() ) {
	node = new_HnSRTreeNode(info, block);
	stack.push(node);
    }
    else if ( block.isLeaf() ) {
	leaf = new_HnSRTreeLeaf(info, block);
	stack.push(leaf);
    }
    else {
	HnAbort("unexpected block type.");
    }

    for ( ;; ) {
	if ( stack.isTopNode() ) {
	    node = stack.getTopNode();
		
	    /*
	     * search for overlapping entries
	     */

	    if ( node.getCount() == 0 ) {
		found = HnFALSE;
	    }
	    else {
		for ( ;; ) {
		    int cursor = stack.getCursor();
		    HnRect rect = node.getClusterAt(cursor).getRect();

		    if ( debug ) {
			fprintf(stderr,
				"comparing with a rect #%d at a node %08X "
				"on the level %d.\n",
				cursor,
				(int)node.getOffset(), stack.getDepth() - 1);
		    }

		    if ( rect.includes(point) ) {
			found = HnTRUE;
			break;
		    }

		    if ( !stack.hasMore() ) {
			found = HnFALSE;
			break;
		    }

		    stack.advance();
		}
	    }

	    if ( found ) {
		int cursor = stack.getCursor();
		block = readBlock(node.getOffsetAt(cursor));

		if ( block.isNode() ) {
		    node = new_HnSRTreeNode(info, block);
		    stack.push(node);
		}
		else if ( block.isLeaf() ) {
		    leaf = new_HnSRTreeLeaf(info, block);
		    stack.push(leaf);
		}
		else {
		    HnAbort("unexpected block type.");
		}
	    }
	    else {
		/*
		 * go up until the top node has some remaining
		 * entries or the stack is empty.
		 */

		for ( ;; ) {
		    stack.pop();

		    if ( stack.getDepth() == 0 )
			return HnSRTreeStack::null;

		    if ( stack.hasMore() ) {
			break;
		    }
		}

		stack.advance();
	    }
	}
	else {
	    leaf = stack.getTopLeaf();

	    /*
	     * search for overlapping entries
	     */

	    if ( leaf.getCount() == 0 ) {
		found = HnFALSE;
	    }
	    else {
		for ( ;; ) {
		    int cursor = stack.getCursor();
		    HnPoint leafPoint = leaf.getPointAt(cursor);
		    HnDataItem leafDataItem = leaf.getDataItemAt(cursor);

		    if ( debug ) {
			fprintf(stderr,
				"comparing with a point #%d at a leaf %08X "
				"on the level %d.\n",
				cursor,
				(int)leaf.getOffset(), stack.getDepth() - 1);
		    }

		    if ( leafPoint.equals(point) &&
			 leafDataItem.equals(dataItem) ) {
			found = HnTRUE;
			break;
		    }

		    if ( !stack.hasMore() ) {
			found = HnFALSE;
			break;
		    }

		    stack.advance();
		}
	    }

	    if ( found ) {
		return stack;
	    }
	    else {
		/*
		 * go up until the top node has some remaining
		 * entries or the stack is empty.
		 */

		for ( ;; ) {
		    stack.pop();

		    if ( stack.getDepth() == 0 ) {
			return HnSRTreeStack::null;
		    }

		    if ( stack.hasMore() ) {
			break;
		    }
		}

		stack.advance();
	    }
	}
    }
}

