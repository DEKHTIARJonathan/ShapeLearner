/*
 * HnGenericArray.cc
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
 * $Id: HnGenericArray.cc,v 1.2 2000/06/10 11:18:24 katayama Exp $
 */

#include "HnSRTree/HnGenericArray.hh"

#include <stdlib.h>
#include "HnSRTree/HnString.hh"

/*
 * HnGenericArrayObj
 */

HnGenericArrayObj::HnGenericArrayObj(const HnGenericArrayHandler *handler,
				     int len)
{
    this->handler = handler;
    allocate(len);
}

HnGenericArrayObj::HnGenericArrayObj(const HnGenericArrayHandler *handler,
				     const HnGenericArray &ptr)
{
    this->handler = handler;
    allocate(ptr.getLength());
    assignElements(ptr);
}

HnGenericArrayObj::~HnGenericArrayObj(void)
{
    dispose();
}

void
HnGenericArrayObj::assignElements(const HnGenericArray &ptr)
{
    int i;

    for ( i=0; i<length; i++ ) {
	handler->assignElement(getElementAt(i), ptr.getElementAt(i));
    }
}

HnBool
HnGenericArrayObj::equals(const HnGenericArray &ptr) const
{
    if ( length == ptr.getLength() ) {
	int i;

	for ( i=0; i<length; i++ ) {
	    if ( !handler->equalElements(getElementAt(i),
					 ptr.getElementAt(i)) ) {
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
HnGenericArrayObj::toString(void) const
{
    HnString string;
    int i;

    string = handler->getClassName();

    string += "ArrayObj[ ";
    for ( i=0; i<length; i++ ) {
	if ( i != 0 ) {
	    string += ",";
	}
	string += handler->elementToString(getElementAt(i));
    }
    string += " ]";

    return string;
}

/*
 * HnGenericArray
 */

const HnGenericArray HnGenericArray::null;

HnString
HnGenericArray::toString(void) const
{
    return getObject()->toString();
}

/*
 * HnGenericVectorObj
 */

const int HnGenericVectorObj::initialSize = 4;

void
HnGenericVectorObj::initialize(void)
{
    array = NULL;
    length = 0;
    allocatedSize = 0;
}

void
HnGenericVectorObj::dispose(void)
{
    if ( array != NULL ) {
	int i;
	for ( i=0; i<length; i++ ) {
	    handler->releaseElement(array[i]);
	}
	HnFree(array, sizeof(void *) * allocatedSize );
    }
    array = NULL;
    length = 0;
    allocatedSize = 0;
}

void
HnGenericVectorObj::ensureSize(int requiredSize)
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

    array = (void **)HnRealloc(array, 
			       sizeof(void *) * oldSize,
			       sizeof(void *) * newSize);

    allocatedSize = newSize;
}

HnGenericVectorObj::HnGenericVectorObj(const HnGenericVectorHandler *handler)
{
    this->handler = handler;
    initialize();
}

HnGenericVectorObj::HnGenericVectorObj(const HnGenericVectorHandler *handler,
				       const HnGenericVector &ptr)
{
    this->handler = handler;
    initialize();

    removeAllElements();
    addElements(ptr);
}

HnGenericVectorObj::~HnGenericVectorObj(void)
{
    dispose();
}

void
HnGenericVectorObj::removeAllElements(void)
{
    dispose();
}

void
HnGenericVectorObj::addElement(void *newElement)
{
    ensureSize(length + 1);

    array[length] = handler->allocateElement(newElement);
    length ++;
}

void
HnGenericVectorObj::addElements(const HnGenericVector &ptr)
{
    int i;

    ensureSize(length + ptr.size());

    for ( i=0; i<ptr.size(); i++ ) {
	addElement(ptr.elementAt(i));
    }
}

void
HnGenericVectorObj::insertElementAt(void *newElement, int index)
{
#ifdef DEBUGGING
    if ( index < 0 || index > length ) {
	HnAbort("%sVectorObj::insertElementAt: index is out of bounds.",
		handler->getClassName());
    }
#endif
    ensureSize(length + 1);

    memmove(array + index + 1, array + index,
	    sizeof(void *) * (length - index));

    array[index] = handler->allocateElement(newElement);
    length ++;
}

void
HnGenericVectorObj::removeElementAt(int index)
{
#ifdef DEBUGGING
    if ( index < 0 || index >= length ) {
	HnAbort("%sVectorObj::removeElementAt: index is out of bounds.",
		handler->getClassName());
    }
#endif
    handler->releaseElement(array[index]);

    memmove(array + index, array + index + 1,
	    sizeof(void *) * (length - index - 1));
    length --;
}

void
HnGenericVectorObj::setElementAt(void *newElement, int index)
{
#ifdef DEBUGGING
    if ( index < 0 || index >= length ) {
	HnAbort("%sVectorObj::setElementAt: index is out of bounds.",
		handler->getClassName());
    }
#endif
    handler->releaseElement(array[index]);

    array[index] = handler->allocateElement(newElement);
}

void
HnGenericVectorObj::swapElementsAt(int i, int j)
{
#ifdef DEBUGGING
    if ( i < 0 || i >= length || j < 0 || j >= length ) {
	HnAbort("%sVectorObj::swapElementsAt: index is out of bounds.",
		handler->getClassName());
    }
#endif
    void *ptr = array[i];
    array[i] = array[j];
    array[j] = ptr;
}

void
HnGenericVectorObj::setSize(int newSize)
{
    int i;

    if ( newSize > length ) {
	ensureSize(newSize);

	for ( i=length; i<newSize; i++ ) {
	    array[i] = handler->allocateElement(NULL);
	}

	length = newSize;
    }
    else {
	for ( i=newSize; i<length; i++) {
	    handler->releaseElement(array[i]);
	}

	length = newSize;
    }
}

HnBool
HnGenericVectorObj::equals(const HnGenericVector &ptr) const {
    int i;

    if ( length != ptr.size() ) {
	return HnFALSE;
    }

    for ( i=0; i<length; i++ ) {
	if ( !handler->equalElements(array[i], ptr.elementAt(i)) ) {
	    return HnFALSE;
	}
    }

    return HnTRUE;
}

int
HnGenericVectorObj::indexOf(void *element, int fromIndex) const
{
    int i;

    for ( i=fromIndex; i<length; i++ ) {
	if ( handler->equalElements(array[i], element) ) {
	    return i;
	}
    }

    return -1;
}

void *
HnGenericVectorObj::lastElement(void) const
{
#ifdef DEBUGGING
    if ( length == 0 ) {
	HnAbort("%sVectorObj::lastElement: vector is empty.",
		handler->getClassName());
    }
#endif
    return array[length - 1];
}

HnString
HnGenericVectorObj::toString(void) const
{
    HnString string;

    string = handler->getClassName();

    string += "VectorObj[ ";
    for ( int i=0; i<length; i++ ) {
	if ( i != 0 ) {
	    string += ",";
	}
	string += handler->elementToString(array[i]);
    }
    string += " ]";

    return string;
}

/*
 * HnGenericVector
 */

const HnGenericVector HnGenericVector::null;

HnString
HnGenericVector::toString(void) const
{
    return getObject()->toString();
}
