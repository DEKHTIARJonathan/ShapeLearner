/*
 * HnDataItemObj.cc
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
 * 03/27/96 katayama@rd.nacsis.ac.jp
 * $Id: HnDataItemObj.cc,v 1.2 2000/06/10 11:11:55 katayama Exp $
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HnSRTree/HnDataItem.hh"
#include "HnSRTree/HnDataItemObj.hh"

HnDataItemObj::HnDataItemObj(void)
{
    initialize();

    buffer = NULL;
    len = 0;
}

HnDataItemObj::HnDataItemObj(int n)
{
    initialize();

    buffer = (char *)HnMalloc(n);
    memset(buffer, 0, n);
    len = n;
}

HnDataItemObj::HnDataItemObj(const void *ptr, int n)
{
    initialize();

    buffer = (char *)HnMalloc(n);
    memcpy(buffer, ptr, n);
    len = n;
}

HnDataItemObj::HnDataItemObj(const void *ptr1, int n1,
			     const void *ptr2, int n2)
{
    initialize();

    buffer = (char *)HnMalloc(n1 + n2);
    memcpy(buffer, ptr1, n1);
    memcpy(buffer + n1, ptr2, n2);
    len = n1 + n2;
}

HnDataItemObj::HnDataItemObj(int n, HnBlockStream &blockStream)
{
    initialize();

    buffer = (char *)HnMalloc(n);
    len = n;

    blockStream.readCharArray(buffer, len);
}

HnDataItemObj::~HnDataItemObj(void)
{
    dispose();
}

HnBool
HnDataItemObj::equals(const HnDataItem &dataItem) const
{
    if ( len == dataItem.length() &&
	 memcmp(buffer, dataItem.toCharArray(), len) == 0 ) {
	return HnTRUE;
    }
    else {
	return HnFALSE;
    }
}
