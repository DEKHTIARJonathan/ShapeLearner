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
 * $Log: PolyLineWarper.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __POLY_LINE_WARPER_H__
#define __POLY_LINE_WARPER_H__

#include "stdafx.h"

namespace dml {
typedef SmartArray< SmartMatrix<MEMDATA> > MEMORY1;

/*struct MINMAX
{
	int min, max;
	MINMAX(int mi, int ma) { min = mi; max = ma; }
};*/

class PolyLineWarper
{
	double m_scale;
	double m_step;

	/*MEMORY1 m_minerrors;
	LineSegmentArray m_segments;
	POINTS m_points;
	SmartArray<double> m_minLineLen;
	SmartArray<double> m_minDataLen;
	double m_minLenCoeff, m_totalMinLineLen, m_totalMaxLineLen, m_totalDataLen;*/

public:

	PolyLineWarper() { }

	double Warp(const LineSegmentArray& src, const LineSegmentArray& tgt,
		const POINTS& tgtPts);
};
} //namespace dml

#endif //__POLY_LINE_WARPER_H__
