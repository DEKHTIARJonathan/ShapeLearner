/*
 * HnBlockStreamObj.hh
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
 * 02/04/99 Norio KATAYAMA
 * $Id: HnBlockStreamObj.hh,v 1.3 2002/09/10 11:48:35 katayama Exp $
 */

#ifndef _HnBlockStreamObj_hh
#define _HnBlockStreamObj_hh

#include <math.h>
#include "HnSRTree/HnObject.hh"

class HnBlockStreamObj: public HnObject {
private:
    int size;

    /* for first-class streams */
    unsigned char *buffer;

    /* for sub-streams */
    HnBlockStream parent;
    int offset;

    unsigned char *start;
    unsigned char *end;
    unsigned char *ptr;

public:
/*{
#ifdef _MSC_VER
    static const int CHAR_SIZE;
    static const int SHORT_SIZE;
    static const int INT_SIZE;
    static const int LONG_SIZE;
    static const int FLOAT_SIZE;
    static const int DOUBLE_SIZE;
#else
    static const int CHAR_SIZE = sizeof(char);
    static const int SHORT_SIZE = sizeof(short);
    static const int INT_SIZE = sizeof(int);
    static const int LONG_SIZE = sizeof(long);
    static const int FLOAT_SIZE = sizeof(float);
    static const int DOUBLE_SIZE = sizeof(double);
#endif
}*/

    HnBlockStreamObj(int size);
    HnBlockStreamObj(const HnBlockStream &parent, int offset, int size);
    ~HnBlockStreamObj(void);

    /* attributes */
    int getSize(void) const {
	return size;
    }
    char *toCharArray(void) const {
	return (char *)start;
    }

    /* seek */
    void rewind(void) {
	ptr = start;
    }
    void seek(int newPos) {
#ifdef DEBUGGING
	if ( newPos < 0 || newPos > size ) {
	    HnAbort("``newPos'' is out of bounds: %d.", newPos);
	}
#endif
	ptr = start + newPos;
    }
    void skip(int length) {
	unsigned char *newPtr = ptr + length;
#ifdef DEBUGGING
	if ( newPtr < start || newPtr > end ) {
	    HnAbort("``length'' is out of bounds: %d.", length);
	}
#endif
	ptr = newPtr;
    }
    int getPosition(void) const {
	return ptr - start;
    }

    /* read */
    char readChar(void) {
	char value;
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::CHAR_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(&value, ptr, HnBlockStream::CHAR_SIZE);
	ptr += HnBlockStream::CHAR_SIZE;
	return value;
    }
    short readShort(void) {
	short value;
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::SHORT_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(&value, ptr, HnBlockStream::SHORT_SIZE);
	ptr += HnBlockStream::SHORT_SIZE;
	return value;
    }
    int readInt(void) {
	int value;
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::INT_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(&value, ptr, HnBlockStream::INT_SIZE);
	ptr += HnBlockStream::INT_SIZE;
	return value;
    }
    long readLong(void) {
	long value;
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::LONG_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(&value, ptr, HnBlockStream::LONG_SIZE);
	ptr += HnBlockStream::LONG_SIZE;
	return value;
    }
    float readFloat(void) {
	float value;
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::FLOAT_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(&value, ptr, HnBlockStream::FLOAT_SIZE);
	ptr += HnBlockStream::FLOAT_SIZE;
	return value;
    }
    double readDouble(void) {
	double value;
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::DOUBLE_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(&value, ptr, HnBlockStream::DOUBLE_SIZE);
	ptr += HnBlockStream::DOUBLE_SIZE;
	return value;
    }
    void readCharArray(char *charArray, int length) {
#ifdef DEBUGGING
	if ( end - ptr < length ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(charArray, ptr, length);
	ptr += length;
    }

    /* write */
    void writeChar(char value) {
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::CHAR_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(ptr, &value, HnBlockStream::CHAR_SIZE);
	ptr += HnBlockStream::CHAR_SIZE;
    }
    void writeShort(short value) {
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::SHORT_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(ptr, &value, HnBlockStream::SHORT_SIZE);
	ptr += HnBlockStream::SHORT_SIZE;
    }
    void writeInt(int value) {
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::INT_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(ptr, &value, HnBlockStream::INT_SIZE);
	ptr += HnBlockStream::INT_SIZE;
    }
    void writeLong(long value) {
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::LONG_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(ptr, &value, HnBlockStream::LONG_SIZE);
	ptr += HnBlockStream::LONG_SIZE;
    }
    void writeFloat(float value) {
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::FLOAT_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(ptr, &value, HnBlockStream::FLOAT_SIZE);
	ptr += HnBlockStream::FLOAT_SIZE;
    }
    void writeDouble(double value) {
#ifdef DEBUGGING
	if ( end - ptr < HnBlockStream::DOUBLE_SIZE ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(ptr, &value, HnBlockStream::DOUBLE_SIZE);
	ptr += HnBlockStream::DOUBLE_SIZE;
    }
    void writeCharArray(const char *charArray, int length) {
#ifdef DEBUGGING
	if ( end - ptr < length ) {
	    HnAbort("end of block.");
	}
#endif
	memcpy(ptr, charArray, length);
	ptr += length;
    }
};

#endif /* _HnBlockStreamObj_h */
