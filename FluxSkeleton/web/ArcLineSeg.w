@q -*- mode: web; mode: cweb; -*- @>
@q   Program:	ArcLineSeg.w @>
@q   Created:	May 27, 2002 @>
@q   Author: 	Pavel Dimitrov @>


@*1 ArcLineSeg (implements |CurveSeg|).

Here is its definition:

@(ArcLineSeg.hpp@>=
#ifndef ArcLineSeg_HPP
#define ArcLineSeg_HPP
  @<ArcLineSeg includes@>@;


  namespace sg{
    @<ArcLineSeg definition@>
  }

#endif
@ and the implementation will follow:

@c
#include "ArcLineSeg.hpp"
@<ArcLineSeg implementation@>

@ @<ArcLineSeg includes@>=
#include "SG.hpp"
#include "Point.hpp"
#include "Vector.hpp"

@ @<ArcLineSeg definition@>=

typedef ArcLineType enum {Arc=1, Line};

class ArcLineSeg {
 protected:@;
 
 ArcLineType type;
 Point startPt;
 Point endPt;
 double radius;
 Point centre;  
};

@ Now, we implement the functions from |CurveSeg|. The first one is
computes the shortest distance between an arc-line segment and a given
point. 
@<ArcLineSeg implementation@>=
double ArcLineSeg::distToPt(Point p){
  
}

@ Now, we implement the functions from |CurveSeg|. The first one is
computes the shortest distance between an arc-line segment and a given
point. 
@<ArcLineSeg implementation@>+=
double ArcLineSeg::distToPt(Point p){
  
}
@ 
