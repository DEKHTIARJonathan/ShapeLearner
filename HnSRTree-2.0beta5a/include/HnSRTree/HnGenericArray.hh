/*
 * HnGenericArray.hh
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
 * 11/23/95 katayama@rd.nacsis.ac.jp
 * $Id: HnGenericArray.hh,v 1.3 2002/09/11 02:55:24 katayama Exp $
 */

#ifndef _HnGenericArray_hh
#define _HnGenericArray_hh

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnPointer.hh"

/*
 * HnGenericArrayHandler
 */

struct HnGenericArrayHandler {
    char *(*getClassName)(void);
    void *(*allocateArray)(int len);
    void (*releaseArray)(void *array);
    void *(*getElementAt)(void *array, int index);
    void (*assignElement)(void *dst, void *src);
    HnBool (*equalElements)(void *element1, void *element2);
    HnString (*elementToString)(void *element);
};

/*
 * HnGenericArray
 */

class _HNSRTIMP HnGenericArray: public HnPointer {
private:
    friend class HnGenericArrayObj;
    HnGenericArray(const HnGenericArrayObj *ptr) {
	HnPointer::assign((HnObject *)ptr);
    }
    HnGenericArrayObj *getObject(void) const {
	return (HnGenericArrayObj *)HnPointer::getObject();
    }

public:
    static const HnGenericArray null;
    HnGenericArray(void) {}

public:
    /* array and length */
    void *getArray(void) const;
    int getLength(void) const;

    /* getElementAt */
    void *getElementAt(int index) const;

    /* utilities */
    HnBool equals(const HnGenericArray &ptr) const;
    HnString toString(void) const;
};

/*
 * HnGenericArrayObj
 */

class HnGenericArrayObj: public HnObject {
private:
    const HnGenericArrayHandler *handler;
    void *array;
    int length;

private:
    void allocate(int len) {
	this->array = handler->allocateArray(len);
	this->length = len;
    }
    void dispose(void) {
	handler->releaseArray(array);
    }
    void assignElements(const HnGenericArray &ptr);

public:
    /* constructor and destructor */
    HnGenericArrayObj(const HnGenericArrayHandler *handler, int length);
    HnGenericArrayObj(const HnGenericArrayHandler *handler,
		      const HnGenericArray &ptr);
    ~HnGenericArrayObj(void);

    /* array and length */
    void *getArray(void) const {
	return array;
    }
    int getLength(void) const {
	return length;
    }

    /* getElementAt */
    void *getElementAt(int index) const {
	return handler->getElementAt(array, index);
    }

    /* utilities */
    HnBool equals(const HnGenericArray &ptr) const;
    HnString toString(void) const;
};

/*
 * HnGenericArray (inline methods)
 */

inline void *
HnGenericArray::getArray(void) const
{
    return getObject()->getArray();
}

inline int
HnGenericArray::getLength(void) const
{
    return getObject()->getLength();
}

inline void *
HnGenericArray::getElementAt(int i) const
{
    return getObject()->getElementAt(i);
}

inline HnBool
HnGenericArray::equals(const HnGenericArray &ptr) const
{
    return getObject()->equals(ptr);
}

/*
 * HnGenericVectorHandler
 */

struct HnGenericVectorHandler {
    char *(*getClassName)(void);
    void *(*allocateElement)(void *element);
    void (*releaseElement)(void *element);
    HnBool (*equalElements)(void *element1, void *element2);
    HnString (*elementToString)(void *element);
};

/*
 * HnGenericVector
 */

class _HNSRTIMP HnGenericVector: public HnPointer {
private:
    friend class HnGenericVectorObj;
    HnGenericVector(const HnGenericVectorObj *ptr) {
	HnPointer::assign((HnObject *)ptr);
    }
    HnGenericVectorObj *getObject(void) const {
	return (HnGenericVectorObj *)HnPointer::getObject();
    }

public:
    static const HnGenericVector null;
    HnGenericVector(void) {}

public:
    /* editing */
    void removeAllElements(void);
    void addElement(void *newElement);
    void addElements(const HnGenericVector &ptr);
    void insertElementAt(void *newElement, int index);
    void removeElementAt(int index);
    void setElementAt(void *newElement, int index);
    void swapElementsAt(int i, int j);
    void setSize(int newSize);

    /* reading */
    int size(void) const;
    void *elementAt(int i) const;

    /* utilities */
    HnBool equals(const HnGenericVector &ptr) const;
    int indexOf(void *element, int fromIndex) const;
    void *lastElement(void) const;
    HnString toString(void) const;
};

/*
 * HnGenericVectorObj
 */

class HnGenericVectorObj: public HnObject {
private:
    const HnGenericVectorHandler *handler;
    void **array;
    int length;
    int allocatedSize;

    static const int initialSize;

    void initialize(void);
    void dispose(void);
    void ensureSize(int requiredSize);

public:
    /* constructor and destructor */
    HnGenericVectorObj(const HnGenericVectorHandler *handler);
    HnGenericVectorObj(const HnGenericVectorHandler *handler,
		       const HnGenericVector &ptr);
    ~HnGenericVectorObj(void);

    /* editing */
    void removeAllElements(void);
    void addElement(void *newElement);
    void addElements(const HnGenericVector &ptr);
    void insertElementAt(void *newElement, int index);
    void removeElementAt(int index);
    void setElementAt(void *newElement, int index);
    void swapElementsAt(int i, int j);
    void setSize(int newSize);

    /* reading */
    int size(void) const { return length; }
    void *elementAt(int i) const {
#ifdef DEBUGGING
	if ( i < 0 || i >= length ) {
	    HnAbort("%s::elementAt: index is out of bounds.",
		    handler->getClassName());
	}
#endif
	return array[i];
    }

    /* utilities */
    HnBool equals(const HnGenericVector &ptr) const;
    int indexOf(void *element, int fromIndex) const;
    void *lastElement(void) const;
    HnString toString(void) const;
};

/*
 * HnGenericVector (inline methods)
 */

inline void
HnGenericVector::removeAllElements(void)
{
    getObject()->removeAllElements();
}

inline void
HnGenericVector::addElement(void *newElement)
{
    getObject()->addElement(newElement);
}

inline void
HnGenericVector::addElements(const HnGenericVector &ptr)
{
    getObject()->addElements(ptr);
}

inline void
HnGenericVector::insertElementAt(void *newElement, int index)
{
    getObject()->insertElementAt(newElement, index);
}

inline void
HnGenericVector::removeElementAt(int index)
{
    getObject()->removeElementAt(index);
}

inline void
HnGenericVector::setElementAt(void *newElement, int index)
{
    getObject()->setElementAt(newElement, index);
}

inline void
HnGenericVector::swapElementsAt(int i, int j)
{
    getObject()->swapElementsAt(i, j);
}

inline void
HnGenericVector::setSize(int newSize)
{
    getObject()->setSize(newSize);
}

inline int
HnGenericVector::size(void) const
{
    return getObject()->size();
}

inline void *
HnGenericVector::elementAt(int i) const
{
    return getObject()->elementAt(i);
}

inline HnBool
HnGenericVector::equals(const HnGenericVector &ptr) const
{
    return getObject()->equals(ptr);
}

inline int
HnGenericVector::indexOf(void *element, int fromIndex) const
{
    return getObject()->indexOf(element, fromIndex);
}

inline void *
HnGenericVector::lastElement(void) const
{
    return getObject()->lastElement();
}

#endif /* _HnGenericArray_hh */
