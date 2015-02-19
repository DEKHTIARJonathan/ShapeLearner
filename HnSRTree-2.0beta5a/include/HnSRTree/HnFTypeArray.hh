/*
 * HnFTypeArray.hh
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
 * $Id: HnFTypeArray.hh,v 1.4 2002/09/11 02:54:04 katayama Exp $
 */

#ifndef HnFTypeName
#define HnFTypeName	HnFType
#endif

#define TypeNameString0(type)		#type
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

#include "HnSRTree/HnPointer.hh"
#include "HnSRTree/HnObject.hh"

/*
 * HnFTypeArray
 */

class _HNSRTIMP HnFTypeArray: public HnPointer {
public:
    int length;

private:
    friend class HnFTypeArrayObj;
    HnFTypeArray(const HnFTypeArrayObj *ptr);
    HnFTypeArrayObj *getObject(void) const {
	return (HnFTypeArrayObj *)HnPointer::getObject();
    }

public:
    static const HnFTypeArray null;
    HnFTypeArray(void) {
	this->length = -1;
    }

public:
    /* constructors */
    _HNSRTIMP friend HnFTypeArray new_HnFTypeArray(int length);
    _HNSRTIMP friend HnFTypeArray new_HnFTypeArray(const HnFTypeArray &ptr);

    /* getElementAt */
    HnFType &getElementAt(int i) const;
    HnFType &operator[](int i) const;
    HnFType *getElements(void) const;

    /* utilities */
    HnBool equals(const HnFTypeArray &ptr) const;
    HnString toString(void) const;
};

/*
 * HnFTypeArrayObj
 */

class HnFTypeArrayObj: public HnObject {
private:
    HnFType *array;
    int length;

    void allocate(int len) {
	this->array = (HnFType *)HnMalloc(sizeof(HnFType) * len);
	this->length = len;
    }

public:
    /* constructor and destructor */
    HnFTypeArrayObj(int length);
    HnFTypeArrayObj(const HnFTypeArray &ptr);
    ~HnFTypeArrayObj(void);

    /* length and getElementAt */
    int getLength(void) const {
	return length;
    }
    HnFType &getElementAt(int index) const {
#ifdef DEBUGGING
	if ( index < 0 || index >= length ) {
	    HnAbort(HnFTypeNameString "ArrayObj::getElementAt: "
		    "index is out of bounds.");
	}
#endif
	return array[index];
    }
    HnFType *getElements(void) const {
	return array;
    }

    /* utilities */
    HnBool equals(const HnFTypeArray &ptr) const;
    HnString toString(void) const;
};

/*
 * HnFTypeArray (inline methods)
 */

inline HnFType &
HnFTypeArray::getElementAt(int i) const
{
    return getObject()->getElementAt(i);
}

inline HnFType &
HnFTypeArray::operator[](int i) const
{
    return getElementAt(i);
}

inline HnFType *
HnFTypeArray::getElements(void) const
{
    return getObject()->getElements();
}

inline HnBool
HnFTypeArray::equals(const HnFTypeArray &ptr) const
{
    return getObject()->equals(ptr);
}

inline HnString
HnFTypeArray::toString(void) const
{
    return getObject()->toString();
}

/*
 * HnFTypeVector
 */

class _HNSRTIMP HnFTypeVector: public HnPointer {
private:
    friend class HnFTypeVectorObj;
    HnFTypeVector(const HnFTypeVectorObj *ptr) {
	HnPointer::assign((HnObject *)ptr);
    }
    HnFTypeVectorObj *getObject(void) const {
	return (HnFTypeVectorObj *)HnPointer::getObject();
    }

public:
    static const HnFTypeVector null;
    HnFTypeVector(void) {}

public:
    /* constructors */
    _HNSRTIMP friend HnFTypeVector new_HnFTypeVector(void);
    _HNSRTIMP friend HnFTypeVector new_HnFTypeVector(const HnFTypeVector &ptr);

    /* editing */
    void removeAllElements(void);
    void addElement(const HnFType &ptr);
    void addElements(const HnFTypeVector &ptr);
    void insertElementAt(const HnFType &ptr, int index);
    HnFType removeElementAt(int index);
    void setElementAt(const HnFType &ptr, int index);
    void swapElementsAt(int i, int j);
    void setSize(int newSize);

    /* reading */
    int size(void) const;
    HnFType &elementAt(int i) const;
    HnFType &operator[](int i) const;

    /* utilities */
    void pushElement(const HnFType &ptr);
    HnFType popElement(void);
    HnBool equals(const HnFTypeVector &ptr) const;
    int indexOf(const HnFType &ptr, int fromIndex) const;
    int indexOf(const HnFType &ptr) const;
    HnFType &lastElement(void) const;
    void copyInto(const HnFTypeArray &array) const;
    HnString toString(void) const;
};

/*
 * HnFTypeVectorObj
 */

class HnFTypeVectorObj: public HnObject {
private:
    HnFType *array;
    int length;
    int allocatedSize;

    static const int initialSize;

    void initialize(void);
    void dispose(void);
    void ensureSize(int requiredSize);

public:
    /* constructor and destructor */
    HnFTypeVectorObj(void);
    HnFTypeVectorObj(const HnFTypeVector &ptr);
    ~HnFTypeVectorObj(void);

    /* editing */
    void removeAllElements(void);
    void addElement(const HnFType &ptr);
    void addElements(const HnFTypeVector &ptr);
    void insertElementAt(const HnFType &ptr, int index);
    HnFType removeElementAt(int index);
    void setElementAt(const HnFType &ptr, int index);
    void swapElementsAt(int i, int j);
    void setSize(int newSize);

    /* reading */
    int size(void) const { return length; }
    HnFType &elementAt(int i) const {
#ifdef DEBUGGING
	if ( i < 0 || i >= length ) {
	    HnAbort(HnFTypeNameString "VectorObj::elementAt: "
		    "index is out of bounds.");
	}
#endif
	return array[i];
    }

    /* utilities */
    HnBool equals(const HnFTypeVector &ptr) const;
    int indexOf(const HnFType &ptr, int fromIndex) const;
    HnFType &lastElement(void) const;
    void copyInto(const HnFTypeArray &array) const;
    HnString toString(void) const;
};

/*
 * HnFTypeVector (inline methods)
 */

inline void
HnFTypeVector::removeAllElements(void)
{
    getObject()->removeAllElements();
}

inline void
HnFTypeVector::addElement(const HnFType &ptr)
{
    getObject()->addElement(ptr);
}

inline void
HnFTypeVector::addElements(const HnFTypeVector &ptr)
{
    getObject()->addElements(ptr);
}

inline void
HnFTypeVector::insertElementAt(const HnFType &ptr, int index)
{
    getObject()->insertElementAt(ptr, index);
}

inline HnFType
HnFTypeVector::removeElementAt(int index)
{
    return getObject()->removeElementAt(index);
}

inline void
HnFTypeVector::setElementAt(const HnFType &ptr, int index)
{
    getObject()->setElementAt(ptr, index);
}

inline void
HnFTypeVector::swapElementsAt(int i, int j)
{
    getObject()->swapElementsAt(i, j);
}

inline void
HnFTypeVector::setSize(int newSize)
{
    getObject()->setSize(newSize);
}

inline int
HnFTypeVector::size(void) const
{
    return getObject()->size();
}

inline HnFType &
HnFTypeVector::elementAt(int i) const
{
    return getObject()->elementAt(i);
}

inline HnFType &
HnFTypeVector::operator[](int i) const
{
    return elementAt(i);
}

inline void
HnFTypeVector::pushElement(const HnFType &ptr)
{
    addElement(ptr);
}

inline HnFType
HnFTypeVector::popElement(void)
{
    return removeElementAt(size() - 1);
}

inline HnBool
HnFTypeVector::equals(const HnFTypeVector &ptr) const
{
    return getObject()->equals(ptr);
}

inline int
HnFTypeVector::indexOf(const HnFType &ptr, int fromIndex) const
{
    return getObject()->indexOf(ptr, fromIndex);
}

inline int
HnFTypeVector::indexOf(const HnFType &ptr) const
{
    return indexOf(ptr, 0);
}

inline HnFType &
HnFTypeVector::lastElement(void) const
{
    return getObject()->lastElement();
}

inline void
HnFTypeVector::copyInto(const HnFTypeArray &array) const
{
    getObject()->copyInto(array);
}

inline HnString
HnFTypeVector::toString(void) const
{
    return getObject()->toString();
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
