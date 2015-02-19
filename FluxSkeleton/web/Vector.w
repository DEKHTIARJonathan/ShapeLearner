@*1 Vector. %  -*- mode: web; mode: cweb; -*- 
This is a support class.

@ @(Vector.hpp@>=
  
#ifndef Vector_HPP
#define Vector_HPP
  
    @<vector include files@>@;

  
  namespace sg {
    
    @<Vector class definition@>
    
  }
#endif

@ Here are the actual definitions.

@<vector include files@>=
#include <math.h>

@ @<Vector class definition@>=

class Vector {
  public:
  
  double x;
  double y;
  
  Vector(double x, double y) { 
    this->x = x;
    this->y = y;
  }
  
  double norm() {return sqrt(x*x+y*y);}
  
  double dot(Vector &v) { return x*v.x + y*v.y}
};

@q@>
