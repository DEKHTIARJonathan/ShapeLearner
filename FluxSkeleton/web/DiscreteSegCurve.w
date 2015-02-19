@q -*- mode: web; mode: cweb; -*- @>
@q   Program:	DiscreteSegCurve.w @>
@q   Created:	May 27, 2002 @>
@q   Author: 	Pavel Dimitrov @>

@*1 DiscreteSegCurve(implements |Curve|).
This is the first concrete class of type |Curve|. It represents
curves as a finite number of segments. The segments are not
predefined (there is an abstract type |CurveSeg|), so arc-lines and
spline representations can be implemented through this class.

 Here is the definition:
@(DiscreteSegCurve.hpp@>=
  
#ifndef DiscreteSegCurve_HPP
#define DiscreteSegCurve_HPP

  @<DiscreteSegCurve includes@>@;
  
  namespace sg {
    @<DiscreteSegCurve definition@>
  }
#endif

@ and the implementation

@c
#include "DiscreteSegCurve.hpp"
@<DiscreteSegCurve implementation@>

@ @<DiscreteSegCurve includes@>=
#include "Curve.hpp"
#include "CurveSeg.hpp"

@ @<DiscreteSegCurve definition@>=
class DiscreteSegCurve : public Curve {
  protected:
    
  List<Point> points;@;

  List<CurveSeg> segments;
  

};

@ @<DiscreteSegCurve implementation@>=

double DiscreteSegCurve::distToPt(Point p){

}


@q@>