/*
 * HnClassArraySt.c
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
 * $Id: HnClassArraySt.c,v 1.2 2000/06/10 11:12:41 katayama Exp $
 */

#define	ClassSt0(type)			type ## St
#define ClassSt_free0(type)		type ## St_free

#define	ClassSt(type)			ClassSt0(type)
#define ClassSt_free(type)		ClassSt_free0(type)

#define HnClassSt			ClassSt(HnClass)
#define HnClassSt_free			ClassSt_free(HnClass)

#define ArraySt0(type)			type ## ArraySt
#define ArraySt_allocate0(type)		type ## ArraySt_allocate
#define ArraySt_free0(type)		type ## ArraySt_free
#define ArraySt_freeElements0(type)	type ## ArraySt_freeElements

#define ArraySt(type)			ArraySt0(type)
#define ArraySt_allocate(type)		ArraySt_allocate0(type)
#define ArraySt_free(type)		ArraySt_free0(type)
#define ArraySt_freeElements(type)	ArraySt_freeElements0(type)

#define HnClassArraySt			ArraySt(HnClass)
#define HnClassArraySt_allocate		ArraySt_allocate(HnClass)
#define HnClassArraySt_free		ArraySt_free(HnClass)
#define HnClassArraySt_freeElements	ArraySt_freeElements(HnClass)

#define VectorSt0(type)			type ## VectorSt
#define VectorSt_allocate0(type)	type ## VectorSt_allocate
#define VectorSt_free0(type)		type ## VectorSt_free
#define VectorSt_freeElements0(type)	type ## VectorSt_freeElements
#define VectorSt_addElement0(type)	type ## VectorSt_addElement
#define VectorSt_insertElementAt0(type)	type ## VectorSt_insertElementAt
#define VectorSt_removeElementAt0(type)	type ## VectorSt_removeElementAt
#define VectorSt_setElementAt0(type)	type ## VectorSt_setElementAt
#define VectorSt_swapElementsAt0(type)	type ## VectorSt_swapElementsAt
#define VectorSt_setSize0(type)		type ## VectorSt_setSize
#define VectorSt_ensureSize0(type)	type ## VectorSt_ensureSize
#define VectorSt_INITIAL_SIZE0(type)	type ## VectorSt_INITIAL_SIZE

#define VectorSt(type)			VectorSt0(type)
#define VectorSt_allocate(type)		VectorSt_allocate0(type)
#define VectorSt_free(type)		VectorSt_free0(type)
#define VectorSt_freeElements(type)	VectorSt_freeElements0(type)
#define VectorSt_addElement(type)	VectorSt_addElement0(type)
#define VectorSt_insertElementAt(type)	VectorSt_insertElementAt0(type)
#define VectorSt_removeElementAt(type)	VectorSt_removeElementAt0(type)
#define VectorSt_setElementAt(type)	VectorSt_setElementAt0(type)
#define VectorSt_swapElementsAt(type)	VectorSt_swapElementsAt0(type)
#define VectorSt_setSize(type)		VectorSt_setSize0(type)
#define VectorSt_ensureSize(type)	VectorSt_ensureSize0(type)
#define VectorSt_INITIAL_SIZE(type)	VectorSt_INITIAL_SIZE0(type)

#define HnClassVectorSt			VectorSt(HnClass)
#define HnClassVectorSt_allocate	VectorSt_allocate(HnClass)
#define HnClassVectorSt_free		VectorSt_free(HnClass)
#define HnClassVectorSt_freeElements	VectorSt_freeElements(HnClass)
#define HnClassVectorSt_addElement	VectorSt_addElement(HnClass)
#define HnClassVectorSt_insertElementAt	VectorSt_insertElementAt(HnClass)
#define HnClassVectorSt_removeElementAt	VectorSt_removeElementAt(HnClass)
#define HnClassVectorSt_setElementAt	VectorSt_setElementAt(HnClass)
#define HnClassVectorSt_swapElementsAt	VectorSt_swapElementsAt(HnClass)
#define HnClassVectorSt_setSize		VectorSt_setSize(HnClass)
#define HnClassVectorSt_ensureSize	VectorSt_ensureSize(HnClass)
#define HnClassVectorSt_INITIAL_SIZE	VectorSt_INITIAL_SIZE(HnClass)

#include <string.h>
#include "HnSRTree/HnMalloc.h"

/*
 * Array
 */

HnClassArraySt *
HnClassArraySt_allocate(int length)
{
    HnClassArraySt *array;
    int i;

    array = (HnClassArraySt *)HnMalloc(sizeof(HnClassArraySt));

    array->elements = (HnClassSt **)HnMalloc(sizeof(HnClassSt *) * length);
    array->length = length;

    for ( i=0; i<length; i++ ) {
	array->elements[i] = NULL;
    }

    return array;
}

void
HnClassArraySt_free(HnClassArraySt *array)
{
    if ( array->elements != NULL ) {
	HnFree(array->elements, sizeof(HnClassSt *) * array->length);

	array->elements = NULL;
	array->length = 0;
    }

    HnFree(array, sizeof(HnClassArraySt));
}

void
HnClassArraySt_freeElements(HnClassArraySt *array)
{
    int i;

    for ( i=0; i<array->length; i++ ) {
	if ( array->elements[i] != NULL ) {
	    HnClassSt_free(array->elements[i]);
	    array->elements[i] = NULL;
	}
    }
}

/*
 * Vector
 */

static int HnClassVectorSt_INITIAL_SIZE = 4;

static void
HnClassVectorSt_ensureSize(HnClassVectorSt *vector, int requiredSize)
{
    int oldSize, newSize;

    if ( (oldSize = vector->allocatedSize) >= requiredSize ) {
	return;
    }

    if ( (newSize = oldSize) == 0 ) {
	newSize = HnClassVectorSt_INITIAL_SIZE;
    }

    while ( newSize < requiredSize ) {
	newSize *= 2;
    }

    vector->elements = (HnClassSt **)HnRealloc(vector->elements,
					       sizeof(HnClassSt *) * oldSize,
					       sizeof(HnClassSt *) * newSize);

    vector->allocatedSize = newSize;
}

HnClassVectorSt *
HnClassVectorSt_allocate(void)
{
    HnClassVectorSt *vector;

    vector = (HnClassVectorSt *)HnMalloc(sizeof(HnClassVectorSt));

    vector->elements = NULL;
    vector->allocatedSize = 0;
    vector->size = 0;

    return vector;
}

void
HnClassVectorSt_free(HnClassVectorSt *vector)
{
    if ( vector->elements != NULL ) {
	HnFree(vector->elements, sizeof(HnClassSt *) * vector->allocatedSize);

	vector->allocatedSize = 0;
	vector->size = 0;
	vector->elements = NULL;
    }

    HnFree(vector, sizeof(HnClassVectorSt));
}

void
HnClassVectorSt_freeElements(HnClassVectorSt *vector)
{
    int i;

    for ( i=0; i<vector->size; i++ ) {
	if ( vector->elements[i] != NULL ) {
	    HnClassSt_free(vector->elements[i]);
	    vector->elements[i] = NULL;
	}
    }
}
    
void
HnClassVectorSt_addElement(HnClassVectorSt *vector, HnClassSt *newElement)
{
    HnClassVectorSt_ensureSize(vector, vector->size + 1);

    vector->elements[vector->size] = newElement;
    vector->size ++;
}

void
HnClassVectorSt_insertElementAt(HnClassVectorSt *vector,
				HnClassSt *newElement, int index)
{
    HnClassVectorSt_ensureSize(vector, vector->size + 1);

    memmove(vector->elements + index + 1, vector->elements + index,
            sizeof(HnClassSt *) * (vector->size - index));

    vector->elements[index] = newElement;
    vector->size ++;
}

void
HnClassVectorSt_removeElementAt(HnClassVectorSt *vector, int index)
{
    memmove(vector->elements + index, vector->elements + index + 1,
            sizeof(HnClassSt *) * (vector->size - index - 1));
    vector->size --;
}

void
HnClassVectorSt_setElementAt(HnClassVectorSt *vector,
			     HnClassSt *newElement, int index)
{
    vector->elements[index] = newElement;
}

void
HnClassVectorSt_swapElementsAt(HnClassVectorSt *vector, int i, int j)
{
    HnClassSt *temp;

    temp = vector->elements[i];
    vector->elements[i] = vector->elements[j];
    vector->elements[j] = temp;
}

void
HnClassVectorSt_setSize(HnClassVectorSt *vector, int newSize)
{
    int i;

    if ( newSize >  vector->size ) {
        HnClassVectorSt_ensureSize(vector, newSize);

        for ( i=vector->size; i<newSize; i++ ) {
            vector->elements[i] = NULL;
        }

        vector->size = newSize;
    }
    else {
        for ( i=newSize; i<vector->size; i++) {
            vector->elements[i] = NULL;
        }

        vector->size = newSize;
    }

}

#undef	ClassSt0
#undef	ClassSt_free0

#undef	ClassSt
#undef	ClassSt_free

#undef	HnClass
#undef	HnClass_free

#undef	ArraySt0
#undef	ArraySt_allocate0
#undef	ArraySt_free0
#undef	ArraySt_freeElements0

#undef	ArraySt
#undef	ArraySt_allocate
#undef	ArraySt_free
#undef	ArraySt_freeElements

#undef	HnClassArraySt
#undef	HnClassArraySt_allocate
#undef	HnClassArraySt_free
#undef	HnClassArraySt_freeElements

#undef	VectorSt0
#undef	VectorSt_allocate0
#undef	VectorSt_free0
#undef	VectorSt_freeElements0
#undef	VectorSt_addElement0
#undef	VectorSt_insertElementAt0
#undef	VectorSt_removeElementAt0
#undef	VectorSt_setElementAt0
#undef	VectorSt_swapElementsAt0
#undef	VectorSt_setSize0
#undef	VectorSt_ensureSize0
#undef	VectorSt_INITIAL_SIZE0

#undef	VectorSt
#undef	VectorSt_allocate
#undef	VectorSt_free
#undef	VectorSt_freeElements
#undef	VectorSt_addElement
#undef	VectorSt_insertElementAt
#undef	VectorSt_removeElementAt
#undef	VectorSt_setElementAt
#undef	VectorSt_swapElementsAt
#undef	VectorSt_setSize
#undef	VectorSt_ensureSize
#undef	VectorSt_INITIAL_SIZE

#undef	HnClassVectorSt
#undef	HnClassVectorSt_allocate
#undef	HnClassVectorSt_free
#undef	HnClassVectorSt_freeElements
#undef	HnClassVectorSt_addElement
#undef	HnClassVectorSt_insertElementAt
#undef	HnClassVectorSt_removeElementAt
#undef	HnClassVectorSt_setElementAt
#undef	HnClassVectorSt_swapElementsAt
#undef	HnClassVectorSt_setSize
#undef	HnClassVectorSt_ensureSize
#undef	HnClassVectorSt_INITIAL_SIZE

#undef	HnClass
