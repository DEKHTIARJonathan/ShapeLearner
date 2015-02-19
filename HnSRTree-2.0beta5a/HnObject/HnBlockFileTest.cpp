/*
 * HnBlockFileTest.cc
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
 * Jul. 6,1999 Norio KATAYAMA
 * $Id: HnBlockFileTest.cc,v 1.2 2000/06/10 11:18:19 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnBlockFile.hh"

#define FILE_PATH	"/tmp/test.dat"
#define NUM_WRITES	1000

static void
writeAndReadIntegers(HnBlockFile &blockFile)
{
    int blockSize = blockFile.getBlockSize();
    HnBlockStream blockStream;
    int i, n;

    printf("Writing integers.\n");

    for ( i=1; i<=NUM_WRITES; i++ ) {
	blockStream = new_HnBlockStream(blockSize);
	blockStream.rewind();
	blockStream.writeInt(i);
	blockFile.writeBlock(i * blockSize, blockStream);

	if ( blockFile.getFileSize() != (i + 1) * blockSize ) {
	    fprintf(stderr,
		    "Error: unexpected file size `%d': expected `%d'.\n",
		    (int)blockFile.getFileSize(), (i + 1) * blockSize);
	    exit(1);
	}
    }

    printf("Reading integers.\n");

    for ( i=1; i<=NUM_WRITES; i++ ) {
	blockStream = new_HnBlockStream(blockSize);
	blockFile.readBlock(i * blockSize, blockStream);
	blockStream.rewind();
	n = blockStream.readInt();
	if ( n != i ) {
	    fprintf(stderr,
		    "Error: unexpected value `%d': expected `%d'.\n", n, i);
	    exit(1);
	}
    }
}

static void
writeAndReadIntegers2(HnBlockFile &blockFile)
{
    int blockSize = blockFile.getBlockSize();
    HnBlockStream blockStream;
    int i, n;
    int gapStart, nonGapStart;

    if ( NUM_WRITES % 2 == 0 ) {
	nonGapStart = 2;
	gapStart = 1;
    }
    else {
	nonGapStart = 1;
	gapStart = 2;
    }

    printf("Writing integers with gaps.\n");

    for ( i=nonGapStart; i<=NUM_WRITES; i+=2 ) {
	blockStream = new_HnBlockStream(blockSize);
	blockStream.rewind();
	blockStream.writeInt(i);
	blockFile.writeBlock(i * blockSize, blockStream);

	if ( blockFile.getFileSize() != (i + 1) * blockSize ) {
	    fprintf(stderr,
		    "Error: unexpected file size `%d': expected `%d'.\n",
		    (int)blockFile.getFileSize(), (i + 1) * blockSize);
	    exit(1);
	}
    }

    printf("Reading integers from non-gap blocks.\n");

    for ( i=nonGapStart; i<=NUM_WRITES; i+=2 ) {
	blockStream = new_HnBlockStream(blockSize);
	blockFile.readBlock(i * blockSize, blockStream);
	blockStream.rewind();
	n = blockStream.readInt();
	if ( n != i ) {
	    fprintf(stderr,
		    "Error: unexpected value `%d': expected `%d'.\n", n, i);
	    exit(1);
	}
    }

    printf("Reading integers from gap blocks.\n");

    for ( i=gapStart; i<=NUM_WRITES; i+=2 ) {
	blockStream = new_HnBlockStream(blockSize);
	blockFile.readBlock(i * blockSize, blockStream);
	blockStream.rewind();
	n = blockStream.readInt();
	if ( n != 0 ) {
	    fprintf(stderr,
		    "Error: unexpected value `%d': expected `%d'.\n", n, 0);
	    exit(1);
	}
    }

    printf("Writing integers in gap blocks.\n");

    for ( i=gapStart; i<=NUM_WRITES; i+=2 ) {
	blockStream = new_HnBlockStream(blockSize);
	blockStream.rewind();
	blockStream.writeInt(i);
	blockFile.writeBlock(i * blockSize, blockStream);

	if ( blockFile.getFileSize() != (NUM_WRITES + 1) * blockSize ) {
	    fprintf(stderr,
		    "Error: unexpected file size `%d': expected `%d'.\n",
		    (int)blockFile.getFileSize(),
		    (NUM_WRITES + 1) * blockSize);
	    exit(1);
	}
    }

    printf("Reading integers from all blocks.\n");

    for ( i=1; i<=NUM_WRITES; i++ ) {
	blockStream = new_HnBlockStream(blockSize);
	blockFile.readBlock(i * blockSize, blockStream);
	blockStream.rewind();
	n = blockStream.readInt();
	if ( n != i ) {
	    fprintf(stderr,
		    "Error: unexpected value `%d': expected `%d'.\n", n, 0);
	    exit(1);
	}
    }
}

static void
readIntegers(HnBlockFile &blockFile)
{
    int blockSize = blockFile.getBlockSize();
    HnBlockStream blockStream;
    int i, n;

    if ( blockFile.getFileSize() != (NUM_WRITES + 1) * blockSize ) {
	fprintf(stderr,
		"Error: unexpected file size `%d': expected `%d'.\n",
		(int)blockFile.getFileSize(),
		(NUM_WRITES + 1) * blockSize);
	exit(1);
    }

    printf("Reading integers.\n");

    for ( i=1; i<=NUM_WRITES; i++ ) {
	blockStream = new_HnBlockStream(blockSize);
	blockFile.readBlock(i * blockSize, blockStream);
	blockStream.rewind();
	n = blockStream.readInt();
	if ( n != i ) {
	    fprintf(stderr,
		    "Error: unexpected value `%d': expected `%d'.\n", n, i);
	    exit(1);
	}
    }
}

int
main(void)
{
    HnBlockFile blockFile;
    int blockSize = sizeof(int);
    HnBlockStream blockStream;

    printf("Creating a block file on disk ``%s''.\n", FILE_PATH);
    blockFile = new_HnBlockFile(FILE_PATH, blockSize);
    if ( blockFile == HnBlockFile::null ) {
	fprintf(stderr, "Error: new_HnBlockFile: %s.\n", HnGetErrorMessage());
	exit(1);
    }
    writeAndReadIntegers(blockFile);

    printf("Creating a block file on memory.\n");
    blockFile = new_HnBlockFile("", blockSize);
    if ( blockFile == HnBlockFile::null ) {
	fprintf(stderr, "Error: new_HnBlockFile: %s.\n", HnGetErrorMessage());
	exit(1);
    }
    writeAndReadIntegers(blockFile);

    printf("Creating a block file on disk ``%s''.\n", FILE_PATH);
    blockFile = new_HnBlockFile(FILE_PATH, blockSize);
    if ( blockFile == HnBlockFile::null ) {
	fprintf(stderr, "Error: new_HnBlockFile: %s.\n", HnGetErrorMessage());
	exit(1);
    }
    writeAndReadIntegers2(blockFile);

    printf("Creating a block file on memory.\n");
    blockFile = new_HnBlockFile("", blockSize);
    if ( blockFile == HnBlockFile::null ) {
	fprintf(stderr, "Error: new_HnBlockFile: %s.\n", HnGetErrorMessage());
	exit(1);
    }
    writeAndReadIntegers2(blockFile);

    printf("Opening a block file on disk ``%s'' for read/write.\n", FILE_PATH);
    blockFile = new_HnBlockFile(FILE_PATH, "rw");
    if ( blockFile == HnBlockFile::null ) {
	fprintf(stderr, "Error: new_HnBlockFile: %s.\n", HnGetErrorMessage());
	exit(1);
    }
    readIntegers(blockFile);

    printf("Opening a block file on disk ``%s'' for read.\n", FILE_PATH);
    blockFile = new_HnBlockFile(FILE_PATH, "r");
    if ( blockFile == HnBlockFile::null ) {
	fprintf(stderr, "Error: new_HnBlockFile: %s.\n", HnGetErrorMessage());
	exit(1);
    }
    readIntegers(blockFile);

    printf("Opening /tmp for write.\n");
    blockFile = new_HnBlockFile("/tmp", blockSize);
    if ( blockFile != HnBlockFile::null ) {
	fprintf(stderr, "Error: constructor did not failed.\n");
	exit(1);
    }
    else {
	printf("Constructor failed as expected: %s.\n", HnGetErrorMessage());
	HnClearErrorMessage();
    }

    printf("Opening /tmp for read.\n");
    blockFile = new_HnBlockFile("/tmp", "r");
    if ( blockFile != HnBlockFile::null ) {
	fprintf(stderr, "Error: constructor did not failed.\n");
	exit(1);
    }
    else {
	printf("Constructor failed as expected: %s.\n", HnGetErrorMessage());
	HnClearErrorMessage();
    }

    return 0;
}
