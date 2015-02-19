@q -*- mode: web; mode: cweb; -*- @>
@q   Program:	CurveSeg.w @>
@q   Created:	May 27, 2002 @>
@q   Author: 	Pavel Dimitrov @>


@*1 CurveSeg (Interface).
Here is the definition:
@(CurveSeg.hpp@>=
#ifndef CurveSeg_HPP
#define CurveSeg_HPP

  @<CurveSeg includes@>@;
  
  namespace sg {
    @<CurveSeg class definition@>
  }

#endif

@ @<CurveSeg includes@>=
#include "SG.hpp"
#include "Point.hpp"
#include "Vector.hpp"

@ @<CurveSeg class definition@>=
class CurveSeg {
  protected:@;

  double length;
  public:@;

  virtual double distToPt(Point p);
  virtual Point closestPt(Point p);
  virtual double getLength();
  virtual Vector normal(double t);
  
};
@q@>





