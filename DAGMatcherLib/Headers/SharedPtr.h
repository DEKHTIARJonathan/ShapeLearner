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
 * $Log: SharedPtr.h,v $
 * Revision 1.2  2002/05/16 22:51:05  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/
#ifndef _SHARED_PTR_H_
#define _SHARED_PTR_H_

#include "SmartPtr.h"

/*!
	\brief Wrapper class of a pointer to class T. It handles the destruction of T.

	This class wraps a pointer to an object of class T, an keeps track
	of the number of references to the same object. When the number of references
	becomes zero, the referenced object is destroyed.
*/
template<class T> class SharedPtr : public SmartPtr<T>
{
public:
	SharedPtr() { }
	SharedPtr(T* p) : SmartPtr<T>(p) { }
	SharedPtr(const T& obj) : SmartPtr<T>(obj) { }
	SharedPtr(const SharedPtr<T>& x) : SmartPtr<T>(x) { }

	//! Clones the referred object and returns a ptr with ref count equal to 1
	SharedPtr<T> Clone() const
	{
		ASSERT(!this->IsNull());
		return SharedPtr<T>(*this->ptr->pData);
	}

	bool operator==(const SharedPtr<T>& rhs) const
	{
		return SmartPtr<T>::operator==(rhs);
	}

	SharedPtr<T>& operator=(const SharedPtr<T>& rhs) {
		SmartPtr<T>::operator=(rhs);
		return *this;
	}

	SharedPtr<T>& operator=(T* rhs) {
		SmartPtr<T>::operator=(rhs);
		return *this;
	}

	/*!
		@brief The non-constant arrow operator of a SharedPtr<T> dereferences
		the data pointer without creating a copy of the data.

		This is the only functions that is different among all others
		in the base class SmartPtr<T>.
	*/
	T* operator->() {
		return this->ptr->pData;
	}

	const T* operator->() const {
		return this->ptr->pData;
	}

	T& operator*() { return *operator->(); }
	const T& operator*() const	{ return *operator->(); }
};

#endif //_SHARED_PTR_H_
