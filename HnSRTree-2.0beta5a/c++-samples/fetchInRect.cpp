/*
 * fetchInRect.cc
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
 * $Id: fetchInRect.cc,v 1.9 2002/09/13 05:13:46 katayama Exp $
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

#define DEFAULT_RANGE_WIDTH	0.0

static void printUsage(void);
static void fetchInRect(const char *recordFileName, int maxCount,
			const char *indexFileName,
			const HnProperties &properties,
			HnBool debug, HnBool verify,
			double rangeWidth);

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
    double rangeWidth;
    char *recordFileName, *indexFileName;
    HnStringBuffer sb;
    HnProperties properties;

    int c, errflag;
    extern int optind;
    extern char *optarg;

    maxCount = -1;
    debug = 0;
    rangeWidth = DEFAULT_RANGE_WIDTH;
    verify = 0;

    sb = new_HnStringBuffer();

    errflag = 0;
    while ( (c = getopt(argc, argv, "c:dp:vw:")) != EOF ) {
	switch ( c ) {
	case 'c':
	    maxCount = atoi(optarg);
	    break;
	case 'd':
	    debug = 1;
	    break;
	case 'p':
	    sb.append(optarg);
	    sb.append('\n');
	    break;
	case 'v':
	    verify = 1;
	    break;
	case 'w':
	    if ( (rangeWidth = atof(optarg)) < 0 ) {
		fprintf(stderr,
			"%s: the argument of the option `-w' must be "
			"equal to or greater than zero.\n", argv[0]);
		exit(1);
	    }
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

    fetchInRect(recordFileName, maxCount,
		indexFileName, properties, debug, verify,
		rangeWidth);

    return 0;
}

static void
printUsage(void)
{
    fprintf(stderr, "\
Usage: fetchInRect [options] recordFile indexFile\n\
Options\n\
    -c count     set the number of records to be read\n\
    -d           turn on the debug mode\n\
    -p property  set the property of the index\n\
    -v verify    verify record existence\n\
    -w width     set the width of the query rectangle (%g by default)\n\
", DEFAULT_RANGE_WIDTH);
}

static void
fetchInRect(const char *recordFileName, int maxCount,
	    const char *indexFileName, const HnProperties &properties,
	    HnBool debug, HnBool verify,
	    double rangeWidth)
{
    RecordFileSt *recordFile;
    HnSRTreeFile indexFile;
    int count;
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

    count = 0;

    while ( maxCount < 0 || count < maxCount ) {
	HnRect queryRect;
	HnPoint recordPoint;
	HnDataItem recordDataItem;
	HnPointVector points;
	HnDataItemVector dataItems;
	int i;

	if ( RecordFileSt_getRecord(recordFile,
				    &recordPoint, &recordDataItem) < 0 ) {
	    break;
	}

	/* extend the rectangle by the given width */
	queryRect = new_HnRect(recordFile->dimension);
	for ( i=0; i<recordFile->dimension; i++ ) {
	    double x = recordPoint.getCoordAt(i);
	    queryRect.setRangeAt(x - rangeWidth / 2, x + rangeWidth / 2, i);
	}

	/* search data */
	printf("Searching %s\n", (char *)recordPoint.toString());

	startProfiling(indexFile);
	indexFile.getInRect(queryRect, &points, &dataItems);
	endProfiling(indexFile);

	for ( i=0; i<dataItems.size(); i++ ) {
	    printf("%s\n", dataItems.elementAt(i).toCharArray());
	}
	printf("\n");

	if ( verify ) {
	    HnBool found = HnFALSE;

	    for ( i=0; i<points.size(); i++ ) {
		if ( points.elementAt(i).equals(recordPoint) &&
		     dataItems.elementAt(i).equals(recordDataItem) ) {
		    found = HnTRUE;
		}
	    }

	    if ( !found ) {
		fprintf(stderr, "Error: record is not found.\n");
		exit(1);
	    }
	}

	count ++;
    }

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

