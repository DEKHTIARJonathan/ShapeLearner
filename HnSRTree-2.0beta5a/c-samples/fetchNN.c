/*
 * fetchNN.c
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
 * $Id: fetchNN.c,v 1.9 2002/09/13 05:14:32 katayama Exp $
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
static void fetchNN(const char *recordFileName, int maxCount,
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
    int numNeighbors;
    char *recordFileName, *indexFileName;
    HnStringBufferSt *sb;
    HnPropertiesSt *properties;

    int c, errflag;
    extern int optind;
    extern char *optarg;

    maxCount = -1;
    debug = 0;
    numNeighbors = DEFAULT_NUM_NEIGHBORS;
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
	    if ( (numNeighbors = atoi(optarg)) < 1 ) {
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

    fetchNN(recordFileName, maxCount,
	    indexFileName, properties, debug, verify,
	    numNeighbors);

    HnStringBufferSt_free(sb);
    HnPropertiesSt_free(properties);

    return 0;
}

static void
printUsage(void)
{
    fprintf(stderr, "\
Usage: fetchNN [options] recordFile indexFile\n\
Options\n\
    -c count      set the number of records to be read\n\
    -d            turn on the debug mode\n\
    -n neighbors  set the number of neighbors to be fetched (%d by default)\n\
    -p property   set the property of the index\n\
    -v verify     verify record existence\n\
", DEFAULT_NUM_NEIGHBORS);
}

static void
fetchNN(const char *recordFileName, int maxCount,
	const char *indexFileName, const HnPropertiesSt *properties,
	HnBool debug, HnBool verify,
	int numNeighbors)
{
    RecordFileSt *recordFile;
    HnSRTreeFileSt *indexFile;
    int count;
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

    count = 0;

    while ( maxCount < 0 || count < maxCount ) {
	HnPointSt *recordPoint;
	HnDataItemSt *recordDataItem;
	HnPointVectorSt *points;
	HnDataItemVectorSt *dataItems;
	int i;

	if ( RecordFileSt_getRecord(recordFile,
				    &recordPoint, &recordDataItem) < 0 ) {
	    break;
	}

	/* search data */
	printf("Searching %s\n", HnPointSt_toString(recordPoint));

	startProfiling(indexFile);
	HnSRTreeFileSt_getNeighbors(indexFile,
				    recordPoint, numNeighbors,
				    &points, &dataItems);
	endProfiling(indexFile);

	for ( i=0; i<points->size; i++ ) {
	    printf("%s: %g\n",
		   dataItems->elements[i]->buffer,
		   HnPointSt_getDistance(points->elements[i], recordPoint));
	}
	printf("\n");

	if ( verify ) {
	    HnBool found = HnFALSE;

	    for ( i=0; i<points->size; i++ ) {
		if ( HnPointSt_equals(points->elements[i], recordPoint) &&
		     HnDataItemSt_equals(dataItems->elements[i],
					 recordDataItem) ) {
		    found = HnTRUE;
		    break;
		}
	    }

	    if ( !found ) {
		fprintf(stderr, "Error: record is not found.\n");
		exit(1);
	    }
	}

	HnPointSt_free(recordPoint);
	HnDataItemSt_free(recordDataItem);
	HnPointVectorSt_freeElements(points);
	HnPointVectorSt_free(points);
	HnDataItemVectorSt_freeElements(dataItems);
	HnDataItemVectorSt_free(dataItems);

	count ++;
    }

    RecordFileSt_close(recordFile);
    HnSRTreeFileSt_close(indexFile);

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
