/*
 * RecordFileSt.h
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
 * 03/28/96 katayama@rd.nacsis.ac.jp
 * $Id: RecordFileSt.h,v 1.2 2000/06/10 11:21:18 katayama Exp $
 */

#ifndef _RecordFileSt_h
#define _RecordFileSt_h

#include "HnSRTree/HnPointSt.h"
#include "HnSRTree/HnDataItemSt.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    FILE *fp;
    int dimension;
    long firstRecordPos;
} RecordFileSt;

RecordFileSt *RecordFileSt_open(const char *recordFileName);
void RecordFileSt_close(RecordFileSt *file);

int RecordFileSt_getRecord(RecordFileSt *file,
			   HnPointSt **point_return,
			   HnDataItemSt **dataItem_return);

void RecordFileSt_printHeaderTo(int dimension, FILE *fp);
void RecordFileSt_printRecordTo(const HnPointSt *point,
			      const HnDataItemSt *dataItem,
			      FILE *fp);

#ifdef __cplusplus
};
#endif

#endif /* _RecordFileSt_h */
