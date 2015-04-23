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
 * Author: Diego Macrini (dmac@cs.toronto.edu)
 *
 * $Log: PlotView.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef _PLOT_VIEW_H_
#define _PLOT_VIEW_H_

#include "stdafx.h"

//#include <LEDA/graphics/window.h>
//#include "PiecewiseApprox.h"

namespace dml {
/*!
	@brief Window object to display plots of scattered points.
*/
class PlotView : public leda::window
{
	enum MARGINS {LEFT_MARGIN = 15, RIGHT_MARGIN = 15, BOTTOM_MARGIN = 15,
		TOP_MARGIN = 15, LBL_MARGIN = 2};

	DAGView* m_pParent;
	bool m_bVisible;

	double m_xmax;
	double m_ymax;
	char m_szXMaxLabel[50];
	char m_szYMaxLabel[50];

	POINTS m_datapts[2];                //!< Datapoints to display
	LineSegmentArray m_pwlFunctions[2]; //!< Piece-wise linear functions
	ColorArray m_colors;

	bool m_bIsFunctionReversed[2];

	char m_szCaption[100];

	static void OnDrawCallback(leda::window* pWnd);
	static void OnDeleteCallback(leda::window* pWnd);

	void OnDraw();

	leda::point ReMapPt(const double& x, const double& y)
	{
		//return leda::point(real_to_pix(x), real_to_pix(y));
		return leda::point(x, y);
	}

	leda::point ReMapPt(const POINT& pt)
	{
		//return leda::point(real_to_pix(x), real_to_pix(y));
		return leda::point(pt.x, pt.y);
	}

	static double Width(const double& xmax, const double& ymax)
	{
		return (xmax + RIGHT_MARGIN + LEFT_MARGIN) * 6;
	}

	static double Hight(const double& xmax, const double& ymax)
	{
		return (ymax + TOP_MARGIN + BOTTOM_MARGIN) * 6;
	}

public:
	PlotView(const double& w, const double& h, const char* title,
		const ColorArray& colors, DAGView* pParent)
		: leda::window((int)Width(w, h), (int)Hight(w, h), title)
	{
		m_pParent = pParent;
		set_redraw(OnDrawCallback);
		set_window_delete_handler(OnDeleteCallback);

		//set_grid_style(leda::point_grid);
		//set_grid_dist(5);

		m_xmax = w;
		m_ymax = h;
		m_colors = colors;
		m_bVisible = false;

		sprintf(m_szXMaxLabel, "%.0f", m_xmax);
		sprintf(m_szYMaxLabel, "%.0f", m_ymax);

		m_bIsFunctionReversed[0] = false;
		m_bIsFunctionReversed[1] = false;

		*m_szCaption = '\0';

		init(-LEFT_MARGIN, m_xmax + RIGHT_MARGIN, -BOTTOM_MARGIN);
	}

	void Display()
	{
		display();
		m_bVisible = true;
	}

	void Close()
	{
		close();
		m_bVisible = false;
	}

	bool IsVisible() const { return m_bVisible; }

	bool GetReverseStatus(int idx) const
	{
		return m_bIsFunctionReversed[idx];
	}

	void SetReverseStatus(int idx, bool status)
	{
		m_bIsFunctionReversed[idx] = status;
	}

	void SetDataPoints(int idx, POINTS datapts, LineSegmentArray pwlFunc)
	{
		ASSERT(idx >= 0 && idx < 2);

		m_datapts[idx] = datapts;
		m_pwlFunctions[idx] = pwlFunc;

		if (!m_pwlFunctions[0].IsEmpty() && !m_pwlFunctions[1].IsEmpty())
		{
			XORIntegral xi(m_pwlFunctions[0], m_pwlFunctions[1]);

			double areaDiff = xi.XORArea();

			double area0 = xi.FunctionArea(0);
			double area1 = xi.FunctionArea(1);

			double areaSum = area0 + area1;

			double fsim = xi.FunctionSimilarity(DAGMatcher::GetMatchParams().dSlopeSigma);

			sprintf(m_szCaption, "[%d %d] fsim = %.2f, sim = %.2f, area diff = %.0f, "
				"area sum = %.0f, area0 = %.1f, area1 = %.1f",
				m_bIsFunctionReversed[0], m_bIsFunctionReversed[1], fsim,
				1 - areaDiff / areaSum, areaDiff, areaSum, area0, area1);
		}

		redraw();
	}
};
} // namespace dml

#endif //_PLOT_VIEW_H_
