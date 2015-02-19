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

#ifndef SG_BOUNDARY_INFO_H
#define SG_BOUNDARY_INFO_H

#include "Vector.h"
#include "BoundaryPoint.h"

namespace sg {
	
struct BoundaryInfo
{
	BoundaryPoint first, second;
	double cumAxisDist; //<! cumulative skeleton distance from pt 0 to current

#ifdef USE_EXTRA_BOUNDARY_INFO
	double alpha;       //<! angle between tangent and first boundary point
	double alpha_p;     //<! alpha prime (d alpha / ds) for boundary-to-axis ratio 
	Vector tangent;     //<! tangent at the skeleton point

	BoundaryInfo() : tangent(0, 0) { alpha_p = alpha = cumAxisDist = 0; }
#else
	BoundaryInfo() { cumAxisDist = 0; }
#endif

	void clear()
	{
		first.clear();
		second.clear();
		cumAxisDist = 0;
	}

	const BoundaryPoint& operator[](char i) const { return (i == '1') ? first:second; }
	BoundaryPoint& operator[](char i)             { return (i == '1') ? first:second; }

	void SwapSides()
	{ 
		BoundaryPoint aux = first; 
		first = second; 
		second = aux; 
	}

	//! It is true iff at least one bndry point belongs to a bndry gap segment
	bool IsInterpolated() const
	{
		return (first.IsInterpolated() || second.IsInterpolated());
	}

	friend std::ostream& operator<<(std::ostream& os, const BoundaryInfo& bi)
	{
		return os << "[bp1:" << bi.first << ",\n bp2:" << bi.second
			<< ", cumAxisDist:" << bi.cumAxisDist << ']';
			//<< ",\n alpha:" << bi.alpha << ", alpha_p:" << bi.alpha_p
			//<< ", tangent:" << bi.tangent << ", cumAxisDist:" << bi.cumAxisDist << ']';
	}
};

} //namespace sg

#endif // SG_BOUNDARY_INFO_H
