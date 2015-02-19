/*
 * HnSRTreeBlockObj.hh
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
 * 08/23/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeBlockObj.hh,v 1.6 2002/09/13 03:53:19 katayama Exp $
 */

#ifndef _HnSRTreeBlockObj_hh
#define _HnSRTreeBlockObj_hh

#include <string.h>
#include "HnSRTree/HnObject.hh"

/*{
#ifndef _MSC_VER
#include <unistd.h>
#include <sys/types.h>
#endif
#include "HnSRTree/HnBlockStream.hh"
#include "HnSRTree/HnSRTreeInfo.hh"
}*/

class HnSRTreeBlockObj: public HnObject {
private:
    long offset;
    int size;
    HnBlockStream blockStream;
    HnSRTreeBlock::Type type;

public:
/*{
    typedef enum { NODE, LEAF, FREE } Type;
}*/
    HnSRTreeBlockObj(long offset, int size, HnSRTreeBlock::Type type) {
	this->offset = offset;
	this->size = size;
	this->blockStream = new_HnBlockStream(size);

	blockStream.rewind();
	blockStream.writeInt(type);
    }
    HnSRTreeBlockObj(long offset, HnBlockStream &blockStream) {
	this->offset = offset;
	this->size = blockStream.getSize();
	this->blockStream = blockStream;

	this->blockStream.rewind();
	this->type = (HnSRTreeBlock::Type)blockStream.readInt();

	if ( type != HnSRTreeBlock::NODE && type != HnSRTreeBlock::LEAF &&
	     type != HnSRTreeBlock::FREE ) {
	    HnAbort("unexpected type for ``HnSRTreeBlock::Type'': %d.", type);
	}
    }

    /* attributes */
    long getOffset(void) const {
	return offset;
    }
    int getSize(void) const {
	return size;
    }
    HnSRTreeBlock::Type getType(void) const {
	return type;
    }
    const HnBlockStream &getBlockStream(void) const {
	return blockStream;
    }

    HnBlockStream getBody(void) const {
	return new_HnBlockStream(blockStream,
				 HnBlockStream::INT_SIZE,
				 size - HnBlockStream::INT_SIZE);
    }

    HnBool isNode(void) const {
	return (getType() == HnSRTreeBlock::NODE);
    }
    HnBool isLeaf(void) const {
	return (getType() == HnSRTreeBlock::LEAF);
    }
    HnBool isFree(void) const {
	return (getType() == HnSRTreeBlock::FREE);
    }

    static int getBodyCapacity(const HnSRTreeInfo &info) {
	return info.getBlockSize() - HnBlockStream::INT_SIZE;
    }
};

#endif /* _HnSRTreeBlockObj_hh */
