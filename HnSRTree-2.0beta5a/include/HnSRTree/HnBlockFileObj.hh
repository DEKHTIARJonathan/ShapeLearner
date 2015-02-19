/*
 * HnBlockFileObj.hh
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
 * Jul. 6,1999 Norio KATAYAMA
 * $Id: HnBlockFileObj.hh,v 1.4 2002/09/10 11:40:26 katayama Exp $
 */

#ifndef _HnBlockFileObj_hh
#define _HnBlockFileObj_hh

#include "HnSRTree/HnObject.hh"

/*{
#ifdef _MSC_VER
#include <sys/types.h>
#else
#include <unistd.h>
#endif
#include "HnSRTree/HnBlockStream.hh"
}*/

class HnBlockFileObj: public HnObject {
private:
    enum { MEMORY, DISK } type;
    FILE *fp;
    HnBlockStreamVector blockStreams;
    int blockSize;
    off_t fileSize;

private:
    HnBool createDiskFile(const char *fileName, int blockSize);
    HnBool updateDiskFile(const char *fileName);
    HnBool openDiskFile(const char *fileName);
    void createMemoryFile(int blockSize);

public:
    HnBlockFileObj(const char *fileName, int blockSize);
    HnBlockFileObj(const char *fileName, const char *mode);
    HnBlockFileObj(int blockSize);

    int getBlockSize(void) {
	return blockSize;
    }
    off_t getFileSize(void) {
	return fileSize;
    }

    /* super block (the first block is reserved for the super block) */
    int getSuperBlockCapacity(void) const {
	return blockSize - HnBlockStream::INT_SIZE;
    }
    void readSuperBlock(HnBlockStream &blockStream);
    void writeSuperBlock(const HnBlockStream &blockStream);

    /* regular blocks */
    void readBlock(off_t offset, HnBlockStream &blockStream);
    void writeBlock(off_t offset, const HnBlockStream &blockStream);

    void close(void);

    HnBool dumpToFile(const char *fileName);
    HnBool dumpToFileStream(FILE *fp);
};

#endif /* _HnBlockFileObj_hh */
