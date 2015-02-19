/*
 * fetchColoredNN.cc
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
 * $Id: fetchColoredNN.cc,v 1.9 2002/09/13 05:13:46 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include "HnSRTree/HnGetOpt.h"
#else
#include <unistd.h>
#endif
#include "RecordFileSt.hh"
#include "HnSRTree/HnStringBuffer.hh"
#include "HnSRTree/HnTimesSt.h"
#include "HnSRTree/HnTimesStatsSt.h"
#include "HnSRTree/HnSRTreeStatsSt.h"
#include "HnSRTree/HnSRTreeFile.hh"

#define DEFAULT_NUM_NEIGHBORS	1

static void printUsage(void);
static void fetchColoredNN(const char *recordFileName, int maxCount,
			   const char *indexFileName,
			   const HnProperties &properties,
			   HnBool debug, HnBool verify,
			   int numNeighbors);

/* profiling */
static void initializeProfileData(void);
static void startProfiling(HnSRTreeFile indexFile);
static void endProfiling(HnSRTreeFile indexFile);
static void printProfileData(void);
static void freeProfileData(void);

int
main(int argc, char *argv[])
{
    int maxCount, debug, verify;
    int numColoredNeighbors;
    char *recordFileName, *indexFileName;
    HnStringBuffer sb;
    HnProperties properties;

    int c, errflag;
    extern int optind;
    extern char *optarg;

    maxCount = -1;
    debug = 0;
    numColoredNeighbors = DEFAULT_NUM_NEIGHBORS;
    verify = 0;

    sb = new_HnStringBuffer();

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
	    sb.append(optarg);
	    sb.append('\n');
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

    properties = new_HnProperties();
    properties.load(sb.toString());

    fetchColoredNN(recordFileName, maxCount,
		   indexFileName, properties, debug, verify,
		   numColoredNeighbors);

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
    HnString color;
    HnPoint point;
    HnDataItem dataItem;
} ColoredRecord;

static int
compColoredRecords(const void *ptr1, const void *ptr2)
{
    const ColoredRecord *record1 = (const ColoredRecord *)ptr1;
    const ColoredRecord *record2 = (const ColoredRecord *)ptr2;
    int length1, length2, dimension1, dimension2, sign, axis;

    /* color */
    length1 = record1->color.length();
    length2 = record2->color.length();

    if ( length1 != length2 ) {
	return (length1 < length2 ? -1 : 1);
    }

    if ( (sign = strcmp((char *)record1->color,
			(char *)record2->color)) != 0 ) {
	return sign;
    }

    /* point */
    dimension1 = record1->point.getDimension();
    dimension2 = record2->point.getDimension();

    if ( dimension1 != dimension2 ) {
	fprintf(stderr, "Error: mismatch in dimensions.\n");
	exit(1);
    }

    for ( axis=0; axis<dimension1; axis++ ) {
	double coord1 = record1->point.getCoordAt(axis);
	double coord2 = record2->point.getCoordAt(axis);

	if ( coord1 != coord2 ) {
	    return (coord1 < coord2 ? -1 : 1);
	}
    }

    /* dataItem */
    length1 = record1->dataItem.length();
    length2 = record2->dataItem.length();

    if ( length1 != length2 ) {
	return (length1 < length2 ? -1 : 1);
    }

    return memcmp(record1->dataItem.toCharArray(),
		  record2->dataItem.toCharArray(), length1);
}	

#define COLOR_SEPARATOR	'!'

static HnString
getColor(const HnDataItem &dataItem)
{
    char *head, *tail;

    head = dataItem.toCharArray();

    if ( (tail = strchr(head, COLOR_SEPARATOR)) == NULL ) {
	fprintf(stderr,
		"Error: cannot find `%c' in the data: `%s'.\n",
		COLOR_SEPARATOR, head);
	exit(1);
    }

    return new_HnString(head, tail - head);
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
	       const HnProperties &properties,
	       HnBool debug, HnBool verify,
	       int numNeighbors)
{
    RecordFileSt *recordFile;
    HnSRTreeFile indexFile;
    HnPointVector recordPoints;
    HnDataItemVector recordDataItems;
    int i, numRecords;
    ColoredRecord *records;
    int colorIndex, recordIndex;
    HnProperties treeProperties;

    HnSRTreeFile::setDebug(debug);

    recordFile = RecordFileSt_open(recordFileName);

    indexFile = new_HnSRTreeFile(indexFileName, "r");

    if ( indexFile == HnSRTreeFile::null ) {
	perror(indexFileName);
	exit(1);
    }

    indexFile.setProperties(properties);

    if ( indexFile.getDimension() != recordFile->dimension ) {
	fprintf(stderr,
		"Error: mismatch in dimensions: "
		"the dimension of the record file is %d, while the dimension "
		"of the index file is %d.\n",
		recordFile->dimension, indexFile.getDimension());
	exit(1);
    }

    treeProperties = indexFile.getProperties();
    treeProperties.print();

    initializeProfileData();

    /* collect records */
    recordPoints = new_HnPointVector();
    recordDataItems = new_HnDataItemVector();

    for ( ;; ) {
	HnPoint recordPoint;
	HnDataItem recordDataItem;

	if ( RecordFileSt_getRecord(recordFile,
				    &recordPoint, &recordDataItem) < 0 ) {
	    break;
	}

	recordPoints.addElement(recordPoint);
	recordDataItems.addElement(recordDataItem);
    }

    numRecords = recordPoints.size();

    records = new ColoredRecord[numRecords];

    for ( i=0; i<numRecords; i++ ) {
	records[i].color = getColor(recordDataItems.elementAt(i));
	records[i].point = recordPoints.elementAt(i);
	records[i].dataItem = recordDataItems.elementAt(i);
    }

    qsort(records, numRecords, sizeof(ColoredRecord), compColoredRecords);

    colorIndex = 0;
    recordIndex = 0;

    while ( recordIndex < numRecords &&
	    (maxCount < 0 || colorIndex < maxCount) ) {
	HnString color;
	HnPointVector queryPoints;
	HnDataItemVector queryDataItems;
	HnPointVector points;
	HnDataItemVector dataItems;
	int i, j;
	HnBool found;

	/* collect records for the color */
	color = records[recordIndex].color;

	queryPoints = new_HnPointVector();
	queryDataItems = new_HnDataItemVector();

	while ( recordIndex < numRecords &&
		records[recordIndex].color.equals(color) ) {
	    queryPoints.addElement(records[recordIndex].point);
	    queryDataItems.addElement(records[recordIndex].dataItem);
	    recordIndex ++;
	}

	/* search data */
	printf("Searching %s: %s\n",
	       (char *)color, (char *)queryPoints.toString());

	startProfiling(indexFile);
	indexFile.getColoredNeighbors(queryPoints, numNeighbors,
				      &points, &dataItems, compColors);
	endProfiling(indexFile);

	for ( i=0; i<points.size(); i++ ) {
	    double minDistance = -1;

	    for ( j=0; j<queryPoints.size(); j++ ) {
		double distance =
		    points.elementAt(i).getDistance(queryPoints.elementAt(j));
		if ( j==0 || distance<minDistance ) {
		    minDistance = distance;
		}
	    }
	    printf("%s: %g\n",
		   dataItems.elementAt(i).toCharArray(), minDistance);
	}
	printf("\n");

	if ( verify ) {
	    found = HnFALSE;

	    for ( i=0; i<points.size(); i++ ) {
		for ( j=0; j<queryPoints.size(); j++ ) {
		    if ( points.elementAt(i).
			 equals(queryPoints.elementAt(j)) &&
			 dataItems.elementAt(i).
			 equals(queryDataItems.elementAt(j)) ) {
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

	colorIndex ++;
    }

    delete[] records;

    RecordFileSt_close(recordFile);
    indexFile.close();

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
startProfiling(HnSRTreeFile indexFile)
{
    indexFile.resetProfile();
    HnTimesSt_setCurrentTimes(startTimes);
}

static void
endProfiling(HnSRTreeFile indexFile)
{
    HnTimesSt_setCurrentTimes(endTimes);
    indexFile.copyProfileInto(treeProfile);

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

