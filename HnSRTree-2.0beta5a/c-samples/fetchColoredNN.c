/*
 * fetchColoredNN.c
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
 * 10/14/96 katayama@rd.nacsis.ac.jp
 * $Id: fetchColoredNN.c,v 1.10 2002/09/13 05:14:31 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include "HnSRTree/HnGetOpt.h"
#else
#include <unistd.h>
#endif
#include "RecordFileSt.h"
#include "HnSRTree/HnStringBufferSt.h"
#include "HnSRTree/HnTimesSt.h"
#include "HnSRTree/HnTimesStatsSt.h"
#include "HnSRTree/HnSRTreeStatsSt.h"
#include "HnSRTree/HnSRTreeFileSt.h"

#define DEFAULT_NUM_NEIGHBORS	1

static void printUsage(void);
static void fetchColoredNN(const char *recordFileName, int maxCount,
			   const char *indexFileName,
			   const HnPropertiesSt *properties,
			   HnBool debug, HnBool verify,
			   int numNeighbors);

/* profiling */
static void initializeProfileData(void);
static void startProfiling(HnSRTreeFileSt *indexFile);
static void endProfiling(HnSRTreeFileSt *indexFile);
static void printProfileData(void);
static void freeProfileData(void);

int
main(int argc, char *argv[])
{
    int maxCount, debug, verify;
    int numColoredNeighbors;
    char *recordFileName, *indexFileName;
    HnStringBufferSt *sb;
    HnPropertiesSt *properties;

    int c, errflag;
    extern int optind;
    extern char *optarg;

    maxCount = -1;
    debug = 0;
    numColoredNeighbors = DEFAULT_NUM_NEIGHBORS;
    verify = 0;

    sb = HnStringBufferSt_allocate();

    errflag = 0;
    while ( (c = getopt(argc, argv, "c:dn:p:v")) != EOF ) {
	switch ( c ) {
	case 'c':
	    maxCount = atoi(optarg);
	    break;
	case 'd':
	    debug = 1;
	    break;
	case 'n':
	    if ( (numColoredNeighbors = atoi(optarg)) < 1 ) {
		fprintf(stderr,
			"%s: the argument of the option `-n' must be "
			"equal to or greater than one.\n", argv[0]);
		exit(1);
	    }
	    break;
	case 'p':
	    HnStringBufferSt_appendString(sb, optarg);
	    HnStringBufferSt_appendChar(sb, '\n');
	    break;
	case 'v':
	    verify = 1;
	    break;
	case '?':
	    errflag = 1;
	    break;
	}
    }
    if ( errflag || optind != argc - 2 ) {
	printUsage();
	return 1;
    }

    recordFileName = argv[optind];
    indexFileName = argv[optind + 1];

    properties = HnPropertiesSt_allocate();
    HnPropertiesSt_load(properties, sb->buffer);

    fetchColoredNN(recordFileName, maxCount,
		   indexFileName, properties, debug, verify,
		   numColoredNeighbors);

    HnStringBufferSt_free(sb);
    HnPropertiesSt_free(properties);

    return 0;
}

static void
printUsage(void)
{
    fprintf(stderr, "\
Usage: fetchColoredNN [options] recordFile indexFile\n\
Options\n\
    -c count      set the number of records to be read\n\
    -d            turn on the debug mode\n\
    -n neighbors  set the number of neighbors to be fetched (%d by default)\n\
    -p property   set the property of the index\n\
    -v verify     verify record existence\n\
", DEFAULT_NUM_NEIGHBORS);
}

/*
 * Colored Nearest-Neighbor Search
 */

typedef struct {
    HnStringSt *color;
    HnPointSt *point;
    HnDataItemSt *dataItem;
} ColoredRecord;

static int
compColoredRecords(const void *ptr1, const void *ptr2)
{
    const ColoredRecord *record1 = (const ColoredRecord *)ptr1;
    const ColoredRecord *record2 = (const ColoredRecord *)ptr2;
    int length1, length2, dimension1, dimension2, sign, axis;

    /* color */
    length1 = record1->color->length;
    length2 = record2->color->length;

    if ( length1 != length2 ) {
	return (length1 < length2 ? -1 : 1);
    }

    if ( (sign = strcmp(record1->color->buffer,
			record2->color->buffer)) != 0 ) {
	return sign;
    }

    /* point */
    dimension1 = record1->point->dimension;
    dimension2 = record2->point->dimension;

    if ( dimension1 != dimension2 ) {
	fprintf(stderr, "Error: mismatch in dimensions.\n");
	exit(1);
    }

    for ( axis=0; axis<dimension1; axis++ ) {
	double coord1 = record1->point->coords[axis];
	double coord2 = record2->point->coords[axis];

	if ( coord1 != coord2 ) {
	    return (coord1 < coord2 ? -1 : 1);
	}
    }

    /* dataItem */
    length1 = record1->dataItem->length;
    length2 = record2->dataItem->length;

    if ( length1 != length2 ) {
	return (length1 < length2 ? -1 : 1);
    }

    return memcmp(record1->dataItem->buffer,
		  record2->dataItem->buffer, length1);
}	

#define COLOR_SEPARATOR	'!'

static HnStringSt *
getColor(const HnDataItemSt *dataItem)
{
    char *head, *tail;

    head = dataItem->buffer;

    if ( (tail = strchr(head, COLOR_SEPARATOR)) == NULL ) {
	fprintf(stderr,
		"Error: cannot find `%c' in the data: `%s'.\n",
		COLOR_SEPARATOR, head);
	exit(1);
    }

    return HnStringSt_allocateN(head, tail - head);
}

static int
compColors(const void *data1, int size1, const void *data2, int size2)
{
    const char *string1 = (const char *)data1;
    const char *string2 = (const char *)data2;
    int len1, len2;
    const char *tail;

    if ( (tail = strchr(string1, COLOR_SEPARATOR)) == NULL ) {
	fprintf(stderr, "Error: `%c' is not found in the data stored in "
		"the index tree: `%s'.\n",
		COLOR_SEPARATOR, string1);
	exit(1);
    }
    len1 = tail - string1;

    if ( (tail = strchr(string2, COLOR_SEPARATOR)) == NULL ) {
	fprintf(stderr, "Error: `%c' is not found in the data stored in "
		"the index tree: `%s'.\n",
		COLOR_SEPARATOR, string2);
	exit(1);
    }
    len2 = tail - string2;

    if ( len1 == len2 ) {
	return strncmp(string1, string2, len1);
    }
    else if ( len1 < len2 ) {
	return -1;
    }
    else {
	return 1;
    }
}

static void
fetchColoredNN(const char *recordFileName, int maxCount,
	       const char *indexFileName,
	       const HnPropertiesSt *properties,
	       HnBool debug, HnBool verify,
	       int numNeighbors)
{
    RecordFileSt *recordFile;
    HnSRTreeFileSt *indexFile;
    HnPointVectorSt *recordPoints;
    HnDataItemVectorSt *recordDataItems;
    int i, numRecords;
    ColoredRecord *records;
    int colorIndex, recordIndex;
    HnPropertiesSt *treeProperties;

    HnSRTreeFileSt_setDebug(debug);

    recordFile = RecordFileSt_open(recordFileName);

    indexFile = HnSRTreeFileSt_open(indexFileName, "r");

    if ( indexFile == NULL ) {
	perror(indexFileName);
	exit(1);
    }

    HnSRTreeFileSt_setProperties(indexFile, properties);

    if ( HnSRTreeFileSt_getDimension(indexFile) != recordFile->dimension ) {
	fprintf(stderr,
		"Error: mismatch in dimensions: "
		"the dimension of the record file is %d, while the dimension "
		"of the index file is %d.\n",
		recordFile->dimension, HnSRTreeFileSt_getDimension(indexFile));
	exit(1);
    }

    treeProperties = HnPropertiesSt_allocate();
    HnSRTreeFileSt_getProperties(indexFile, treeProperties);
    HnPropertiesSt_print(treeProperties);
    HnPropertiesSt_free(treeProperties);

    initializeProfileData();

    /* collect records */
    recordPoints = HnPointVectorSt_allocate();
    recordDataItems = HnDataItemVectorSt_allocate();

    for ( ;; ) {
	HnPointSt *recordPoint;
	HnDataItemSt *recordDataItem;

	if ( RecordFileSt_getRecord(recordFile,
				    &recordPoint, &recordDataItem) < 0 ) {
	    break;
	}

	HnPointVectorSt_addElement(recordPoints, recordPoint);
	HnDataItemVectorSt_addElement(recordDataItems, recordDataItem);
    }

    numRecords = recordPoints->size;

    if ( (records = (ColoredRecord *)
	  malloc(sizeof(ColoredRecord) * numRecords)) == NULL ) {
	perror("malloc");
	exit(1);
    }

    for ( i=0; i<numRecords; i++ ) {
	records[i].color = getColor(recordDataItems->elements[i]);
	records[i].point = recordPoints->elements[i];
	records[i].dataItem = recordDataItems->elements[i];
    }

    qsort(records, numRecords, sizeof(ColoredRecord), compColoredRecords);

    colorIndex = 0;
    recordIndex = 0;

    while ( recordIndex < numRecords &&
	    (maxCount < 0 || colorIndex < maxCount) ) {
	HnStringSt *color;
	HnPointVectorSt *queryPoints;
	HnDataItemVectorSt *queryDataItems;
	HnPointVectorSt *points;
	HnDataItemVectorSt *dataItems;
	int i, j;
	HnBool found;

	/* collect records for the color */
	color = records[recordIndex].color;

	queryPoints = HnPointVectorSt_allocate();
	queryDataItems = HnDataItemVectorSt_allocate();

	while ( recordIndex < numRecords &&
		HnStringSt_equals(records[recordIndex].color, color) ) {
	    HnPointVectorSt_addElement(queryPoints,
				       records[recordIndex].point);
	    HnDataItemVectorSt_addElement(queryDataItems,
					  records[recordIndex].dataItem);
	    recordIndex ++;
	}

	/* search data */
	printf("Searching %s: HnPointVectorSt[ ", color->buffer);
	for ( i=0; i<queryPoints->size; i++ ) {
	    if ( i != 0 ) {
		printf(",");
	    }
	    printf("%s", HnPointSt_toString(queryPoints->elements[i]));
	}
	printf(" ]\n");

	startProfiling(indexFile);
	HnSRTreeFileSt_getColoredNeighbors(indexFile,
					   queryPoints, numNeighbors,
					   &points, &dataItems, compColors);
	endProfiling(indexFile);

	for ( i=0; i<points->size; i++ ) {
	    double minDistance = -1;

	    for ( j=0; j<queryPoints->size; j++ ) {
		double distance =
		    HnPointSt_getDistance(points->elements[i],
					  queryPoints->elements[j]);
		if ( j==0 || distance<minDistance ) {
		    minDistance = distance;
		}
	    }
	    printf("%s: %g\n",
		   dataItems->elements[i]->buffer, minDistance);
	}
	printf("\n");

	if ( verify ) {
	    found = HnFALSE;

	    for ( i=0; i<points->size; i++ ) {
		for ( j=0; j<queryPoints->size; j++ ) {
		    if ( HnPointSt_equals(points->elements[i],
					  queryPoints->elements[j]) &&
			 HnDataItemSt_equals(dataItems->elements[i],
					     queryDataItems->elements[j]) ) {
			found = HnTRUE;
			break;
		    }
		}
	    }

	    if ( !found ) {
		fprintf(stderr, "Error: record is not found.\n");
		exit(1);
	    }
	}

	HnPointVectorSt_free(queryPoints);
	HnDataItemVectorSt_free(queryDataItems);
	HnPointVectorSt_freeElements(points);
	HnPointVectorSt_free(points);
	HnDataItemVectorSt_freeElements(dataItems);
	HnDataItemVectorSt_free(dataItems);

	colorIndex ++;
    }

    free(records);

    RecordFileSt_close(recordFile);
    HnSRTreeFileSt_close(indexFile);

    HnPointVectorSt_freeElements(recordPoints);
    HnPointVectorSt_free(recordPoints);
    HnDataItemVectorSt_freeElements(recordDataItems);
    HnDataItemVectorSt_free(recordDataItems);

    printf("**** Processing Cost ****\n");
    printProfileData();
    freeProfileData();
}

/*
 * Profiling
 */

static HnTimesSt *startTimes, *endTimes, *runningTimes;
static HnTimesStatsSt *timesStats;
static HnSRTreeProfileSt *treeProfile;
static HnSRTreeStatsSt *treeStats;

static void
initializeProfileData(void)
{
    startTimes = HnTimesSt_allocate();
    endTimes = HnTimesSt_allocate();
    runningTimes = HnTimesSt_allocate();
    timesStats = HnTimesStatsSt_allocate();
    treeProfile = HnSRTreeProfileSt_allocate();
    treeStats = HnSRTreeStatsSt_allocate();
}

static void
startProfiling(HnSRTreeFileSt *indexFile)
{
    HnSRTreeFileSt_resetProfile(indexFile);
    HnTimesSt_setCurrentTimes(startTimes);
}

static void
endProfiling(HnSRTreeFileSt *indexFile)
{
    HnTimesSt_setCurrentTimes(endTimes);
    HnSRTreeFileSt_copyProfileInto(indexFile, treeProfile);

    HnTimesSt_subtract(runningTimes, endTimes, startTimes);
    HnTimesStatsSt_addSample(timesStats, runningTimes);
    HnSRTreeStatsSt_addSample(treeStats, treeProfile);
}

static void
printProfileData(void)
{
    HnTimesStatsSt_print(timesStats);
    HnSRTreeStatsSt_print(treeStats);
}

static void
freeProfileData(void)
{
    HnTimesSt_free(startTimes);
    HnTimesSt_free(endTimes);
    HnTimesSt_free(runningTimes);
    HnTimesStatsSt_free(timesStats);
    HnSRTreeProfileSt_free(treeProfile);
    HnSRTreeStatsSt_free(treeStats);
}
