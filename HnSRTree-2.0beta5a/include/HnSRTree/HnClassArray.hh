/*
 * HnClassArray.hh
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
 * $Id: HnClassArray.hh,v 1.4 2002/09/11 02:45:16 katayama Exp $
 */

#include "HnSRTree/HnGenericArray.hh"

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

/*
 * HnClassArray
 */

class _HNSRTIMP HnClassArray: public HnGenericArray {
public:
    int length;

private:
    friend class HnClassArrayObj;
    HnClassArray(const HnClassArrayObj *ptr);
    HnClassArrayObj *getObject(void) const {
	return (HnClassArrayObj *)HnPointer::getObject();
    }

public:
    static const HnClassArray null;
    HnClassArray(void) {
	this->length = -1;
    }

public:
    /* constructors */
    _HNSRTIMP friend HnClassArray new_HnClassArray(int length);
    _HNSRTIMP friend HnClassArray new_HnClassArray(const HnClassArray &ptr);

    /* getElementAt */
#ifdef DEBUGGING
    HnClass &getElementAt(int i) const;
#else
    HnClass &getElementAt(int i) const {
	return ((HnClass *)HnGenericArray::getArray())[i];
    }
#endif
    HnClass &operator[](int i) const {
	return getElementAt(i);
    }

    /* utilities */
    HnBool equals(const HnClassArray &ptr) const;
    HnString toString(void) const;
};

/*
 * HnClassArrayObj
 */

class HnClassArrayObj: public HnGenericArrayObj {
public:
    /* constructor and destructor */
    HnClassArrayObj(int length);
    HnClassArrayObj(const HnClassArray &ptr);
};

/*
 * HnClassArray (inline methods)
 */

inline HnBool
HnClassArray::equals(const HnClassArray &ptr) const
{
    return HnGenericArray::equals(ptr);
}

inline HnString
HnClassArray::toString(void) const
{
    return HnGenericArray::toString();
}

/*
 * HnClassVector
 */

class _HNSRTIMP HnClassVector: public HnGenericVector {
private:
    friend class HnClassVectorObj;
    HnClassVector(const HnClassVectorObj *ptr) {
	HnPointer::assign((HnObject *)ptr);
    }
    HnClassVectorObj *getObject(void) const {
	return (HnClassVectorObj *)HnPointer::getObject();
    }

public:
    static const HnClassVector null;
    HnClassVector(void) {}

public:
    /* constructors */
    _HNSRTIMP friend HnClassVector new_HnClassVector(void);
    _HNSRTIMP friend HnClassVector new_HnClassVector(const HnClassVector &ptr);

    /* reading */
    HnClass &elementAt(int i) const;
    HnClass &operator[](int i) const;

    /* editing */
    void addElement(const HnClass &ptr);
    void addElements(const HnClassVector &ptr);
    void insertElementAt(const HnClass &ptr, int index);
    HnClass removeElementAt(int index);
    void setElementAt(const HnClass &ptr, int index);

    /* utilities */
    void pushElement(const HnClass &ptr);
    HnClass popElement(void);
    HnBool equals(const HnClassVector &ptr) const;
    int indexOf(const HnClass &ptr, int fromIndex) const;
    int indexOf(const HnClass &ptr) const;
    HnClass &lastElement(void) const;
    void copyInto(const HnClassArray &array) const;
};

/*
 * HnClassVectorObj
 */

class HnClassVectorObj: public HnGenericVectorObj {
protected:
    char *getClassName(void) const { return HnClassNameString; }
    void *allocateElement(void *element) const;
    void releaseElement(void *element) const;
    HnBool equalElements(void *element1, void *element2) const;
    HnString elementToString(void *element) const;

public:
    /* constructor and destructor */
    HnClassVectorObj(void);
    HnClassVectorObj(const HnClassVector &ptr);

    /* utilities */
    void copyInto(const HnClassArray &array) const;
};

/*
 * HnClassVector (inline methods)
 */

inline void
HnClassVector::addElement(const HnClass &ptr)
{
    HnGenericVector::addElement((void *)&ptr);
}

inline void
HnClassVector::addElements(const HnClassVector &ptr)
{
    HnGenericVector::addElements(ptr);
}

inline void
HnClassVector::insertElementAt(const HnClass &ptr, int index)
{
    HnGenericVector::insertElementAt((void *)&ptr, index);
}

inline HnClass
HnClassVector::removeElementAt(int index)
{
    HnClass ptr = *((HnClass *)HnGenericVector::elementAt(index));
    HnGenericVector::removeElementAt(index);
    return ptr;
}

inline void
HnClassVector::setElementAt(const HnClass &ptr, int index)
{
    HnGenericVector::setElementAt((void *)&ptr, index);
}

inline HnClass &
HnClassVector::elementAt(int i) const
{
    return *((HnClass *)HnGenericVector::elementAt(i));
}

inline HnClass &
HnClassVector::operator[](int i) const
{
    return elementAt(i);
}

inline void
HnClassVector::pushElement(const HnClass &ptr)
{
    addElement(ptr);
}

inline HnClass
HnClassVector::popElement(void)
{
    return removeElementAt(size() - 1);
}

inline HnBool
HnClassVector::equals(const HnClassVector &ptr) const
{
    return HnGenericVector::equals(ptr);
}

inline int
HnClassVector::indexOf(const HnClass &ptr, int fromIndex) const
{
    return HnGenericVector::indexOf((void *)&ptr, fromIndex);
}

inline int
HnClassVector::indexOf(const HnClass &ptr) const  {
    return indexOf(ptr, 0);
}

inline HnClass &
HnClassVector::lastElement(void) const
{
    return *((HnClass *)HnGenericVector::lastElement());
}

inline void
HnClassVector::copyInto(const HnClassArray &array) const
{
    getObject()->copyInto(array);
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
