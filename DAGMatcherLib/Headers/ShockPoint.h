/**------------------------------------------------------------------------
 * @file
 *
 * Copyright 2001 by the University of Toronto (UT).
 *
 *                       All Rights Reserved
 *
 * Permission to copy and modify this software and its documentation
 * only for internal use in your organization is hereby granted,
 * provided that this notice is retained thereon and on all copies.
 * UT makes no representations as to the suitability and operability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * UT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
 * SHALL UT BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * No other rights, including for example, the right to redistribute
 * this software and its documentation or the right to prepare
 * derivative works, are granted unless specifically provided in a
 * separate license agreement.
 *
 * Author: Diego Macrini
 *
 *-----------------------------------------------------------------------*/
#ifndef __SHOCK_POINT_H__
#define __SHOCK_POINT_H__

#include "DAGNode.h"
#include "DAGEdge.h"
#include "PolyBezierApprox.h"
#include "BoundaryCorner.h"

namespace dml {
/*!
	@brief Point in a bone graph node
*/
struct ShockPoint
{
	POINT pt;
	double radius, axisDist;
	sg::BoundaryInfo bndryInfo;

	const sg::Point& BndryPt1() const { return bndryInfo.first.pt; }
	const sg::Point& BndryPt2() const { return bndryInfo.second.pt; }

	//! Gets the x coordinate of the skeletal point (i==1) or the boundary points (i=={0,2})
	const double& X(int i) const
	{
		ASSERT(i >= 0 && i < 3);
		const double* xs[] = {&bndryInfo.first.pt.x, &pt.x, &bndryInfo.second.pt.x};
		return *xs[i];
	}

	//! Gets the x coordinate of the skeletal point (i==1) or the boundary points (i=={0,2})
	double& X(int i)
	{
		ASSERT(i >= 0 && i < 3);
		double* xs[] = {&bndryInfo.first.pt.x, &pt.x, &bndryInfo.second.pt.x};
		return *xs[i];
	}

	//! Gets the y coordinate of the skeletal point (i==1) or the boundary points (i=={0,2})
	const double& Y(int i) const
	{
		ASSERT(i >= 0 && i < 3);
		const double* ys[] = {&bndryInfo.first.pt.y, &pt.y, &bndryInfo.second.pt.y};
		return *ys[i];
	}

	//! Gets the y coordinate of the skeletal point (i==1) or the boundary points (i=={0,2})
	double& Y(int i)
	{
		ASSERT(i >= 0 && i < 3);
		double* ys[] = {&bndryInfo.first.pt.y, &pt.y, &bndryInfo.second.pt.y};
		return *ys[i];
	}

	void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const
	{
		if (bXMLFormat)
		{
			XML_OPEN_ATTS(os, "point");
			XML_PRINT_ATT2(os, "xcoord", pt.x);
			XML_PRINT_ATT2(os, "ycoord", pt.y);
			XML_PRINT_ATT(os, radius);
			XML_CLOSE_ATTS(os);
		}
	}
};

typedef SmartArray<ShockPoint> ShockPoints;
} //namespace dml

#endif //__SHOCK_POINT_H__
