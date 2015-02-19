/*
 * HnObject.hh
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
 * $Id: HnObject.hh,v 1.7 2000/06/10 11:18:26 katayama Exp $
 */

#ifndef _HnObject_hh
#define _HnObject_hh

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/config.h"
#include "HnSRTree/HnBool.h"
#include "HnSRTree/HnAbort.h"
#include "HnSRTree/HnMalloc.h"

#ifndef HAVE_MEMMOVE
extern "C" {
	extern void bcopy(void *s1, void *s2, int len);
};
#define memmove(dst, src, n)	bcopy(src, dst, n)
#endif

class HnPointer;
class HnString;

class HnObject {
    friend HnPointer;
private:
    /* class variables */
    static HnObject *busyObject;
    static int numBusyObjects;

    static HnObject *freeObject;
    static int numFreeObjects;

    static int MAGIC;

private:
    /* reference */
    int magic;
    int refCount;
    HnObject *prev;
    HnObject *next;

    void setBusy(void) {
	prev = busyObject;
	next = NULL;
	busyObject = this;

	if ( prev != NULL ) {
	    prev->next = this;
	}

	numBusyObjects ++;
    }
    void unsetBusy(void) {
	if ( prev != NULL ) {
	    prev->next = next;
	}
	if ( next != NULL ) {
	    next->prev = prev;
	}
	else {
	    busyObject = prev;
	}

	numBusyObjects --;
    }
    void setFree(void) {
	prev = freeObject;
	next = NULL;
	freeObject = this;

	if ( prev != NULL ) {
	    prev->next = this;
	}

	numFreeObjects ++;
    }
    void unsetFree(void) {
	if ( prev != NULL ) {
	    prev->next = next;
	}
	if ( next != NULL ) {
	    next->prev = prev;
	}
	else {
	    freeObject = prev;
	}

	numFreeObjects --;
    }

    /* constructor failure flag */
    HnBool constructorFailureFlag;

protected:
    void setConstructorFailureFlag(void) {
	constructorFailureFlag = HnTRUE;
    }

public:
    HnObject(void);
    virtual ~HnObject(void);
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

    static int countObjects(void) {
	return numBusyObjects + numFreeObjects;
    }
    static int countBusyObjects(void) {
	return numBusyObjects;
    }
    static int countFreeObjects(void) {
	return numFreeObjects;
    }
    static void deleteFreeObjects(void) {
	HnObject *object, *next;

	object = freeObject;
	while ( object != NULL ) {
	    next = object->next;
	    if ( object->refCount == 0 ) {
		delete object;
	    }
	    object = next;
	}
    }

    friend void HnRef(HnObject *object) {
	if ( object->refCount == 0 ) {
	    object->unsetFree();
	    object->setBusy();
	}
	object->refCount ++;
    }
    friend void HnUnref(HnObject *object) {
	object->refCount --;
	if ( object->refCount == 0 ) {
	    object->unsetBusy();
	    object->setFree();
	    delete object;
	}
    }

    virtual HnString toString(void) const;

    HnBool hasConstructorFailed() {
	return constructorFailureFlag;
    }
};

#endif /* HnObject_hh */
