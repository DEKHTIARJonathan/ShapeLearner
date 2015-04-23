/**------------------------------------------------------------------------
 * @file
 *
 * Copyright 2001 by the University of Toronto (UT).
 *
 *                       All Rights Reserved
 *
 * Permission to copy and modify this software and its documentation
 * only for internal use in your organization is hereby granted,
 * provided that this notice is retained thereon and on all copies.
 * UT makes no representations as to the suitability and operability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * UT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
 * SHALL UT BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * No other rights, including for example, the right to redistribute
 * this software and its documentation or the right to prepare
 * derivative works, are granted unless specifically provided in a
 * separate license agreement.
 *
 * Author: Diego Macrini (dmac@cs.toronto.edu)
 *
 * $Log: SmartPtr.h,v $
 * Revision 1.2  2002/05/16 22:51:05  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/
#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_

#include "stdafx.h"

template<class T> class SmartPtr;

template<class T> std::ostream& operator<<(std::ostream& os, const SmartPtr<T>& x);
template<class T> std::istream& operator>>(std::istream& is, SmartPtr<T>& x);

/*!
	\brief Wrapper class of a pointer to class T. It handles the destruction of T.

	This class wraps a pointer to an object of class T, an keeps track
	of the number of references to the same object. When the number of references
	becomes zero, the referenced object is destroyed.

	If the non-constant version of operator->() is called for an object with
	number of references grater than 1, a copy of the referenced object is performed
	by calling its member function CreateObject(). This implies that class T must
	have this function implemented.

	The need for requiring a function T::CreateObject() is that we don't know
	whether we have a pointer to an object T or to a class derived from T. Thus,
	the only solution is to force T to have a virtual function that will
	create and copy the correct type of object.
*/

template<class T> class SmartPtr
{
protected:
	struct DATAREF {
		T* pData;
		int nLinks;
		DATAREF() { nLinks = 1; }
	};

	DATAREF* ptr;

public:

	SmartPtr()
	{
		ptr = new DATAREF;
		ptr->pData = NULL;
	}

	SmartPtr(T* p)
	{
		ptr = new DATAREF;
		ptr->pData = p;
	}

	SmartPtr(const T* p)
	{
		ptr = new DATAREF;
		ptr->pData = (T*)p; //Supposely it's safe to discard 'const' here

		// There is someone else ponting at this object, so add one ref
		// s.t. we don't destroy the actual data in the destructor.
		ptr->nLinks++;
	}

	SmartPtr(const T& obj)
	{
		ptr = new DATAREF;
		ptr->pData = new T;
		*ptr->pData = obj; // Clone object
	}

	SmartPtr(const SmartPtr<T>& x)
	{
		x.ptr->nLinks++;
		ptr = x.ptr;
	}

	~SmartPtr()
	{
		if (--ptr->nLinks == 0)
		{
			if (ptr->pData)
				delete ptr->pData;

			delete ptr;
		}
	}

	//! Clones the referred object and returns a ptr with ref count equal to 1
	SmartPtr<T> Clone() const
	{
		ASSERT(!IsNull());
		return SmartPtr<T>(*ptr->pData);
	}

	const T* operator->() const
	{
		return ptr->pData;
	}

	bool operator==(const SmartPtr<T>& rhs) const
	{
		return ptr->pData == rhs.ptr->pData;
	}

	bool IsNull() const
	{
		return ptr->pData == NULL;
	}

	/*!
		@brief The non-constant arrow operator of a SmartPtr<T>
		implements a copy-on-write (COW) behaviour in which in is
		assumed that the pointer dereference is used to modify
		(write) the data. Thus, a copy of the data is created
		in order to protect its original state.

		The arrw operator is the only function that deals with
		the COW behaviour.
	*/
	T* operator->()
	{
		if (ptr->nLinks > 1)
		{
			ptr->nLinks--;
			T* p = ptr->pData->CreateObject();
			*p = *ptr->pData;

			ptr = new DATAREF;
			ptr->pData = p;

			WARNING(true, "possible not desired copy of data");
		}

		return ptr->pData;
	}

	SmartPtr<T>& operator=(const SmartPtr<T>& rhs)
	{
		rhs.ptr->nLinks++;	// protect against 'x = x'

		if (--ptr->nLinks == 0)
		{
			delete ptr->pData;
			delete ptr;
		}

		ptr = rhs.ptr;
		return *this;
	}

	SmartPtr<T>& operator=(T* rhs)
	{
		if (ptr->nLinks > 1)
		{
			ptr->nLinks--;
			ptr = new DATAREF;
		}
		else
		{
			delete ptr->pData;
		}

		ptr->pData = rhs;
		return *this;
	}

	int Links() const { return ptr->nLinks; }

	operator const T*() const { return ptr->pData; }
	operator const T&() const { return *ptr->pData; }
	operator const unsigned long() const { return (const unsigned long) ptr->pData; }

	T& operator*()	{ return *operator->(); }
	const T& operator*() const	{ return *operator->(); }

#ifdef WIN32
	template<class T> friend std::ostream& operator<<(std::ostream& os, const SmartPtr<T>& x);
	template<class T> friend std::istream& operator>>(std::istream& is, SmartPtr<T>& x);
#else
	friend std::ostream& operator<< <T>(std::ostream& os, const SmartPtr<T>& x);
	friend std::istream& operator>> <T>(std::istream& is, SmartPtr<T>& x);
#endif //WIN32
};

template<class T>
std::ostream& operator<<(std::ostream& os, const SmartPtr<T>& x) { return os << *(x.ptr->pData); }

template<class T>
std::istream& operator>>(std::istream& is, SmartPtr<T>& x) { return is >> *(x.ptr->pData); }

#endif //_SMART_PTR_H_
