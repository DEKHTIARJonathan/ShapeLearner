/*
 * HnClassArraySt.h
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
 * 07/27/1999 Norio Katayama
 * $Id: HnClassArraySt.h,v 1.2 2002/09/10 12:04:34 katayama Exp $
 */

#define	ClassSt0(type)			type ## St
#define	ClassSt(type)			ClassSt0(type)
#define HnClassSt			ClassSt(HnClass)

#define ArraySt0(type)			type ## ArraySt
#define ArraySt_allocate0(type)		type ## ArraySt_allocate
#define ArraySt_free0(type)		type ## ArraySt_free
#define ArraySt_freeElements0(type)	type ## ArraySt_freeElements

#define ArraySt(type)			ArraySt0(type)
#define ArraySt_allocate(type)		ArraySt_allocate0(type)
#define ArraySt_free(type)		ArraySt_free0(type)
#define ArraySt_freeElements(type)	ArraySt_freeElements0(type)

#define HnClassArraySt			ArraySt(HnClass)
#define HnClassArraySt_allocate		ArraySt_allocate(HnClass)
#define HnClassArraySt_free		ArraySt_free(HnClass)
#define HnClassArraySt_freeElements	ArraySt_freeElements(HnClass)

#define VectorSt0(type)			type ## VectorSt
#define VectorSt_allocate0(type)	type ## VectorSt_allocate
#define VectorSt_free0(type)		type ## VectorSt_free
#define VectorSt_freeElements0(type)	type ## VectorSt_freeElements
#define VectorSt_addElement0(type)	type ## VectorSt_addElement
#define VectorSt_insertElementAt0(type)	type ## VectorSt_insertElementAt
#define VectorSt_removeElementAt0(type)	type ## VectorSt_removeElementAt
#define VectorSt_setElementAt0(type)	type ## VectorSt_setElementAt
#define VectorSt_swapElementsAt0(type)	type ## VectorSt_swapElementsAt
#define VectorSt_setSize0(type)		type ## VectorSt_setSize

#define VectorSt(type)			VectorSt0(type)
#define VectorSt_allocate(type)		VectorSt_allocate0(type)
#define VectorSt_free(type)		VectorSt_free0(type)
#define VectorSt_freeElements(type)	VectorSt_freeElements0(type)
#define VectorSt_addElement(type)	VectorSt_addElement0(type)
#define VectorSt_insertElementAt(type)	VectorSt_insertElementAt0(type)
#define VectorSt_removeElementAt(type)	VectorSt_removeElementAt0(type)
#define VectorSt_setElementAt(type)	VectorSt_setElementAt0(type)
#define VectorSt_swapElementsAt(type)	VectorSt_swapElementsAt0(type)
#define VectorSt_setSize(type)		VectorSt_setSize0(type)

#define HnClassVectorSt			VectorSt(HnClass)
#define HnClassVectorSt_allocate	VectorSt_allocate(HnClass)
#define HnClassVectorSt_free		VectorSt_free(HnClass)
#define HnClassVectorSt_freeElements	VectorSt_freeElements(HnClass)
#define HnClassVectorSt_addElement	VectorSt_addElement(HnClass)
#define HnClassVectorSt_insertElementAt	VectorSt_insertElementAt(HnClass)
#define HnClassVectorSt_removeElementAt	VectorSt_removeElementAt(HnClass)
#define HnClassVectorSt_setElementAt	VectorSt_setElementAt(HnClass)
#define HnClassVectorSt_swapElementsAt	VectorSt_swapElementsAt(HnClass)
#define HnClassVectorSt_setSize		VectorSt_setSize(HnClass)

/*
 * Structure
 */

typedef struct {
  int length;
  HnClassSt **elements;
} HnClassArraySt;

typedef struct {
    int allocatedSize;
    int size;
    HnClassSt **elements;
} HnClassVectorSt;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP HnClassArraySt *HnClassArraySt_allocate(int length);
_HNSRTIMP void HnClassArraySt_free(HnClassArraySt *array);
_HNSRTIMP void HnClassArraySt_freeElements(HnClassArraySt *array);

_HNSRTIMP HnClassVectorSt *HnClassVectorSt_allocate(void);
_HNSRTIMP void HnClassVectorSt_free(HnClassVectorSt *vector);
_HNSRTIMP void HnClassVectorSt_freeElements(HnClassVectorSt *vector);

_HNSRTIMP void HnClassVectorSt_addElement(HnClassVectorSt *vector,
						 HnClassSt *newElement);
_HNSRTIMP void HnClassVectorSt_insertElementAt(HnClassVectorSt *vector,
					       HnClassSt *newElement,
					       int index);
_HNSRTIMP void HnClassVectorSt_removeElementAt(HnClassVectorSt *vector,
					       int index);
_HNSRTIMP void HnClassVectorSt_setElementAt(HnClassVectorSt *vector,
					    HnClassSt *newElement, int index);
_HNSRTIMP void HnClassVectorSt_swapElementsAt(HnClassVectorSt* vector,
					      int i, int j);
_HNSRTIMP void HnClassVectorSt_setSize(HnClassVectorSt* vector, int newSize);

#ifdef __cplusplus
};
#endif

#undef	ClassSt0
#undef	ClassSt
#undef	HnClassSt

#undef	ArraySt0
#undef	ArraySt_allocate0
#undef	ArraySt_free0
#undef	ArraySt_freeElements0

#undef	ArraySt
#undef	ArraySt_allocate
#undef	ArraySt_free
#undef	ArraySt_freeElements

#undef	HnClassArraySt
#undef	HnClassArraySt_allocate
#undef	HnClassArraySt_free
#undef	HnClassArraySt_freeElements

#undef	VectorSt0
#undef	VectorSt_allocate0
#undef	VectorSt_free0
#undef	VectorSt_freeElements0
#undef	VectorSt_addElement0
#undef	VectorSt_insertElementAt0
#undef	VectorSt_removeElementAt0
#undef	VectorSt_setElementAt0
#undef	VectorSt_swapElementsAt0
#undef	VectorSt_setSize0

#undef	VectorSt
#undef	VectorSt_allocate
#undef	VectorSt_free
#undef	VectorSt_freeElements
#undef	VectorSt_addElement
#undef	VectorSt_insertElementAt
#undef	VectorSt_removeElementAt
#undef	VectorSt_setElementAt
#undef	VectorSt_swapElementsAt
#undef	VectorSt_setSize

#undef	HnClassVectorSt
#undef	HnClassVectorSt_allocate
#undef	HnClassVectorSt_free
#undef	HnClassVectorSt_freeElements
#undef	HnClassVectorSt_addElement
#undef	HnClassVectorSt_insertElementAt
#undef	HnClassVectorSt_removeElementAt
#undef	HnClassVectorSt_setElementAt
#undef	HnClassVectorSt_swapElementsAt
#undef	HnClassVectorSt_setSize

#undef	HnClass
