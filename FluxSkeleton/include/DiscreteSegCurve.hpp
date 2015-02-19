/*1:*/
#line 13 "DiscreteSegCurve.w"


#ifndef DiscreteSegCurve_HPP
#define DiscreteSegCurve_HPP

/*3:*/
#line 31 "DiscreteSegCurve.w"

#include "Curve.hpp"
#include "CurveSeg.hpp"

/*:3*/
#line 18 "DiscreteSegCurve.w"


namespace sg{
/*4:*/
#line 35 "DiscreteSegCurve.w"

class DiscreteSegCurve:public Curve{
protected:

List<Point> points;

List<CurveSeg> segments;


};

/*:4*/
#line 21 "DiscreteSegCurve.w"

}
#endif

/*:1*/
