/************************************************************************

File:		SimpleShapeMaker.cpp

Author(s):		Pavel Dimitrov

Created:		23 Jun 2002

Last Revision:	$Date: 2002/07/25 20:50:49 $

Description:	

$Revision: 1.8 $

$Log: SimpleShapeMaker.cpp,v $
Revision 1.8  2002/07/25 20:50:49  pdimit
Making release 0.1

Revision 1.7  2002/07/23 21:02:53  pdimit
The branch segmentation of the thinned DivMap is improved --
the two pixel branches are now included. Still, the degenerate case
of the square of junction-points is not taken care of.

Also, a DiscreteDivergenceSkeleton is created which still does not know
of left and right for the branches, i.e. the contour has not been cut.

Revision 1.6  2002/06/26 12:00:53  pdimit
Fixed the zero length segment problem in SimpleShapeMaker

Revision 1.5  2002/06/26 11:51:37  pdimit
Implemented the DivergenceMap class. It is supposed to be a base
class for other implementations. It has very dumb algorithms, but
which seem to work just fine. Look at testDivergenceMap to see how
to use it. Also, testSimpleShapeMaker has a much nicer interface --
exactly the same as for testDivergenceMap...

Revision 1.4  2002/06/26 07:47:27  pdimit
Just added the class SimpleShape to the mix.
It is precisely that, simple. It consist of a
single header file -- SimpleShape.h.
However, SimpleShapeMaker does not compute the
xmin/xmax and ymin/ymax values yet.

Revision 1.3  2002/06/26 06:34:45  pdimit
After adding a more successful smoothing.
A point becomes the average of its previou nbr, next nbr and
itself. The first and end points (which are the same) do not change.
This smoothing primitive is applied 5 times to get a nicer result.

Revision 1.2  2002/06/26 04:30:44  pdimit
After the failed attempt at getting the spline smoothing to work

Revision 1.1  2002/06/24 09:04:41  pdimit
First commit of this. The contour tracing (Pavlidis) seems to be
working.



Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

***********************************************************************/

#define SIMPLESHAPEMAKER_CPP

#include "SimpleShapeMaker.h"
#include "DiscreteSegCurve.h"
#include "LineSeg.h"
#include "CircSeg.h"
#include "Vector.h"

#include "SplineSmooth.h"

#include "BasicUtils.h"

using namespace sg;

void spit_it(std::vector<Point> ctr, int xs, int ys)
{
	std::cout << "P3\n" << xs << " " << ys << "\n256\n";

	bool *out = new bool[xs*ys];
	int i;

	for (i=0; i<xs*ys; i++)
		out[i] = false;

	std::vector<Point>::iterator I;

	for(I = ctr.begin(); I != ctr.end(); I++){
		Point p = *I;

		out[(int)p.y*xs + (int)p.x] = true;
	}

	for (i=0; i<xs*ys; i++){
		if (out[i])
			std::cout << " 200 0 0 ";
		else
			std::cout << " 0 0 200 ";
	}

	delete[] out;
}

// the shape is 0 and background is 1 (or false and true resp.)
// A point P has the following 8-contour
// 1 2 3
// 8 P 4
// 7 6 5

// this also computes xmin/xmax and ymin/ymax
Curve *SimpleShapeMaker::getContour(bool foreground){

	DiscreteSegCurve *dsc;

#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

	int dir = NORTH; // current trace direction

	// these are the offsets for P1,P2,P3 for the four directions
	Vector front_pt[][3] = {{Vector(-1,1), Vector(0,1), Vector(1,1)},    // north
	{Vector(1,-1), Vector(0,-1), Vector(-1,-1)}, // south
	{Vector(1,1), Vector(1,0), Vector(1,-1)},    // east
	{Vector(-1,-1), Vector(-1,0), Vector(-1,1)}  // west
	};

	int dir_change [][3] = {{WEST, NORTH, NORTH},   // current is NORTH
	{EAST, SOUTH, SOUTH},   // current is SOUTH
	{NORTH, EAST, EAST},  // current is EAST
	{SOUTH, WEST, WEST},  // current is WEST
	};

	// rotating in the clockwise dir
	int dir_rot [] = { EAST,   // current is NORTH
		WEST,   // current is SOUTH
		SOUTH,  // current is EAST
		NORTH  // current is WEST
	};

	// offsets for the 8-neighbours of point P
	//    Vector nbrs[] = {Vector(-1,1), Vector(0,1), Vector(1,1),   // 1 2 3
	//  		   Vector(1,0), Vector(1,-1), Vector(0,-1),  // 4 5 6
	//  		   Vector(-1,-1), Vector(-1,0)               // 7 8
	//    };


	std::vector<Point> contour_pts;

	Point p;  // current pixel
	Point sp; // starting pixel

	// start by finding a suitable first point
	int i = 0;
	while (arr[i] != foreground && i < xSize*ySize)
		i++;

	// there may not be a single foreground pixel !
	if(arr[i] == foreground){
		int x,y;

		y = i / xSize;
		x = i % xSize;

		p = Point((double)x, (double)y);
		contour_pts.push_back(p); // insert the first point into the list
		sp = p; // the current and start point are the same ...

		// now determine a good starting direction
		dir = NORTH;
		if(x!=0){
			if(operator()(x-1,y) != foreground)
				dir = NORTH;
		}
		else if(x<xSize-1){
			if(operator()(x+1,y) != foreground)
				dir = SOUTH;
		}
		else if(y>1){
			if(operator()(x,y-1) != foreground)
				dir = WEST;
		} 
		else if(y<ySize-1)
			if(operator()(x,y+1) != foreground)
				dir = EAST;

		// now find the next pixel

		Point np;
		bool found_pix = false;

		// try rotating
		for(int j=0; j < 4 && (found_pix==false); j++)
		{
			if(j!=0)
				dir = dir_rot[dir];

			// try the three pixels in front (according to dir)
			for (int i=0; i<3 && found_pix == false; i++)
			{
				np.x = p.x+front_pt[dir][i].x; // set the new pt to P(i+1)
				np.y = p.y+front_pt[dir][i].y; 

				if(withinBounds((int)np.x, (int)np.y))
				{
					if(operator()((int)np.x, (int)np.y) == foreground)
					{
						found_pix = true;

						p = np;
						contour_pts.push_back(p);
						// update the new direction
						dir = dir_change[dir][i];
					}
				}
			} // end of P1,P2,P3 tries
		} // end of 90deg rotations

		while (p.x != sp.x || p.y != sp.y)
		{
			found_pix = false;

			// try rotating
			for(int j=0; j < 4 && (found_pix==false); j++)
			{
				if(j!=0)
					dir = dir_rot[dir];

				// try the three pixels in front (according to dir)
				for (int i=0; i<3 && (found_pix == false); i++)
				{
					np.x = p.x+front_pt[dir][i].x; // set the new pt to P(i+1)
					np.y = p.y+front_pt[dir][i].y; 

					if(withinBounds((int)np.x, (int)np.y))
					{
						if(operator()((int)np.x, (int)np.y) == foreground)
						{
							found_pix = true;

							p = np;
							contour_pts.push_back(p);
							// update the new direction
							dir = dir_change[dir][i];

						}
					}
				} // end of P1,P2,P3 tries
			} // end of 90deg rotations
		} // end of while loop

		// Now that the contour is traced, smooth it out...
		for (int k = 0; k < m_nSmoothContourIterations; k++)
			smooth_closed_curve_pts(contour_pts);

		// compute xmin/xmax ymin/ymax
		std::vector<Point>::iterator I;
		xmin = (double)2*xSize; xmax = -1; // the values are always positive
		ymin = (double)2*ySize; ymax = -1;
		for(I=contour_pts.begin(); I!=contour_pts.end(); I++){
			Point p = *I;

			if(p.x < xmin) xmin = p.x;
			if(p.x > xmax) xmax = p.x;
			if(p.y < ymin) ymin = p.y;
			if(p.y > ymax) ymax = p.y;
		}

		// Finally, make the shape
		I = contour_pts.begin();
		Point pp = *I; I++;
		Point fp = pp; // the first point

		std::vector<CurveSeg*> segs;
		for(int i=0; I != contour_pts.end(); I++,i++){
			Point p = *I;

			LineSeg *ls = new LineSeg(pp, p);
			pp = p;

			segs.push_back(ls);
		}

		if(pp.x !=fp.x || pp.y!= fp.y){
			LineSeg *ls = new LineSeg(pp, fp);
			segs.push_back(ls);

			//std::cerr << "added\n";
		}

		dsc = new DiscreteSegCurve(segs, true);

		//spit_it(contour_pts, xSize, ySize);
	}
	else
		return 0;

	return dsc;
}

// Here we average the points. Diego: Bug fixed. it was "eating" one point.
void sg::smooth_closed_curve_pts(std::vector<Point>& pts)
{
	std::vector<Point> tpts;
	std::vector<Point>::iterator it0;
	std::vector<Point>::iterator itN;

	tpts.reserve(pts.size()); //Diego: makes it faster

	// Everything is easier if the first and last point are the same, so make sure they are
	if (pts.front() != pts.back())
		pts.push_back(pts.front());

	// Read the first point and its right neighbour
	it0 = pts.begin();
	Point pt0 = *it0; it0++;
	Point nextPt = *it0;

	// Read the left neighbour of the first point, i.e., the "true" last point
	itN = pts.end(); itN--; itN--;
	Point prevPt = *itN;

	// Compute the avg centered at the first point -- which is a special case
	pt0.x = (prevPt.x + pt0.x + nextPt.x) / 3.0;
	pt0.y = (prevPt.y + pt0.y + nextPt.y) / 3.0;

	// Save the new smoothed point
	tpts.push_back(pt0);

	// Now do all the other points, until (but including) the "true" last point
	std::vector<Point>::iterator left, right;
	Point currPt;

	itN++; // itN is now the repeated version of the first point

	for(; it0 != itN; it0++)
	{
		left  = it0; left--;
		right = it0; right++;

		prevPt = *left;
		nextPt = *right;
		currPt = *it0;

		// Average the current point
		currPt.x = (prevPt.x + currPt.x + nextPt.x) / 3.0;
		currPt.y = (prevPt.y + currPt.y + nextPt.y) / 3.0;

		tpts.push_back(currPt);
	}

	// Finally add the last point -- which is the same as the first
	tpts.push_back(pt0);

	pts = tpts;
}


Shape *SimpleShapeMaker::getShape(bool foreground){

	Curve* pContour = getContour(foreground);

	SimpleShape* ss = new SimpleShape(pContour); // Diego: ss now owns countour

	ss->setBounds(xmin, xmax, ymin, ymax);

	return ss;
}

Curve* sg::compute_curve_from_contour(std::vector<Point>& contour_pts, int nSmoothIter /*=5*/)
{
	DiscreteSegCurve *dsc;

	if (contour_pts.empty())
		return NULL;

	// smooth contour
	for (int j = 0; j < nSmoothIter; j++)
		smooth_closed_curve_pts(contour_pts);

	// Finally, make the shape
	std::vector<Point>::iterator I = contour_pts.begin();

	Point pp = *I; 

	I++;

	Point fp = pp; // the first point

	std::vector<CurveSeg*> segs;

	for(int i = 0; I != contour_pts.end(); I++, i++)
	{
		Point p = *I;

		LineSeg *ls = new LineSeg(pp, p);
		pp = p;

		segs.push_back(ls);
	}

	if(pp.x != fp.x || pp.y != fp.y)
	{
		LineSeg *ls = new LineSeg(pp, fp);
		segs.push_back(ls);
	}

	if (segs.empty())
		return NULL;

	dsc = new DiscreteSegCurve(segs, true);

	return dsc;
}

