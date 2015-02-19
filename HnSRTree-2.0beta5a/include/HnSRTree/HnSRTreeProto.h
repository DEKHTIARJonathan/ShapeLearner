/*
 * HnSRTreeProto.h
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
 * 10/14/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeProto.h,v 1.5 2000/12/15 12:47:28 katayama Exp $
 */

#ifndef _HnSRTreeProto_h
#define _HnSRTreeProto_h

#define HnSRTreeMagic	(('H' << 24) | ('n' << 16) | ('S' << 8) | 'R')

/* persistent properties */
#define HnSRTreeBlockSize		"HnSRTreeBlockSize"
#define HnSRTreeSplitFactor		"HnSRTreeSplitFactor"
#define HnSRTreeReinsertFactor		"HnSRTreeReinsertFactor"
#define HnSRTreeStaticAlgorithm		"HnSRTreeStaticAlgorithm"
#define HnSRTreeNonLeafFloatType	"HnSRTreeNonLeafFloatType"

/* volatile properties */
#define HnSRTreeNeighborAlgorithm	"HnSRTreeNeighborAlgorithm"

/* default values */
#define HnSRTreeDefaultBlockSize                "8192"
#define HnSRTreeDefaultSplitFactor              "40"
#define HnSRTreeDefaultReinsertFactor           "30"
#define HnSRTreeDefaultStaticAlgorithm          "VAM_CORRECTED"
#define HnSRTreeDefaultNonLeafFloatType         "NON_LEAF_DOUBLE"
#define HnSRTreeDefaultNeighborAlgorithm        "DEPTH_FIRST"

#ifdef __cplusplus
extern "C" {
#endif

typedef int HnSRTreeCompColorsFunc(const void *data1, int size1,
				   const void *data2, int size2);

#ifdef __cplusplus
};
#endif

#endif /* _HnSRTreeProto_h */
