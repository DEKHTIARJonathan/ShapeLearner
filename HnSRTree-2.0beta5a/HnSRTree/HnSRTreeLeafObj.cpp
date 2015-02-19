/*
 * HnSRTreeLeafObj.cc
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
 * $Id: HnSRTreeLeafObj.cc,v 1.8 2000/12/15 12:42:41 katayama Exp $
 */

#include "HnSRTree/HnSRTreeLeaf.hh"
#include "HnSRTree/HnSRTreeLeafObj.hh"
#include "HnSRTree/HnFPControl.h"

void
HnSRTreeLeafObj::initialize(const HnSRTreeInfo &info, long offset)
{
    this->info = info;

    this->maxCount = getMaxCount(info);

    this->offset = offset;

    points = new_HnPointVector();
    dataItems = new_HnDataItemVector();
}

/*
 * Entry
 */

void
HnSRTreeLeafObj::writeEntryTo(const HnSRTreeInfo &info,
			      const HnPoint &point, const HnDataItem &dataItem,
			      HnBlockStream &blockStream)
{
    int length;

    /* point */
    point.writeTo(blockStream);

    /* dataItem */
    length = dataItem.length();
    blockStream.writeInt(length);
    dataItem.writeTo(blockStream);
    blockStream.skip(info.getDataItemSize() - length);
}

void
HnSRTreeLeafObj::readEntryFrom(const HnSRTreeInfo &info,
			       HnBlockStream &blockStream,
			       HnPoint *point_return,
			       HnDataItem *dataItem_return)
{
    int dimension = info.getDimension();
    HnPoint &point = *point_return;
    HnDataItem &dataItem = *dataItem_return;
    int length;

    /* point */
    point = new_HnPoint(dimension, blockStream);

    /* dataItem */
    length = blockStream.readInt();
    dataItem = new_HnDataItem(length, blockStream);
    blockStream.skip(info.getDataItemSize() - length);
}

int
HnSRTreeLeafObj::getEntrySize(const HnSRTreeInfo &info)
{
    return (HnPoint::getSizeInBlockStream(info.getDimension()) +
	    HnBlockStream::INT_SIZE +
	    HnDataItem::getSizeInBlockStream(info.getDataItemSize()));
}

/*
 * Constructors
 */

HnSRTreeLeafObj::HnSRTreeLeafObj(const HnSRTreeInfo &info, long offset)
{
    initialize(info, offset);
}

HnSRTreeLeafObj::HnSRTreeLeafObj(const HnSRTreeInfo &info,
				 const HnSRTreeBlock &block)
{
    HnBlockStream body;
    int i, count;

    initialize(info, block.getOffset());

    if ( block.getType() != HnSRTreeBlock::LEAF ) {
	HnAbort("the given block is not of a leaf.");
    }

    body = block.getBody();
    body.rewind();

    points.removeAllElements();
    dataItems.removeAllElements();

    count = body.readInt();

    for ( i=0; i<count; i++ ) {
	HnPoint point;
	HnDataItem dataItem;

	readEntryFrom(info, body, &point, &dataItem);

	points.addElement(point);
	dataItems.addElement(dataItem);
    }
}

HnSRTreeBlock
HnSRTreeLeafObj::toBlock(void) const
{
    int count = getCount();
    HnSRTreeBlock block;
    HnBlockStream body;
    int i;

    block = new_HnSRTreeBlock(offset, info.getBlockSize(),
			      HnSRTreeBlock::LEAF);

    body = block.getBody();
    body.rewind();

    /* count */
    body.writeInt(count);

    for ( i=0; i<count; i++ ) {
	writeEntryTo(info, points[i], dataItems[i], body);
    }

    return block;
}

HnPoint
HnSRTreeLeafObj::getCentroid(void) const
{
    int dimension = info.getDimension();
    int count = getCount();
    int i, axis;
    double sum;
    HnPoint centroid;

    centroid = new_HnPoint(dimension);

    for ( axis=0; axis<dimension; axis++ ) {
	sum = 0;
	for ( i=0; i<count; i++ ) {
	    sum += points[i].getCoordAt(axis);
	}
	centroid.setCoordAt(sum / count, axis);
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
HnSRTreeLeafObj::getWeight(void) const
{
    return getCount();
}

HnSphere
HnSRTreeLeafObj::getBoundingSphere(const HnPoint &center) const
{
    int count = getCount();
    int i;
    double maxDistance;

    maxDistance = -1;
    for ( i=0; i<count; i++ ) {
	/*
	 * NOTE:
	 *
	 *    Dec.13,2000 Norio KATAYAMA
	 *
	 *    Here, ``getUpperBoundDistance()'' should be used instead of
	 *    ``getDistance()'' in order to ensure that the sphere
	 *    includes every point.
	 */
	double distance = points[i].getUpperBoundDistance(center);

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
HnSRTreeLeafObj::getBoundingRect(void) const
{
    HnPointArray array;
    HnRect boundingRect;

    array = new_HnPointArray(points.size());
    points.copyInto(array);

    boundingRect = HnRect::getBoundingRect(array);

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
HnSRTreeLeafObj::getCluster(void) const
{
    if ( getCount() == 0 ) {
	return HnSRTreeCluster::null;
    }
    else {
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
}

HnString
HnSRTreeLeafObj::toString(void) const
{
    HnString string;
    char buffer[256];
    int i;

    string = "[ ";

    sprintf(buffer, "type=LEAF, offset=0x%08X", (unsigned int)getOffset());
    string += buffer;

    for ( i=0; i<getCount(); i++ ) {
	sprintf(buffer, ", points[%d]=", i);
	string += buffer;
	string += (char *)getPointAt(i).toString();

	sprintf(buffer, ", dataItems[%d].length()=%d",
		i, getDataItemAt(i).length());
	string += buffer;
    }

    string += " ]";

    return string;
}

int
HnSRTreeLeafObj::getMaxCount(const HnSRTreeInfo &info)
{
    int bodyCapacity = HnSRTreeBlock::getBodyCapacity(info);
    int countSize = HnBlockStream::INT_SIZE;
    int entrySize = getEntrySize(info);

    return (bodyCapacity - countSize) / entrySize;
}

int
HnSRTreeLeafObj::getContentSize(void) const
{
    int blockSize = info.getBlockSize();
    int bodyCapacity = HnSRTreeBlock::getBodyCapacity(info);
    int countSize = HnBlockStream::INT_SIZE;
    int entrySize = getEntrySize(info);

    return (blockSize - bodyCapacity) + countSize + entrySize * getCount();
}
