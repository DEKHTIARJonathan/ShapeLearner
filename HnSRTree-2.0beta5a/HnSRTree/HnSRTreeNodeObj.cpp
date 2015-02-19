/*
 * HnSRTreeNodeObj.cc
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
 * 03/22/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeNodeObj.cc,v 1.8 2000/12/15 12:44:48 katayama Exp $
 */

#include "HnSRTree/HnSRTreeNode.hh"
#include "HnSRTree/HnSRTreeNodeObj.hh"
#include "HnSRTree/HnFPControl.h"

void
HnSRTreeNodeObj::initialize(const HnSRTreeInfo &info, long offset)
{
    this->info = info;

    this->maxCount = getMaxCount(info);

    this->offset = offset;

    clusters = new_HnSRTreeClusterVector();
    offsets = new_HnFTlongVector();
}

int
HnSRTreeNodeObj::getContentSize(void) const
{
    int blockSize = info.getBlockSize();
    int bodyCapacity = HnSRTreeBlock::getBodyCapacity(info);
    int entrySize = getEntrySize(info);

    return (blockSize - bodyCapacity) +
	HnBlockStream::INT_SIZE + entrySize * getCount();
}

int
HnSRTreeNodeObj::getMaxCount(const HnSRTreeInfo &info)
{
    int bodyCapacity = HnSRTreeBlock::getBodyCapacity(info);
    int entrySize = getEntrySize(info);

    return (bodyCapacity - HnBlockStream::INT_SIZE) / entrySize;
}

/*
 * Entry
 */

void
HnSRTreeNodeObj::writeEntryTo(const HnSRTreeInfo &info,
			      const HnSRTreeCluster &cluster, long offset,
			      HnBlockStream &blockStream)
{
    writeClusterTo(info, cluster, blockStream);
    blockStream.writeLong(offset);
}

void
HnSRTreeNodeObj::readEntryFrom(const HnSRTreeInfo &info,
			       HnBlockStream &blockStream,
			       HnSRTreeCluster *cluster_return,
			       long *offset_return)
{
    *cluster_return = readClusterFrom(info, blockStream);
    *offset_return = blockStream.readLong();
}

int
HnSRTreeNodeObj::getEntrySize(const HnSRTreeInfo &info)
{
    return getClusterSize(info) + HnBlockStream::LONG_SIZE;
}

/*
 * Cluster
 */

void
HnSRTreeNodeObj::writeClusterTo(const HnSRTreeInfo &info,
				const HnSRTreeCluster &cluster,
				HnBlockStream &blockStream)
{
    if ( info.getNonLeafFloatType() == HnSRTreeInfo::NON_LEAF_DOUBLE ) {
	/* centroid */
	cluster.getCentroid().writeTo(blockStream);

	/* weight */
	blockStream.writeInt(cluster.getWeight());

	/*
	 * NOTE:
	 *	    It is not necessary to write out the center of the sphere,
	 *	    since the center is either `centroid' or `the center of the
	 *      rectangle'.
	 */

	/* radius */
	blockStream.writeDouble(cluster.getSphere().getRadius());

	/* rect */
	cluster.getRect().writeTo(blockStream);
    }
    else if ( info.getNonLeafFloatType() == HnSRTreeInfo::NON_LEAF_SINGLE ) {
	int dimension = info.getDimension();
	int axis;
	HnPoint centroid = cluster.getCentroid();
	HnRect rect = cluster.getRect();

	/* centroid */
	for ( axis=0; axis<dimension; axis++ ) {
	    blockStream.writeFloat(centroid.getCoordAt(axis));
	}

	/* weight */
	blockStream.writeInt(cluster.getWeight());

	/* radius */
	blockStream.writeFloat(cluster.getSphere().getRadius());

	/* rect */
	for ( axis=0; axis<dimension; axis++ ) {
	    blockStream.writeFloat(rect.getRangeAt(axis).getMin());
	    blockStream.writeFloat(rect.getRangeAt(axis).getMax());
	}
    }
    else {
	HnAbort("unexpected value for ``NonLeafFloatType'' (%d).",
		info.getNonLeafFloatType());
    }
}

HnSRTreeCluster
HnSRTreeNodeObj::readClusterFrom(const HnSRTreeInfo &info,
				 HnBlockStream &blockStream)
{
    if ( info.getNonLeafFloatType() == HnSRTreeInfo::NON_LEAF_DOUBLE ) {
	int dimension = info.getDimension();
	HnPoint centroid;
	int weight;
	double radius;
	HnRect rect;
	HnSphere sphere;

	/* centroid */
	centroid = new_HnPoint(dimension, blockStream);

	/* weight */
	weight = blockStream.readInt();

	/* radius */
	radius = blockStream.readDouble();

	/* rect */
	rect = new_HnRect(dimension, blockStream);

	/* sphere */
	sphere = new_HnSphere(centroid, radius);

	return new_HnSRTreeCluster(centroid, weight, sphere, rect);
    }
    else if ( info.getNonLeafFloatType() == HnSRTreeInfo::NON_LEAF_SINGLE ) {
	int dimension = info.getDimension();
	int axis;
	HnPoint centroid;
	int weight;
	double radius;
	HnRect rect;
	HnSphere sphere;

	/* centroid */
	centroid = new_HnPoint(dimension);
	for ( axis=0; axis<dimension; axis++ ) {
	    centroid.setCoordAt(blockStream.readFloat(), axis);
	}

	/* weight */
	weight = blockStream.readInt();

	/* radius */
	radius = blockStream.readFloat();

	/* rect */
	rect = new_HnRect(dimension);
	for ( axis=0; axis<dimension; axis++ ) {
	    rect.setRangeAt(blockStream.readFloat(),
			    blockStream.readFloat(),
			    axis);
	}

	/* sphere */
	sphere = new_HnSphere(centroid, radius);

	return new_HnSRTreeCluster(centroid, weight, sphere, rect);
    }
    else {
	HnAbort("unexpected value for ``NonLeafFloatType'' (%d).",
		info.getNonLeafFloatType());
    }
}

int
HnSRTreeNodeObj::getClusterSize(const HnSRTreeInfo &info)
{
    if ( info.getNonLeafFloatType() == HnSRTreeInfo::NON_LEAF_DOUBLE ) {
	int dimension = info.getDimension();
	int size = 0;

	/* centroid */
	size += HnPoint::getSizeInBlockStream(dimension);

	/* weight */
	size += HnBlockStream::INT_SIZE;

	/* radius */
	size += HnBlockStream::DOUBLE_SIZE;

	/* rect */
	size += HnRect::getSizeInBlockStream(dimension);

	return size;
    }
    else if ( info.getNonLeafFloatType() == HnSRTreeInfo::NON_LEAF_SINGLE ) {
	int dimension = info.getDimension();
	int size = 0;

	/* centroid */
	size += HnBlockStream::FLOAT_SIZE * dimension;

	/* weight */
	size += HnBlockStream::INT_SIZE;

	/* radius */
	size += HnBlockStream::FLOAT_SIZE;

	/* rect */
	size += HnBlockStream::FLOAT_SIZE * dimension * 2;

	return size;
    }
    else {
	HnAbort("unexpected value for ``NonLeafFloatType'' (%d).",
		info.getNonLeafFloatType());
    }
}

/*
 * Constructors
 */

HnSRTreeNodeObj::HnSRTreeNodeObj(const HnSRTreeInfo &info, long offset)
{
    initialize(info, offset);
}

HnSRTreeNodeObj::HnSRTreeNodeObj(const HnSRTreeInfo &info,
				 const HnSRTreeBlock &block)
{
    HnBlockStream body;
    int i, count;

    initialize(info, block.getOffset());

    if ( block.getType() != HnSRTreeBlock::NODE ) {
	HnAbort("the given block is not of a node.");
    }

    body = block.getBody();
    body.rewind();

    clusters.removeAllElements();
    offsets.removeAllElements();

    count = body.readInt();

    for ( i=0; i<count; i++ ) {
	HnSRTreeCluster cluster;
	long offset;

	readEntryFrom(info, body, &cluster, &offset);

	clusters.addElement(cluster);
	offsets.addElement(offset);
    }
}

HnPoint
HnSRTreeNodeObj::getCentroid(void) const
{
    int dimension = info.getDimension();
    int count = getCount();
    HnPoint centroid;
    int weight;
    int i, axis;
    double sum;

    /* weight */

    weight = 0;
    for ( i=0; i<count; i++ ) {
	weight += clusters[i].getWeight();
    }

    /* centroid */

    centroid = new_HnPoint(dimension);

    for ( axis=0; axis<dimension; axis++ ) {
	sum = 0;

	for ( i=0; i<count; i++ ) {
	    double coord = clusters[i].getCentroid().getCoordAt(axis);
	    sum += coord * clusters[i].getWeight();
	}

	centroid.setCoordAt(sum / weight, axis);
    }

    /* round to single precision if necessary */
    if ( info.getNonLeafFloatType() == HnSRTreeInfo::NON_LEAF_SINGLE ) {
	for ( axis=0; axis<dimension; axis++ ) {
	    centroid.setCoordAt((float)centroid.getCoordAt(axis), axis);
	}
    }

    return centroid;
}

int
HnSRTreeNodeObj::getWeight(void) const
{
    int count = getCount();
    int i, weight;

    weight = 0;

    for ( i=0; i<count; i++ ) {
	weight += clusters[i].getWeight();
    }

    return weight;
}

HnSphere
HnSRTreeNodeObj::getBoundingSphere(const HnPoint &center) const
{
    int count = getCount();
    int i;
    double maxDistance;

    maxDistance = -1;

    for ( i=0; i<count; i++ ) {
	double sphereDistance, rectDistance, distance;

	/*
	 * NOTE:
	 *
	 *    Dec.13,2000 Norio KATAYAMA
	 *
	 *    Here ``getUpperBoundMaxDistance()'' is used for spheres
	 *    in order to prevent false drops caused by round-off
	 *    errors. As for rectangles, it is not necessary to use
	 *    ``getUpperBoundMaxDistance()'' because the distance
	 *    obtained by ``getMaxDistance()'' is guaranteed to be
	 *    equal to or greater than the distance to the farthest
	 *    point.
	 *
	 * Additional Note:
	 *
	 *    The above comment is satisfied only when the distance to
	 *    a point is always measured by ``getDistance()'' and when
	 *    bounding rectangles have the same precision with points.
	 *    It is better to use ``getUpperBoundMaxDistance()'' instead
	 *    of ``getMaxDistance()''.
	 */

	sphereDistance =
	    clusters[i].getSphere().getUpperBoundMaxDistance(center);
	rectDistance =
	    clusters[i].getRect().getUpperBoundMaxDistance(center);

	if ( sphereDistance < rectDistance ) {
	    distance = sphereDistance;
	}
	else {
	    distance = rectDistance;
	}

	if ( maxDistance < 0 || distance > maxDistance ) {
	    maxDistance = distance;
	}
    }

    /* round to single precision if necessary */
    if ( info.getNonLeafFloatType() == HnSRTreeInfo::NON_LEAF_SINGLE ) {
	int flag;

	flag = HnFPControl_setRound(HnFPControl_ROUND_PLUS);

	maxDistance = (float)maxDistance;

	HnFPControl_setRound(flag);
    }

    return new_HnSphere(center, maxDistance);
}

HnRect
HnSRTreeNodeObj::getBoundingRect(void) const
{
    int count = getCount();
    HnRectArray rects;
    int i;
    HnRect boundingRect;

    rects = new_HnRectArray(count);

    for ( i=0; i<count; i++ ) {
	rects[i] = clusters[i].getRect();
    }

    boundingRect = HnRect::getBoundingRect(rects);

    /* round to single precision if necessary */
    if ( info.getNonLeafFloatType() == HnSRTreeInfo::NON_LEAF_SINGLE ) {
	int dimension = info.getDimension();
	int axis, flag;
	float minValue, maxValue;

	for ( axis=0; axis<dimension; axis++ ) {
	    flag = HnFPControl_setRound(HnFPControl_ROUND_MINUS);

	    minValue = (float)boundingRect.getRangeAt(axis).getMin();

	    HnFPControl_setRound(HnFPControl_ROUND_PLUS);

	    maxValue = (float)boundingRect.getRangeAt(axis).getMax();

	    HnFPControl_setRound(flag);

	    boundingRect.setRangeAt(minValue, maxValue, axis);
	}
    }

    return boundingRect;
}

HnSRTreeCluster
HnSRTreeNodeObj::getCluster(void) const
{
    HnPoint centroid;
    int weight;
    HnSphere sphere;
    HnRect rect;

    centroid = getCentroid();
    weight = getWeight();
    rect = getBoundingRect();
    sphere = getBoundingSphere(centroid);

    return new_HnSRTreeCluster(centroid, weight, sphere, rect);
}

HnSRTreeBlock
HnSRTreeNodeObj::toBlock(void) const
{
    int count = getCount();
    HnSRTreeBlock block;
    HnBlockStream body;
    int i;

    block = new_HnSRTreeBlock(offset, info.getBlockSize(),
			      HnSRTreeBlock::NODE);

    body = block.getBody();
    body.rewind();

    /* count */
    body.writeInt(count);

    for ( i=0; i<count; i++ ) {
	writeEntryTo(info, clusters[i], offsets[i], body);
    }

    return block;
}

HnString
HnSRTreeNodeObj::toString(void) const
{
    HnString string;
    char buffer[256];
    int i;

    string = "[";

    sprintf(buffer, "type=NODE, offset=0x%08X", (unsigned int)getOffset());
    string += buffer;

    for ( i=0; i<getCount(); i++ ) {
	sprintf(buffer, ",clusters[%d]=", i);
	string += buffer;
	string += (char *)getClusterAt(i).toString();

	sprintf(buffer, ",offsets[%d]=0x%08X",
		i, (unsigned int)getOffsetAt(i));
	string += buffer;
    }

    string += "]";

    return string;
}
