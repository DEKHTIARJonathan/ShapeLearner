/**************************************************************************

   File:                DiscreteShape.h

   Author(s):           Pavel Dimitrov

   Created:             17 Jun 2002

   Last Revision:       $Date: 2002/06/27 14:14:40 $

   Description: Implements Shape.

   $Revision: 1.2 $

   $Log: DiscreteShape.h,v $
   Revision 1.2  2002/06/27 14:14:40  pdimit
   Just updated the descriptions in the header files.

   Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
   Initial import


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 **************************************************************************/

#ifndef DISCRETESHAPE_H
#define DISCRETESHAPE_H

#include <math.h>
#include <vector>

#include "defines.h"

#include "Point.h"
#include "Curve.h"

#include "Shape.h"         // base class
#include "RectLattice.h"   // second parent class
#include "Drawable.h"   // second parent class

//////
#define POINT_AT(x,y) shapeArray[(y)*xSize+(x)]

#define INSIDE_COL 0
#define CONTOUR_COL 10
#define BACK_COL 100



namespace sg{
  
  class DiscreteShape : public Shape, public RectLattice,
			public Drawable{
    
    char *shapeArray; // this is the in/out flag array

    void do_line(int x1, int y1, int x2, int y2, char v);

    void flood_fill(int x, int y); // starts at 0,0 in the array coords

  public:

    
    // constructors/destructors
    DiscreteShape(double x0, double y0,
		  int xSize, int ySize,
		  double xScale, double yScale,
		  Curve *contour){

      this->x0 = x0;
      this->y0 = y0;
      this->xSize = xSize;
      this->ySize = ySize;
      this->xScale = xScale;
      this->yScale = yScale;

      //shapeArray = new char[xSize*ySize];

     // use a copy of the contour, not the one given!
      Curve *thec = contour->clone();
      contourCurves.push_back(thec);


      //clear(); // clear the array first !
      //thec->drawCurve(*this);

      //flood_fill(0,0); // the 
    }

    ~DiscreteShape() {std::cerr << "DESTROYING shape\n";}
    
    // methods
      bool isInside(double x, double y){
	return false;
      }
    
//      bool isInside(double x, double y){
//        getIndex(x,y);      // return is in the protected var. index[][]
//        if(index[0][0] < 0) // index[0][0]<0 means out_of_bounds
//  	return false;
      
//        else // either INSIDE_COL or CONTOUR_COL is inside, hence this V
//  	{
//  	  return (POINT_AT(index[0][0], index[0][1]) != BACK_COL);
//  	}
//      }

    Shape *clone() const{
      return new DiscreteShape(x0,y0, xSize,ySize, xScale,yScale, 
			       contourCurves.front());
    }

    // Drawable implementation
    void clear() {
      for(int i=0; i<xSize*ySize; i++)
	shapeArray[i] = INSIDE_COL;
    } 

    void drawPoint(double x, double y, int col=0) {
      getIndex(x,y); // first, compute the position
      POINT_AT(index[0][0], index[0][1]) = true; // then, draw it
    }
    
    void drawLine(double x1, double y1, 
		  double x2, double y2, int col=0){
      int x,y, xx, yy;

      getIndex(x1,y1);
      x = index[0][0];
      y = index[0][1];

      if (x<0) return;

      getIndex(x2,y2);
      xx = index[0][0];
      yy = index[0][1];

      if (xx<0) return;

      do_line(x,y, xx,yy, CONTOUR_COL);
    }

    // testing code
    void spit(){
      std::cout << "P3\n" << xSize << " " << ySize << "\n256\n";

      for(int i=0; i < xSize*ySize; i++){
	if (shapeArray[i] == CONTOUR_COL)
	  std::cout << "0 255 0\n";
	else if (shapeArray[i] == BACK_COL)
	  std::cout << "0 0 255\n";
	//if (shapeArray[i] == INSIDE_COL)
	else std::cout << "255 0 0\n";
      }
    }
    
  };

}


#endif  /* DISCRETESHAPE_H */
