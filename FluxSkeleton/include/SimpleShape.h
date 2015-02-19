/**************************************************************************

File:                SimpleShape.h

Author(s):           Pavel Dimitrov

Created:             26 Jun 2002

Last Revision:       $Date: 2002/06/27 14:14:40 $

Description: Implements Shape in the simplest possible way. It
contains no optimization whatsoever. Meant for testing
when correctness is most important.

$Revision: 1.2 $

$Log: SimpleShape.h,v $
Revision 1.2  2002/06/27 14:14:40  pdimit
Just updated the descriptions in the header files.

Revision 1.1  2002/06/26 07:47:25  pdimit
Just added the class SimpleShape to the mix.
It is precisely that, simple. It consist of a
single header file -- SimpleShape.h.
However, SimpleShapeMaker does not compute the
xmin/xmax and ymin/ymax values yet.



Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

**************************************************************************/

#ifndef SIMPLESHAPE_H
#  define SIMPLESHAPE_H

#include "Shape.h"
#include "DiscreteSegCurve.h"

namespace sg{

	class SimpleShape: public Shape 
	{
	public:
		SimpleShape(std::vector<CurveSeg*>& segs, bool closed)
		{
			DiscreteSegCurve* pContour = new DiscreteSegCurve(segs, closed);

			contourCurves.push_back(pContour);
		}

		SimpleShape(Curve* pContour) 
		{
			contourCurves.push_back(pContour);
		}

		SimpleShape(const Curve& contour) 
		{
			contourCurves.push_back(contour.clone());
		}

		virtual ~SimpleShape() 
		{
			for (unsigned int i = 0; i < contourCurves.size(); i++)
				delete contourCurves[i];

			/*Curve *contour = contourCurves.front();
			//since we cloned it in the constructor
			delete contour;*/
		}

		// methods

		Shape* clone() const
		{
			Shape* pShape = new SimpleShape(*contourCurves.front());

			pShape->setBounds(xmin, xmax, ymin, ymax);

			return pShape;
		}

		// must implement this...
		bool isInside(double x, double y){ return false; }

	};
}


#endif  /* SIMPLESHAPE_H */
