/**************************************************************************

   File:                DiscreteDivergenceSkeleton.h

   Author(s):           Pavel Dimitrov

   Created:             22 Jul 2002

   Last Revision:       $Date: 2002/07/25 20:50:47 $

   Description: 

   $Revision: 1.2 $

   $Log: DiscreteDivergenceSkeleton.h,v $
   Revision 1.2  2002/07/25 20:50:47  pdimit
   Making release 0.1

   Revision 1.1  2002/07/23 21:02:51  pdimit
   The branch segmentation of the thinned DivMap is improved --
   the two pixel branches are now included. Still, the degenerate case
   of the square of junction-points is not taken care of.

   Also, a DiscreteDivergenceSkeleton is created which still does not know
   of left and right for the branches, i.e. the contour has not been cut.


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 **************************************************************************/

#ifndef SG_BOUNDARY_SEGMENT_LISTS_H
#define SG_BOUNDARY_SEGMENT_LISTS_H

#include <vector>
#include "Point.h"

namespace sg {

//! Boundary point array used to represent a contour with "gaps" in it
struct BoundarySegment
{
	int start, end;            //<! limits of the boundary segment
	std::vector<Point> gapPts; //<! points between this segment and the next bndry segment

	const Point& operator[](unsigned int i) const { return gapPts[i]; }
	Point& operator[](unsigned int i) { return gapPts[i]; }

	//! Add pt at the back of the gapPts vector
	void addGapPoint(const Point& pt) { gapPts.push_back(pt); }

	void addGapPoints(const Point& pt0, const Point& pt1, bool bIncludeLast);
};

typedef std::vector<BoundarySegment> BoundarySegments;

//! Lists of boundary segments and gaps on each side of the branch
struct BoundarySegmentLists
{
	BoundarySegments first, second;

	BoundarySegments& operator[](char i)
	{
		return (i == '1') ? first : second;
	}

	const BoundarySegments& operator[](char i) const
	{
		return (i == '1') ? first : second;
	}

	bool empty() const { return first.empty() && second.empty(); }
};

} //namespace sg

#endif // SG_BOUNDARY_SEGMENT_LISTS_H
