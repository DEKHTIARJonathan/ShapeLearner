/*
 * RecordFileSt.cc
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
 * $Id: RecordFileSt.cc,v 1.1 2000/06/06 13:47:38 katayama Exp $
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RecordFileSt.hh"

#define LINE_BUFFER_SIZE	4096

char *
skipSpaces(char *ptr)
{
    while ( *ptr != 0 && isspace(*ptr) ) {
	ptr ++;
    }

    return ptr;
}

void
removeTrailingSpaces(char *buffer)
{
    char *bp;

    bp = buffer + strlen(buffer) - 1;

    if ( bp >= buffer && (isspace(*bp) || *bp == '\n' || *bp == '\r') ) {
	bp --;
    }
    bp[1] = 0;
}

RecordFileSt *
RecordFileSt_open(const char *recordFileName)
{
    RecordFileSt *file;
    char line[LINE_BUFFER_SIZE];
    char *head, *tail;

    file = new RecordFileSt();

    if ( (file->fp = fopen(recordFileName, "r")) == NULL ) {
	perror(recordFileName);
	exit(1);
    }

    if ( fgets(line, sizeof(line), file->fp) == NULL ) {
	if ( ferror(file->fp) ) {
	    perror("fgets");
	    exit(1);
	}
	else {
	    fprintf(stderr,
		    "Error: unexpected EOF in the record file `%s'.\n",
		    recordFileName);
	    exit(1);
	}
    }
    removeTrailingSpaces(line);

    head = skipSpaces(line);

    file->dimension = strtol(head, &tail, 10);

    if ( *tail != 0 ) {
	fprintf(stderr, "Error: invalid number `%s'.\n", head);
	exit(1);
    }

    file->firstRecordPos = ftell(file->fp);

    return file;
}

void
RecordFileSt_close(RecordFileSt *file)
{
    fclose(file->fp);
    delete file;
}

void
RecordFileSt_rewind(RecordFileSt *file)
{
    if ( fseek(file->fp, file->firstRecordPos, SEEK_SET) < 0 ) {
	perror("fseek");
	exit(1);
    }
}

int
RecordFileSt_getRecord(RecordFileSt *file,
		       HnPoint *point_return, HnDataItem *dataItem_return)
{
    char *head, *tail;
    char line[LINE_BUFFER_SIZE];
    int i;
    HnPoint point;
    HnDataItem dataItem;

    do {
	if ( fgets(line, sizeof(line), file->fp) == NULL ) {
	    if ( feof(file->fp) ) {
		return -1;
	    }
	    else {
		perror("fgets");
		exit(1);
	    }
	}
	removeTrailingSpaces(line);

	head = skipSpaces(line);
    } while ( *head == '#' );

    /* point */
    point = new_HnPoint(file->dimension);

    for ( i=0; i<file->dimension; i++ ) {
	double coord;
	char *endptr;

	if ( (tail = strchr(head, ':')) == NULL ) {
	    fprintf(stderr,
		    "Error: `:' is missing in the line `%s'.\n", line);
	    exit(1);
	}

	coord = strtod(head, &endptr);

	if ( skipSpaces(endptr) != tail ) {
	    char buffer[LINE_BUFFER_SIZE];

	    strncpy(buffer, head, tail - head);
	    buffer[tail - head] = 0;
	    fprintf(stderr, "Error: invalid number `%s'.\n", buffer);
	    exit(1);
	}

	point.setCoordAt(coord, i);

	head = skipSpaces(tail + 1);
    }

    /* data item */
    dataItem = new_HnDataItem(head, strlen(head) + 1);

    *point_return = point;
    *dataItem_return = dataItem;

    return 0;
}

void
RecordFileSt_printHeaderTo(int dimension, FILE *fp)
{
    fprintf(fp, "%d\n", dimension);
}

void
RecordFileSt_printRecordTo(const HnPoint &point, const HnDataItem &dataItem,
			   FILE *fp)
{
    int dimension = point.getDimension();
    int i;

    for ( i=0; i<dimension; i++ ) {
	fprintf(fp, "%g:", point.getCoordAt(i));
    }
    fputs(dataItem.toCharArray(), fp);
    putc('\n', fp);
}
