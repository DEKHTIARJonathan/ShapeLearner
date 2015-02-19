/*
 * HnSRTreeFileObj.cc
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
 * $Id: HnSRTreeFileObj.cc,v 1.19 2002/09/13 03:48:26 katayama Exp $
 */

#include <math.h>
#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <sys/types.h>
#include "HnSRTree/HnSRTreeFile.hh"
#include "HnSRTree/HnSRTreeFileObj.hh"

HnBool HnSRTreeFileObj::debug = HnFALSE;

/*
 * Properties
 */

HnProperties
HnSRTreeFileObj::getDefaultProperties(void) const
{
    HnProperties props = new_HnProperties();

    props.setProperty(HnSRTreeBlockSize, HnSRTreeDefaultBlockSize);
    props.setProperty(HnSRTreeSplitFactor, HnSRTreeDefaultSplitFactor);
    props.setProperty(HnSRTreeReinsertFactor, HnSRTreeDefaultReinsertFactor);
    props.setProperty(HnSRTreeStaticAlgorithm, HnSRTreeDefaultStaticAlgorithm);
    props.setProperty(HnSRTreeNonLeafFloatType,
		      HnSRTreeDefaultNonLeafFloatType);

    props.setProperty(HnSRTreeNeighborAlgorithm,
		      HnSRTreeDefaultNeighborAlgorithm);

    return props;
}

HnProperties
HnSRTreeFileObj::getOverriddenProperties(const HnProperties &properties) const
{
    HnProperties newProps = new_HnProperties(getDefaultProperties());
    int i, size;

    if ( properties != HnProperties::null ) {
	size = properties.size();
	for ( i=0; i<size; i++ ) {
	    HnString key, value;

	    key = properties.getKeyAt(i);
	    value = properties.getValueAt(i);

	    if ( newProps.getProperty(key) == HnString::null ) {
		HnAbort("invalid property key `%s'.", (char *)key);
	    }

	    newProps.setProperty(key, value);
	}
    }

    return newProps;
}

void
HnSRTreeFileObj::parseProperties(const HnProperties &properties,
				 int dimension,
				 int *blockSize_return,
				 int *splitFactor_return,
				 int *reinsertFactor_return,
				 HnSRTreeInfo::StaticAlgorithm *
				 staticAlgorithm_return,
				 HnSRTreeInfo::NonLeafFloatType *
				 nonLeafFloatType_return,
				 HnSRTreeInfo::NeighborAlgorithm *
				 neighborAlgorithm_return)
{
    HnString key, value;
    int blockSize, splitFactor, reinsertFactor;
    HnSRTreeInfo::StaticAlgorithm staticAlgorithm;
    HnSRTreeInfo::NonLeafFloatType nonLeafFloatType;
    HnSRTreeInfo::NeighborAlgorithm neighborAlgorithm;

    /* blockSize */
    key = HnSRTreeBlockSize;
    if ( (value = properties.getProperty(key)) == HnString::null ) {
	HnAbort("property `%s' is not found.", (char *)key);
    }
    if ( (blockSize = HnString::parseInt(value)) <= 0 ) {
	HnAbort("block size %d is out of range.", blockSize);
    }

    /* splitFactor */
    key = HnSRTreeSplitFactor;
    if ( (value = properties.getProperty(key)) == HnString::null ) {
	HnAbort("property `%s' is not found.", (char *)key);
    }
    splitFactor = HnString::parseInt(value);
    if ( splitFactor < 0 || splitFactor > 50 ) {
	HnAbort("split factor %d is out of range.", splitFactor);
    }

    /* reinsertFactor */
    key = HnSRTreeReinsertFactor;
    if ( (value = properties.getProperty(key)) == HnString::null ) {
	HnAbort("property `%s' is not found.", (char *)key);
    }
    reinsertFactor = HnString::parseInt(value);
    if ( reinsertFactor < 0 || reinsertFactor > 50 ) {
	HnAbort("reinsert factor %d is out of range.", reinsertFactor);
    }

    /* staticAlgorithm */
    key = HnSRTreeStaticAlgorithm;
    if ( (value = properties.getProperty(key)) == HnString::null ) {
	HnAbort("property `%s' is not found.", (char *)key);
    }
    staticAlgorithm = stringToStaticAlgorithm(value);
    if ( staticAlgorithm == HnSRTreeInfo::NONSTATIC ) {
	HnAbort("the string `%s' is invalid for the property `%s'.",
		(char *)value, (char *)key);
    }

    /* nonLeafFloatType */
    key = HnSRTreeNonLeafFloatType;
    if ( (value = properties.getProperty(key)) == HnString::null ) {
	HnAbort("property `%s' is not found.", (char *)key);
    }
    nonLeafFloatType = stringToNonLeafFloatType(value);

    /* neighborAlgorithm */
    key = HnSRTreeNeighborAlgorithm;
    if ( (value = properties.getProperty(key)) == HnString::null ) {
	HnAbort("property `%s' is not found.", (char *)key);
    }
    neighborAlgorithm = stringToNeighborAlgorithm(value);

    *blockSize_return = blockSize;
    *splitFactor_return = splitFactor;
    *reinsertFactor_return = reinsertFactor;
    *staticAlgorithm_return = staticAlgorithm;
    *nonLeafFloatType_return = nonLeafFloatType;
    *neighborAlgorithm_return = neighborAlgorithm;
}

HnProperties
HnSRTreeFileObj::getProperties(void) const
{
    HnProperties props = new_HnProperties();

    props.setProperty(HnSRTreeBlockSize,
		      HnString::valueOf(info.getBlockSize()));

    props.setProperty(HnSRTreeSplitFactor,
		      HnString::valueOf(info.getSplitFactor()));

    props.setProperty(HnSRTreeReinsertFactor,
		      HnString::valueOf(info.getReinsertFactor()));

    props.setProperty(HnSRTreeStaticAlgorithm,
		      staticAlgorithmToString(info.getStaticAlgorithm()));

    props.setProperty(HnSRTreeNonLeafFloatType,
		      nonLeafFloatTypeToString(info.getNonLeafFloatType()));

    props.setProperty(HnSRTreeNeighborAlgorithm,
		      neighborAlgorithmToString(info.getNeighborAlgorithm()));

    return props;
}

void
HnSRTreeFileObj::setProperties(const HnProperties &properties)
{
    int i, size;

    size = properties.size();
    for ( i=0; i<size; i++ ) {
	HnString key, value;

	key = properties.getKeyAt(i);
	value = properties.getValueAt(i);

	if ( key.equals(HnSRTreeNeighborAlgorithm) ) {
	    info.setNeighborAlgorithm(stringToNeighborAlgorithm(value));
	}
	else {
	    HnAbort("setting the property ``%s'' is not allowed.",
		    (char *)key);
	}
    }
}

HnSRTreeInfo::StaticAlgorithm
HnSRTreeFileObj::stringToStaticAlgorithm(const HnString &value)
{
    if ( value.equals("NONSTATIC") ) {
	return HnSRTreeInfo::NONSTATIC;
    }
    else if ( value.equals("VAM_ORIGINAL") ) {
	return HnSRTreeInfo::VAM_ORIGINAL;
    }
    else if ( value.equals("VAM_CORRECTED") ) {
	return HnSRTreeInfo::VAM_CORRECTED;
    }
    else {
	HnAbort("invalid value for `StaticAlgorithm' (`%s').", (char *)value);
    }
}

HnString
HnSRTreeFileObj::staticAlgorithmToString(HnSRTreeInfo::StaticAlgorithm value)
{
    switch ( value ) {
    case HnSRTreeInfo::NONSTATIC:
	return "NONSTATIC";
    case HnSRTreeInfo::VAM_ORIGINAL:
	return "VAM_ORIGINAL";
    case HnSRTreeInfo::VAM_CORRECTED:
	return "VAM_CORRECTED";
    default:
	HnAbort("unexpected value for `StaticAlgorithm' (%d).", value);
    }
}

HnSRTreeInfo::NonLeafFloatType
HnSRTreeFileObj::stringToNonLeafFloatType(const HnString &value)
{
    if ( value.equals("NON_LEAF_SINGLE") ) {
	return HnSRTreeInfo::NON_LEAF_SINGLE;
    }
    else if ( value.equals("NON_LEAF_DOUBLE") ) {
	return HnSRTreeInfo::NON_LEAF_DOUBLE;
    }
    else {
	HnAbort("invalid value for `NonLeafFloatType' (`%s').", (char *)value);
    }
}

HnString
HnSRTreeFileObj::nonLeafFloatTypeToString(HnSRTreeInfo::NonLeafFloatType value)
{
    switch ( value ) {
    case HnSRTreeInfo::NON_LEAF_SINGLE:
	return "NON_LEAF_SINGLE";
    case HnSRTreeInfo::NON_LEAF_DOUBLE:
	return "NON_LEAF_DOUBLE";
    default:
	HnAbort("unexpected value for `NonLeafFloatType' (%d).", value);
    }
}

HnSRTreeInfo::NeighborAlgorithm
HnSRTreeFileObj::stringToNeighborAlgorithm(const HnString &value)
{
    if ( value.equals("DEPTH_FIRST") ) {
	return HnSRTreeInfo::DEPTH_FIRST;
    }
    else if ( value.equals("BREADTH_FIRST") ) {
	return HnSRTreeInfo::BREADTH_FIRST;
    }
    else {
	HnAbort("invalid value for `NeighborAlgorithm' (`%s').",
		(char *)value);
    }
}

HnString
HnSRTreeFileObj::neighborAlgorithmToString(HnSRTreeInfo::NeighborAlgorithm
					   value)
{
    switch ( value ) {
    case HnSRTreeInfo::DEPTH_FIRST:
	return "DEPTH_FIRST";
    case HnSRTreeInfo::BREADTH_FIRST:
	return "BREADTH_FIRST";
    default:
	HnAbort("unexpected value for `NeighborAlgorithm' (%d).", value);
    }
}

/*
 * Constructor and Destructor
 */

HnSRTreeFileObj::HnSRTreeFileObj(const char *path,
				 int dimension, int dataItemSize,
				 const HnProperties &properties)
{
    int blockSize, splitFactor, reinsertFactor;
    HnSRTreeInfo::StaticAlgorithm staticAlgorithm;
    HnSRTreeInfo::NonLeafFloatType nonLeafFloatType;
    HnSRTreeInfo::NeighborAlgorithm neighborAlgorithm;
    long fileSize, freeOffset, rootOffset;
    int height;
    HnSRTreeLeaf leaf;

    initialize();

    parseProperties(getOverriddenProperties(properties),
		    dimension,
		    &blockSize, &splitFactor, &reinsertFactor,
		    &staticAlgorithm, &nonLeafFloatType,
		    &neighborAlgorithm);

    fileSize = blockSize;
    freeOffset = -1;
    rootOffset = 0;
    height = 0;

    staticAlgorithm = HnSRTreeInfo::NONSTATIC;

    info = new_HnSRTreeInfo(dimension, dataItemSize,
			    fileSize, freeOffset, rootOffset, height,
			    blockSize, splitFactor, reinsertFactor,
			    staticAlgorithm, nonLeafFloatType,
			    neighborAlgorithm);

    if ( HnSRTreeNode::getMaxCount(info) < 2 ) {
	HnAbort("the block size (%d) is too small; "
		"at least two entries should be allocated "
		"in a non-leaf node.", blockSize);
    }
    if ( HnSRTreeLeaf::getMaxCount(info) < 1 ) {
	HnAbort("the block size (%d) is too small; "
		"at least one entry should be allocated in a leaf node.",
		blockSize);
    }

    blockFile = new_HnBlockFile(path, blockSize);
    if ( blockFile == HnBlockFile::null ) {
	setConstructorFailureFlag();
	return;
    }

    leaf = allocateLeaf();
    info.setRootOffset(leaf.getOffset());
    info.setHeight(info.getHeight() + 1);

    writeLeaf(leaf);
    writeSuperBlock();
}

HnSRTreeFileObj::HnSRTreeFileObj(const char *path,
				 int dimension, int dataItemSize,
				 HnPointVector &points,
				 HnDataItemVector &dataItems,
				 const HnProperties &properties)
{
    int blockSize, splitFactor, reinsertFactor;
    HnSRTreeInfo::StaticAlgorithm staticAlgorithm;
    HnSRTreeInfo::NonLeafFloatType nonLeafFloatType;
    HnSRTreeInfo::NeighborAlgorithm neighborAlgorithm;
    long fileSize, freeOffset, rootOffset;
    int height;

    initialize();

    parseProperties(getOverriddenProperties(properties), dimension,
		    &blockSize, &splitFactor, &reinsertFactor,
		    &staticAlgorithm, &nonLeafFloatType,
		    &neighborAlgorithm);

    fileSize = blockSize;
    freeOffset = -1;
    rootOffset = 0;
    height = 0;

    info = new_HnSRTreeInfo(dimension, dataItemSize,
			    fileSize, freeOffset, rootOffset, height,
			    blockSize, splitFactor, reinsertFactor,
			    staticAlgorithm, nonLeafFloatType,
			    neighborAlgorithm);

    /*
     * NOTE:
     *	    The algorithm of the VAMSplit R-tree requires that
     *	    the capacity of a non-leaf node should be greater than or
     *      equal to three.
     */
    if ( HnSRTreeNode::getMaxCount(info) < 3 ) {
	HnAbort("the block size (%d) is too small; "
		"at least three entries should be allocated "
		"in a non-leaf node.", blockSize);
    }
    if ( HnSRTreeLeaf::getMaxCount(info) < 1 ) {
	HnAbort("the block size (%d) is too small; "
		"at least one entry should be allocated in a leaf node.",
		blockSize);
    }

    blockFile = new_HnBlockFile(path, blockSize);
    if ( blockFile == HnBlockFile::null ) {
	setConstructorFailureFlag();
	return;
    }

    switch ( info.getStaticAlgorithm() ) {
    case HnSRTreeInfo::VAM_ORIGINAL:
    case HnSRTreeInfo::VAM_CORRECTED:
	constructTree_VAM(points, dataItems);
	break;
    default:
	HnAbort("unexpected value for ``staticAlgorithm'' (%d).",
		info.getStaticAlgorithm());
    }
}

HnSRTreeFileObj::HnSRTreeFileObj(const char *path, const char *mode)
{
    initialize();

    if ( strcmp(mode, "rw") == 0 ) {
	blockFile = new_HnBlockFile(path, "rw");
	if ( blockFile == HnBlockFile::null ) {
	    setConstructorFailureFlag();
	    return;
	}
    }
    else if ( strcmp(mode, "r") == 0 ) {
	blockFile = new_HnBlockFile(path, "r");
	if ( blockFile == HnBlockFile::null ) {
	    setConstructorFailureFlag();
	    return;
	}
    }
    else {
	HnAbort("mode must be `r' or `rw'");
    }

    readSuperBlock();
}

HnSRTreeFileObj::~HnSRTreeFileObj(void)
{
    if ( blockFile != HnBlockFile::null ) {
	blockFile.close();
    }

    dispose();
}

void
HnSRTreeFileObj::close(void)
{
    if ( blockFile != HnBlockFile::null ) {
	blockFile.close();
    }
    blockFile = HnBlockFile::null;
}

/*
 * Super Block
 */

void
HnSRTreeFileObj::writeSuperBlock(void)
{
    HnBlockStream blockStream;

    blockStream = new_HnBlockStream(blockFile.getSuperBlockCapacity());
    blockStream.rewind();
    info.writeTo(blockStream);

    blockFile.writeSuperBlock(blockStream);

    profile->numSuperBlockWrites ++;
}

void
HnSRTreeFileObj::readSuperBlock(void)
{
    HnBlockStream blockStream;

    blockStream = new_HnBlockStream(blockFile.getSuperBlockCapacity());
    blockFile.readSuperBlock(blockStream);

    blockStream.rewind();
    info = new_HnSRTreeInfo(blockStream);

    profile->numSuperBlockReads ++;
}

/*
 * Block
 */

void
HnSRTreeFileObj::writeBlock(const HnSRTreeBlock &block)
{
    blockFile.writeBlock(block.getOffset(), block.getBlockStream());

    if ( block.isNode() ) {
	profile->numNodeBlockWrites ++;
    }
    else {
	profile->numLeafBlockWrites ++;
    }
}

HnSRTreeBlock
HnSRTreeFileObj::readBlock(long offset)
{
    HnBlockStream blockStream;
    HnSRTreeBlock block;

    blockStream = new_HnBlockStream(blockFile.getBlockSize());
    blockFile.readBlock(offset, blockStream);
    block = new_HnSRTreeBlock(offset, blockStream);

    if ( block.isNode() ) {
	profile->numNodeBlockReads ++;
    }
    else {
	profile->numLeafBlockReads ++;
    }

    return block;
}

long
HnSRTreeFileObj::allocateBlock(void)
{
    long offset;

    if ( info.getFreeOffset() < 0 ) {
	offset = info.getFileSize();
	info.setFileSize(info.getFileSize() + info.getBlockSize());

	writeSuperBlock();
    }
    else {
	HnSRTreeBlock block;
	HnBlockStream body;

	offset = info.getFreeOffset();

	block = readBlock(info.getFreeOffset());

	if ( block.getType() != HnSRTreeBlock::FREE ) {
	    HnAbort("busy block is included in the free block chain.");
	}

	body = block.getBody();
	body.rewind();
	info.setFreeOffset(body.readLong());

	writeSuperBlock();
    }

    return offset;
}

void
HnSRTreeFileObj::releaseBlock(long offset)
{
    HnSRTreeBlock block;
    HnBlockStream body;

    block = new_HnSRTreeBlock(offset, info.getBlockSize(),
			      HnSRTreeBlock::FREE);

    body = block.getBody();
    body.rewind();
    body.writeLong(info.getFreeOffset());

    writeBlock(block);

    info.setFreeOffset(offset);
    writeSuperBlock();
}

/*
 * Leaf
 */

void
HnSRTreeFileObj::writeLeaf(const HnSRTreeLeaf &leaf)
{
    HnSRTreeBlock block;

    if ( debug ) {
	fprintf(stderr, "HnSRTreeFileObj::writeLeaf: %s\n",
		(char *)leaf.toString());
    }

    block = leaf.toBlock();
    writeBlock(block);
}

HnSRTreeLeaf
HnSRTreeFileObj::allocateLeaf(void)
{
    long offset;

    offset = allocateBlock();
    return new_HnSRTreeLeaf(info, offset);
}

/*
 * Node
 */

void
HnSRTreeFileObj::writeNode(const HnSRTreeNode &node)
{
    HnSRTreeBlock block;

    block = node.toBlock();
    writeBlock(block);
}

HnSRTreeNode
HnSRTreeFileObj::allocateNode(void)
{
    long offset;

    offset = allocateBlock();
    return new_HnSRTreeNode(info, offset);
}

/*
 * Check
 */

void
HnSRTreeFileObj::check(void)
{
    checkBlock(info.getRootOffset(), HnSRTreeNode::null, -1);
    checkInclusion(info.getRootOffset());
}

int
HnSRTreeFileObj::checkBlock(long offset,
			    const HnSRTreeNode &parent, int childIndex)
{
    HnSRTreeBlock block;
    HnSRTreeNode node;
    HnSRTreeLeaf leaf;
    int i, code, level = -1;

    block = readBlock(offset);

    if ( block.isNode() ) {
	node = new_HnSRTreeNode(info, block);

	if ( parent != HnSRTreeNode::null ) {
	    HnSRTreeCluster cluster = parent.getClusterAt(childIndex);

	    if ( !node.getCluster().equals(cluster) ) {
		HnAbort("mismatch in clusters %s and %s.",
			(char *)node.getCluster().toString(),
			(char *)cluster.toString());
	    }
	}

	for ( i=0; i<node.getCount(); i++ ) {
	    code = checkBlock(node.getOffsetAt(i), node, i);

	    /* test the balance of the tree */
	    if ( i == 0 ) {
		level = code;
	    }
	    else {
		if ( code != level ) {
		    HnAbort("the tree is not balanced at a node %08X.",
			    (int)offset);
		}
	    }
	}
    }
    else if ( block.isLeaf() ) {
	leaf = new_HnSRTreeLeaf(info, block);

	if ( parent != HnSRTreeNode::null ) {
	    HnSRTreeCluster cluster = parent.getClusterAt(childIndex);

	    if ( !leaf.getCluster().equals(cluster) ) {
		HnAbort("mismatch in clusters %s and %s.",
			(char *)leaf.getCluster().toString(),
			(char *)cluster.toString());
	    }
	}
    }
    else {
	HnAbort("unexpected block type.");
    }
		

    /*
     * The `level' variable is used to confirm that the tree is balanced.
     */

    return level + 1;
}

HnPointVector
HnSRTreeFileObj::checkInclusion(long offset)
{
    HnSRTreeBlock block;
    HnPointVector sum = new_HnPointVector();

    block = readBlock(offset);

    if ( block.isNode() ) {
	HnSRTreeNode node = new_HnSRTreeNode(info, block);
	int i, j;

	for ( i=0; i<node.getCount(); i++ ) {
	    HnPointVector points = checkInclusion(node.getOffsetAt(i));
	    HnSphere sphere = node.getClusterAt(i).getSphere();

	    for ( j=0; j<points.size(); j++ ) {
		if ( !sphere.includes(points[j]) ) {
		    describeExclusion(node.getOffsetAt(i), sphere.getCenter());
		    HnAbort("point (%s) is not included "
			    "in the sphere (%s) of the node %08X "
			    "when the distance from the center of "
			    "the sphere is %g.",
			    (char *)points[j].toString(),
			    (char *)sphere.toString(), (int)offset,
			    sphere.getCenter().getDistance(points[j]));
		}
	    }

	    sum.addElements(points);
	}
    }
    else if ( block.isLeaf() ) {
	HnSRTreeLeaf leaf = new_HnSRTreeLeaf(info, block);
	int i;

	for ( i=0; i<leaf.getCount(); i++ )
	    sum.addElement(leaf.getPointAt(i));
    }
    else
	HnAbort("unexpected block type.");

    return sum;
}

void
HnSRTreeFileObj::describeExclusion(long offset, const HnPoint &center)
{
    HnSRTreeBlock block = readBlock(offset);

    if ( block.isNode() ) {
	HnSRTreeNode node = new_HnSRTreeNode(info, block);
	int i;

	for ( i=0; i<node.getCount(); i++ ) {
	    HnSRTreeCluster cluster = node.getClusterAt(i);
	    double sphereMaxDistance, rectMaxDistance;

	    sphereMaxDistance =
		cluster.getSphere().getUpperBoundMaxDistance(center);
	    rectMaxDistance =
		cluster.getRect().getUpperBoundMaxDistance(center);
			
	    printf("    %2d: cluster = %s, "
		   "sphereMaxDistance = %g, rectMaxDistance = %g\n",
		   i, (char *)cluster.toString(),
		   sphereMaxDistance, rectMaxDistance);
	}
    }
    else if ( block.isLeaf() ) {
	HnSRTreeLeaf leaf = new_HnSRTreeLeaf(info, block);
	int i;

	for ( i=0; i<leaf.getCount(); i++ ) {
	    HnPoint point = leaf.getPointAt(i);
	    double distance = point.getDistance(center);

	    printf("    %2d: point = %s, distance = %g\n",
		   i, (char *)point.toString(), distance);
	}
    }
    else
	HnAbort("unexpected block type.");
}

/*
 * Print
 */

void
HnSRTreeFileObj::print(HnBool verbose)
{
    int blockSize = info.getBlockSize();
    long offset, size;
    double entryUtilization;
    double blockUtilization;
    int numNodes, numLeaves, numLeafEntries, numFreeBlocks;

    HnStatistics rootEntryUtilization;
    HnStatistics nodeEntryUtilization, leafEntryUtilization;
    HnStatistics totalBlockUtilization, rootBlockUtilization;
    HnStatistics nodeBlockUtilization, leafBlockUtilization;
    HnStatistics nodeSphereRadius, nodeSphereVolume;
    HnStatistics nodeRectDiagonal, nodeRectVolume;
    HnStatistics leafSphereRadius, leafSphereVolume;
    HnStatistics leafRectDiagonal, leafRectVolume;

    rootEntryUtilization = new_HnStatistics();
    nodeEntryUtilization = new_HnStatistics();
    leafEntryUtilization = new_HnStatistics();

    totalBlockUtilization = new_HnStatistics();
    rootBlockUtilization = new_HnStatistics();
    nodeBlockUtilization = new_HnStatistics();
    leafBlockUtilization = new_HnStatistics();

    nodeSphereRadius = new_HnStatistics();
    nodeSphereVolume = new_HnStatistics();

    nodeRectDiagonal = new_HnStatistics();
    nodeRectVolume = new_HnStatistics();

    leafSphereRadius = new_HnStatistics();
    leafSphereVolume = new_HnStatistics();

    leafRectDiagonal = new_HnStatistics();
    leafRectVolume = new_HnStatistics();

    readSuperBlock();

    size = blockFile.getFileSize();

    numNodes = 0;
    numLeaves = 0;
    numLeafEntries = 0;
    numFreeBlocks = 0;

    for ( offset = blockSize; offset < size; offset += blockSize ) {
	HnSRTreeBlock block = readBlock(offset);

	if ( block.isNode()) {
	    HnSRTreeNode node = new_HnSRTreeNode(info, block);

	    if ( verbose ) {
		printNode(node);
	    }

	    entryUtilization =
		(double)node.getCount() / HnSRTreeNode::getMaxCount(info);
	    blockUtilization =
		(double)node.getContentSize() / info.getBlockSize();

	    if ( offset == info.getRootOffset() ) {
		rootEntryUtilization.addSample(entryUtilization);
		rootBlockUtilization.addSample(blockUtilization);
		totalBlockUtilization.addSample(blockUtilization);
	    }
	    else {
		nodeEntryUtilization.addSample(entryUtilization);
		nodeBlockUtilization.addSample(blockUtilization);
		totalBlockUtilization.addSample(blockUtilization);
	    }

	    numNodes ++;
	}
	else if ( block.isLeaf() ) {
	    HnSRTreeLeaf leaf = new_HnSRTreeLeaf(info, block);

	    if ( verbose ) {
		printLeaf(leaf);
	    }

	    entryUtilization =
		(double)leaf.getCount() / HnSRTreeLeaf::getMaxCount(info);
	    blockUtilization =
		(double)leaf.getContentSize() / info.getBlockSize();

	    leafEntryUtilization.addSample(entryUtilization);
	    leafBlockUtilization.addSample(blockUtilization);
	    totalBlockUtilization.addSample(blockUtilization);

	    numLeaves ++;
	    numLeafEntries += leaf.getCount();
	}
	else if ( block.isFree() ) {
	    numFreeBlocks ++;
	}
	else {
	    HnAbort("unexpected block type.");
	}
    }

    measureClusters(info.getRootOffset(), 0,
		    nodeSphereRadius, nodeSphereVolume,
		    nodeRectDiagonal, nodeRectVolume,
		    leafSphereRadius, leafSphereVolume,
		    leafRectDiagonal, leafRectVolume);

    printf("SuperBlock\n");
    printf("    magic               : %c%c%c%c\n",
	   (HnSRTreeMagic >> 24) & 0xff, (HnSRTreeMagic >> 16) & 0xff,
	   (HnSRTreeMagic >>  8) & 0xff, (HnSRTreeMagic      ) & 0xff);
    printf("    dimension           : %d\n", info.getDimension());
    printf("    dataItemSize        : %d\n", info.getDataItemSize());
    printf("    fileSize            : %ld\n", info.getFileSize());
    printf("    freeOffset          : 0x%08X\n",
	   (unsigned int)info.getFreeOffset());
    printf("    rootOffset          : 0x%08X\n",
	   (unsigned int)info.getRootOffset());
    printf("    height              : %d\n", info.getHeight());
    printf("    blockSize           : %d\n", info.getBlockSize());
    printf("    splitFactor         : %d\n", info.getSplitFactor());
    printf("    reinsertFactor      : %d\n", info.getReinsertFactor());
    printf("    staticAlgorithm     : %s\n",
	   (char *)staticAlgorithmToString(info.getStaticAlgorithm()));
    printf("    nonLeafFloatType    : %s\n",
	   (char *)nonLeafFloatTypeToString(info.getNonLeafFloatType()));

    printf("Node maxCount           : %d\n", HnSRTreeNode::getMaxCount(info));
    printf("Leaf maxCount           : %d\n", HnSRTreeLeaf::getMaxCount(info));
    printf("File Size is 0x%08X\n", (unsigned int)size);

    printf("Root entry utilization  : "
	   "    %8.5f %%\n",
	   rootEntryUtilization.getAvg() * 100);

    printf("Node entry utilization  : "
	   "avg %8.5f %%, min %8.5f %%, max %8.5f %%\n",
	   nodeEntryUtilization.getAvg() * 100,
	   nodeEntryUtilization.getMin() * 100,
	   nodeEntryUtilization.getMax() * 100);

    printf("Leaf entry utilization  : "
	   "avg %8.5f %%, min %8.5f %%, max %8.5f %%\n",
	   leafEntryUtilization.getAvg() * 100,
	   leafEntryUtilization.getMin() * 100,
	   leafEntryUtilization.getMax() * 100);

    printf("Root block utilization  : "
	   "    %8.5f %%\n",
	   rootBlockUtilization.getAvg() * 100);
    printf("Node block utilization  : "
	   "avg %8.5f %%, min %8.5f %%, max %8.5f %%\n",
	   nodeBlockUtilization.getAvg() * 100,
	   nodeBlockUtilization.getMin() * 100,
	   nodeBlockUtilization.getMax() * 100);
    printf("Leaf block utilization  : "
	   "avg %8.5f %%, min %8.5f %%, max %8.5f %%\n",
	   leafBlockUtilization.getAvg() * 100,
	   leafBlockUtilization.getMin() * 100,
	   leafBlockUtilization.getMax() * 100);
    printf("Total block utilization : "
	   "avg %8.5f %%, min %8.5f %%, max %8.5f %%\n",
	   totalBlockUtilization.getAvg() * 100,
	   totalBlockUtilization.getMin() * 100,
	   totalBlockUtilization.getMax() * 100);

    printf("Number of nodes : %d\n", numNodes);
    printf("Number of leaves: %d\n", numLeaves);
    printf("Number of free blocks: %d\n", numFreeBlocks);
    printf("Number of leaf entries: %d\n", numLeafEntries);

    printf("Node sphere radius: avg %13.6g, min %13.6g, max %13.6g\n",
	   nodeSphereRadius.getAvg(),
	   nodeSphereRadius.getMin(),
	   nodeSphereRadius.getMax());
    printf("Node sphere volume: avg %13.6g, min %13.6g, max %13.6g\n",
	   nodeSphereVolume.getAvg(),
	   nodeSphereVolume.getMin(),
	   nodeSphereVolume.getMax());

    printf("Node rect diagonal: avg %13.6g, min %13.6g, max %13.6g\n",
	   nodeRectDiagonal.getAvg(),
	   nodeRectDiagonal.getMin(),
	   nodeRectDiagonal.getMax());
    printf("Node rect volume  : avg %13.6g, min %13.6g, max %13.6g\n",
	   nodeRectVolume.getAvg(),
	   nodeRectVolume.getMin(),
	   nodeRectVolume.getMax());

    printf("Leaf sphere radius: avg %13.6g, min %13.6g, max %13.6g\n",
	   leafSphereRadius.getAvg(),
	   leafSphereRadius.getMin(),
	   leafSphereRadius.getMax());
    printf("Leaf sphere volume: avg %13.6g, min %13.6g, max %13.6g\n",
	   leafSphereVolume.getAvg(),
	   leafSphereVolume.getMin(),
	   leafSphereVolume.getMax());

    printf("Leaf rect diagonal: avg %13.6g, min %13.6g, max %13.6g\n",
	   leafRectDiagonal.getAvg(),
	   leafRectDiagonal.getMin(),
	   leafRectDiagonal.getMax());
    printf("Leaf rect volume  : avg %13.6g, min %13.6g, max %13.6g\n",
	   leafRectVolume.getAvg(),
	   leafRectVolume.getMin(),
	   leafRectVolume.getMax());
}

void
HnSRTreeFileObj::printNode(const HnSRTreeNode &node)
{
    int i;

    printf("Block (0x%08X)\n", (unsigned int)node.getOffset());
    printf("    type              : NODE\n");
    printf("    entry utilization : %d (%g %%)\n",
	   node.getCount(),
	   (double)node.getCount() / HnSRTreeNode::getMaxCount(info) * 100);
    printf("    block utilization : %d (%g %%)\n",
	   node.getContentSize(),
	   (double)node.getContentSize() / info.getBlockSize() * 100);

    /* children */
    for ( i=0; i<node.getCount(); i++ ) {
	printf("    %5d: cluster = %s, offset = 0x%08X\n",
	       i,
	       (char *)node.getClusterAt(i).toString(),
	       (unsigned int)node.getOffsetAt(i));
    }
}

void
HnSRTreeFileObj::printLeaf(const HnSRTreeLeaf &leaf)
{
    int i;

    printf("Block (0x%08X)\n", (unsigned int)leaf.getOffset());
    printf("    type              : LEAF\n");
    printf("    entry utilization : %d (%g %%)\n",
	   leaf.getCount(),
	   (double)leaf.getCount() / HnSRTreeLeaf::getMaxCount(info) * 100);
    printf("    block utilization : %d (%g %%)\n",
	   leaf.getContentSize(),
	   (double)leaf.getContentSize() / info.getBlockSize() * 100);

    for ( i=0; i<leaf.getCount(); i++ ) {
	printf("    %5d: %s\n", i, (char *)leaf.getPointAt(i).toString());
    }
}

void
HnSRTreeFileObj::measureClusters(long offset, int level,
				 HnStatistics nodeSphereRadius,
				 HnStatistics nodeSphereVolume,
				 HnStatistics nodeRectDiagonal,
				 HnStatistics nodeRectVolume,
				 HnStatistics leafSphereRadius,
				 HnStatistics leafSphereVolume,
				 HnStatistics leafRectDiagonal,
				 HnStatistics leafRectVolume)
{
    HnSRTreeBlock block = readBlock(offset);

    if ( block.isNode() ) {
	HnSRTreeNode node;
	int i;

	if ( level >= info.getHeight() - 1 ) {
	    HnAbort("tree is not balanced.");
	}

	node = new_HnSRTreeNode(info, block);

	for ( i=0; i<node.getCount(); i++ ) {
	    HnSRTreeCluster cluster;
	    HnSphere sphere;
	    HnRect rect;

	    cluster = node.getClusterAt(i);
	    sphere = cluster.getSphere();
	    rect = cluster.getRect();

	    if ( level == info.getHeight() - 2 ) {
		leafSphereRadius.addSample(sphere.getRadius());
		leafSphereVolume.addSample(sphere.getVolume());
		leafRectDiagonal.addSample(rect.getDiagonal());
		leafRectVolume.addSample(rect.getVolume());
	    }
	    else {
		nodeSphereRadius.addSample(sphere.getRadius());
		nodeSphereVolume.addSample(sphere.getVolume());
		nodeRectDiagonal.addSample(rect.getDiagonal());
		nodeRectVolume.addSample(rect.getVolume());
	    }

	    measureClusters(node.getOffsetAt(i), level + 1,
			    nodeSphereRadius, nodeSphereVolume,
			    nodeRectDiagonal, nodeRectVolume,
			    leafSphereRadius, leafSphereVolume,
			    leafRectDiagonal, leafRectVolume);
	}
    }
    else if ( block.isLeaf() ) {
	if ( level != info.getHeight() - 1 ) {
	    HnAbort("tree is not balanced.");
	}
    }
    else {
	HnAbort("unexpected block type.");
    }
}

HnBool
HnSRTreeFileObj::dumpToFile(const char *fileName)
{
    return blockFile.dumpToFile(fileName);
}

HnBool
HnSRTreeFileObj::dumpToFileStream(FILE *fp)
{
    return blockFile.dumpToFileStream(fp);
}
