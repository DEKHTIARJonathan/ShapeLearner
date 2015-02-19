/**************************************************************************

   File:                CurveSeg.h

   Author(s):           Pavel Dimitrov

   Created:             11 Jun 2002

   Last Revision:       $Date: 2002/07/25 20:50:47 $

   Description: This is an abstract class. It is like Curve, but it
                allows CurveSeg's to be stuck one after the
		other. Technically, this could be a subclass of
		Curve; it is not. The main reason is that the notion
		of a subsegment (coming from Curve's subCurve()) does
		not make much sense.

		The initial intent is to have these as self-contained
		pieces of cuves that can compute distances and such to
		themselves precisely. In other words, in using them,
		it may be reasonable to assume that the distance or
		any other calculation performed by a CurveSeg will
		take a short (constant) time, i.e. there would not be
		subparts called. This is not a strict rule, however.

   $Revision: 1.3 $

   $Log: CurveSeg.h,v $
   Revision 1.3  2002/07/25 20:50:47  pdimit
   Making release 0.1

   Revision 1.2  2002/06/27 14:14:40  pdimit
   Just updated the descriptions in the header files.

   Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
   Initial import


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 **************************************************************************/

#ifndef CURVESEG_H
#  define CURVESEG_H

#include "Drawable.h"
#include "Point.h"
#include "Vector.h"
#include "Curve.h"

namespace sg{
  // abstract class
  class CurveSeg{
  public:
    // the position of the starting point of this segment in the curve
    double startLength;


    virtual ~CurveSeg() {};

    virtual Point atT(double t) = 0;
    virtual double distToPt(const Point &p) = 0;
    virtual double closestToPt(const Point &p) = 0; // returns the t-value
    virtual Distance computeDistance(const Point &p) = 0;
    virtual Distance computeDistance(const Point &p, double t1, double t2) = 0;
    virtual Vector tangent(double t) = 0; // unit tangent
    virtual Vector normal(double t) { return tangent(t).getPerp(); } 
    virtual double getLength() = 0;
    virtual CurveSeg *clone() = 0;

    virtual void drawSeg(Drawable &d) = 0;
  };

}

#endif  /* CURVESEG_H */
