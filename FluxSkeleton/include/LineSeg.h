/**************************************************************************

   File:                LineSeg.h

   Author(s):           Pavel Dimitrov

   Created:             11 Jun 2002

   Last Revision:       $Date: 2002/07/25 20:50:47 $

   Description: This is an implementation of CurveSeg. It provides
                line segments. (See CurveSeg.h)

   $Revision: 1.3 $

   $Log: LineSeg.h,v $
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

#ifndef LINESEG_H
#define LINESEG_H

#include <iostream>

#include "defines.h"

#include "CurveSeg.h"
#include "Point.h"
#include "Vector.h"

namespace sg{
  


  // concrete class
  class LineSeg : public CurveSeg{
  public:
    Point startPt;
    Point endPt;
    
    Vector v; // tangent vector
    double length; // of the line segment
 
  public:

    //constructors
    LineSeg(Point sP, Point eP, double sl=0) : startPt(sP), endPt(eP){
      startLength = sl;
      v.x = eP.x - sP.x;
      v.y = eP.y - sP.y;
      
      length = v.norm();

      v.x /= length; // in order to parametrize by t=arc_length
      v.y /= length;
    }

    virtual ~LineSeg() {}

    //methods
    double getLength() { return length; }

    //inherited
    Point atT(double t);
    double distToPt(const Point &p);
    double closestToPt(const Point &p); // returns the t-value
    virtual Distance computeDistance(const Point &p);
    virtual Distance computeDistance(const Point &p, double t1, double t2);

    Vector tangent(double t) { // maybe should check for bounds ... throw Excep.
      return v;
    }


    CurveSeg *clone(){
      LineSeg *ls = new LineSeg(startPt, endPt, startLength);
      ls->v = v;
      ls->length = length;
      return ls;
    }
    
    void drawSeg(Drawable &d) { 
      d.drawLine(startPt, endPt);
    } 

    friend std::ostream &operator<< (std::ostream &Out, LineSeg &ls){
      Out << "LineSeg[(" << ls.startPt.x << ", " << ls.startPt.y << ")-";
      Out << "(" << ls.endPt.x << ", " << ls.endPt.y << "), " << ls.length << "]";

      return Out;
    }
  };

}

#endif  /* LINESEG_H */
