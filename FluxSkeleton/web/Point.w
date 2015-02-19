@*1 Point. % -*- mode: web; mode: cweb; -*- 
This is a support class.


@c
#include "Point.hpp"


@ @(Point.hpp@>=
  
#ifndef Point_HPP
#define Point_HPP
  
    @<point include files@>@;
    
    
    namespace sg {
    
      @<Point class definition@>
	  
  }
#endif

@ Here are the actual definitions.

@<point include files@>=
#include <math.h>

@ @<Point class definition@>=
class Point {
  public:
  
  double x;
  double y;
  
  Point(double x, double y) { 
    this->x = x;
    this->y = y;
  }
  
  bool equal(Point p) {return (x == p.x && 
			       y == p.y);}
  
  bool isClose(Point p, double epsilon) {return equal(p);}
};

@q@>
