/*
 * HnBlockStreamObj.cc
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
 * $Id: HnBlockStreamObj.cc,v 1.3 2002/09/10 11:49:17 katayama Exp $
 */

#include <stdio.h>
#include <string.h>
#include "HnSRTree/HnBlockStream.hh"
#include "HnSRTree/HnBlockStreamObj.hh"

#ifdef _MSC_VER
const int HnBlockStream::CHAR_SIZE = sizeof(char);
const int HnBlockStream::SHORT_SIZE = sizeof(short);
const int HnBlockStream::INT_SIZE = sizeof(int);
const int HnBlockStream::LONG_SIZE = sizeof(long);
const int HnBlockStream::FLOAT_SIZE = sizeof(float);
const int HnBlockStream::DOUBLE_SIZE = sizeof(double);
#endif

HnBlockStreamObj::HnBlockStreamObj(int size)
{
    this->size = size;

    this->buffer = (unsigned char *)HnMalloc(size);
    memset(this->buffer, 0, size);

    this->parent = HnBlockStream::null;
    this->offset = -1;

    this->start = this->buffer;
    this->end = this->start + size;
    this->ptr = this->start;
}

HnBlockStreamObj::HnBlockStreamObj(const HnBlockStream &parent,
				   int offset, int size)
{
#if DEBUGGING
    if ( offset < 0 || offset >= parent.getSize() ) {
	HnAbort("``offset'' is out of bounds: %d.", offset);
    }
    if ( offset + size > parent.getSize() ) {
	HnAbort("``size'' is out of bounds: %d.", size);
    }
#endif
    this->size = size;

    this->buffer = NULL;

    this->parent = parent;
    this->offset = offset;

    this->start = (unsigned char *)parent.toCharArray() + offset;
    this->end = this->start + size;
    this->ptr = this->start;
}

HnBlockStreamObj::~HnBlockStreamObj(void)
{
    if ( buffer != NULL ) {
	HnFree(buffer, size);
	buffer = NULL;
    }
}

