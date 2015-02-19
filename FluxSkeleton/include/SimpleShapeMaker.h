/**************************************************************************

   File:                SimpleShapeMaker.h

   Author(s):           Pavel Dimitrov

   Created:             22 Jun 2002

   Last Revision:       $Date: 2002/07/25 20:50:47 $

   Description: Implements ShapeMaker. This is here both for testing
                purposes and as a sample implementation. It is
		actually a bollean array that knows how to create a
		SimpleShape. It also provides the containing contour
		of the binary image as a closed Curve. 

		It is reasonably fast.

   $Revision: 1.6 $

   $Log: SimpleShapeMaker.h,v $
   Revision 1.6  2002/07/25 20:50:47  pdimit
   Making release 0.1

   Revision 1.5  2002/06/27 14:14:40  pdimit
   Just updated the descriptions in the header files.

   Revision 1.4  2002/06/26 11:51:35  pdimit
   Implemented the DivergenceMap class. It is supposed to be a base
   class for other implementations. It has very dumb algorithms, but
   which seem to work just fine. Look at testDivergenceMap to see how
   to use it. Also, testSimpleShapeMaker has a much nicer interface --
   exactly the same as for testDivergenceMap...

   Revision 1.3  2002/06/26 07:47:25  pdimit
   Just added the class SimpleShape to the mix.
   It is precisely that, simple. It consist of a
   single header file -- SimpleShape.h.
   However, SimpleShapeMaker does not compute the
   xmin/xmax and ymin/ymax values yet.

   Revision 1.2  2002/06/26 04:30:41  pdimit
   After the failed attempt at getting the spline smoothing to work

   Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
   Initial import


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 **************************************************************************/

#ifndef SIMPLESHAPEMAKER_H
#define SIMPLESHAPEMAKER_H

#define DEF_SMOOTH_ITER 5

#include "ShapeMaker.h"
#include "SimpleShape.h"
#include <vector>

namespace sg{

  void smooth_closed_curve_pts(std::vector<Point>& pts);
  Curve* compute_curve_from_contour(std::vector<Point>& contour_pts, int nSmoothIter = DEF_SMOOTH_ITER);
  
  class SimpleShapeMaker: public ShapeMaker {

    double xmin, xmax, ymin, ymax;
    
    int xSize, ySize;
    bool *arr;

    bool bl;

	int m_nSmoothContourIterations; // Add by Diego

  public:

    // constructors/destructors
    SimpleShapeMaker(int xs, int ys, int nSmoothContourIterations = DEF_SMOOTH_ITER, bool bInit = true) 
	{
		xSize = xs;
		ySize = ys;

		m_nSmoothContourIterations = nSmoothContourIterations;

		arr = new bool[xs*ys];

		if (bInit)
			for(int i=0; i<xs*ys; i++)
				arr[i] = false;

		bl = false;
    }

    SimpleShapeMaker(int xs, int ys, bool *array, int nSmoothContourIterations = DEF_SMOOTH_ITER) 
	{
		xSize = xs;
		ySize = ys;

		m_nSmoothContourIterations = nSmoothContourIterations;

		arr = new bool[xs*ys];

		for(int i=0; i<xs*ys; i++)
			arr[i] = array[i];

		bl = false;
    }

    ~SimpleShapeMaker() {
      delete[] arr;
    }

    // methods
    bool operator()(int x, int y) const
	{
		if (x>=0 && x<xSize && y>=0 && y<ySize)
			return arr[y*xSize + x];
		else
			return false;
    }

    bool& operator()(int x, int y)
	{
		if (x>=0 && x<xSize && y>=0 && y<ySize)
			return arr[y*xSize + x];
		else
			return (bool &)bl;
    }

    Shape* getShape(bool foreground=false);

    Curve* getContour(bool foreground=false);

    bool withinBounds(int x, int y) {
      return (x>=0 && x < xSize && y>=0 && y < ySize);
	
    }

  };

}

#endif  /* SIMPLESHAPEMAKER_H */
