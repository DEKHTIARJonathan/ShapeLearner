/*
 * staticBuild.c
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
 * 10/14/96 katayama@rd.nacsis.ac.jp
 * $Id: staticBuild.c,v 1.9 2002/09/13 05:14:32 katayama Exp $
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
#include "HnSRTree/HnSRTreeFileSt.h"

#include "HnSRTree/HnStringBufferSt.h"
#include "HnSRTree/HnTimesSt.h"

#define DEFAULT_ATTRIBUTE_SIZE  128

static void printUsage(void);
static void staticBuild(const char *recordFileName, int maxCount,
			const char *indexFileName, int dataItemSize,
			const HnPropertiesSt *properties, HnBool debug);

/* profiling */
static void initializeProfileData(void);
static void startProfiling(void);
static void endProfiling(HnSRTreeFileSt *indexFile);
static void printProfileData(void);
static void freeProfileData(void);

int
main(int argc, char *argv[])
{
    char *recordFileName, *indexFileName;
    int dataItemSize;
    int maxCount;
    HnBool debug;
    HnStringBufferSt *sb;
    HnPropertiesSt *properties;

    int c, errflag;
    extern int optind;
    extern char *optarg;

    dataItemSize = DEFAULT_ATTRIBUTE_SIZE;
    maxCount = -1;
    debug = HnFALSE;

    sb = HnStringBufferSt_allocate();

    errflag = 0;
    while ( (c = getopt(argc, argv, "c:dp:s:")) != EOF ) {
	switch ( c ) {
	case 'c':
	    maxCount = atoi(optarg);
	    break;
	case 'd':
	    debug = HnTRUE;
	    break;
	case 'p':
	    HnStringBufferSt_appendString(sb, optarg);
	    HnStringBufferSt_appendChar(sb, '\n');
	    break;
	case 's':
	    dataItemSize = atoi(optarg);
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

    staticBuild(recordFileName, maxCount,
		indexFileName, dataItemSize,
		properties, debug);

    HnStringBufferSt_free(sb);
    HnPropertiesSt_free(properties);

    return 0;
}

static void
printUsage(void)
{
    fprintf(stderr, "\
Usage: staticBuild [options] recordFile indexFile\n\
Options\n\
    -c count     set the number of records to be read from recordFile\n\
    -d           turn on the debug mode\n\
    -p property  set the property of the index\n\
    -s dataSize  set the size of data attributes (%d by default)\n\
", DEFAULT_ATTRIBUTE_SIZE);
}

static void
staticBuild(const char *recordFileName, int maxCount,
	    const char *indexFileName, int dataItemSize,
	    const HnPropertiesSt *properties, HnBool debug)
{
    RecordFileSt *recordFile;
    int dimension;
    HnSRTreeFileSt *indexFile;
    HnPointVectorSt *points;
    HnDataItemVectorSt *dataItems;
    int count;
    HnPropertiesSt *treeProperties;

    recordFile = RecordFileSt_open(recordFileName);
    dimension = recordFile->dimension;

    points = HnPointVectorSt_allocate();
    dataItems = HnDataItemVectorSt_allocate();

    count = 0;

    while ( maxCount < 0 || count < maxCount ) {
	HnPointSt *point;
	HnDataItemSt *dataItem;

	if ( RecordFileSt_getRecord(recordFile, &point, &dataItem) < 0 ) {
	    break;
	}

	HnPointVectorSt_addElement(points, point);
	HnDataItemVectorSt_addElement(dataItems, dataItem);

	count ++;
    }

    RecordFileSt_close(recordFile);

    HnSRTreeFileSt_setDebug(debug);

    initializeProfileData();
    startProfiling();

    indexFile = HnSRTreeFileSt_build(indexFileName,
				     dimension, dataItemSize,
				     points, dataItems,
				     properties);

    if ( indexFile == NULL ) {
	perror(indexFileName);
	exit(1);
    }

    treeProperties = HnPropertiesSt_allocate();
    HnSRTreeFileSt_getProperties(indexFile, treeProperties);
    HnPropertiesSt_print(treeProperties);
    HnPropertiesSt_free(treeProperties);

    endProfiling(indexFile);

    HnSRTreeFileSt_close(indexFile);

    printf("**** Processing Cost ****\n");
    printProfileData();
    freeProfileData();

    HnPointVectorSt_freeElements(points);
    HnPointVectorSt_free(points);

    HnDataItemVectorSt_freeElements(dataItems);
    HnDataItemVectorSt_free(dataItems);
}

/*
 * Profiling
 */

static HnTimesSt *startTimes, *endTimes, *runningTimes;
static HnSRTreeProfileSt *treeProfile;

static void
initializeProfileData(void)
{
    startTimes = HnTimesSt_allocate();
    endTimes = HnTimesSt_allocate();
    runningTimes = HnTimesSt_allocate();
    treeProfile = HnSRTreeProfileSt_allocate();
}

static void
startProfiling(void)
{
    HnTimesSt_setCurrentTimes(startTimes);
}

static void
endProfiling(HnSRTreeFileSt *indexFile)
{
    HnTimesSt_setCurrentTimes(endTimes);
    HnSRTreeFileSt_copyProfileInto(indexFile, treeProfile);

    HnTimesSt_subtract(runningTimes, endTimes, startTimes);
}

static void
printProfileData(void)
{
    HnTimesSt_print(runningTimes);
    HnSRTreeProfileSt_print(treeProfile);
}

static void
freeProfileData(void)
{
    HnTimesSt_free(startTimes);
    HnTimesSt_free(endTimes);
    HnTimesSt_free(runningTimes);
    HnSRTreeProfileSt_free(treeProfile);
}
