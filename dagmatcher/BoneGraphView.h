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
 * $Log: BoneGraphView.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef _BONE_GRAPH_VIEW_H_
#define _BONE_GRAPH_VIEW_H_

#include "DAGView.h"
#include "BoneGraph.h"
#include "PlotView.h"

namespace dml {
/*!
	@brief This class groups all the operations needed to visualize a BoneGraph object.
*/
class BoneGraphView : public DAGView
{
	struct ShockPointInfo
	{
		ShockPoint sp;
		leda::node parentNode;
		leda::edge parentEdge;
	};

protected:
	bool        m_bShowBGSpokes;
	bool        m_bDrawCorners;
	bool        m_bUseMinCorners;
	bool        m_bShowBGAxis;
	bool        m_bShowGapFunctions;
	int         m_nWarpIndex;

	sg::Point m_lastCornerPtShown;

	PlotView* m_pPlotView;

	const BGElement* m_pSelectedBGElements[2];
	ColorArray m_selectionColors;

	const BGElement* m_pBGElementsToWarp[2];
	SmartPtr<BGElement> m_ptrWarpedBGElement;
	POINTS m_srcPts, m_tgtPts;
	int m_nCurrentWarpIndex;

public:
	BoneGraphView();

	void DrawBGElementInfo(const BGElement* pBGElem, const ImageInfo* ii,
		const char* szLabel, leda::color c, int width = -1) const;

	void DrawShockDisks(const BGElement* pBGElem, const ImageInfo* ii,
		leda::color disc_color) const;

	const BoneGraph* GetGraph(const ImageInfo* ii) const
	{
		return dynamic_cast<const BoneGraph*>(ii->pDAG);
	}

	const BGNode* GetBGNode(const ImageInfo* ii, leda::node v) const
	{
		return dynamic_cast<const BGNode*>((const DAGNode*)ii->pDAG->GetNode(v));
	}

	const BGEdge* GetBGEdge(const ImageInfo* ii, leda::edge e) const
	{
		return dynamic_cast<const BGEdge*>((const DAGEdge*)ii->pDAG->GetEdge(e));
	}

	void FindClosestShockPoint(const double& x, const double& y,
		const ShockPoints& pts, double* pMinSqDist, ShockPoint* pClosestPt) const;

	double FindClosestShockPoint(const double& x, const double& y,
		const ImageInfo* ii, ShockPointInfo* pSPI) const;

	ShockPoint FindMaxShockPointValues() const;

	bool IsSelected(const BGElement* pBGElem, leda::color* pColor) const
	{
		for (int i = 0; i < 2; i++)
		{
			if (pBGElem == m_pSelectedBGElements[i])
			{
				*pColor = m_selectionColors[i];
				return true;
			}
		}

		return false;
	}

	void WarpBGElements(const BGElement* pElem0, const BGElement* pElem1);

	virtual void DrawDAGInfo(const ImageInfo* ii) const;
	virtual void DrawNodeOrderedInfo(const DAGNode* pDAGNode, const ImageInfo* ii) const;
	virtual void DrawNodeInfo(const DAGNode* pDAGNode, const ImageInfo* ii) const;
	virtual void DrawEdgeInfo(const DAGEdge* pDAGEdge, const ImageInfo* ii) const;
	virtual void DrawMatchingLines() const;
	virtual void DrawWarpedShape();

	virtual bool OnKeyPress(int val);
	virtual void ShowWindowCommandHelp() const;
	virtual void OnMouseMove(const leda::point& pt, const ImageInfo* ii);
	virtual void OnMouseButtonRelease(int val, const leda::point& pt, const ImageInfo* ii);

	virtual void OnWindowClose(leda::window* pWnd)
	{
		if (m_pPlotView)
			m_pPlotView->close();

		DAGView::OnWindowClose(pWnd);
	}
};
} // namespace dml

#endif //_BONE_GRAPH_VIEW_H_
