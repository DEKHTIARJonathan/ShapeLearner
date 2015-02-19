/*
 * HnFTypeArray.cc
 * Copyright (C) 1998 Norio Katayama
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
 * 10/21/98 katayama@rd.nacsis.ac.jp
 * $Id: HnFTypeArray.cc,v 1.1 2000/06/06 08:30:33 katayama Exp $
 */

#ifndef HnFTypeName
#define HnFTypeName	HnFType
#endif

#define TypeNameString0(type)		"HnFT" #type
#define PointerName0(type,suffix)	HnFT ## type ## suffix
#define ObjectName0(type,suffix)	HnFT ## type ## suffix ## Obj
#define ConstructorName0(type,suffix)	new_HnFT ## type ## suffix

#define TypeNameString(type)		TypeNameString0(type)
#define PointerName(type,suffix)	PointerName0(type,suffix)
#define ObjectName(type,suffix)		ObjectName0(type,suffix)
#define ConstructorName(type,suffix)	ConstructorName0(type,suffix)

#define HnFTypeNameString	TypeNameString(HnFTypeName)
#define HnFTypeArray		PointerName(HnFTypeName,Array)
#define HnFTypeArrayObj		ObjectName(HnFTypeName,Array)
#define new_HnFTypeArray	ConstructorName(HnFTypeName,Array)
#define HnFTypeVector		PointerName(HnFTypeName,Vector)
#define HnFTypeVectorObj	ObjectName(HnFTypeName,Vector)
#define new_HnFTypeVector	ConstructorName(HnFTypeName,Vector)

#include <stdlib.h>

/*
 * HnFTypeArrayObj
 */

HnFTypeArrayObj::HnFTypeArrayObj(int len)
{
    allocate(len);
    memset(this->array, 0, sizeof(HnFType) * len);
}

HnFTypeArrayObj::HnFTypeArrayObj(const HnFTypeArray &ptr)
{
    allocate(ptr.length);
    memcpy(this->array, ptr.getElements(), sizeof(HnFType) * ptr.length);
}

HnFTypeArrayObj::~HnFTypeArrayObj(void)
{
    HnFree(array, sizeof(HnFType) * length);
}

HnBool
HnFTypeArrayObj::equals(const HnFTypeArray &ptr) const
{
    if ( length == ptr.length ) {
	int i;

	for ( i=0; i<length; i++ ) {
	    if ( array[i] != ptr[i] ) {
		return HnFALSE;
	    }
	}

	return HnTRUE;
    }
    else {
	return HnFALSE;
    }
}

HnString
HnFTypeArrayObj::toString(void) const
{
    HnString string;
    int i;

    string = HnFTypeNameString "Array[ ";
    for ( i=0; i<length; i++ ) {
	if ( i != 0 ) {
	    string += ",";
	}
	string += HnString::valueOf(array[i]);
    }
    string += " ]";

    return string;
}

/*
 * HnFTypeArray
 */

const HnFTypeArray HnFTypeArray::null;

HnFTypeArray::HnFTypeArray(const HnFTypeArrayObj *ptr)
{
    HnPointer::assign((HnObject *)ptr);
    if ( ptr != NULL ) {
	this->length = ptr->getLength();
    }
    else {
	this->length = -1;
    }
}

HnFTypeArray
new_HnFTypeArray(int length)
{
    return new HnFTypeArrayObj(length);
}

HnFTypeArray
new_HnFTypeArray(const HnFTypeArray &value)
{
    if ( value.isInvalid() ) {
	return (HnFTypeArrayObj *)NULL;
    }
    else {
	return new HnFTypeArrayObj(value);
    }
}

/*
 * HnFTypeVectorObj
 */

const int HnFTypeVectorObj::initialSize = 4;

void
HnFTypeVectorObj::initialize(void)
{
    array = NULL;
    length = 0;
    allocatedSize = 0;
}

void
HnFTypeVectorObj::dispose(void)
{
    if ( array != NULL ) {
	HnFree(array, sizeof(HnFType) * allocatedSize);
    }
    array = NULL;
    length = 0;
    allocatedSize = 0;
}

void
HnFTypeVectorObj::ensureSize(int requiredSize)
{
    int oldSize, newSize;

    if ( (oldSize = allocatedSize) >= requiredSize ) {
	return;
    }

    if ( (newSize = oldSize) == 0 ) {
	newSize = initialSize;
    }

    while ( newSize < requiredSize ) {
	newSize *= 2;
    }

    array = (HnFType *)HnRealloc(array,
				 sizeof(HnFType) * oldSize,
				 sizeof(HnFType) * newSize);

    allocatedSize = newSize;
}

HnFTypeVectorObj::HnFTypeVectorObj(void)
{
    initialize();
}

HnFTypeVectorObj::HnFTypeVectorObj(const HnFTypeVector &ptr)
{
    initialize();

    removeAllElements();
    addElements(ptr);
}

HnFTypeVectorObj::~HnFTypeVectorObj(void)
{
    dispose();
}

void
HnFTypeVectorObj::removeAllElements(void)
{
    dispose();
}

void
HnFTypeVectorObj::addElement(const HnFType &ptr)
{
    ensureSize(length + 1);

    array[length] = ptr;
    length ++;
}

void
HnFTypeVectorObj::addElements(const HnFTypeVector &ptr)
{
    int i;

    ensureSize(length + ptr.size());

    for ( i=0; i<ptr.size(); i++ ) {
	addElement(ptr[i]);
    }
}

void
HnFTypeVectorObj::insertElementAt(const HnFType &ptr, int index)
{
#ifdef DEBUGGING
    if ( index < 0 || index > length ) {
	HnAbort(HnFTypeNameString "VectorObj::insertElementAt: "
		"index is out of bounds.");
    }
#endif
    ensureSize(length + 1);

    memmove(array + index + 1, array + index,
	    sizeof(HnFType) * (length - index));

    array[index] = ptr;
    length ++;
}

HnFType
HnFTypeVectorObj::removeElementAt(int index)
{
    HnFType ptr = elementAt(index);
#ifdef DEBUGGING
    if ( index < 0 || index >= length ) {
	HnAbort(HnFTypeNameString "VectorObj::removeElementAt: "
		"index is out of bounds.");
    }
#endif
    memmove(array + index, array + index + 1,
	    sizeof(HnFType) * (length - index - 1));
    length --;

    return ptr;
}

void
HnFTypeVectorObj::setElementAt(const HnFType &ptr, int index)
{
#ifdef DEBUGGING
    if ( index < 0 || index >= length ) {
	HnAbort(HnFTypeNameString "VectorObj::setElementAt: "
		"index is out of bounds.");
    }
#endif
    array[index] = ptr;
}

void
HnFTypeVectorObj::swapElementsAt(int i, int j)
{
#ifdef DEBUGGING
    if ( i < 0 || i >= length || j < 0 || j >= length ) {
	HnAbort(HnFTypeNameString "VectorObj::swapElementsAt: "
		"index is out of bounds.");
    }
#endif
    HnFType temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

void
HnFTypeVectorObj::setSize(int newSize)
{
    if ( newSize > length ) {
	int i;

	ensureSize(newSize);

	for ( i=length; i<newSize; i++ ) {
	    array[i] = 0;
	}

	length = newSize;
    }
    else {
	length = newSize;
    }
}

HnBool
HnFTypeVectorObj::equals(const HnFTypeVector &ptr) const {
    int i;

    if ( length != ptr.size() ) {
	return HnFALSE;
    }

    for ( i=0; i<length; i++ ) {
	if ( array[i] != ptr[i] ) {
	    return HnFALSE;
	}
    }

    return HnTRUE;
}

int
HnFTypeVectorObj::indexOf(const HnFType &ptr, int fromIndex) const
{
    int i;

    for ( i=fromIndex; i<length; i++ ) {
	if ( array[i] == ptr ) {
	    return i;
	}
    }

    return -1;
}

HnFType &
HnFTypeVectorObj::lastElement(void) const
{
#ifdef DEBUGGING
    if ( length == 0 ) {
        HnAbort(HnFTypeNameString "VectorObj::lastElement: "
                "vector is empty.");
    }
#endif
    return array[length - 1];
}	

void
HnFTypeVectorObj::copyInto(const HnFTypeArray &ptr) const
{
    int i;

    for ( i=0; i<length; i++ ) {
	ptr[i] = elementAt(i);
    }
}

HnString
HnFTypeVectorObj::toString(void) const
{
    HnString string;

    string = HnFTypeNameString "Vector[ ";
    for ( int i=0; i<length; i++ ) {
	if ( i != 0 ) {
	    string += ",";
	}
	string += HnString::valueOf(array[i]);
    }
    string += " ]";

    return string;
}

/*
 * HnFTypeVector
 */

const HnFTypeVector HnFTypeVector::null;

HnFTypeVector
new_HnFTypeVector(void)
{
    return new HnFTypeVectorObj();
}

HnFTypeVector
new_HnFTypeVector(const HnFTypeVector &value)
{
    if ( value.isInvalid() ) {
	return (HnFTypeVectorObj *)NULL;
    }
    else {
	return new HnFTypeVectorObj(value);
    }
}

#undef TypeNameString0
#undef PointerName0
#undef ObjectName0
#undef ConstructorName0

#undef TypeNameString
#undef PointerName
#undef ObjectName
#undef ConstructorName

#undef HnFTypeNameString
#undef HnFTypeArray
#undef HnFTypeArrayObj
#undef new_HnFTypeArray
#undef HnFTypeVector
#undef HnFTypeVectorObj
#undef new_HnFTypeVector

#undef HnFTypeName
#undef HnFType
