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
 * $Log: PolyLineApprox.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __POLY_LINE_APROX_H__
#define __POLY_LINE_APROX_H__

#include "PiecewiseApprox.h"
#include "LineSegment.h"

namespace dml {
class PolyLineApprox : public PiecewiseApprox<LineSegment>
{
protected:
	double m_dMinSlope;

public:
	PolyLineApprox(double dMinError, double dMinSlope, int nMaxSegments, double dMaxYDiff)
		: PiecewiseApprox<LineSegment>(dMinError, nMaxSegments, dMaxYDiff)
	{
		m_dMinSlope =  dMinSlope;
	}

	virtual double LeastSquares(const POINT* vertices, int n, LineSegment& s);
	virtual int GetSegmentDirection(const LineSegment& s) const;
	virtual void PlotKnots(int seg_num) const;

	const double& GetSlope(int i) {	return GetPointSegment(i).m; }
};
} //namespace dml

#endif //__POLY_LINE_APROX_H__
