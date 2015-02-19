/*
 * HnStringBufferObj.cc
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
 * 01/29/99 katayama@rd.nacsis.ac.jp
 * $Id: HnStringBufferObj.cc,v 1.4 2003/04/30 05:36:24 katayama Exp $
 */

#include "HnSRTree/HnStringBuffer.hh"
#include "HnSRTree/HnStringBufferObj.hh"

#include <string.h>

#ifdef _MSC_VER
const int HnStringBufferObj::initialSize = 16;
#endif

HnStringBufferObj::HnStringBufferObj(void)
{
    initialize();
    ensureSize(initialSize);
    buffer[0] = 0;
}

HnStringBufferObj::~HnStringBufferObj(void)
{
    dispose();
}

void
HnStringBufferObj::ensureSize(int size)
{
    int oldSize, newSize;

    if ( (oldSize = this->size) >= size ) {
	return;
    }

    if ( (newSize = oldSize) == 0 ) {
	newSize = initialSize;
    }

    while ( newSize < size ) {
	newSize *= 2;
    }

    buffer = (char *)HnRealloc(buffer, oldSize, newSize);

    this->size = newSize;
}

HnStringBuffer
HnStringBufferObj::append(const char *ptr, int n)
{
    ensureSize(len + n + 1);

    memcpy(buffer + len, ptr, n);
    len += n;
    buffer[len] = 0;

    return this;
}

HnStringBuffer
HnStringBufferObj::append(const HnString &str)
{
    return append(str.chars(), str.length());
}

HnStringBuffer
HnStringBufferObj::append(const char *ptr)
{
    return append(ptr, ((ptr == NULL) ? 0 : strlen(ptr)));
}

HnStringBuffer
HnStringBufferObj::append(char c)
{
    return append(&c, 1);
}

HnStringBuffer
HnStringBufferObj::reverse(void)
{
    int index1, index2;

    index1 = 0;
    index2 = len - 1;

    while ( index1 < index2 ) {
	char temp = buffer[index1];
	buffer[index1] = buffer[index2];
	buffer[index2] = temp;
	index1 ++;
	index2 --;
    }

    return this;
}

/*
 * HnStringBuffer
 */

HnStringBuffer::HnStringBuffer(const char *ptr)
{
    HnStringBuffer sb = new_HnStringBuffer();
    sb.append(ptr);
    assign(sb);
}
