/*
 * RecordFileSt.hh
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
 * $Id: RecordFileSt.hh,v 1.2 2000/06/10 11:20:56 katayama Exp $
 */

#ifndef _RecordFileSt_hh
#define _RecordFileSt_hh

#include "HnSRTree/HnPoint.hh"
#include "HnSRTree/HnDataItem.hh"

struct RecordFileSt {
    FILE *fp;
    int dimension;
    long firstRecordPos;
};

RecordFileSt *RecordFileSt_open(const char *recordFileName);
void RecordFileSt_close(RecordFileSt *file);

void RecordFileSt_rewind(RecordFileSt *file);
int RecordFileSt_getRecord(RecordFileSt *file,
			   HnPoint *point_return, HnDataItem *dataItem_return);

void RecordFileSt_printHeaderTo(int dimension, FILE *fp);
void RecordFileSt_printRecordTo(const HnPoint &point,
				const HnDataItem &dataItem,
				FILE *fp);

#endif /* _RecordFileSt_hh */
