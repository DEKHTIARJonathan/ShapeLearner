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
 * $Log: PolyBezierApprox.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __POLY_BEZIER_APROX_H__
#define __POLY_BEZIER_APROX_H__

#include "stdafx.h"

namespace dml {
class PolyBezierApprox : public PiecewiseApprox<CubicBezier>
{
	int m_nMaxRetry;
	double* m_curvePoints;

public:
	PolyBezierApprox(double dMinError, int nMaxSegments, int nMaxRetry = 1);
	~PolyBezierApprox() { delete[] m_curvePoints; }

	virtual double LeastSquares(const POINT* vertices, int n, CubicBezier& s);
	virtual int GetSegmentDirection(const CubicBezier& s) const { return 0; }
	virtual void PlotKnots(int seg_num) const { /*Must write code here*/ };

	void Fit(const POINTS vertices);

	//! Returns the tanget of the point on the curve closest to input point i in constant time
	POINT GetTangent(int i) const
	{
		ASSERT(m_curvePoints);

		CubicBezierParams cbp(GetPointSegment(i));

		return cbp.GetTangent(m_curvePoints[i]);
	}

	/*!
		@brief Returns the curve "segment" parameter 'u' corresponding to the point,
		on "some" curve segment, that is the closest, between ALL curve points, to
		the input point i. It takes constant time. To access the actual point coordinates,
		the curve segment associated with paramater 'u' must be known.

		@See GetPointSegment().
	*/
	double GetCurveParam(int i) const
	{
		ASSERT(m_curvePoints);
		return m_curvePoints[i];
	}

	 // The following functions depend on the current values of shared variables
protected:
	void GetDistanceFromPointToCurvePoint(const POINT& pt, const double& u,
		const CubicBezierParams& cbp, double* s, double* z);

	double GetTotalDistance(const POINT* rawdata, int n, const CubicBezier& cbs,
		double* curvePoints = NULL);
};
} //namespace dml

#endif //__POLY_BEZIER_APROX_H__
