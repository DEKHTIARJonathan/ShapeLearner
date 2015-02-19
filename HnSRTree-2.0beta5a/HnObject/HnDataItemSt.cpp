/*
 * HnDataItemSt.c
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
 * 06/29/96 katayama@rd.nacsis.ac.jp
 * $Id: HnDataItemSt.c,v 1.3 2000/06/10 11:12:42 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnDataItemSt.h"

HnDataItemSt *
HnDataItemSt_allocate(const char *bytes, int length)
{
    HnDataItemSt *dataItem;

    dataItem = (HnDataItemSt *)HnMalloc(sizeof(HnDataItemSt));

    dataItem->length = length;
    dataItem->buffer = (unsigned char *)HnMalloc(length);

    memcpy(dataItem->buffer, bytes, length);

    return dataItem;
}

void
HnDataItemSt_free(HnDataItemSt *dataItem)
{
    HnFree(dataItem->buffer, dataItem->length);

    dataItem->buffer = 0;
    dataItem->length = 0;

    HnFree(dataItem, sizeof(HnDataItemSt));
}

HnBool
HnDataItemSt_equals(const HnDataItemSt *dataItem1,
		    const HnDataItemSt *dataItem2)
{
    if ( dataItem1->length == dataItem2->length &&
	 memcmp(dataItem1->buffer, dataItem2->buffer,
		dataItem1->length) == 0 ) {
	return HnTRUE;
    }
    else {
	return HnFALSE;
    }
}

#define HnClass HnDataItem
#include "HnSRTree/HnClassArraySt.c"
