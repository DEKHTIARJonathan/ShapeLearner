/**************************************************************************

   File:                DiscreteSegCurve.h

   Author(s):           Pavel Dimitrov

   Created:             11 Jun 2002

   Last Revision:       $Date: 2002/07/25 20:50:47 $

   Description: This is a type of Curve (see Curve.h) that is made up of
                some number of CurveSeg's. These can be anything, so
		long as they provide the necessary functionality; that
		is the same as a Curve but being aware that they
		follow one another (see CurveSeg.h).

		This class is meant as a "container" for any mix of
		curve segments, e.g. lines, arcs or spline segments.

   $Revision: 1.3 $

   $Log: DiscreteSegCurve.h,v $
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

#ifndef DISCRETESEGCURVE_H
#define DISCRETESEGCURVE_H

#include <vector>
#include "defines.h"
#include "Point.h"
#include "Vector.h"
#include "CurveSeg.h"
#include "Curve.h" // parent class

namespace sg{

  class DiscreteSegCurve : public Curve
  {
  public:
    std::vector<CurveSeg*> segments;

    double shortest_seg;

  public:
    // constructors/destructors
    DiscreteSegCurve() {}
    DiscreteSegCurve(std::vector<CurveSeg*> &segs, bool closed);
    
    virtual ~DiscreteSegCurve() 
	{ 
		// remove segs since they were cloned
		std::vector<CurveSeg*>::iterator I;

		for(I = segments.begin(); I != segments.end(); I++)
		{
			delete *I;
		}
    }
    
    // methods
    double distToPt(const Point &p);
    Point atT(double t);
    //Point getPoint(double t) { return atT(t);}
    Vector tangent(double t);
    Vector tangent(unsigned int idx, Point* pPoint = NULL);
    //    Vector normal(double t);

    Curve *subCurve(double t0, double t1) {return this;}
    double closestToPt(const Point &p);
    virtual Distance computeDistance(const Point &p);
    virtual Distance computeDistance(const Point &p, double t1, double t2);

    Curve* clone() const;

    void drawCurve(Drawable &d);

    friend std::ostream &operator<< (std::ostream &Out, DiscreteSegCurve &dsc){

      Out << "[DiscreteSegCurve]: closed:" << dsc.closed << " length:" << dsc.length;
      Out << " #segs:" << dsc.segments.size() << " shortest:" << dsc.shortest_seg;
      
      return Out;
    }

    int get_num_segs() { return segments.size(); }

    // returns the length of the shortest segment
    double get_shortest() { return shortest_seg; }
  };
  
}

#endif  /* DISCRETESEGCURVE_H */
