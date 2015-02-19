/*
 * HnMalloc.h
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
 * 03/16/99 katayama@rd.nacsis.ac.jp
 * $Id: HnMalloc.h,v 1.1 2000/06/06 08:52:45 katayama Exp $
 */

#ifndef _HnMalloc_h
#define _HnMalloc_h

#ifdef __cplusplus
extern "C" {
#endif

extern void *HnMalloc(size_t size);
extern void HnFree(void *ptr, size_t size);
extern void *HnRealloc(void *oldPtr, size_t oldSize, size_t newSize);

#ifdef __cplusplus
};
#endif

#endif /* _HnMalloc_h */
