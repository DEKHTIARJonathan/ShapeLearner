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
 * $Log: PlotView.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"


using namespace dml;

/*!
	@brief Closes the window without terminating the program.

	Called when the window is destroy. The default behaviour
	is to terminate the program. Instead, we simply close the
	window.
*/
/*static*/ void PlotView::OnDeleteCallback(leda::window* pBaseWnd)
{
	PlotView* pView = dynamic_cast<PlotView*>(pBaseWnd);
	ASSERT(pView);

	pView->Close();
}

/*!
	@brief Handles the draw event by calling the class's OnDraw() function
*/
/*static*/ void PlotView::OnDrawCallback(leda::window* pBaseWnd)
{
	PlotView* pView = dynamic_cast<PlotView*>(pBaseWnd);
	ASSERT(pView);

	pView->start_buffering();

	pView->clear();
	pView->OnDraw();

	pView->flush_buffer();
	pView->stop_buffering();
}

/*!
	@brief Responds to a (re-)drawing event
*/
void PlotView::OnDraw()
{
	leda::point pt0, pt1;

	pt0 = ReMapPt(m_xmax / 2.0, m_ymax + 2 * LBL_MARGIN);
	draw_ctext(pt0, "Radius as a function of axis distance");

	// Draw caption
	draw_ctext(ReMapPt(m_xmax / 2.0, -3 * LBL_MARGIN), m_szCaption, leda::black);

	// Draw axes
	pt0 = ReMapPt(m_xmax, m_ymax);
	draw_segment(0, 0, pt0.xcoord(), 0, leda::black);
	draw_segment(0, 0, 0, pt0.ycoord(), leda::black);

	// Draw axis labels
	draw_ctext(ReMapPt(0, -LBL_MARGIN), "0", leda::black);
	draw_ctext(ReMapPt(m_xmax, -LBL_MARGIN), m_szXMaxLabel, leda::black);

	draw_ctext(ReMapPt(-LBL_MARGIN, 0), "0", leda::black);
	draw_ctext(ReMapPt(-LBL_MARGIN, m_ymax), m_szYMaxLabel, leda::black);

	// Draw the points in each dataset
	int i, j;

	ASSERT(m_colors.Size() == 2);

	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < m_datapts[i].Size(); j++)
		{
			pt0 = ReMapPt(m_datapts[i][j].x, m_datapts[i][j].y);
			draw_point(pt0, m_colors[i]);
		}

		for (j = 0; j < m_pwlFunctions[i].Size(); j++)
		{
			pt0 = ReMapPt(m_pwlFunctions[i][j].GetP0());
			pt1 = ReMapPt(m_pwlFunctions[i][j].GetP1());
			draw_segment(pt0, pt1, m_colors[i]);
		}
	}
}