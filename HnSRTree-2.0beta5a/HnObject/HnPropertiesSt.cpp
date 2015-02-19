/*
 * HnPropertiesSt.c
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
 * $Id: HnPropertiesSt.c,v 1.2 2000/06/10 11:12:42 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HnSRTree/HnAbort.h"
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnStringBufferSt.h"
#include "HnSRTree/HnPropertiesSt.h"

HnPropertiesSt *
HnPropertiesSt_allocate(int size)
{
    HnPropertiesSt *properties;

    properties = (HnPropertiesSt *)HnMalloc(sizeof(HnPropertiesSt));

    properties->keys = HnStringVectorSt_allocate();
    properties->values = HnStringVectorSt_allocate();

    return properties;
}

void
HnPropertiesSt_free(HnPropertiesSt *properties)
{
    HnStringVectorSt_freeElements(properties->keys);
    HnStringVectorSt_free(properties->keys);

    HnStringVectorSt_freeElements(properties->values);
    HnStringVectorSt_free(properties->values);

    HnFree(properties, sizeof(HnPropertiesSt));
}

void
HnPropertiesSt_removeAllProperties(HnPropertiesSt *properties)
{
    HnStringVectorSt_freeElements(properties->keys);
    HnStringVectorSt_setSize(properties->keys, 0);

    HnStringVectorSt_freeElements(properties->values);
    HnStringVectorSt_setSize(properties->values, 0);
}

void
HnPropertiesSt_setProperty(HnPropertiesSt *properties,
			   const char *key, const char *value)
{
    int i, index;

    if ( key == NULL ) {
	HnAbort("null is invalid.");
    }
    if ( value == NULL ) {
	HnAbort("null is invalid.");
    }

    index = -1;
    for ( i=0; i<properties->keys->size; i++ ) {
	if ( strcmp(properties->keys->elements[i]->buffer, key) == 0 ) {
	    index = i;
	    break;
	}
    }

    if ( index >= 0 ) {
	HnStringSt *oldValue;

	oldValue = properties->values->elements[index];
	HnStringSt_free(oldValue);

	properties->values->elements[index] = HnStringSt_allocate(value);
    }
    else {
	HnStringVectorSt_addElement(properties->keys,
				    HnStringSt_allocate(key));
	HnStringVectorSt_addElement(properties->values,
				    HnStringSt_allocate(value));
    }
}

const char *
HnPropertiesSt_getProperty(const HnPropertiesSt *properties, const char *key)
{
    int i, index;

    index = -1;
    for ( i=0; i<properties->keys->size; i++ ) {
	if ( strcmp(properties->keys->elements[i]->buffer, key) == 0 ) {
	    index = i;
	    break;
	}
    }

    if ( index >= 0 ) {
	return properties->values->elements[index]->buffer;
    }
    else {
	return NULL;
    }
}

static const char *
skipSpaces(const char *bp)
{
    while ( *bp == ' ' || *bp == '\t' ) {
        bp ++;
    }

    return bp;
}

static const char *
readOneChar(const char *bp, char *ch_return)
{
    int i, ch;

    ch = 0;
    for ( i=0; i<2; i++ ) {
	int digit;

	switch ( *bp ) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	    digit = *bp - '0';
	    bp ++;
	    break;
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
	    digit = *bp - 'A';
	    bp ++;
	    break;
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	    digit = *bp - 'a';
	    bp ++;
	    break;
	default:
	    digit = -1;
	    break;
	}
	if ( digit < 0 ) {
	    break;
	}
	else {
	    ch = (ch << 4) + digit;
	}
    }

    *ch_return = ch;
    return bp;
}

static const char *
readOneProperty(HnPropertiesSt *properties, const char *bp)
{
    HnStringBufferSt *key = HnStringBufferSt_allocate();
    HnStringBufferSt *value = HnStringBufferSt_allocate();
    char ch;

    while ( *bp != 0 && *bp != '=' && *bp != ':' &&
	    *bp != ' ' && *bp != '\t' && *bp != '\n' && *bp != '\r' ) {
	HnStringBufferSt_appendChar(key, *bp);
	bp ++;
    }
    bp = skipSpaces(bp);
    if ( *bp == '=' || *bp == ':' ) {
	bp ++;
    }
    bp = skipSpaces(bp);
    while ( *bp != 0 && *bp != '\n' && *bp != '\r' ) {
	if ( *bp == '\\' ) {
	    bp ++;
	    switch (*bp) {
	    case '\r':
		bp ++;
		if ( *bp == '\n' ) {
		    bp ++;
		}
		bp = skipSpaces(bp);
		break;
	    case '\n':
		bp ++;
		if ( *bp == '\r' ) {
		    bp ++;
		}
		bp = skipSpaces(bp);
		break;
	    case 't':
		HnStringBufferSt_appendChar(value, '\t');
		bp ++;
		break;
	    case 'n':
		HnStringBufferSt_appendChar(value, '\n');
		bp ++;
		break;
	    case 'r':
		HnStringBufferSt_appendChar(value, '\r');
		bp ++;
		break;
	    case 'u':
		bp ++;
		bp = readOneChar(bp, &ch);
		HnStringBufferSt_appendChar(value, ch);
		break;
	    default:
		HnStringBufferSt_appendChar(value, *bp);
		bp ++;
		break;
	    }
	}
	else {
	    HnStringBufferSt_appendChar(value, *bp);
	    bp ++;
	}
    }

    HnPropertiesSt_setProperty(properties, key->buffer, value->buffer);

    HnStringBufferSt_free(key);
    HnStringBufferSt_free(value);

    return bp;
}

void
HnPropertiesSt_load(HnPropertiesSt *properties, const char *buffer)
{
    const char *bp = buffer;

    while ( *bp != 0 ) {
        if ( *bp == '#' || *bp == '!' ) {
            bp ++;
            while ( *bp != 0 && *bp != '\n' && *bp != '\r') {
                bp ++;
            }
        }
        else if ( *bp == '\n' || *bp == '\r' ) {
            bp ++;
        }
        else if ( *bp == ' ' || *bp == '\t' ) {
            bp = skipSpaces(bp);
        }
        else {
            bp = readOneProperty(properties, bp);
        }
    }
}

void
HnPropertiesSt_print(const HnPropertiesSt *properties)
{
    int i;

    printf("HnPropertiesSt[\n");
    for ( i=0; i<properties->keys->size; i++ ) {
	printf("    %s=%s\n",
	       properties->keys->elements[i]->buffer,
	       properties->values->elements[i]->buffer);
    }
    printf("]\n");
}

#define HnClass HnProperties
#include "HnSRTree/HnClassArraySt.c"
