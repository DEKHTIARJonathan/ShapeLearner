/*
 * HnSRTreeFileObj3.cc (search methods)
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
 * $Id: HnSRTreeFileObj3.cc,v 1.9 2002/09/13 17:21:25 katayama Exp $
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
#include "HnSRTree/HnQuickSort.hh"

/*
 * Sequential Access
 */

void
HnSRTreeFileObj::getFirst(HnPoint *point_return, HnDataItem *dataItem_return)
{
    getFirst(NULL, point_return, dataItem_return);
}

class HnSRTreeRectQueryRegion: public HnSRTreeQueryRegion {
private:
    HnRect rect;
public:
    HnSRTreeRectQueryRegion(HnRect rect) {
	this->rect = rect;
    }
    HnBool overlaps(const HnSRTreeCluster &cluster) const {
	return (rect.overlaps(cluster.getRect()) &&
		rect.overlaps(cluster.getSphere()));
    }
    HnBool includes(const HnPoint &point) const {
	return rect.includes(point);
    }
};

void
HnSRTreeFileObj::getFirst(const HnRect &rect,
			  HnPoint *point_return, HnDataItem *dataItem_return)
{
    if ( rect == HnRect::null ) {
	getFirst(NULL, point_return, dataItem_return);
    }
    else {
	getFirst(new HnSRTreeRectQueryRegion(rect),
		 point_return, dataItem_return);
    }
}

class HnSRTreeSphereQueryRegion: public HnSRTreeQueryRegion {
private:
    HnSphere sphere;
public:
    HnSRTreeSphereQueryRegion(HnSphere sphere) {
	this->sphere = sphere;
    }
    HnBool overlaps(const HnSRTreeCluster &cluster) const {
	return (cluster.getRect().overlaps(sphere) &&
		cluster.getSphere().overlaps(sphere));
    }
    HnBool includes(const HnPoint &point) const {
	return sphere.includes(point);
    }
};

void
HnSRTreeFileObj::getFirst(const HnSphere &sphere,
			  HnPoint *point_return, HnDataItem *dataItem_return)
{
    if ( sphere == HnSphere::null ) {
	getFirst(NULL, point_return, dataItem_return);
    }
    else {
	getFirst(new HnSRTreeSphereQueryRegion(sphere),
		 point_return, dataItem_return);
    }
}

void
HnSRTreeFileObj::getFirst(HnSRTreeQueryRegion *queryRegion,
			  HnPoint *point_return, HnDataItem *dataItem_return)
{
    if ( context.queryRegion != NULL ) {
	delete context.queryRegion;
	context.queryRegion = NULL;
    }

    context.stack = new_HnSRTreeStack();
    context.queryRegion = queryRegion;

    getNext(point_return, dataItem_return);
}

void
HnSRTreeFileObj::getNext(HnPoint *point_return, HnDataItem *dataItem_return)
{
    HnSRTreeStack &stack = context.stack;
    HnSRTreeBlock block;
    HnSRTreeNode node;
    HnSRTreeLeaf leaf;
    HnBool found;

    if ( stack.getDepth() == 0 ) {
	/* initialize */
	block = readBlock(info.getRootOffset());
	if ( block.isNode() ) {
	    node = new_HnSRTreeNode(info, block);
	    stack.push(node);
	    profile->numVisitedNodes ++;
	}
	else if ( block.isLeaf() ) {
	    leaf = new_HnSRTreeLeaf(info, block);
	    stack.push(leaf);
	    profile->numVisitedLeaves ++;
	}
	else {
	    HnAbort("unexpected block type.");
	}
    }
    else {
	/* proceed */
	if ( stack.hasMore() ) {
	    stack.advance();
	}
	else {
	    for ( ;; ) {
		stack.pop();

		if ( stack.getDepth() == 0 ) {
		    *point_return = HnPoint::null;
		    *dataItem_return = HnDataItem::null;
		    return;
		}

		if ( stack.hasMore() ) {
		    break;
		}
	    }

	    stack.advance();
	}
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
	    else if ( context.queryRegion == NULL ) {
		found = HnTRUE;
	    }
	    else {
		for ( ;; ) {
		    int cursor = stack.getCursor();
		    HnSRTreeCluster cluster = node.getClusterAt(cursor);

		    if ( debug ) {
			fprintf(stderr,
				"comparing with a rect #%d at a node %08X "
				"on the level %d.\n",
				cursor,
				(int)node.getOffset(), stack.getDepth() - 1);
		    }

		    profile->numComparedNodeEntries ++;

		    if ( context.queryRegion->overlaps(cluster) ) {
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
		    profile->numVisitedNodes ++;
		}
		else if ( block.isLeaf() ) {
		    leaf = new_HnSRTreeLeaf(info, block);
		    stack.push(leaf);
		    profile->numVisitedLeaves ++;
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

		    if ( stack.getDepth() == 0 ) {
			*point_return = HnPoint::null;
			*dataItem_return = HnDataItem::null;
			return;
		    }

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
	    else if ( context.queryRegion == NULL ) {
		found = HnTRUE;
	    }
	    else {
		for ( ;; ) {
		    int cursor = stack.getCursor();
		    HnPoint point = leaf.getPointAt(cursor);

		    if ( debug ) {
			fprintf(stderr,
				"comparing with a point #%d at a leaf %08X "
				"on the level %d.\n",
				cursor,
				(int)leaf.getOffset(), stack.getDepth() - 1);
		    }

		    profile->numComparedLeafEntries ++;

		    if ( context.queryRegion->includes(point) ) {
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
		*point_return = leaf.getPointAt(cursor);
		*dataItem_return = leaf.getDataItemAt(cursor);
		return;
	    }
	    else {
		/*
		 * go up until the top node has some remaining
		 * entries or the stack is empty.
		 */

		for ( ;; ) {
		    stack.pop();

		    if ( stack.getDepth() == 0 ) {
			*point_return = HnPoint::null;
			*dataItem_return = HnDataItem::null;
			return;
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

class HnSRTreeRecordSort: HnQuickSort {
private:
    HnPointVector points;
    HnDataItemVector dataItems;

public:
    HnSRTreeRecordSort(HnPointVector points, HnDataItemVector dataItems) {
	this->points = points;
	this->dataItems = dataItems;
    }
    int getNumElements(void) {
	return points.size();
    }
    int compareElementsAt(int i, int j) {
	HnPoint point1 = points.elementAt(i);
	HnPoint point2 = points.elementAt(j);
	HnDataItem dataItem1 = dataItems.elementAt(i);
	HnDataItem dataItem2 = dataItems.elementAt(j);
	int axis, dimension;

	if ( (dimension = point1.getDimension()) != point2.getDimension() ) {
	    HnAbort("mismatch in dimensions.");
	}
	
	for ( axis=0; axis<dimension; axis++ ) {
	    double coord1 = point1.getCoordAt(axis);
	    double coord2 = point2.getCoordAt(axis);

	    if ( coord1 < coord2 ) {
		return -1;
	    }
	    if ( coord1 > coord2 ) {
		return 1;
	    }
	}

	if ( dataItem1.length() < dataItem2.length() ) {
	    return -1;
	}
	else if ( dataItem1.length() > dataItem2.length() ) {
	    return 1;
	}
	else {
	    return memcmp(dataItem1.toCharArray(), dataItem2.toCharArray(),
			  dataItem1.length());
	}
    }
    void swapElementsAt(int i, int j) {
	points.swapElementsAt(i, j);
	dataItems.swapElementsAt(i, j);
    }
    static void sort(HnPointVector points, HnDataItemVector dataItems) {
	HnSRTreeRecordSort sorter(points, dataItems);
	sorter.sortElements();
    }
};

void
HnSRTreeFileObj::getInRect(const HnRect &rect,
			   HnPointVector *points_return,
			   HnDataItemVector *dataItems_return)
{
    HnPointVector points;
    HnDataItemVector dataItems;
    HnPoint point;
    HnDataItem dataItem;

    /* collect results */
    points = new_HnPointVector();
    dataItems = new_HnDataItemVector();

    getFirst(rect, &point, &dataItem);

    while ( point != HnPoint::null ) {
	points.addElement(point);
	dataItems.addElement(dataItem);

	getNext(&point, &dataItem);
    }

    /* sort results */
    HnSRTreeRecordSort::sort(points, dataItems);

    *points_return = points;
    *dataItems_return = dataItems;
}

void
HnSRTreeFileObj::getInSphere(const HnSphere &sphere,
			     HnPointVector *points_return,
			     HnDataItemVector *dataItems_return)
{
    HnPointVector points;
    HnDataItemVector dataItems;
    HnPoint point;
    HnDataItem dataItem;

    /* collect results */
    points = new_HnPointVector();
    dataItems = new_HnDataItemVector();

    getFirst(sphere, &point, &dataItem);

    while ( point != HnPoint::null ) {
	points.addElement(point);
	dataItems.addElement(dataItem);

	getNext(&point, &dataItem);
    }

    /* sort results */
    HnSRTreeRecordSort::sort(points, dataItems);

    *points_return = points;
    *dataItems_return = dataItems;
}

/*
 * Neighbor Search
 */

void
HnSRTreeFileObj::getNeighbors(const HnPoint &queryPoint, int maxCount,
			      HnPointVector *points_return,
			      HnDataItemVector *dataItems_return)
{
    switch ( info.getNeighborAlgorithm() ) {
    case HnSRTreeInfo::DEPTH_FIRST:
	getNeighborsByDepthFirst(queryPoint, maxCount,
				 points_return, dataItems_return);
	break;
    case HnSRTreeInfo::BREADTH_FIRST:
	getNeighborsByBreadthFirst(queryPoint, maxCount,
				   points_return, dataItems_return);
	break;
    default:
	HnAbort("unexpected value for ``NeighborAlgorithm'': %d.",
		info.getNeighborAlgorithm());
    }
}

double
HnSRTreeFileObj::getMinDistance(const HnPoint &point,
				const HnSRTreeCluster &cluster)
{
    double sphereMinDistance, rectMinDistance, minDistance;

    /*
     * NOTE:
     *
     *    Dec.13,2000 Norio KATAYAMA
     *
     *    Here ``getLowerBoundMinDistance()'' is used for spheres in
     *    order to prevent false drops caused by round-off errors.
     *	  As for rectangles, it is not necessary to use
     *	  ``getLowerBoundMinDistance()'' because the distance obtained
     *	  by ``getMinDistance()'' is guaranteed to be equal to or
     *    less than the distance to the nearest point.
     *
     * Additional Note:
     *
     *    The above comment is satisfied only when the distance to
     *    a point is always measured by ``getDistance()'' and when
     *    bounding rectangles have the same precision with points.
     *    It is better to use ``getLowerBoundMinDistance()'' instead
     *    of ``getMinDistance()''.
     */

    /* sphere distance */
    sphereMinDistance = cluster.getSphere().getLowerBoundMinDistance(point);

    /* rect distance */
    rectMinDistance = cluster.getRect().getLowerBoundMinDistance(point);

    /* compare distances */
    if ( sphereMinDistance == rectMinDistance ) {
	minDistance = sphereMinDistance;
	profile->numEqualDistances ++;
    }
    else if ( sphereMinDistance > rectMinDistance ) {
	minDistance = sphereMinDistance;
	profile->numFartherSpheres ++;
    }
    else {
	minDistance = rectMinDistance;
	profile->numFartherRects ++;
    }

    return minDistance;
}

double
HnSRTreeFileObj::getMaxDistance(const HnPoint &point,
				const HnSRTreeCluster &cluster)
{
    double sphereMaxDistance, rectMaxDistance, maxDistance;

    /*
     * NOTE:
     *
     *    Dec.13,2000 Norio KATAYAMA
     *
     *    Here ``getUpperBoundMaxDistance()'' is used for spheres in
     *    order to prevent false drops caused by round-off errors.
     *	  As for rectangles, it is not necessary to use
     *	  ``getUpperBoundMaxDistance()'' because the distance obtained
     *	  by ``getMaxDistance()'' is guaranteed to be equal to or
     *    greater than the distance to the farthest point.
     *
     * Additional Note:
     *
     *    The above comment is satisfied only when the distance to
     *    a point is always measured by ``getDistance()'' and when
     *    bounding rectangles have the same precision with points.
     *    It is better to use ``getUpperBoundMaxDistance()'' instead
     *    of ``getMaxDistance()''.
     */

    /* sphere distance */
    sphereMaxDistance = cluster.getSphere().getUpperBoundMaxDistance(point);

    /* rect distance */
    rectMaxDistance = cluster.getRect().getUpperBoundMaxDistance(point);

    /* compare distances */
    if ( sphereMaxDistance < rectMaxDistance ) {
	maxDistance = sphereMaxDistance;
    }
    else {
	maxDistance = rectMaxDistance;
    }

    return maxDistance;
}

class HnSRTreeNeighborSearch: HnBinarySearch {
private:
    HnSRTreeNeighborVector vector;
    HnSRTreeNeighbor key;

private:
    int getNumElements(void) {
	return vector.size();
    }
    int compareToElementAt(int i) {
	return key.compareTo(vector.elementAt(i));
    }

    HnSRTreeNeighborSearch(const HnSRTreeNeighborVector &vector,
			   const HnSRTreeNeighbor &key) {
	this->vector = vector;
	this->key = key;
    }

public:
    static void search(const HnSRTreeNeighborVector &vector,
		       const HnSRTreeNeighbor &key,
		       HnBool *found_return, int *index_return) {
	HnSRTreeNeighborSearch searcher(vector, key);
	searcher.searchElements(found_return, index_return);
    }
};

struct HnSRTreeNodeNeighborEntry {
    int index;
    double minDistance;
    double maxDistance;
};

static int
HnSRTreeCompareNodeNeighborEntries(const void *ptr1, const void *ptr2)
{
    HnSRTreeNodeNeighborEntry *entry1 = (HnSRTreeNodeNeighborEntry *)ptr1;
    HnSRTreeNodeNeighborEntry *entry2 = (HnSRTreeNodeNeighborEntry *)ptr2;

    /* minDistance */
    if ( entry1->minDistance != entry2->minDistance ) {
	if ( entry1->minDistance < entry2->minDistance ) {
	    return -1;
	}
	else {
	    return 1;
	}
    }
    
    /* maxDistance */
    if ( entry1->maxDistance != entry2->maxDistance ) {
	if ( entry1->maxDistance < entry2->maxDistance ) {
	    return -1;
	}
	else {
	    return 1;
	}
    }

    /* index */
    if ( entry1->index != entry2->index ) {
	if ( entry1->index < entry2->index ) {
	    return -1;
	}
	else {
	    return 1;
	}
    }

    return 0;
}

/*
 * Neighbor Search (DepthFirst)
 */

void
HnSRTreeFileObj::getNeighborsByDepthFirst(const HnPoint &queryPoint,
					  int maxCount,
					  HnPointVector *points_return,
					  HnDataItemVector *dataItems_return)
{
    HnSRTreeNeighborVector neighbors;
    HnPointVector points;
    HnDataItemVector dataItems;
    int i;

    if ( queryPoint.getDimension() != getDimension() ) {
	HnAbort("mismatch in the dimensions (queryPoint: %d, file: %d)",
		queryPoint.getDimension(), getDimension());
    }

    if ( maxCount <= 0 ) {
	HnAbort("invalid max count %d.", maxCount);
    }

    neighbors = new_HnSRTreeNeighborVector();

    chooseNeighbors(info.getRootOffset(), queryPoint, maxCount, neighbors);

    /* make output */
    points = new_HnPointVector();
    dataItems = new_HnDataItemVector();

    for ( i=0; i<neighbors.size(); i++ ) {
	points.addElement(neighbors[i].getPoint());
	dataItems.addElement(neighbors[i].getDataItem());
    }

    *points_return = points;
    *dataItems_return = dataItems;
}

void
HnSRTreeFileObj::chooseNeighbors(long offset,
				 const HnPoint &queryPoint, int maxCount,
				 HnSRTreeNeighborVector &neighbors)
{
    HnSRTreeBlock block = readBlock(offset);

    if ( block.isNode() ) {
	chooseNeighborsInNode(block, queryPoint, maxCount, neighbors);
    }
    else if ( block.isLeaf() ) {
	chooseNeighborsInLeaf(block, queryPoint, maxCount, neighbors);
    }
    else {
	HnAbort("unexpected block type.");
    }
}

void
HnSRTreeFileObj::chooseNeighborsInNode(const HnSRTreeBlock &block,
				       const HnPoint &queryPoint, int maxCount,
				       HnSRTreeNeighborVector &neighbors)
{
    HnSRTreeNode node = new_HnSRTreeNode(info, block);
    int numEntries = node.getCount();
    int i;
    HnSRTreeNodeNeighborEntry *entries;
		
    profile->numVisitedNodes ++;
    profile->numComparedNodeEntries += numEntries;

    /* initialize entries */
    entries = (HnSRTreeNodeNeighborEntry *)
	HnMalloc(sizeof(HnSRTreeNodeNeighborEntry) * numEntries);

    for ( i=0; i<numEntries; i++ ) {
	entries[i].index = i;
	entries[i].minDistance =
	    getMinDistance(queryPoint, node.getClusterAt(i));
	entries[i].maxDistance =
	    getMaxDistance(queryPoint, node.getClusterAt(i));
    }

    /* sort entries */
    qsort(entries, numEntries, sizeof(HnSRTreeNodeNeighborEntry),
	  HnSRTreeCompareNodeNeighborEntries);

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::chooseNeighborsInNode: "
		"sorted rectangles\n");
	for ( i=0; i<numEntries; i++ ) {
	    HnRect rect = node.getClusterAt(entries[i].index).getRect();
	    fprintf(stderr,
		    "	 %2d: cnt = %7.6f, "
		    "min = %7.6f, max = %7.6f, inc = %s\n",
		    i, queryPoint.getDistance(rect.getCenterPoint()),
		    entries[i].minDistance, entries[i].maxDistance,
		    (rect.includes(queryPoint) ? "yes" : "no"));
	}
    }

    for ( i=0; i<numEntries; i++ ) {
	long offset = node.getOffsetAt(entries[i].index);
	double minDistance = entries[i].minDistance;

	if ( neighbors.size() < maxCount ) {
	    /*
	     * If the number of neighbors does not reach the max,
	     * try every children to collect neighbors.
	     */
	    chooseNeighbors(offset, queryPoint, maxCount, neighbors);
	}
	else {
	    HnSRTreeNeighbor &farthest = neighbors.lastElement();
	    double farthestDistance = farthest.getDistance();

	    if ( minDistance <= farthestDistance ) {
		chooseNeighbors(offset, queryPoint, maxCount, neighbors);
	    }
	}
    }

    HnFree(entries, sizeof(HnSRTreeNodeNeighborEntry) * numEntries);
}

void
HnSRTreeFileObj::chooseNeighborsInLeaf(const HnSRTreeBlock &block,
				       const HnPoint &queryPoint, int maxCount,
				       HnSRTreeNeighborVector &neighbors)
{
    HnSRTreeLeaf leaf = new_HnSRTreeLeaf(info, block);
    int numEntries = leaf.getCount();
    int i, count;

    profile->numVisitedLeaves ++;
    profile->numComparedLeafEntries += numEntries;

    /* add points in the leaf */
    for ( i=0; i<numEntries; i++ ) {
	HnPoint &point = leaf.getPointAt(i);
	HnDataItem &dataItem = leaf.getDataItemAt(i);
	double distance;
	HnSRTreeNeighbor newNeighbor;
	HnBool found;
	int index;

	distance = queryPoint.getDistance(point);

	/* skip such a point that is farther than the farthest candidate */
	if ( neighbors.size() >= maxCount &&
	     distance > neighbors.lastElement().getDistance() ) {
	    continue;
	}

	newNeighbor = new_HnSRTreeNeighbor(point, dataItem, distance);

	HnSRTreeNeighborSearch::search(neighbors, newNeighbor,
				       &found, &index);

	neighbors.insertElementAt(newNeighbor, index);
    }

    /* select closer neighbors up to the max count */
    for ( count = 0; count < neighbors.size(); count ++ ) {
	if ( count >= maxCount ) {
	    /*
	     * NOTE:
	     *	    Even if the count is greater than the max,
	     *	    points that have the same distance
	     *	    need to be appended together.
	     */

	    double lastDistance = neighbors[count - 1].getDistance();
	    double nextDistance = neighbors[count    ].getDistance();

	    if ( lastDistance != nextDistance ) {
		break;
	    }
	}
    }
    neighbors.setSize(count);

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::getNeighborsInLeaf: \n");
	for ( i=0; i<neighbors.size(); i++ ) {
	    fprintf(stderr, "	 neighbors[%d] = %s\n",
		    i, (char *)neighbors[i].toString());
	}
    }
}

/*
 * Neighbor Search (BreadthFirst)
 */

void
HnSRTreeFileObj::getNeighborsByBreadthFirst(const HnPoint &queryPoint,
					    int maxCount,
					    HnPointVector *points_return,
					    HnDataItemVector *dataItems_return)
{
    HnSRTreeNeighborVector neighbors = new_HnSRTreeNeighborVector();
    int numPointsInVector = 0;
    HnSRTreeBlock block;
    HnPointVector points;
    HnDataItemVector dataItems;
    int i;

    if ( queryPoint.getDimension() != info.getDimension() ) {
	HnAbort("mismatch in the dimensions (queryPoint: %d, file: %d)",
		queryPoint.getDimension(), info.getDimension());
    }

    if ( maxCount <= 0 ) {
	HnAbort("invalid max count %d.", maxCount);
    }

    insertNeighbors(neighbors, numPointsInVector, queryPoint, maxCount,
		    info.getRootOffset());

    for ( ;; ) {
	HnBool everyNeighborIsPoint = HnTRUE;
	long offset;
	int index;

	for ( index=0; index<maxCount && index<neighbors.size(); index++ ) {
	    if ( !neighbors.elementAt(index).isPoint() ) {
		everyNeighborIsPoint = HnFALSE;
		break;
	    }
	}
	if ( everyNeighborIsPoint ) {
	    break;
	}

	offset = neighbors.elementAt(index).getOffset();
	neighbors.removeElementAt(index);
	insertNeighbors(neighbors, numPointsInVector,
			queryPoint, maxCount, offset);

	/*
	 * Discard useless neighbors.
	 * It is unnecessary to keep points more than the max count.
	 */
	numPointsInVector = 0;
	for ( index=0; index<neighbors.size(); index++ ) {
	    if ( numPointsInVector >= maxCount ) {
		/*
		 * NOTE:
		 *	Even if the number of points is greater than
		 *	the max, neighbors that have the same distance
		 *	need to be appended together.
		 */
		double lastDistance = neighbors[index-1].getDistance();
		double nextDistance = neighbors[index  ].getDistance();

		if ( lastDistance != nextDistance ) {
		    break;
		}
	    }
	    if ( neighbors.elementAt(index).isPoint() ) {
		numPointsInVector ++;
	    }
	}
	neighbors.setSize(index);
    }

    /* make output */
    points = new_HnPointVector();
    dataItems = new_HnDataItemVector();

    for ( i=0; i<maxCount && i<neighbors.size(); i++ ) {
	points.addElement(neighbors.elementAt(i).getPoint());
	dataItems.addElement(neighbors.elementAt(i).getDataItem());
    }

    while ( i<neighbors.size() &&
	    neighbors.elementAt(i).isPoint() &&
	    neighbors.elementAt(i).getDistance() ==
	    neighbors.elementAt(i-1).getDistance() ) {
	points.addElement(neighbors.elementAt(i).getPoint());
	dataItems.addElement(neighbors.elementAt(i).getDataItem());
	i ++;
    }

    *points_return = points;
    *dataItems_return = dataItems;
}

void
HnSRTreeFileObj::insertNeighbors(HnSRTreeNeighborVector &neighbors,
				 int &numPointsInVector,
				 const HnPoint &queryPoint, int maxCount,
				 long offset)
{
    HnSRTreeBlock block = readBlock(offset);
    int i;

    if ( block.isLeaf() ) {
	HnSRTreeLeaf leaf = new_HnSRTreeLeaf(info, block);
	int numEntries = leaf.getCount();

	for ( i=0; i<numEntries; i++ ) {
	    HnPoint &point = leaf.getPointAt(i);
	    HnDataItem &dataItem = leaf.getDataItemAt(i);
	    double distance;
	    HnSRTreeNeighbor newNeighbor;

	    distance = point.getDistance(queryPoint);

	    /* skip such a point that is farther than the farthest candidate */
	    if ( numPointsInVector >= maxCount &&
		 distance > neighbors.lastElement().getDistance() ) {
		continue;
	    }

	    newNeighbor = new_HnSRTreeNeighbor(point, dataItem, distance);

	    insertNeighbor(neighbors, newNeighbor);

	    numPointsInVector ++;
	}

	profile->numVisitedLeaves ++;
	profile->numComparedLeafEntries += numEntries;
    }
    else {
	HnSRTreeNode node = new_HnSRTreeNode(info, block);
	int numEntries = node.getCount();

	for ( i=0; i<numEntries; i++ ) {
	    double distance;
	    HnSRTreeNeighbor newNeighbor;

	    distance = getMinDistance(queryPoint, node.getClusterAt(i));

	    /* skip such a node that is farther than the farthest candidate */
	    if ( numPointsInVector >= maxCount &&
		 distance > neighbors.lastElement().getDistance() ) {
		continue;
	    }

	    newNeighbor = new_HnSRTreeNeighbor(node.getOffsetAt(i), distance);

	    insertNeighbor(neighbors, newNeighbor);
	}

	profile->numVisitedNodes ++;
	profile->numComparedNodeEntries += numEntries;
    }
}

void
HnSRTreeFileObj::insertNeighbor(HnSRTreeNeighborVector &neighbors,
				const HnSRTreeNeighbor &newNeighbor)
{
    HnBool found;
    int index;

    HnSRTreeNeighborSearch::search(neighbors, newNeighbor, &found, &index);

    neighbors.insertElementAt(newNeighbor, index);
}

/*
 * Colored Neighbor Search
 */

void
HnSRTreeFileObj::getColoredNeighbors(const HnPointVector &queryPoints,
				     int maxCount,
				     HnPointVector *points_return,
				     HnDataItemVector *dataItems_return)
{
    getColoredNeighbors(queryPoints, maxCount, points_return, dataItems_return,
			NULL);
}

static int
defaultCompColorsFunc(const void *data1, int size1,
		      const void *data2, int size2)
{
    if ( size1 == size2 ) {
	return memcmp(data1, data2, size1);
    }
    else if ( size1 < size2 ) {
	return -1;
    }
    else {
	return 1;
    }
}

void
HnSRTreeFileObj::getColoredNeighbors(const HnPointVector &queryPoints,
				     int maxCount,
				     HnPointVector *points_return,
				     HnDataItemVector *dataItems_return,
				     HnSRTreeCompColorsFunc *compColorsFunc)
{
    switch ( info.getNeighborAlgorithm() ) {
    case HnSRTreeInfo::DEPTH_FIRST:
	getColoredNeighborsByDepthFirst(queryPoints, maxCount,
					points_return, dataItems_return,
					compColorsFunc);
	break;
    case HnSRTreeInfo::BREADTH_FIRST:
	getColoredNeighborsByBreadthFirst(queryPoints, maxCount,
					  points_return, dataItems_return,
					  compColorsFunc);
	break;
    default:
	HnAbort("unexpected value for ``NeighborAlgorithm'': %d.",
		info.getNeighborAlgorithm());
    }
}

double
HnSRTreeFileObj::getDistance(const HnPointVector &queryPoints,
			     const HnPoint &point) 
{
    int i;
    double min;

    min = 0;
    for ( i=0; i<queryPoints.size(); i++ ) {
	double distance = queryPoints[i].getDistance(point);

	if ( i == 0 || distance < min ) {
	    min = distance;
	}
    }

    return min;
}

double
HnSRTreeFileObj::getMinDistance(const HnPointVector &queryPoints,
				const HnSRTreeCluster &cluster)
{
    int i;
    double min;

    min = 0;
    for ( i=0; i<queryPoints.size(); i++ ) {
	double distance = getMinDistance(queryPoints[i], cluster);

	if ( i == 0 || distance < min ) {
	    min = distance;
	}
    }

    return min;
}

double
HnSRTreeFileObj::getMaxDistance(const HnPointVector &queryPoints,
				const HnSRTreeCluster &cluster)
{
    int i;
    double max;

    max = 0;
    for ( i=0; i<queryPoints.size(); i++ ) {
	double distance = getMaxDistance(queryPoints[i], cluster);

	if ( i == 0 || distance > max ) {
	    max = distance;
	}
    }

    return max;
}

class HnSRTreeColoredNeighborSearch: HnBinarySearch {
private:
    HnSRTreeNeighborVector vector;
    HnSRTreeNeighbor key;

    HnSRTreeCompColorsFunc *compColorsFunc;
    enum Type { DISTANCE_VECTOR, COLOR_VECTOR } type;

private:
    int getNumElements(void) {
	return vector.size();
    }
    int compareToElementAt(int i) {
	HnSRTreeNeighbor element = vector.elementAt(i);

	if ( key.isPoint() && element.isPoint() ) {
	    if ( type == DISTANCE_VECTOR ) {
		double distance1 = key.getDistance();
		double distance2 = element.getDistance();

		if ( distance1 == distance2 ) {
		    HnDataItem dataItem1 = key.getDataItem();
		    HnDataItem dataItem2 = element.getDataItem();

		    return compColorsFunc(dataItem1.toCharArray(),
					  dataItem1.length(),
					  dataItem2.toCharArray(),
					  dataItem2.length());
		}
		else if ( distance1 < distance2 ) {
		    return -1;
		}
		else {
		    return 1;
		}
	    }
	    else if ( type == COLOR_VECTOR ) {
		HnDataItem dataItem1 = key.getDataItem();
		HnDataItem dataItem2 = element.getDataItem();

		return compColorsFunc(dataItem1.toCharArray(),
				      dataItem1.length(),
				      dataItem2.toCharArray(),
				      dataItem2.length());
	    }
	    else {
		HnAbort("invalid value for ``type'': %d", type);
	    }
	}
	else {
	    return key.compareTo(element);
	}
    }

    HnSRTreeColoredNeighborSearch(const HnSRTreeNeighborVector &vector,
				  const HnSRTreeNeighbor &key,
				  HnSRTreeCompColorsFunc *compColorsFunc,
				  Type type) {
	this->vector = vector;
	this->key = key;

	this->compColorsFunc = compColorsFunc;
	this->type = type;
    }

public:
    static void searchDistanceVector(const HnSRTreeNeighborVector &vector,
				     const HnSRTreeNeighbor &key,
				     HnSRTreeCompColorsFunc *compColorsFunc,
				     HnBool *found_return, int *index_return) {
	HnSRTreeColoredNeighborSearch searcher(vector, key, compColorsFunc,
					       DISTANCE_VECTOR);
	searcher.searchElements(found_return, index_return);
    }
    static void searchColorVector(const HnSRTreeNeighborVector &vector,
				  const HnSRTreeNeighbor &key,
				  HnSRTreeCompColorsFunc *compColorsFunc,
				  HnBool *found_return, int *index_return) {
	HnSRTreeColoredNeighborSearch searcher(vector, key, compColorsFunc,
					       COLOR_VECTOR);
	searcher.searchElements(found_return, index_return);
    }
};

void
HnSRTreeFileObj::
searchDistanceVector(const HnSRTreeNeighborVector &vector,
		     const HnSRTreeNeighbor &key,
		     HnSRTreeCompColorsFunc *compColorsFunc,
		     HnBool *found_return, int *index_return)
{
    HnSRTreeColoredNeighborSearch::
	searchDistanceVector(vector, key, compColorsFunc,
			     found_return, index_return);
}

void
HnSRTreeFileObj::
searchColorVector(const HnSRTreeNeighborVector &vector,
		  const HnSRTreeNeighbor &key,
		  HnSRTreeCompColorsFunc *compColorsFunc,
		  HnBool *found_return, int *index_return)
{
    HnSRTreeColoredNeighborSearch::
	searchColorVector(vector, key, compColorsFunc,
			  found_return, index_return);
}

/*
 * Colored Neighbor Search (DepthFirst)
 */

void
HnSRTreeFileObj::
getColoredNeighborsByDepthFirst(const HnPointVector &queryPoints, int maxCount,
				HnPointVector *points_return,
				HnDataItemVector *dataItems_return,
				HnSRTreeCompColorsFunc *compColorsFunc)
{
    HnSRTreeNeighborVector distanceVector, colorVector;
    HnPointVector points;
    HnDataItemVector dataItems;
    int i;

    if ( compColorsFunc == NULL ) {
	compColorsFunc = defaultCompColorsFunc;
    }

    if ( queryPoints.size() == 0 ) {
	HnAbort("no query point is given.");
    }

    if ( maxCount <= 0 ) {
	HnAbort("invalid max count %d.", maxCount);
    }

    distanceVector = new_HnSRTreeNeighborVector();
    colorVector = new_HnSRTreeNeighborVector();

    chooseColoredNeighbors(info.getRootOffset(), queryPoints, maxCount,
			   distanceVector, colorVector, compColorsFunc);

    /* make output */
    points = new_HnPointVector();
    dataItems = new_HnDataItemVector();

    for ( i=0; i<distanceVector.size(); i++ ) {
	points.addElement(distanceVector[i].getPoint());
	dataItems.addElement(distanceVector[i].getDataItem());
    }

    *points_return = points;
    *dataItems_return = dataItems;
}

void
HnSRTreeFileObj::chooseColoredNeighbors(long offset,
					const HnPointVector &queryPoints,
					int maxCount,
					HnSRTreeNeighborVector &distanceVector,
					HnSRTreeNeighborVector &colorVector,
					HnSRTreeCompColorsFunc *compColorsFunc)
{
    HnSRTreeBlock block = readBlock(offset);

    if ( block.isNode() ) {
	chooseColoredNeighborsInNode(block, queryPoints, maxCount,
				     distanceVector, colorVector,
				     compColorsFunc);
    }
    else if ( block.isLeaf() ) {
	chooseColoredNeighborsInLeaf(block, queryPoints, maxCount,
				     distanceVector, colorVector,
				     compColorsFunc);
    }
    else {
	HnAbort("unexpected block type.");
    }
}

void
HnSRTreeFileObj::
chooseColoredNeighborsInNode(const HnSRTreeBlock &block,
			     const HnPointVector &queryPoints, int maxCount,
			     HnSRTreeNeighborVector &distanceVector,
			     HnSRTreeNeighborVector &colorVector,
			     HnSRTreeCompColorsFunc *compColorsFunc)
{
    HnSRTreeNode node = new_HnSRTreeNode(info, block);
    int numEntries = node.getCount();
    int i;
    HnSRTreeNodeNeighborEntry *entries;
		
    profile->numVisitedNodes ++;
    profile->numComparedNodeEntries += numEntries;

    /* initialize entries */
    entries = (HnSRTreeNodeNeighborEntry *)
	HnMalloc(sizeof(HnSRTreeNodeNeighborEntry) * numEntries);

    for ( i=0; i<numEntries; i++ ) {
	entries[i].index = i;
	entries[i].minDistance =
	    getMinDistance(queryPoints, node.getClusterAt(i));
	entries[i].maxDistance =
	    getMaxDistance(queryPoints, node.getClusterAt(i));
    }

    /* sort entries */
    qsort(entries, numEntries, sizeof(HnSRTreeNodeNeighborEntry),
	  HnSRTreeCompareNodeNeighborEntries);

    for ( i=0; i<numEntries; i++ ) {
	long offset = node.getOffsetAt(entries[i].index);
	double minDistance = entries[i].minDistance;

	if ( distanceVector.size() < maxCount ) {
	    /*
	     * If the number of neighbors does not reach the max,
	     * try every children to collect neighbors.
	     */
	    chooseColoredNeighbors(offset, queryPoints, maxCount,
				   distanceVector, colorVector,
				   compColorsFunc);
	}
	else {
	    HnSRTreeNeighbor &farthest = distanceVector.lastElement();
	    double farthestDistance = farthest.getDistance();

	    if ( minDistance <= farthestDistance ) {
		chooseColoredNeighbors(offset, queryPoints, maxCount,
				       distanceVector, colorVector,
				       compColorsFunc);
	    }
	}
    }

    HnFree(entries, sizeof(HnSRTreeNodeNeighborEntry) * numEntries);
}

void
HnSRTreeFileObj::
chooseColoredNeighborsInLeaf(const HnSRTreeBlock &block,
			     const HnPointVector &queryPoints, int maxCount,
			     HnSRTreeNeighborVector &distanceVector,
			     HnSRTreeNeighborVector &colorVector,
			     HnSRTreeCompColorsFunc *compColorsFunc)
{
    HnSRTreeLeaf leaf = new_HnSRTreeLeaf(info, block);
    int numEntries = leaf.getCount();
    int i, count;

    profile->numVisitedLeaves ++;
    profile->numComparedLeafEntries += numEntries;

    /* add points in the leaf */
    for ( i=0; i<numEntries; i++ ) {
	HnPoint &point = leaf.getPointAt(i);
	HnDataItem &dataItem = leaf.getDataItemAt(i);
	double distance;
	HnSRTreeNeighbor newNeighbor;
	HnBool found;
	int colorIndex, distanceIndex;

	distance = getDistance(queryPoints, point);

	/* skip such a point that is farther than the farthest candidate */
	if ( distanceVector.size() >= maxCount &&
	     distance > distanceVector.lastElement().getDistance() ) {
	    continue;
	}

	newNeighbor = new_HnSRTreeNeighbor(point, dataItem, distance);

	searchColorVector(colorVector, newNeighbor, compColorsFunc,
			  &found, &colorIndex);

	if ( found ) {
	    /* a point with the same color already exists. */
	    HnSRTreeNeighbor oldNeighbor = colorVector[colorIndex];

	    /* skip unless the new point is closer than the existing one. */
	    if ( distance > oldNeighbor.getDistance() ||
		 (distance == oldNeighbor.getDistance() &&
		  newNeighbor.compareTo(oldNeighbor) >= 0) ) {
		continue;
	    }

	    searchDistanceVector(distanceVector, oldNeighbor, compColorsFunc,
				 &found, &distanceIndex);
	    if ( !found ) {
		HnAbort("inconsistency between `distanceVector' and "
			"`colorVector'");
	    }
	    distanceVector.removeElementAt(distanceIndex);

	    searchDistanceVector(distanceVector, newNeighbor, compColorsFunc,
				 &found, &distanceIndex);
	    distanceVector.insertElementAt(newNeighbor, distanceIndex);

	    colorVector.setElementAt(newNeighbor, colorIndex);
	}
	else {
	    /* no point has the same color. */
	    searchDistanceVector(distanceVector, newNeighbor, compColorsFunc,
				 &found, &distanceIndex);
	    distanceVector.insertElementAt(newNeighbor, distanceIndex);

	    searchColorVector(colorVector, newNeighbor, compColorsFunc,
			      &found, &colorIndex);
	    colorVector.insertElementAt(newNeighbor, colorIndex);
	}
    }

    /* select closer neighbors up to the max count */
    for ( count = 0; count < distanceVector.size(); count ++ ) {
	if ( count >= maxCount ) {
	    /*
	     * NOTE:
	     *	    Even if the count is greater than the max,
	     *	    points that have the same distance
	     *	    need to be appended together.
	     */

	    double lastDistance = distanceVector[count - 1].getDistance();
	    double nextDistance = distanceVector[count	  ].getDistance();

	    if ( lastDistance != nextDistance ) {
		break;
	    }
	}
    }
    /* points must be removed from both `distanceVector' and `colorVector'. */
    for ( i=count; i<distanceVector.size(); i++ ) {
	HnBool found;
	int index;

	searchColorVector(colorVector, distanceVector[i], compColorsFunc,
			  &found, &index);
	if ( !found ) {
	    HnAbort("inconsistency between `distanceVector' and "
		    "`colorVector'");
	}
	colorVector.removeElementAt(index);
    }
    /* set the size of ``distanceVector'' */
    distanceVector.setSize(count);

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::getNeighborsInLeaf: \n");
	for ( i=0; i<distanceVector.size(); i++ ) {
	    fprintf(stderr, "	 distanceVector[%d] = %s\n",
		    i, (char *)distanceVector[i].toString());
	}
    }
}

/*
 * Colored Neighbor Search (BreadthFirst)
 */

void
HnSRTreeFileObj::
getColoredNeighborsByBreadthFirst(const HnPointVector &queryPoints,
				  int maxCount,
				  HnPointVector *points_return,
				  HnDataItemVector *dataItems_return,
				  HnSRTreeCompColorsFunc *compColorsFunc)
{
    HnSRTreeNeighborVector distanceVector, colorVector;
    int numPointsInVector;
    HnSRTreeBlock block;
    HnPointVector points;
    HnDataItemVector dataItems;
    int i;

    if ( compColorsFunc == NULL ) {
	compColorsFunc = defaultCompColorsFunc;
    }

    if ( queryPoints.size() == 0 ) {
	HnAbort("no query point is given.");
    }

    if ( maxCount <= 0 ) {
	HnAbort("invalid max count %d.", maxCount);
    }

    distanceVector = new_HnSRTreeNeighborVector();
    colorVector = new_HnSRTreeNeighborVector();
    numPointsInVector = 0;

    insertColoredNeighbors(distanceVector, colorVector, numPointsInVector,
			   queryPoints, maxCount,
			   info.getRootOffset(), compColorsFunc);

    for ( ;; ) {
	HnBool everyNeighborIsPoint = HnTRUE;
	long offset;
	int index;

	for ( index=0;
	      index<maxCount && index<distanceVector.size(); index++ ) {
	    if ( !distanceVector.elementAt(index).isPoint() ) {
		everyNeighborIsPoint = HnFALSE;
		break;
	    }
	}
	if ( everyNeighborIsPoint ) {
	    break;
	}

	offset = distanceVector.elementAt(index).getOffset();
	distanceVector.removeElementAt(index);
	insertColoredNeighbors(distanceVector, colorVector, numPointsInVector,
			       queryPoints, maxCount, offset, compColorsFunc);

	/*
	 * Discard useless neighbors.
	 * It is unnecessary to keep points more than the max count.
	 */
	numPointsInVector = 0;
	for ( index=0; index<distanceVector.size(); index++ ) {
	    if ( numPointsInVector >= maxCount ) {
		/*
		 * NOTE:
		 *	Even if the number of points is greater than
		 *	the max, neighbors that have the same distance
		 *	need to be appended together.
		 */
		double lastDistance = distanceVector[index-1].getDistance();
		double nextDistance = distanceVector[index  ].getDistance();

		if ( lastDistance != nextDistance ) {
		    break;
		}
	    }
	    if ( distanceVector.elementAt(index).isPoint() ) {
		numPointsInVector ++;
	    }
	}
	/*
	 * points must be removed from both `distanceVector' and
	 * `colorVector'.
	 */
	for ( i=index; i<distanceVector.size(); i++ ) {
	    if ( distanceVector[i].isPoint() ) {
		HnBool found;
		int colorIndex;

		searchColorVector(colorVector, distanceVector[i],
				  compColorsFunc,
				  &found, &colorIndex);
		if ( !found ) {
		    HnAbort("inconsistency between `distanceVector' and "
			    "`colorVector'");
		}
		colorVector.removeElementAt(colorIndex);
	    }
	}
	/* set the size of ``distanceVector'' */
	distanceVector.setSize(index);
    }

    /* make output */
    points = new_HnPointVector();
    dataItems = new_HnDataItemVector();

    for ( i=0; i<maxCount && i<distanceVector.size(); i++ ) {
	points.addElement(distanceVector.elementAt(i).getPoint());
	dataItems.addElement(distanceVector.elementAt(i).getDataItem());
    }

    while ( i<distanceVector.size() &&
	    distanceVector.elementAt(i).isPoint() &&
	    distanceVector.elementAt(i).getDistance() ==
	    distanceVector.elementAt(i-1).getDistance() ) {
	points.addElement(distanceVector.elementAt(i).getPoint());
	dataItems.addElement(distanceVector.elementAt(i).getDataItem());
	i ++;
    }

    *points_return = points;
    *dataItems_return = dataItems;
}

void
HnSRTreeFileObj::insertColoredNeighbors(HnSRTreeNeighborVector &distanceVector,
					HnSRTreeNeighborVector &colorVector,
					int &numPointsInVector,
					const HnPointVector &queryPoints,
					int maxCount,
					long offset,
					HnSRTreeCompColorsFunc *compColorsFunc)
{
    HnSRTreeBlock block = readBlock(offset);
    int i;

    if ( block.isLeaf() ) {
	HnSRTreeLeaf leaf = new_HnSRTreeLeaf(info, block);
	int numEntries = leaf.getCount();

	for ( i=0; i<numEntries; i++ ) {
	    HnPoint &point = leaf.getPointAt(i);
	    HnDataItem &dataItem = leaf.getDataItemAt(i);
	    double distance;
	    HnSRTreeNeighbor newNeighbor;

	    distance = getDistance(queryPoints, point);

	    /* skip such a point that is farther than the farthest candidate */
	    if ( numPointsInVector >= maxCount &&
		 distance > distanceVector.lastElement().getDistance() ) {
		continue;
	    }

	    newNeighbor = new_HnSRTreeNeighbor(point, dataItem, distance);

	    insertColoredNeighbor(distanceVector, colorVector,
				  numPointsInVector,
				  newNeighbor, compColorsFunc);
	}

	profile->numVisitedLeaves ++;
	profile->numComparedLeafEntries += numEntries;
    }
    else {
	HnSRTreeNode node = new_HnSRTreeNode(info, block);
	int numEntries = node.getCount();

	for ( i=0; i<numEntries; i++ ) {
	    double distance;
	    HnSRTreeNeighbor newNeighbor;

	    distance = getMinDistance(queryPoints, node.getClusterAt(i));

	    /* skip such a node that is farther than the farthest candidate */
	    if ( numPointsInVector >= maxCount &&
		 distance > distanceVector.lastElement().getDistance() ) {
		continue;
	    }

	    newNeighbor = new_HnSRTreeNeighbor(node.getOffsetAt(i), distance);

	    insertColoredNeighbor(distanceVector, colorVector,
				  numPointsInVector,
				  newNeighbor, compColorsFunc);
	}

	profile->numVisitedNodes ++;
	profile->numComparedNodeEntries += numEntries;
    }
}

void
HnSRTreeFileObj::insertColoredNeighbor(HnSRTreeNeighborVector &distanceVector,
				       HnSRTreeNeighborVector &colorVector,
				       int &numPointsInVector,
				       const HnSRTreeNeighbor &newNeighbor,
				       HnSRTreeCompColorsFunc *compColorsFunc)
{
    if ( newNeighbor.isPoint() ) {
	/* new neighbor is a point */
	HnBool found;
	int colorIndex, distanceIndex;

	searchColorVector(colorVector, newNeighbor, compColorsFunc,
			  &found, &colorIndex);

	if ( found ) {
	    /* a point with the same color already exists. */
	    HnSRTreeNeighbor oldNeighbor = colorVector[colorIndex];

	    /* skip unless the new point is closer than the existing one. */
	    if ( newNeighbor.getDistance() > oldNeighbor.getDistance() ||
		 (newNeighbor.getDistance() == oldNeighbor.getDistance() &&
		  newNeighbor.compareTo(oldNeighbor) >= 0) ) {
		return;
	    }

	    searchDistanceVector(distanceVector, oldNeighbor, compColorsFunc,
				 &found, &distanceIndex);
	    if ( !found ) {
		HnAbort("inconsistency between `distanceVector' and "
			"`colorVector'");
	    }
	    distanceVector.removeElementAt(distanceIndex);

	    searchDistanceVector(distanceVector, newNeighbor, compColorsFunc,
				 &found, &distanceIndex);
	    distanceVector.insertElementAt(newNeighbor, distanceIndex);

	    colorVector.setElementAt(newNeighbor, colorIndex);
	}
	else {
	    /* no point has the same color. */
	    searchDistanceVector(distanceVector, newNeighbor, compColorsFunc,
				 &found, &distanceIndex);
	    distanceVector.insertElementAt(newNeighbor, distanceIndex);

	    searchColorVector(colorVector, newNeighbor, compColorsFunc,
			      &found, &colorIndex);
	    colorVector.insertElementAt(newNeighbor, colorIndex);

	    numPointsInVector ++;
	}
    }
    else {
	/* new neighbor is not a point */
	HnBool found;
	int index;

	searchDistanceVector(distanceVector, newNeighbor, compColorsFunc,
			     &found, &index);

	distanceVector.insertElementAt(newNeighbor, index);
    }
}
