/*
 * HnBlockStreamTest.cc
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
 * 02/12/99 katayama@rd.nacsis.ac.jp
 * $Id: HnBlockStreamTest.cc,v 1.2 2000/06/10 11:18:21 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnBlockStream.hh"
#include "HnSRTree/HnTimes.hh"

int
main(void)
{
    HnBlockStream blockStream, parent;
    int numTrials = 10000;
    int numDoubles = 1000;
    int numOperations;
    double *doubles;
    char *bp;
    int i, j;
    HnTimes start, end, interval;

    doubles = new double[numDoubles];
    for ( i=0; i<numDoubles; i++ ) {
	doubles[i] = drand48();
    }

    start = new_HnTimes();
    end = new_HnTimes();

    /*
     * HnBlockStream::writeDouble()
     */

    printf("**** HnBlockStream::writeDouble() ****\n");

    blockStream = new_HnBlockStream(HnBlockStream::DOUBLE_SIZE * numDoubles);

    start.setCurrentTimes();

    for ( i=0; i<numTrials; i++ ) {
	blockStream.rewind();
	for ( j=0; j<numDoubles; j++ ) {
	    blockStream.writeDouble(doubles[j]);
	}
    }

    end.setCurrentTimes();

    interval = end.subtract(start);

    numOperations = numTrials * numDoubles;

    printf("Real Time   : %g (%g)\n",
	   interval.getRealTime(), interval.getRealTime() / numOperations);
    printf("CPU Time    : %g (%g)\n",
	   interval.getCPUTime(), interval.getCPUTime() / numOperations);
    printf("User Time   : %g (%g)\n",
	   interval.getUserTime(), interval.getUserTime() / numOperations);
    printf("System Time : %g (%g)\n",
	   interval.getSystemTime(), interval.getSystemTime() / numOperations);

    /*
     * memcpy()
     */

    printf("**** memcpy ****\n");

    blockStream = new_HnBlockStream(HnBlockStream::DOUBLE_SIZE * numDoubles);

    start.setCurrentTimes();

    for ( i=0; i<numTrials; i++ ) {
	bp = blockStream.toCharArray();
	for ( j=0; j<numDoubles; j++ ) {
	    memcpy(bp, &doubles[j], sizeof(double));
	    bp += sizeof(double);
	}
    }

    end.setCurrentTimes();

    interval = end.subtract(start);

    numOperations = numTrials * numDoubles;

    printf("Real Time   : %g (%g)\n",
	   interval.getRealTime(), interval.getRealTime() / numOperations);
    printf("CPU Time    : %g (%g)\n",
	   interval.getCPUTime(), interval.getCPUTime() / numOperations);
    printf("User Time   : %g (%g)\n",
	   interval.getUserTime(), interval.getUserTime() / numOperations);
    printf("System Time : %g (%g)\n",
	   interval.getSystemTime(), interval.getSystemTime() / numOperations);

    /*
     * HnBlockStream::writeDouble() (child)
     */

    printf("**** HnBlockStream::writeDouble() (child) ****\n");

    parent = new_HnBlockStream(HnBlockStream::INT_SIZE +
			       HnBlockStream::DOUBLE_SIZE * numDoubles);
    blockStream =
	new_HnBlockStream(parent, HnBlockStream::INT_SIZE,
			  parent.getSize() - HnBlockStream::INT_SIZE);

    start.setCurrentTimes();

    for ( i=0; i<numTrials; i++ ) {
	blockStream.rewind();
	for ( j=0; j<numDoubles; j++ ) {
	    blockStream.writeDouble(doubles[j]);
	}
    }

    end.setCurrentTimes();

    interval = end.subtract(start);

    numOperations = numTrials * numDoubles;

    printf("Real Time   : %g (%g)\n",
	   interval.getRealTime(), interval.getRealTime() / numOperations);
    printf("CPU Time    : %g (%g)\n",
	   interval.getCPUTime(), interval.getCPUTime() / numOperations);
    printf("User Time   : %g (%g)\n",
	   interval.getUserTime(), interval.getUserTime() / numOperations);
    printf("System Time : %g (%g)\n",
	   interval.getSystemTime(), interval.getSystemTime() / numOperations);

    return 0;
}

