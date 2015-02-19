/*
 * storeRecords.c
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
 * $Id: storeRecords.c,v 1.9 2002/09/13 05:14:32 katayama Exp $
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

#include "HnSRTree/HnTimesSt.h"

static void printUsage(void);
static void storeRecords(const char *recordFileName, int maxCount,
			 const char *indexFileName, HnBool debug);

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
    int maxCount;
    HnBool debug;

    int c, errflag;
    extern int optind;
    extern char *optarg;

    maxCount = -1;
    debug = HnFALSE;

    errflag = 0;
    while ( (c = getopt(argc, argv, "c:d")) != EOF ) {
	switch ( c ) {
	case 'c':
	    maxCount = atoi(optarg);
	    break;
	case 'd':
	    debug = HnTRUE;
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

    storeRecords(recordFileName, maxCount, indexFileName, debug);

    return 0;
}

static void
printUsage(void)
{
    fprintf(stderr, "\
Usage: storeRecords [options] recordFile indexFile\n\
Options\n\
    -c count          set the number of records to be read from recordFile\n\
    -d                turn on the debug mode\n\
");
}

static void
storeRecords(const char *recordFileName, int maxCount,
	     const char *indexFileName, HnBool debug)
{
    RecordFileSt *recordFile;
    HnSRTreeFileSt *indexFile;
    int count;
    HnPropertiesSt *treeProperties;

    HnSRTreeFileSt_setDebug(debug);

    initializeProfileData();
    startProfiling();

    recordFile = RecordFileSt_open(recordFileName);
    indexFile = HnSRTreeFileSt_open(indexFileName, "rw");

    if ( indexFile == NULL ) {
	perror(indexFileName);
	exit(1);
    }

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

    count = 0;

    while ( maxCount < 0 || count < maxCount ) {
	HnPointSt *point;
	HnDataItemSt *dataItem;

	if ( RecordFileSt_getRecord(recordFile, &point, &dataItem) < 0 ) {
	    break;
	}

	printf("Storing %s\n", HnPointSt_toString(point));

	HnSRTreeFileSt_store(indexFile, point, dataItem);

	HnPointSt_free(point);
	HnDataItemSt_free(dataItem);

	count ++;
    }

    endProfiling(indexFile);

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
