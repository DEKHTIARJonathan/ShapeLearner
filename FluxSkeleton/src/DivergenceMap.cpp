/************************************************************************

   File:		DivergenceMap.cpp

   Author(s):		Pavel Dimitrov

   Created:		26 Jun 2002

   Last Revision:	$Date: 2002/07/23 21:02:53 $

   Description:	

   $Revision: 1.3 $

   $Log: DivergenceMap.cpp,v $
   Revision 1.3  2002/07/23 21:02:53  pdimit
   The branch segmentation of the thinned DivMap is improved --
   the two pixel branches are now included. Still, the degenerate case
   of the square of junction-points is not taken care of.

   Also, a DiscreteDivergenceSkeleton is created which still does not know
   of left and right for the branches, i.e. the contour has not been cut.

   Revision 1.2  2002/06/30 01:22:31  pdimit
   Added DivergenceSkeletonMaker and a test for it. Incomplete functionality
   and the testing is mostly in DivergenceSkeletonMaker.cpp. Only the
   thinning of the DivergenceMap is implemented.

   Revision 1.1  2002/06/26 11:51:37  pdimit
   Implemented the DivergenceMap class. It is supposed to be a base
   class for other implementations. It has very dumb algorithms, but
   which seem to work just fine. Look at testDivergenceMap to see how
   to use it. Also, testSimpleShapeMaker has a much nicer interface --
   exactly the same as for testDivergenceMap...


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 ***********************************************************************/

#define DIVERGENCEMAP_C

#include "DivergenceMap.h"
#include "Vector.h"
#include <math.h>

using namespace sg;

// eps is the half side of the square
double DivergenceMap::getValue(double x, double y, double eps) const
{
	double ret = 0;

	//const int nSamplingResolution = 20; // DIEGO: used to be 10 during Masters experiments

	const double Pi_n = 2.0 * 3.14159265359 / m_nSamplingResolution;
	double xx, yy, alpha;
	Vector N, v;

	for(int i = 0; i < m_nSamplingResolution; i++)
	{
		alpha = i * Pi_n;

		xx = cos(alpha);
		yy = sin(alpha);

		N.x = xx; 
		N.y = yy;

		xx = x + eps * xx;
		yy = y + eps * yy;

		v = dt->getGradValue(xx, yy);

		ret += N.dot(v);
	}

	return Pi_n * ret;
}
