/**************************************************************************

File:                Shape.h

Author(s):           Pavel Dimitrov

Created:             17 Jun 2002

Last Revision:       $Date: 2002/06/30 01:22:30 $

Description: This is an abstract class. It describes continuous
shapes; that is, it contains a list of curves that
define it boundary (contours) and knows which points
in the plane are inside and which are outside. It also
provides size information, i.e. the bounding rectangle
(of course, the shape is assumed to be bounded).

$Revision: 1.4 $

$Log: Shape.h,v $
Revision 1.4  2002/06/30 01:22:30  pdimit
Added DivergenceSkeletonMaker and a test for it. Incomplete functionality
and the testing is mostly in DivergenceSkeletonMaker.cpp. Only the
thinning of the DivergenceMap is implemented.

Revision 1.3  2002/06/27 14:14:40  pdimit
Just updated the descriptions in the header files.

Revision 1.2  2002/06/26 06:34:44  pdimit
After adding a more successful smoothing.
A point becomes the average of its previou nbr, next nbr and
itself. The first and end points (which are the same) do not change.
This smoothing primitive is applied 5 times to get a nicer result.

Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
Initial import



Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

**************************************************************************/

#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include "defines.h"
#include "Point.h"
#include "Curve.h"

namespace sg{

	// abstract
	class Shape {
	protected:
		std::vector<Curve*> contourCurves;

		double xmin, xmax;
		double ymin, ymax;

	public:
		virtual ~Shape() { }

		const std::vector<Curve*>* getCurves() const { return &contourCurves; }
		std::vector<Curve*>* getCurves()             { return &contourCurves; }

		void getBounds(double *xmin, double *xmax,
			double *ymin, double *ymax) const{
				*xmin = this->xmin;
				*xmax = this->xmax;
				*ymin = this->ymin;
				*ymax = this->ymax;
		}

		void setBounds(const double& xmin, const double& xmax,
			const double& ymin, const double& ymax)
		{
			this->xmin = xmin;
			this->xmax = xmax;
			this->ymin = ymin;
			this->ymax = ymax;
		}

		virtual bool isInside(double x, double y) = 0;
		bool isInside(Point p) {return isInside(p.x, p.y);}

		virtual Shape *clone() const = 0;
	};

}

#endif  /* SHAPE_H */
