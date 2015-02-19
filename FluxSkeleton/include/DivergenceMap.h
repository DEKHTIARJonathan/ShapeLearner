/**************************************************************************

File:                DivergenceMap.h

Author(s):           Pavel Dimitrov

Created:             26 Jun 2002

Last Revision:       $Date: 2002/06/30 01:22:30 $

Description: Base class. As with the DistanceTransform, this is a
simple implementation of the divergence map
computation. It uses the DT to get the grad(DT) and
approximates the divergence value at any point in the
plane by taking an epsilon(user specified) radius
circle around it and using 8 points on this circle.

It can return the shape this is the DivMap of -- it
returns a clone of it, actually. Similarly for the DT.

$Revision: 1.3 $

$Log: DivergenceMap.h,v $
Revision 1.3  2002/06/30 01:22:30  pdimit
Added DivergenceSkeletonMaker and a test for it. Incomplete functionality
and the testing is mostly in DivergenceSkeletonMaker.cpp. Only the
thinning of the DivergenceMap is implemented.

Revision 1.2  2002/06/27 14:14:40  pdimit
Just updated the descriptions in the header files.

Revision 1.1  2002/06/26 11:51:35  pdimit
Implemented the DivergenceMap class. It is supposed to be a base
class for other implementations. It has very dumb algorithms, but
which seem to work just fine. Look at testDivergenceMap to see how
to use it. Also, testSimpleShapeMaker has a much nicer interface --
exactly the same as for testDivergenceMap...



Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

**************************************************************************/

#ifndef DIVERGENCEMAP_H
#  define DIVERGENCEMAP_H

#include "Shape.h"
#include "DistanceTransform.h"
#include "Point.h"

#define DIV_MAP_DEF_SAMP_RES 16

namespace sg{


	class DivergenceMap {

		DistanceTransform *dt;
		int m_nSamplingResolution;

	public:

		DivergenceMap(int nSamplingResolution = DIV_MAP_DEF_SAMP_RES)
		{ 
			m_nSamplingResolution = nSamplingResolution; 
		}

		DivergenceMap(DistanceTransform* pGivenDT, 
			int nSamplingResolution = DIV_MAP_DEF_SAMP_RES)
		{
			dt = pGivenDT;
			m_nSamplingResolution = nSamplingResolution;
		}

		DivergenceMap(const DistanceTransform& givenDT, 
			int nSamplingResolution = DIV_MAP_DEF_SAMP_RES)
		{
			dt = givenDT.clone();
			m_nSamplingResolution = nSamplingResolution;
		}

		virtual ~DivergenceMap() { delete dt;}

		virtual double getValue(double x, double y, double eps=0.1) const;
		virtual double getValue(Point p, double eps=0.1) const{ 
			return getValue(p.x,p.y, eps); 
		}

		Shape *getShape() const {return dt->getShape();} 

		void getShapeBounds (double *xmin, double *xmax, 
			double *ymin, double *ymax) const{
				dt->getShapeBounds(xmin,xmax, ymin,ymax);
		}

		DistanceTransform *getDT() const{ return dt->clone();} 

		DivergenceMap *clone() const
		{ 
			return new DivergenceMap(*dt, m_nSamplingResolution);
		}
	};

}


#endif  /* DIVERGENCEMAP_H */
