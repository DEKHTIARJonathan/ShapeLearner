/*
 * HnRangeTest.cc
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
 * 08/27/96 katayama@rd.nacsis.ac.jp
 * $Id: HnRangeTest.cc,v 1.5 2000/06/10 11:19:18 katayama Exp $
 */

#include "HnSRTree/HnString.hh"
#include "HnSRTree/HnRange.hh"

HnRange
new_HnRange(double min, double max)
{
    HnRange range;

    range.set(min, max);
    return range;
}

void
intersect(const HnRange &range1, const HnRange &range2,
	  const HnRange &expectedOverlappingRange,
	  const HnRange &expectedBoundingRange,
	  HnBool expectedOverlaps12, HnBool expectedOverlaps21,
	  HnBool expectedIncludes12, HnBool expectedIncludes21)
{
    HnRange overlappingRange, boundingRange;
    HnBool overlaps12, overlaps21, includes12, includes21;
    HnBool errflag;

    overlappingRange = range1.getOverlappingRange(range2);
    boundingRange = range1.getBoundingRange(range2);

    overlaps12 = range1.overlaps(range2);
    overlaps21 = range2.overlaps(range1);
    includes12 = range1.includes(range2);
    includes21 = range2.includes(range1);

    printf("range1: %s\n", (char *)range1.toString());
    printf("range2: %s\n", (char *)range2.toString());

    errflag = HnFALSE;

    printf("overlapping range: %s\n", (char *)overlappingRange.toString());
    if ( !overlappingRange.equals(expectedOverlappingRange) ) {
	fprintf(stderr, "Error: unexpected result; expecting %s\n",
		(char *)expectedOverlappingRange.toString());
	errflag = HnTRUE;
    }

    printf("bounding range   : %s\n",
	   (char *)boundingRange.toString());
    if ( !boundingRange.equals(expectedBoundingRange) ) {
	fprintf(stderr, "Error: unexpected result; expecting %s\n",
		(char *)expectedBoundingRange.toString());
	errflag = HnTRUE;
    }

    printf("range1.overlaps(range2) : %s\n",
	   (overlaps12 ? "true" : "false"));
    if ( overlaps12 != expectedOverlaps12 ) {
	fprintf(stderr, "Error: unexpected result; expecting %s\n",
		(expectedOverlaps12 ? "true" : "false"));
	errflag = HnTRUE;
    }

    printf("range2.overlaps(range1) : %s\n",
	   (overlaps21 ? "true" : "false"));
    if ( overlaps21 != expectedOverlaps21 ) {
	fprintf(stderr, "Error: unexpected result; expecting %s\n",
		(expectedOverlaps21 ? "true" : "false"));
	errflag = HnTRUE;
    }

    printf("range1.includes(range2) : %s\n",
	   (includes12 ? "true" : "false"));
    if ( includes12 != expectedIncludes12 ) {
	fprintf(stderr, "Error: unexpected result; expecting %s\n",
		(expectedIncludes12 ? "true" : "false"));
	errflag = HnTRUE;
    }

    printf("range2.includes(range1) : %s\n",
	   (includes21 ? "true" : "false"));
    if ( includes21 != expectedIncludes21 ) {
	fprintf(stderr, "Error: unexpected result; expecting %s\n",
		(expectedIncludes21 ? "true" : "false"));
	errflag = HnTRUE;
    }

    if ( errflag ) {
	exit(1);
    }

    printf("\n");
}

int
main(int, char *[])
{
    HnRange range1, range2;
    HnRange expectedOverlappingRange, expectedBoundingRange;
    HnBool expectedOverlaps12, expectedOverlaps21;
    HnBool expectedIncludes12, expectedIncludes21;

    /*
     * [0, 0], [0, 0]
     */

    range1 = new_HnRange(0, 0);
    range2 = new_HnRange(0, 0);

    expectedOverlappingRange = new_HnRange(0, 0);
    expectedBoundingRange = new_HnRange(0, 0);
    expectedOverlaps12 = HnTRUE;
    expectedOverlaps21 = HnTRUE;
    expectedIncludes12 = HnTRUE;
    expectedIncludes21 = HnTRUE;

    intersect(range1, range2,
	      expectedOverlappingRange, expectedBoundingRange,
	      expectedOverlaps12, expectedOverlaps21,
	      expectedIncludes12, expectedIncludes21);

    /*
     * [0, 0], [0, 1]
     */

    range1 = new_HnRange(0, 0);
    range2 = new_HnRange(0, 1);

    expectedOverlappingRange = new_HnRange(0, 0);
    expectedBoundingRange = new_HnRange(0, 1);
    expectedOverlaps12 = HnTRUE;
    expectedOverlaps21 = HnTRUE;
    expectedIncludes12 = HnFALSE;
    expectedIncludes21 = HnTRUE;

    intersect(range1, range2,
	      expectedOverlappingRange, expectedBoundingRange,
	      expectedOverlaps12, expectedOverlaps21,
	      expectedIncludes12, expectedIncludes21);

    /*
     * [0, 1], [1, 2]
     */

    range1 = new_HnRange(0, 1);
    range2 = new_HnRange(1, 2);

    expectedOverlappingRange = new_HnRange(1, 1);
    expectedBoundingRange = new_HnRange(0, 2);
    expectedOverlaps12 = HnTRUE;
    expectedOverlaps21 = HnTRUE;
    expectedIncludes12 = HnFALSE;
    expectedIncludes21 = HnFALSE;

    intersect(range1, range2,
	      expectedOverlappingRange, expectedBoundingRange,
	      expectedOverlaps12, expectedOverlaps21,
	      expectedIncludes12, expectedIncludes21);

    /*
     * [0, 1], [2, 3]
     */

    range1 = new_HnRange(0, 1);
    range2 = new_HnRange(2, 3);

    expectedOverlappingRange = HnRange::null;
    expectedBoundingRange = new_HnRange(0, 3);
    expectedOverlaps12 = HnFALSE;
    expectedOverlaps21 = HnFALSE;
    expectedIncludes12 = HnFALSE;
    expectedIncludes21 = HnFALSE;

    intersect(range1, range2,
	      expectedOverlappingRange, expectedBoundingRange,
	      expectedOverlaps12, expectedOverlaps21,
	      expectedIncludes12, expectedIncludes21);

    /*
     * [1, 1], [0, 2]
     */

    range1 = new_HnRange(1, 1);
    range2 = new_HnRange(0, 2);

    expectedOverlappingRange = new_HnRange(1, 1);
    expectedBoundingRange = new_HnRange(0, 2);
    expectedOverlaps12 = HnTRUE;
    expectedOverlaps21 = HnTRUE;
    expectedIncludes12 = HnFALSE;
    expectedIncludes21 = HnTRUE;

    intersect(range1, range2,
	      expectedOverlappingRange, expectedBoundingRange,
	      expectedOverlaps12, expectedOverlaps21,
	      expectedIncludes12, expectedIncludes21);

    /*
     * [0, 2], [1, 1]
     */

    range1 = new_HnRange(0, 2);
    range2 = new_HnRange(1, 1);

    expectedOverlappingRange = new_HnRange(1, 1);
    expectedBoundingRange = new_HnRange(0, 2);
    expectedOverlaps12 = HnTRUE;
    expectedOverlaps21 = HnTRUE;
    expectedIncludes12 = HnTRUE;
    expectedIncludes21 = HnFALSE;

    intersect(range1, range2,
	      expectedOverlappingRange, expectedBoundingRange,
	      expectedOverlaps12, expectedOverlaps21,
	      expectedIncludes12, expectedIncludes21);

    /*
     * [0, 1], [0, 1]
     */

    range1 = new_HnRange(0, 1);
    range2 = new_HnRange(0, 1);

    expectedOverlappingRange = new_HnRange(0, 1);
    expectedBoundingRange = new_HnRange(0, 1);
    expectedOverlaps12 = HnTRUE;
    expectedOverlaps21 = HnTRUE;
    expectedIncludes12 = HnTRUE;
    expectedIncludes21 = HnTRUE;

    intersect(range1, range2,
	      expectedOverlappingRange, expectedBoundingRange,
	      expectedOverlaps12, expectedOverlaps21,
	      expectedIncludes12, expectedIncludes21);

    printf("sizeof(HnRange): %d\n", sizeof(HnRange));

    return 0;
}
