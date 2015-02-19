/*
 * dynamicBuild.cc
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
 * $Id: dynamicBuild.cc,v 1.9 2002/09/13 05:13:46 katayama Exp $
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
#include "HnSRTree/HnSRTreeFile.hh"

#include "HnSRTree/HnStringBuffer.hh"
#include "HnSRTree/HnTimesSt.h"

#define DEFAULT_ATTRIBUTE_SIZE  128

static void printUsage(void);
static void dynamicBuild(const char *recordFileName, int maxCount,
			 const char *indexFileName, int dataItemSize,
			 const HnProperties &properties, HnBool debug,
			 HnBool onMemory);

/* profiling */
static void initializeProfileData(void);
static void startProfiling(void);
static void endProfiling(HnSRTreeFile indexFile);
static void printProfileData(void);
static void freeProfileData(void);

int
main(int argc, char *argv[])
{
    char *recordFileName, *indexFileName;
    int dataItemSize;
    int maxCount;
    HnBool debug;
    HnBool onMemory;
    HnStringBuffer sb;
    HnProperties properties;

    int c, errflag;
    extern int optind;
    extern char *optarg;

    dataItemSize = DEFAULT_ATTRIBUTE_SIZE;
    maxCount = -1;
    debug = HnFALSE;
    onMemory = HnFALSE;

    sb = new_HnStringBuffer();

    errflag = 0;
    while ( (c = getopt(argc, argv, "c:dmp:s:")) != EOF ) {
	switch ( c ) {
	case 'c':
	    maxCount = atoi(optarg);
	    break;
	case 'd':
	    debug = HnTRUE;
	    break;
	case 'm':
	    onMemory = HnTRUE;
	    break;
	case 'p':
	    sb.append(optarg);
	    sb.append('\n');
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

    properties = new_HnProperties();
    properties.load(sb.toString());

    dynamicBuild(recordFileName, maxCount,
		 indexFileName, dataItemSize,
		 properties, debug, onMemory);

    return 0;
}

static void
printUsage(void)
{
    fprintf(stderr, "\
Usage: dynamicBuild [options] recordFile indexFile\n\
Options\n\
    -c count     set the number of records to be read from recordFile\n\
    -d           turn on the debug mode\n\
    -m           build the tree on memory and finally dump it to the file\n\
    -p property  set the property of the index\n\
    -s dataSize  set the size of data attributes (%d by default)\n\
", DEFAULT_ATTRIBUTE_SIZE);
}

static void
dynamicBuild(const char *recordFileName, int maxCount,
	     const char *indexFileName, int dataItemSize,
	     const HnProperties &properties, HnBool debug,
	     HnBool onMemory)
{
    RecordFileSt *recordFile;
    HnSRTreeFile indexFile;
    int count;
    HnProperties treeProperties;

    HnSRTreeFile::setDebug(debug);

    initializeProfileData();
    startProfiling();

    recordFile = RecordFileSt_open(recordFileName);
    indexFile = new_HnSRTreeFile((onMemory ? (char *)NULL : indexFileName),
				 recordFile->dimension, dataItemSize, 
				 properties);

    if ( indexFile == HnSRTreeFile::null ) {
	perror(indexFileName);
	exit(1);
    }

    treeProperties = indexFile.getProperties();
    treeProperties.print();

    count = 0;

    while ( maxCount < 0 || count < maxCount ) {
	HnPoint point;
	HnDataItem dataItem;

	if ( RecordFileSt_getRecord(recordFile, &point, &dataItem) < 0 ) {
	    break;
	}

	printf("Inserting %s\n", (char *)point.toString());

	indexFile.store(point, dataItem);

	if ( debug ) {
	    indexFile.check();
	}

	count ++;
    }

    endProfiling(indexFile);

    if ( onMemory ) {
	if ( !indexFile.dumpToFile(indexFileName) ) {
	    fprintf(stderr, "Error: %s.\n", HnGetErrorMessage());
	    exit(1);
	}
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
endProfiling(HnSRTreeFile indexFile)
{
    HnTimesSt_setCurrentTimes(endTimes);
    indexFile.copyProfileInto(treeProfile);

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
