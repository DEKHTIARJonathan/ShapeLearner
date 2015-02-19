/*
 * HnPointer.hh
 * Copyright (C) 1997 Norio Katayama
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
 * $Id: HnPointer.hh,v 1.6 2002/09/10 12:07:14 katayama Exp $
 */

#ifndef _HnPointer_hh
#define _HnPointer_hh

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include <stdio.h>
#include "HnSRTree/HnBool.h"
#include "HnSRTree/HnObject.hh"

class _HNSRTIMP HnPointer {
public:
    static const HnPointer null;
    enum HnReference { None, External, Internal };

private:
    HnReference reference;
    HnObject *object;

private:
    void initialize(void) {
	reference = None;
	object = NULL;
    }
    void dispose(void) {
	if ( reference == External && object != NULL ) {
	    HnUnref(object);
	}
	reference = None;
	object = NULL;
    }
    void checkObject(void) const;

protected:
    HnPointer &assign(HnObject *ptr, HnReference type = External);
    HnPointer &assign(const HnPointer &pointer, HnReference type = External) {
	assign(pointer.object, type);
	return *this;
    }
#ifdef DEBUGGING
    HnObject *getObject(void) const;
#else
    HnObject *getObject(void) const {
	return object;
    }
#endif

public:
    HnPointer(void) {
	initialize();
    }
    ~HnPointer(void);
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 8)
    static void *operator new(size_t size) {
	return HnMalloc(size);
    }
    static void operator delete(void *ptr, size_t size) {
	HnFree(ptr, size);
    }
    static void *operator new[](size_t size) {
	return HnMalloc(size);
    }
    static void operator delete[](void *ptr, size_t size) {
	HnFree(ptr, size);
    }
#endif

    int isValid(void) const { return (object != NULL); }
    int isInvalid(void) const { return (object == NULL); }
    HnPointer &invalidate(void) { return assign(NULL, External); }

    HnPointer &operator=(const HnPointer &pointer) {
	assign(pointer.object, pointer.reference);
	return *this;
    }
    /* compares the equivalence of pointers */
    HnBool operator==(const HnPointer &pointer) const {
	if ( isInvalid() ) {
	    if ( pointer.isInvalid() ) {
		return HnTRUE;
	    }
	    else {
		return HnFALSE;
	    }
	}
	else {
	    if ( pointer.isInvalid() ) {
		return HnFALSE;
	    }
	    else {
		return (getObject() == pointer.getObject());
	    }
	}
    }
    HnBool operator!=(const HnPointer &pointer) const {
	return !operator==(pointer);
    }
    HnPointer(const HnPointer &pointer) {
	initialize();
	assign(pointer.object, pointer.reference);
    }

    /* compares the equivalence of objects */
    HnBool equals(const HnPointer &ptr) const;

    HnString toString(void) const;
};

#endif /* _HnPointer_hh */
