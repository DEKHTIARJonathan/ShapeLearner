/*2:*/
#line 4 "Vector.w"


#ifndef Vector_HPP
#define Vector_HPP

/*3:*/
#line 21 "Vector.w"

#include <math.h> 

/*:3*/
#line 9 "Vector.w"



namespace sg{

/*4:*/
#line 24 "Vector.w"


class Vector{
public:

double x;
double y;

Vector(double x,double y){
this->x= x;
this->y= y;
}

double norm(){return sqrt(x*x+y*y);}

double dot(Vector&v){return x*v.x+y*v.y}
};

/*:4*/
#line 14 "Vector.w"


}
#endif

/*:2*/
