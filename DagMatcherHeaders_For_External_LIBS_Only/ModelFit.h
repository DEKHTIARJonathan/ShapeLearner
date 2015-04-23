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
 * $Log: ModelFit.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __MODEL_FIT_H__
#define __MODEL_FIT_H__

#include "PolyLineApprox.h"

namespace dml {
typedef SmartMatrix< SmartMatrix<MEMDATA> > MEMORY2;

struct MINMAX
{
	int min, max;
	MINMAX(int mi, int ma) { min = mi; max = ma; }
};

class ModelFit
{
	MEMORY2 m_minerrors;
	LineSegmentArray m_segments;
	POINTS m_points;
	SmartArray<double> m_minLineLen;
	SmartArray<double> m_minDataLen;
	double m_minLenCoeff, m_totalMinLineLen, m_totalMaxLineLen, m_totalDataLen;

public:
	ModelFit(double minLenCoeff) { m_minLenCoeff = minLenCoeff; }

	double Fit(const POINTS& vertices, const LineSegmentArray& segs);
	MEMDATA Min(int ls, int le, int ps, int pe);
	void UpdateSegments(int ls, int le, int ps, int pe);
	double CompLSError(int ls, int ps, int pe) const;
	MINMAX GetMinMaxLen(int ls, int li, int le, int ps, int pe) const;
	void GetSegmentError(int s, double* error, double* scaleFactor) const;
	void Plot(std::ostream& os, const POINTS& points2, const LineSegmentArray& segments2) const;
};
} //namespace dml

#endif //__MODEL_FIT_H__
