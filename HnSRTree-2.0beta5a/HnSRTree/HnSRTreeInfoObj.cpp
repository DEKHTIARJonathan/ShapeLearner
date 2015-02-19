/*
 * HnSRTreeInfoObj.cc
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
 * 06/15/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeInfoObj.cc,v 1.4 2000/12/15 12:41:58 katayama Exp $
 */

#include "HnSRTree/HnDataItem.hh"
#include "HnSRTree/HnSRTreeProto.h"

#include "HnSRTree/HnSRTreeInfo.hh"
#include "HnSRTree/HnSRTreeInfoObj.hh"

HnSRTreeInfoObj::HnSRTreeInfoObj(HnBlockStream blockStream)
{
    blockStream.rewind();

    if ( blockStream.readInt() != HnSRTreeMagic) {
	HnAbort("this file does not seem to be of ``HnSRTree''.");
    }

    dimension = blockStream.readInt();
    dataItemSize = blockStream.readInt();

    fileSize = blockStream.readLong();
    freeOffset = blockStream.readLong();
    rootOffset = blockStream.readLong();
    height = blockStream.readInt();

    blockSize = blockStream.readInt();
    splitFactor = blockStream.readInt();
    reinsertFactor = blockStream.readInt();
    staticAlgorithm = (HnSRTreeInfo::StaticAlgorithm)blockStream.readInt();
    nonLeafFloatType = (HnSRTreeInfo::NonLeafFloatType)blockStream.readInt();

    /* volatile attributes */
    neighborAlgorithm = HnSRTreeInfo::DEPTH_FIRST;
}

void
HnSRTreeInfoObj::writeTo(HnBlockStream blockStream) const
{
    blockStream.writeInt(HnSRTreeMagic);

    blockStream.writeInt(dimension);
    blockStream.writeInt(dataItemSize);

    blockStream.writeLong(fileSize);
    blockStream.writeLong(freeOffset);
    blockStream.writeLong(rootOffset);
    blockStream.writeInt(height);

    blockStream.writeInt(blockSize);
    blockStream.writeInt(splitFactor);
    blockStream.writeInt(reinsertFactor);
    blockStream.writeInt(staticAlgorithm);
    blockStream.writeInt(nonLeafFloatType);
}

HnBlockStream
HnSRTreeInfoObj::toBlockStream(void) const
{
    HnBlockStream blockStream;

    blockStream = new_HnBlockStream(blockSize);

    blockStream.rewind();
    writeTo(blockStream);

    return blockStream;
}

int
HnSRTreeInfoObj::getBlockStreamSize(void)
{
    HnSRTreeInfoObj dummy;
    HnBlockStream blockStream;

    blockStream = new_HnBlockStream(8192);
    dummy.writeTo(blockStream);

    return blockStream.getPosition();
}
