/************************************************************************

File:		spline_smooth.cpp

Author(s):		Pavel Dimitrov

Created:		24 Jun 2002

Last Revision:	$Date: 2002/06/26 04:32:11 $

Description:	        Modified original by Abeer George Ghuneim.

$Revision: 1.1 $

$Log: SplineSmooth.cpp,v $
Revision 1.1  2002/06/26 04:32:11  pdimit
After the failed attempt at getting the spline smoothing to work



Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

***********************************************************************/

#define SPLINE_SMOOTH_CPP

#include "Point.h"
#include "SplineSmooth.h"
#include <vector>

//extern "C"{
#include "spline_smooth.h"
//};


using namespace sg;

///// Private Types



////// Functions

//std::vector<Point> 
void sg::spline_smooth_pts(std::vector<Point> &pts, float weight)
{
	int npts = pts.size();
	Coordinates *nodes = new Coordinates[npts+2];

	std::vector<Point>::iterator I;
	unsigned int i;

	I=pts.begin();
	Point p = *I;
	nodes[npts+1].xcord = (float)p.x;
	nodes[npts+1].ycord = (float)p.y;
	nodes[npts+1].zcord = (float)0;

	// initialize data struct
	for(I=pts.begin(), i=1; I != pts.end(); I++, i++){
		Point p = *I;
		nodes[i].xcord = (float)p.x;
		nodes[i].ycord = (float)p.y;
		nodes[i].zcord = (float)0;

	}
	std::cerr << *pts.begin() << " == " << *(pts.end()) << "\n";

	std::cerr << i << " npts:" << npts << " initialized\n";

	// smooth
	::Periodic_Smoothing_Spline(nodes, npts, weight);
	std::cerr << "smoothed\n";

	if(1)
		for(I=pts.begin(), i=1; I != pts.end(); I++, i++){
			*I = Point((double)nodes[i].xcord,
				(double)nodes[i].ycord);

			//std::cerr << *I << "  " << nodes[i].xcord<<","<<nodes[i].ycord<<"\n" ;

		}

		//    for(I=pts.begin(), i=1; I != pts.end(); I++, i++){
		//      std::cerr << *I << "  " << nodes[i].xcord<<","<<nodes[i].ycord<<"\n" ;

		//    }


		// create return vector; of right size -- speedup
		//    std::vector<Point> ret_pts; 

		//    for(i=0; i<npts; i++){
		//      ret_pts.push_back(Point((double)nodes[i].xcord,
		//  			    (double)nodes[i].ycord));
		//    }
		//    std::cerr << "created\n";

		delete[] nodes; // clean up
		//    std::cerr << "cleaned up\n";

		//return ret_pts;
}

