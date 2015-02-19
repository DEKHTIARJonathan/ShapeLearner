/*1:*/
#line 11 "ArcLineSeg.w"

#ifndef ArcLineSeg_HPP
#define ArcLineSeg_HPP
/*3:*/
#line 28 "ArcLineSeg.w"

#include "SG.hpp"
#include "Point.hpp"
#include "Vector.hpp"

/*:3*/
#line 14 "ArcLineSeg.w"



namespace sg{
/*4:*/
#line 33 "ArcLineSeg.w"


typedef ArcLineType enum{Arc= 1,Line};

class ArcLineSeg{
protected:

ArcLineType type;
Point startPt;
Point endPt;
double radius;
Point centre;
};

/*:4*/
#line 18 "ArcLineSeg.w"

}

#endif
/*:1*/
