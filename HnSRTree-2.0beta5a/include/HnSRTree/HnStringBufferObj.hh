/*
 * HnStringBufferObj.hh
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
 * $Id: HnStringBufferObj.hh,v 1.3 2002/09/10 11:50:28 katayama Exp $
 */

#ifndef _HnStringBufferObj_hh
#define _HnStringBufferObj_hh

#include "HnSRTree/HnObject.hh"

class HnStringBufferObj: public HnObject {
private:
    char *buffer;
    int len;
    int size;

#ifdef _MSC_VER
    static const int initialSize;
#else
    static const int initialSize = 16;
#endif

    void initialize(void) {
	buffer = NULL;
	len = 0;
	size = 0;
    }
    void dispose(void) {
	if ( buffer != NULL ) {
	    HnFree(buffer, size);
	    buffer = NULL;
	}
	len = 0;
	size = 0;
    }
    void ensureSize(int size);

public:
/*{
    HnStringBuffer(const char *ptr);
    operator char *(void) const;
    HnStringBuffer &operator + (const HnString &str);
    HnStringBuffer &operator + (const char *ptr);
    HnStringBuffer &operator + (char c);
    HnStringBuffer &operator + (int value);
    HnStringBuffer &operator + (double value);
    HnStringBuffer &operator += (const HnString &str);
    HnStringBuffer &operator += (const char *ptr);
    HnStringBuffer &operator += (char c);
    HnStringBuffer &operator += (int value);
    HnStringBuffer &operator += (double value);
}*/
    HnStringBufferObj(void);
    ~HnStringBufferObj(void);

    const char *chars(void) const {
	return buffer;
    }
    int length(void) const {
	return len;
    }

    HnStringBuffer append(const char *ptr, int n);
    HnStringBuffer append(const HnString &str);
    HnStringBuffer append(const char *ptr);
    HnStringBuffer append(char c);

    operator const char *(void) const {
	return buffer;
    }
    HnString toString(void) const {
	return new_HnString(buffer);
    }

    HnStringBuffer reverse(void);
};

/*{
inline
HnStringBuffer::operator char *(void) const
{
    if ( isInvalid() ) {
	return NULL;
    }
    else {
	return (char *)chars();
    }
}

inline HnStringBuffer &
HnStringBuffer::operator + (const HnString &str)
{
    append(str);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator + (const char *ptr)
{
    append(ptr);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator + (char c)
{
    append(c);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator + (int value)
{
    append(HnString::valueOf(value));
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator + (double value)
{
    append(HnString::valueOf(value));
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (const HnString &str)
{
    append(str);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (const char *ptr)
{
    append(ptr);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (char c)
{
    append(c);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (int value)
{
    append(HnString::valueOf(value));
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (double value)
{
    append(HnString::valueOf(value));
    return *this;
}
}*/

#endif /* _HnStringBufferObj_hh */
