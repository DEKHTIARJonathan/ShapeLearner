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
 * $Log: PolyLineTLSApprox.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __POLY_LINE_TLS_APPROX_H__
#define __POLY_LINE_TLS_APPROX_H__

#include "stdafx.h"

namespace dml {
/*!
	The approx poly class that computes the least squares fit as a
	total least squares problem (i.e., perpendicular distances from
	the points to the fitting line).
*/
class PolyLineTLSApprox : public PiecewiseApprox<EuclideanLineSegment>
{
public:
	PolyLineTLSApprox(double dMinError, int nMaxSegments)
		: PiecewiseApprox<EuclideanLineSegment>(dMinError, nMaxSegments)
	{
		/* nothing else to do*/
	}

	virtual double LeastSquares(const POINT* vertices, int n, EuclideanLineSegment& s);
	virtual int GetSegmentDirection(const EuclideanLineSegment& s) const { return 0; }
	virtual void PlotKnots(int seg_num) const { /*Must write code here*/ };
};
} //namespace dml

#endif //__POLY_LINE_TLS_APPROX_H__
