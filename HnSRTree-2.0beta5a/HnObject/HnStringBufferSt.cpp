/*
 * HnStringBufferSt.c
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
 * 07/27/1999 Norio KATAYAMA
 * $Id: HnStringBufferSt.c,v 1.3 2000/06/10 11:12:43 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnStringBufferSt.h"

#define HnStringBufferSt_INITIAL_SIZE	16
static void ensureSize(HnStringBufferSt *strbuf, int requiredSize);

HnStringBufferSt *
HnStringBufferSt_allocate(void)
{
    HnStringBufferSt *strbuf;

    strbuf = (HnStringBufferSt *)HnMalloc(sizeof(HnStringBufferSt));

    strbuf->allocatedSize = 0;
    strbuf->length = 0;
    strbuf->buffer = NULL;

    /* initialize */
    ensureSize(strbuf, HnStringBufferSt_INITIAL_SIZE);
    strbuf->buffer[0] = 0;

    return strbuf;
}

void
HnStringBufferSt_free(HnStringBufferSt *strbuf)
{
    HnFree(strbuf->buffer, strbuf->allocatedSize);

    strbuf->buffer = NULL;
    strbuf->allocatedSize = 0;
    strbuf->length = 0;

    HnFree(strbuf, sizeof(HnStringBufferSt));
}

static void
ensureSize(HnStringBufferSt *strbuf, int requiredSize)
{
    int oldSize, newSize;

    if ( (oldSize = strbuf->allocatedSize) >= requiredSize ) {
	return;
    }

    if ( (newSize = oldSize) == 0 ) {
	newSize = HnStringBufferSt_INITIAL_SIZE;
    }

    while ( newSize < requiredSize ) {
	newSize *= 2;
    }

    strbuf->buffer = (char *)HnRealloc(strbuf->buffer, oldSize, newSize);

    strbuf->allocatedSize = newSize;
}

void
HnStringBufferSt_appendChars(HnStringBufferSt *strbuf, const char *s, int n)
{
    ensureSize(strbuf, strbuf->length + n + 1);

    memcpy(strbuf->buffer + strbuf->length, s, n);
    strbuf->length += n;
    strbuf->buffer[strbuf->length] = 0;
}

void
HnStringBufferSt_appendString(HnStringBufferSt *strbuf, const char *s)
{
    HnStringBufferSt_appendChars(strbuf, s, strlen(s));
}

void
HnStringBufferSt_appendChar(HnStringBufferSt *strbuf, char c)
{
    HnStringBufferSt_appendChars(strbuf, &c, 1);
}

#define HnClass HnStringBuffer
#include "HnSRTree/HnClassArraySt.c"
