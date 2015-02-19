/**************************************************************************

   File:                Curve.h

   Author(s):           Pavel Dimitrov

   Created:             11 Jun 2002

   Last Revision:       $Date: 2002/07/25 20:50:47 $

   Description: This provides the definition of a general purpose Curve.
                It is an abstract class that defines the
		interface. The curve is supposed to be parametrize by
		parameter t, i.e. c(t). One can get c'(t)/|c'(t)| (the
		unit tangent) and hence a unit normal. It is also
		possible to get the closest point on the curve for any
		point in the plane. The distance is SIGNED. The actual
		sign depends on the parametrization and the
		calculation is as follows:
		  given point P in the plane
		 1) find point Q on c(t) closest to P.
		 2) Let T(Q) be the unit tangent of c(t) at Q and
		 denote by v the vector (P-Q), i.e. from the curve to
		 the point P; then the result (signed distance) is
		 given by
		            |T(Q) x v| = T(Q)_x * v_y - T(Q)_y * v_x

	       The sign of this gives the sign of the distance. Note
	       that it may be the case that the above expression is
	       not equal to the signed distance -- this happens
	       whenever there is no unambiguous tangent to the curve
	       at the closest point on the curve. If there is a
	       point Q on the curve where the tangent (derivative c'(t))
	       is not well defined, then it is -- in general --
	       impossible to decide the sign of the points on the
	       plane for which P is the closest point on the curve.

   $Revision: 1.2 $

   $Log: Curve.h,v $
   Revision 1.2  2002/07/25 20:50:47  pdimit
   Making release 0.1

   Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
   Initial import


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 **************************************************************************/

#ifndef CURVE_H
#define CURVE_H

#include "defines.h"
#include "Drawable.h"
#include "Vector.h"

namespace sg {

  // a structure for returning the closest distance to the curve
  typedef struct _Distance {
    double dist;  // the distance
    double t;     // the point as c(t) where c is the curve/segment
    Point p;      // the actual point
    _Distance() { dist = -1; t = -1; }
  }Distance;
  
  // an abstract class
  class Curve {
    
  protected:
    bool closed;
    double length;

  public:
    virtual ~Curve() {}

    bool isClosed() {return closed;}
    double getLength() {return length;}

    virtual double distToPt(const Point &p) = 0;
    virtual double distToPt(double x, double y){
      return distToPt(Point(x,y));
    }
    virtual Point atT(double t) = 0;
    Point getPoint(double t) { return atT(t);}
    virtual Vector tangent(double t) = 0;//the unit tangent at c(t),i.e. c'(t)
    virtual Vector tangent(unsigned int idx, Point* pPoint = NULL) = 0;
    virtual Vector normal(double t) { return tangent(t).getPerp(); }
    virtual Curve *subCurve(double t0, double t1) = 0;
    virtual double closestToPt(const Point &p) = 0;
    virtual Distance computeDistance(const Point &p) = 0;
    virtual Distance computeDistance(const Point &p, double t1, double t2) = 0;

    virtual void drawCurve(Drawable &d) = 0;
    
    virtual Curve* clone() const = 0;
  };

}

#endif  /* CURVE_H */
