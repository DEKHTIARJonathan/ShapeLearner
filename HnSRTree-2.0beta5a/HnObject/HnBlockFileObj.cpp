/*
 * HnBlockFileObj.cc
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
 * $Id: HnBlockFileObj.cc,v 1.3 2002/09/10 11:41:33 katayama Exp $
 */

#include "HnSRTree/HnBlockFile.hh"
#include "HnSRTree/HnBlockFileObj.hh"

HnBlockFileObj::HnBlockFileObj(const char *fileName, int blockSize)
{
    if ( fileName == NULL || strcmp(fileName, "") == 0 ) {
	createMemoryFile(blockSize);
    }
    else {
	if ( !createDiskFile(fileName, blockSize) ) {
	    setConstructorFailureFlag();
	    return;
	}
    }
}

HnBlockFileObj::HnBlockFileObj(const char *fileName, const char *mode)
{
    if ( strcmp(mode, "rw") == 0 ) {
	if ( !updateDiskFile(fileName) ) {
	    setConstructorFailureFlag();
	    return;
	}
    }
    else if ( strcmp(mode, "r") == 0 ) {
	if ( !openDiskFile(fileName) ) {
	    setConstructorFailureFlag();
	    return;
	}
    }
    else {
	HnAbort("mode must be `r' or `rw'");
    }
}

HnBlockFileObj::HnBlockFileObj(int blockSize)
{
    createMemoryFile(blockSize);
}

HnBool
HnBlockFileObj::createDiskFile(const char *fileName, int blockSize)
{
    HnBlockStream blockStream;

    this->type = DISK;

    if ( (this->fp = fopen(fileName, "w+b")) == NULL ) {
	HnSetErrorMessage("fopen(\"%s\", \"w+b\"): %s",
			  fileName, HnGetSystemErrorMessage());
	return HnFALSE;
    }
    this->blockStreams = HnBlockStreamVector::null;

    this->blockSize = blockSize;
    this->fileSize = 0;

    /*
     * initialize super block
     */

    blockStream = new_HnBlockStream(blockSize);
    blockStream.rewind();
    blockStream.writeInt(blockSize);

    if ( fwrite(blockStream.toCharArray(), blockSize, 1, fp) != 1 ) {
	fclose(fp);
	fp = NULL;
	HnSetErrorMessage("fwrite: %s", HnGetSystemErrorMessage());
	return HnFALSE;
    }

    /*
     * initialize fileSize
     */

    this->fileSize = blockSize;

    return HnTRUE;
}

HnBool
HnBlockFileObj::updateDiskFile(const char *fileName)
{
    HnBlockStream blockStream;

    this->type = DISK;

    if ( (this->fp = fopen(fileName, "r+b")) == NULL ) {
	HnSetErrorMessage("fopen(\"%s\", \"r+b\"): %s",
			  fileName, HnGetSystemErrorMessage());
	return HnFALSE;
    }
    this->blockStreams = HnBlockStreamVector::null;

    /*
     * initialize blockSize
     */

    blockStream = new_HnBlockStream(HnBlockStream::INT_SIZE);

    if ( fread(blockStream.toCharArray(),
	       blockStream.getSize(), 1, fp) != 1 ) {
	if ( ferror(fp) ) {
	    HnSetErrorMessage("fread: %s", HnGetSystemErrorMessage());
	}
	else {
	    HnSetErrorMessage("fread: unexpected EOF");
	}

	fclose(fp);
	fp = NULL;

	return HnFALSE;
    }

    blockStream.rewind();
    this->blockSize = blockStream.readInt();

    /*
     * initialize fileSize
     */

    if ( fseek(fp, 0, SEEK_END) < 0 ) {
	fclose(fp);
	fp = NULL;
	HnSetErrorMessage("fseek: %s", HnGetSystemErrorMessage());
	return HnFALSE;
    }
    this->fileSize = ftell(fp);

    return HnTRUE;
}

HnBool
HnBlockFileObj::openDiskFile(const char *fileName)
{
    HnBlockStream blockStream;

    this->type = DISK;

    if ( (this->fp = fopen(fileName, "rb")) == NULL ) {
	HnSetErrorMessage("fopen(\"%s\", \"rb\"): %s",
			  fileName, HnGetSystemErrorMessage());
	return HnFALSE;
    }
    this->blockStreams = HnBlockStreamVector::null;

    /*
     * initialize blockSize
     */

    blockStream = new_HnBlockStream(HnBlockStream::INT_SIZE);

    if ( fread(blockStream.toCharArray(),
	       blockStream.getSize(), 1, fp) != 1 ) {
	if ( ferror(fp) ) {
	    HnSetErrorMessage("fread: %s", HnGetSystemErrorMessage());
	}
	else {
	    HnSetErrorMessage("fread: unexpected EOF");
	}

	fclose(fp);
	fp = NULL;

	return HnFALSE;
    }

    blockStream.rewind();
    this->blockSize = blockStream.readInt();

    /*
     * initialize fileSize
     */

    if ( fseek(fp, 0, SEEK_END) < 0 ) {
	fclose(fp);
	fp = NULL;
	HnSetErrorMessage("fseek: %s", HnGetSystemErrorMessage());
	return HnFALSE;
    }
    this->fileSize = ftell(fp);

    return HnTRUE;
}

void
HnBlockFileObj::createMemoryFile(int blockSize)
{
    HnBlockStream blockStream;

    this->type = MEMORY;
    this->fp = NULL;
    this->blockStreams = new_HnBlockStreamVector();
    this->blockSize = blockSize;
    this->fileSize = 0;

    /*
     * initialize super block
     */

    blockStream = new_HnBlockStream(blockSize);
    blockStream.rewind();
    blockStream.writeInt(blockSize);

    blockStreams.addElement(blockStream);

    /*
     * initialize fileSize
     */

    this->fileSize = blockSize;
}

/*
 * Super Block
 */

void
HnBlockFileObj::readSuperBlock(HnBlockStream &blockStream)
{
#ifdef DEBUGGING
    if ( blockStream.getSize() != getSuperBlockCapacity() ) {
	HnAbort("the size of ``blockStream''(%d) is not the same with "
		"the super block capacity(%d).",
		blockStream.getSize(), getSuperBlockCapacity());
    }
#endif
    if ( type == DISK ) {
	HnBlockStream superBlock;

	superBlock = new_HnBlockStream(blockSize);

	if ( fseek(fp, 0, SEEK_SET) < 0 ) {
	    HnSysError("fseek");
	}
	if ( fread(superBlock.toCharArray(), blockSize, 1, fp) != 1 ) {
	    if ( ferror(fp) ) {
		HnSysError("fread");
	    }
	    else {
		HnAbort("fread: unexpected EOF.");
	    }
	}

	blockStream.rewind();
	blockStream.writeCharArray(superBlock.toCharArray() +
				   HnBlockStream::INT_SIZE,
				   blockSize - HnBlockStream::INT_SIZE);
    }
    else if ( type == MEMORY ) {
	blockStream.rewind();
	blockStream.writeCharArray(blockStreams.elementAt(0).toCharArray() +
				   HnBlockStream::INT_SIZE,
				   blockSize - HnBlockStream::INT_SIZE);
    }
    else {
	HnAbort("unexpected value for ``type'': %d.", type);
    }
}

void
HnBlockFileObj::writeSuperBlock(const HnBlockStream &blockStream)
{
#ifdef DEBUGGING
    if ( blockStream.getSize() != getSuperBlockCapacity() ) {
	HnAbort("the size of ``blockStream''(%d) is not the same with "
		"the super block capacity(%d).",
		blockStream.getSize(), getSuperBlockCapacity());
    }
#endif
    if ( type == DISK ) {
	HnBlockStream superBlock = new_HnBlockStream(blockSize);

	superBlock.rewind();
	superBlock.writeInt(blockSize);
	superBlock.writeCharArray(blockStream.toCharArray(),
				  blockStream.getSize());

	if ( fseek(fp, 0, SEEK_SET) < 0 ) {
	    HnSysError("fseek");
	}
	if ( fwrite(superBlock.toCharArray(), blockSize, 1, fp) != 1 ) {
	    HnSysError("fwrite");
	}
    }
    else if ( type == MEMORY ) {
	HnBlockStream superBlock = blockStreams.elementAt(0);

	superBlock.rewind();
	superBlock.writeInt(blockSize);
	superBlock.writeCharArray(blockStream.toCharArray(),
				  blockStream.getSize());
    }
    else {
	HnAbort("unexpected value for ``type'': %d.", type);
    }
}

/*
 * Regular Blocks
 */

void
HnBlockFileObj::readBlock(off_t offset, HnBlockStream &blockStream)
{
#ifdef DEBUGGING
    if ( blockStream.getSize() != blockSize ) {
	HnAbort("the size of ``blockStream''(%d) is not the same with "
		"``blockSize''(%d).", blockStream.getSize(), blockSize);
    }
#endif
    if ( type == DISK ) {
#ifdef DEBUGGING
	if ( offset % blockSize != 0 ) {
	    HnAbort("``offset'' is not a multiple of ``blockSize''.");
	}
	if ( offset <= 0 || offset >= fileSize ) {
	    HnAbort("``offset'' is out of bounds.");
	}
#endif
	if ( fseek(fp, offset, SEEK_SET) < 0 ) {
	    HnSysError("fseek");
	}
	if ( fread(blockStream.toCharArray(), blockSize, 1, fp) != 1 ) {
	    if ( ferror(fp) ) {
		HnSysError("fread");
	    }
	    else {
		HnAbort("fread: unexpected EOF.");
	    }
	}
    }
    else if ( type == MEMORY ) {
	int index = (int)(offset / blockSize);
#ifdef DEBUGGING
	if ( offset - (off_t)blockSize * index != 0 ) {
	    HnAbort("``offset'' is not a multiple of ``blockSize''.");
	}
	if ( index <= 0 || index >= blockStreams.size() ) {
	    HnAbort("``offset'' is out of bounds.");
	}
#endif
	if ( blockStreams.elementAt(index) == HnBlockStream::null ) {
	    blockStreams.setElementAt(new_HnBlockStream(blockSize), index);
	}

	blockStream.rewind();
	blockStream.writeCharArray(blockStreams.elementAt(index).toCharArray(),
				   blockSize);
    }
    else {
	HnAbort("unexpected value for ``type'': %d.", type);
    }
}

void
HnBlockFileObj::writeBlock(off_t offset, const HnBlockStream &blockStream)
{
#ifdef DEBUGGING
    if ( blockStream.getSize() != blockSize ) {
	HnAbort("the size of ``blockStream''(%d) is not the same with "
		"``blockSize''(%d).", blockStream.getSize(), blockSize);
    }
#endif
    if ( type == DISK ) {
#ifdef DEBUGGING
	if ( offset % blockSize != 0 ) {
	    HnAbort("``offset'' is not a multiple of ``blockSize''.");
	}
	if ( offset <= 0 ) {
	    HnAbort("``offset'' is out of bounds.");
	}
#endif
	if ( fseek(fp, offset, SEEK_SET) < 0 ) {
	    HnSysError("fseek");
	}
	if ( fwrite(blockStream.toCharArray(), blockSize, 1, fp) != 1 ) {
	    HnSysError("fwrite");
	}

	if ( offset + blockSize > fileSize ) {
	    fileSize = offset + blockSize;
	}
    }
    else if ( type == MEMORY ) {
	int index = (int)(offset / blockSize);
#ifdef DEBUGGING
	if ( offset - (off_t)blockSize * index != 0 ) {
	    HnAbort("``offset'' is not a multiple of ``blockSize''.");
	}
	if ( index <= 0 ) {
	    HnAbort("``offset'' is out of bounds.");
	}
#endif
	if ( index >= blockStreams.size() ) {
	    blockStreams.setSize(index + 1);
	    fileSize = blockStreams.size() * blockSize;
	}
	if ( blockStreams.elementAt(index) == HnBlockStream::null ) {
	    blockStreams.setElementAt(new_HnBlockStream(blockSize), index);
	}

	blockStreams.elementAt(index).rewind();
	blockStreams.elementAt(index).writeCharArray(blockStream.toCharArray(),
						     blockSize);
    }
    else {
	HnAbort("unexpected value for ``type'': %d.", type);
    }
}
			       
void
HnBlockFileObj::close(void)
{
    if ( type == DISK ) {
	if ( fp != NULL ) {
	    fclose(fp);
	}
	fp = NULL;
    }
}

HnBool
HnBlockFileObj::dumpToFile(const char *fileName)
{
    FILE *fpw;

    if ( (fpw = fopen(fileName, "wb")) == NULL ) {
	HnSetErrorMessage("fopen(\"%s\", \"wb\"): %s",
			  fileName, HnGetSystemErrorMessage());
	return HnFALSE;
    }

    if ( !dumpToFileStream(fpw) ) {
	fclose(fpw);
	return HnFALSE;
    }

    fclose(fpw);

    return HnTRUE;
}

HnBool
HnBlockFileObj::dumpToFileStream(FILE *fpw)
{
    if ( type == DISK ) {
	char *block = new char[blockSize];
	off_t pos;

	if ( fseek(fp, 0, SEEK_SET) < 0 ) {
	    HnSysError("fseek");
	}

	for ( pos=0;  pos<fileSize; pos += blockSize ) {
	    if ( fread(block, blockSize, 1, fp) != 1 ) {
		if ( ferror(fp) ) {
		    HnSysError("fread");
		}
		else {
		    HnAbort("unexpected EOF.");
		}
	    }

	    if ( fwrite(block, blockSize, 1, fpw) != 1 ) {
		HnSetErrorMessage("fwrite: %s", HnGetSystemErrorMessage());
		return HnFALSE;
	    }
	}

	delete[] block;
    }
    else if ( type == MEMORY ) {
	int i;

	for ( i=0; i<blockStreams.size(); i++ ) {
	    HnBlockStream blockStream = blockStreams.elementAt(i);

	    if ( blockStream != HnBlockStream::null ) {
		if ( fwrite(blockStream.toCharArray(),
			    blockSize, 1, fpw) != 1 ) {
		    HnSetErrorMessage("fwrite: %s",
				      HnGetSystemErrorMessage());
		    return HnFALSE;
		}
	    }
	    else {
		if ( fseek(fpw, blockSize, SEEK_CUR) < 0 ) {
		    HnSetErrorMessage("fseek: %s",
				      HnGetSystemErrorMessage());
		    return HnFALSE;
		}
	    }
	}
    }
    else {
	HnAbort("unexpected value for ``type'': %d.", type);
    }

    return HnTRUE;
}
			  
