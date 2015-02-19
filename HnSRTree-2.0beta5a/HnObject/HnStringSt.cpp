/*
 * HnStringSt.c
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
 * $Id: HnStringSt.c,v 1.2 2000/06/10 11:12:44 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnStringSt.h"

HnStringSt *
HnStringSt_allocate(const char *s)
{
    return HnStringSt_allocateN(s, strlen(s));
}

HnStringSt *
HnStringSt_allocateN(const char *s, int n)
{
    HnStringSt *string;

    string = (HnStringSt *)HnMalloc(sizeof(HnStringSt));

    string->allocatedSize = n + 1;
    string->buffer = (char *)HnMalloc(string->allocatedSize);

    strncpy(string->buffer, s, n);
    string->buffer[n] = 0;
    string->length = n;

    return string;
}

void
HnStringSt_free(HnStringSt *string)
{
    HnFree(string->buffer, string->allocatedSize);

    string->buffer = NULL;
    string->allocatedSize = 0;
    string->length = 0;

    HnFree(string, sizeof(HnStringSt));
}

HnBool
HnStringSt_equals(const HnStringSt *string1, const HnStringSt *string2)
{
    if ( strcmp(string1->buffer, string2->buffer) == 0 ) {
	return HnTRUE;
    }
    else {
	return HnFALSE;
    }
}

#define HnClass HnString
#include "HnSRTree/HnClassArraySt.c"
