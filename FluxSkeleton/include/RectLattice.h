/**************************************************************************

   File:                RectLattice.h

   Author(s):           Pavel Dimitrov

   Created:             17 Jun 2002

   Last Revision:       $Date: 2002/06/23 05:42:08 $

   Description: 

   $Revision: 1.1.1.1 $

   $Log: RectLattice.h,v $
   Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
   Initial import


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 **************************************************************************/

#ifndef RECTLATTICE_H
#  define RECTLATTICE_H

#include <math.h>

namespace sg{

  class RectLattice{
    
  protected:
    double x0, y0;
    int xSize, ySize;
    double xScale, yScale;
    int index[4][2];

  public:
    // the return value is in the protected var. index[][]
    // the index xi,yi in the allocated array is such that
    // the actual location x,y satisfies
    // x = xi*xScale + x0
    // y = yi*yScale + y0
    void getIndex(double x, double y){
     int ind[4][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
     //index = ind; fixed by Diego
     
     for (int i = 0; i < 4; i++)
     	for (int j = 0; j < 2; j++)
     		index[i][j] = ind[i][j];
     
     if (x < x0 || y < y0 || 
	 x >= x0+(double)xSize*xScale ||
	 y >= y0+(double)ySize*yScale )
       {
	 index[0][0] = -1;
	 return;
       }

     double ipart, modpart;
     
     double xx = (x - x0) / xScale;
     double yy = (y - y0) / yScale;

     modpart = (int) modf(xx, &ipart);
     
     index[0][0] = (int)ipart;
     if (ipart >= 0.5) index[0][0]++;
     if (index[0][0] >= xSize) {
       index[0][0] = -1;
       return;  
     }
     
     modpart = (int) modf(yy, &ipart);
     index[0][1] = (int)ipart;
     if (modpart >= 0.5) index[0][1]++;
     if (index[0][1] >= ySize) {
       index[0][0] = -1;
       return;  
     }
     
    }

  };

}

#endif  /* RECTLATTICE_H */
