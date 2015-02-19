/*
 * HnClassArray.cc
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
 * $Id: HnClassArray.cc,v 1.1 2000/06/06 08:29:11 katayama Exp $
 */

#define ClassNameString0(type)		#type
#define PointerName0(type,suffix)	type ## suffix
#define ObjectName0(type,suffix)	type ## suffix ## Obj
#define ConstructorName0(type,suffix)	new_## type ## suffix

#define ClassNameString(type)		ClassNameString0(type)
#define PointerName(type,suffix)	PointerName0(type,suffix)
#define ObjectName(type,suffix)		ObjectName0(type,suffix)
#define ConstructorName(type,suffix)	ConstructorName0(type,suffix)

#define HnClassNameString	ClassNameString(HnClass)
#define HnClassArray		PointerName(HnClass,Array)
#define HnClassArrayObj		ObjectName(HnClass,Array)
#define new_HnClassArray	ConstructorName(HnClass,Array)
#define HnClassVector		PointerName(HnClass,Vector)
#define HnClassVectorObj	ObjectName(HnClass,Vector)
#define new_HnClassVector	ConstructorName(HnClass,Vector)

#include <stdlib.h>

/*
 * HnGenericArrayHandler
 */

static char *
getClassName(void)
{
    return HnClassNameString;
}

static void *
allocateArray(int len)
{
    return new HnClass[len];
}

static void
releaseArray(void *array)
{
    delete[] ((HnClass *)array);
}

static void *
getElementAt(void *array, int index)
{
    return (void *)(((HnClass *)array) + index);
}

static void
assignElement(void *dst, void *src)
{
    *((HnClass *)dst) = *((HnClass *)src);
}

static HnBool
equalElements(void *element1, void *element2)
{
    return ((HnClass *)element1)->equals(*((HnClass *)element2));
}

static HnString
elementToString(void *element)
{
    return ((HnClass *)element)->toString();
}

static HnGenericArrayHandler arrayHandler = {
    getClassName,
    allocateArray,
    releaseArray,
    getElementAt,
    assignElement,
    equalElements,
    elementToString
};

static void *
allocateElement(void *element)
{
    if ( element == NULL ) {
	return new HnClass(HnClass::null);
    }
    else {
	return new HnClass(*((HnClass *)element));
    }
}

static void
releaseElement(void *element)
{
    delete ((HnClass *)element);
}

static HnGenericVectorHandler vectorHandler = {
    getClassName,
    allocateElement,
    releaseElement,
    equalElements,
    elementToString
};

/*
 * HnClassArrayObj
 */

HnClassArrayObj::HnClassArrayObj(int length):
    HnGenericArrayObj(&arrayHandler, length)
{
}

HnClassArrayObj::HnClassArrayObj(const HnClassArray &ptr):
    HnGenericArrayObj(&arrayHandler, ptr)
{
}

/*
 * HnClassArray
 */

const HnClassArray HnClassArray::null;

HnClassArray::HnClassArray(const HnClassArrayObj *ptr)
{
    HnPointer::assign((HnObject *)ptr);
    if ( ptr != NULL ) {
	this->length = ptr->getLength();
    }
    else {
	this->length = -1;
    }
}

HnClassArray
new_HnClassArray(int length)
{
    return new HnClassArrayObj(length);
}

HnClassArray
new_HnClassArray(const HnClassArray &value)
{
    if ( value.isInvalid() ) {
	return (HnClassArrayObj *)NULL;
    }
    else {
	return new HnClassArrayObj(value);
    }
}

#ifdef DEBUGGING
HnClass &
HnClassArray::getElementAt(int index) const
{
    if ( index < 0 || index >= length ) {
	HnAbort(HnClassNameString "ArrayObj::getElementAt: "
		"index is out of bounds.");
    }
    return ((HnClass *)HnGenericArray::getArray())[index];
}
#endif

/*
 * HnClassVectorObj
 */

HnClassVectorObj::HnClassVectorObj(void):
    HnGenericVectorObj(&vectorHandler)
{
}

HnClassVectorObj::HnClassVectorObj(const HnClassVector &ptr):
    HnGenericVectorObj(&vectorHandler, ptr)
{
}

void
HnClassVectorObj::copyInto(const HnClassArray &ptr) const
{
    int length = size();
    int i;

    for ( i=0; i<length; i++ ) {
	ptr[i] = *((HnClass *)elementAt(i));
    }
}

/*
 * HnClassVector
 */

const HnClassVector HnClassVector::null;

HnClassVector
new_HnClassVector(void)
{
    return new HnClassVectorObj();
}

HnClassVector
new_HnClassVector(const HnClassVector &value)
{
    if ( value.isInvalid() ) {
	return (HnClassVectorObj *)NULL;
    }
    else {
	return new HnClassVectorObj(value);
    }
}

#undef ClassNameString0
#undef PointerName0
#undef ObjectName0
#undef ConstructorName0

#undef ClassNameString
#undef PointerName
#undef ObjectName
#undef ConstructorName

#undef HnClassNameString
#undef HnClassArray
#undef HnClassArrayObj
#undef new_HnClassArray
#undef HnClassVector
#undef HnClassVectorObj
#undef new_HnClassVector

#undef HnClass
