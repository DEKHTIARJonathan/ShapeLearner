/**************************************************************************

File:                DistanceTransform.h

Author(s):           Pavel Dimitrov

Created:             19 Jun 2002

Last Revision:       $Date: 2002/11/20 22:03:03 $

Description: Base class. It actually has an implementaion, but a
very minimal one. It is meant to compute the distance
transform of a Shape. As such, it provides a value for
any point in the plane. It also computes the gradient
of the DT by approximating the partials as (e.g.)
DT(x+eps,y) - DT(x-eps,y)
d(DT)/dx (x,y) = -------------------------
eps

where eps can be specified.

Note that the precision of this implementation
depends only on the precision of the distance-to-curve
functions provided by the Curve's that are the
contours of the Shape. This, of course, makes DT run
slowest (which is quite reasonable/sufficient given a
good Curve implementation).

$Revision: 1.7 $

$Log: DistanceTransform.h,v $
Revision 1.7  2002/11/20 22:03:03  pdimit
A LOT has changed since last time.

Revision 1.6  2002/07/23 21:02:51  pdimit
The branch segmentation of the thinned DivMap is improved --
the two pixel branches are now included. Still, the degenerate case
of the square of junction-points is not taken care of.

Also, a DiscreteDivergenceSkeleton is created which still does not know
of left and right for the branches, i.e. the contour has not been cut.

Revision 1.5  2002/06/30 01:22:30  pdimit
Added DivergenceSkeletonMaker and a test for it. Incomplete functionality
and the testing is mostly in DivergenceSkeletonMaker.cpp. Only the
thinning of the DivergenceMap is implemented.

Revision 1.4  2002/06/27 14:14:40  pdimit
Just updated the descriptions in the header files.

Revision 1.3  2002/06/26 11:51:35  pdimit
Implemented the DivergenceMap class. It is supposed to be a base
class for other implementations. It has very dumb algorithms, but
which seem to work just fine. Look at testDivergenceMap to see how
to use it. Also, testSimpleShapeMaker has a much nicer interface --
exactly the same as for testDivergenceMap...

Revision 1.2  2002/06/26 04:30:41  pdimit
After the failed attempt at getting the spline smoothing to work

Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
Initial import



Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

**************************************************************************/

#ifndef DISTANCETRANSFORM_H
#define DISTANCETRANSFORM_H

#include <stdio.h>

#include "defines.h"

#include "Point.h"
#include "Vector.h"
#include "Curve.h"
#include "Shape.h"

namespace sg {

	class DistanceTransform
	{
	protected:
		Shape* shape;

	public:

		DistanceTransform() 
		{
			shape = 0;
		} 

		DistanceTransform(Shape* givenShape) 
		{
			shape = givenShape;
		}

		DistanceTransform(const Shape& givenShape) 
		{
			shape = givenShape.clone();
		}

		virtual ~DistanceTransform () 
		{
			delete shape;
		}

		DistanceTransform* clone() const 
		{ 
			return new DistanceTransform(*shape); 
		}

		Shape* getShape() 
		{ 
			return shape->clone(); 
		} 

		void getShapeBounds (double* xmin, double* xmax, 
			double* ymin, double* ymax) const
		{
			shape->getBounds(xmin, xmax, ymin,ymax);
		}

		double getValue(double x, double y)
		{
			std::vector<Curve*>* curves = shape->getCurves();
			std::vector<Curve*>::iterator I = curves->begin();

			double v = -1;

			for(; I != curves->end(); I++)
			{
				Curve* c = *I;
				double d = c->distToPt(x,y);

				if (v < 0 || v < d)
					v = d;
			}

			return v;
		}

		double getValue(Point p) 
		{ 
			return getValue(p.x, p.y); 
		}

		Distance getClosestPt(double x, double y)
		{
			std::vector<Curve *> *curves = shape->getCurves();
			std::vector<Curve *>::iterator I = curves->begin();

			Distance retd;

			double v = -1;

			for(;I != curves->end(); I++)
			{
				Curve *c = *I;
				Distance d = c->computeDistance(Point(x,y));
				if (v<0 || v < d.dist){
					v = d.dist;
					retd = d;
				}
			}

			return retd;

		}

		Vector getGradValue(double x, double y)
		{
			Vector v;

			Distance d = getClosestPt(x,y);
			Point p = d.p;

			if(d.dist > 0){
				v.x = p.x - x;
				v.y = p.y - y;
			} else {
				v.x = x - p.x;
				v.y = y - p.y;
			} 

			v.normalize();

			// v.x = (getValue(x-eps, y) - getValue(x+eps, y)) / (2.0 * eps);
			// v.y = (getValue(x, y-eps) - getValue(x, y+eps)) / (2.0 * eps);

			return v;
		}

		virtual Vector getGradValue(Point p, double eps=0.001)
		{
			return getGradValue(p.x, p.y);
		}
	};
}

#endif  /* DISTANCETRANSFORM_H */
