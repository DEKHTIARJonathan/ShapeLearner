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
* $Log: ShockGraphView.cpp,v $
*
*-----------------------------------------------------------------------*/

#include "stdafx.h"

// matching window parms
#define Y_OFFSET 10

using namespace dml;

ShockGraphView::ShockGraphView()
{
	// Add colors to the color array
	m_colors.Add(0, 0, 0);       // black
	m_colors.Add(255, 0, 0);     // red
	m_colors.Add(255, 113, 81);  // orange
	m_colors.Add(0, 0, 255);     // blue
	m_colors.Add(204, 102, 51);  // brown

	/*m_lineColors.Add(222, 222, 221);
	m_lineColors.Add(194, 193, 193);
	m_lineColors.Add(170, 169, 169);
	m_lineColors.Add(150, 149, 148);
	m_lineColors.Add(131, 130, 129);
	m_lineColors.Add(114, 112, 111);
	m_lineColors.Add(96, 93, 92);
	m_lineColors.Add(77, 73, 72);
	m_lineColors.Add(56, 52, 49);
	m_lineColors.Add(0, 0, 0);*/

	/*m_lineColors.Add(150, 66, 66);
	m_lineColors.Add(171, 66, 66);
	m_lineColors.Add(194, 50, 50);
	m_lineColors.Add(222, 31, 31);
	m_lineColors.Add(252, 0, 0);*/

	/*m_lineColors.Add(59, 84, 148);
	m_lineColors.Add(59, 84, 184);
	m_lineColors.Add(59, 84, 184);
	m_lineColors.Add(59, 84, 209);
	m_lineColors.Add(59, 84, 255);*/

	m_lineColors.Add(148, 70, 70);
	m_lineColors.Add(184, 70, 70);
	m_lineColors.Add(200, 70, 70);
	m_lineColors.Add(209, 70, 70);
	m_lineColors.Add(255, 70, 70);

	m_lineColors.Add(70, 70, 148);
	m_lineColors.Add(70, 70, 184);
	m_lineColors.Add(70, 70, 184);
	m_lineColors.Add(70, 70, 209);
	m_lineColors.Add(70, 70, 255);
}

void ShockGraphView::OnMouseMove(const leda::point& pt, const ImageInfo* ii)
{
	if (m_bShowPointData)
	{
		sg::FluxPoint fp;
		sg::BoundaryInfo bi;

		double& lastX = m_pWnd->m_lastSkelPtShown.x;
		double& lastY = m_pWnd->m_lastSkelPtShown.y;

		if (FindClosestSkeletonPoint(pt.xcoord(), pt.ycoord(), ii, fp, bi) < 2
			&& (fp.p.x != lastX || fp.p.y != lastY))
		{
			lastX = fp.p.x;
			lastY = fp.p.y;

			std::cout << "\nFlux Point: " << fp
				<< "\nBoundary Info: " << bi << std::endl;
		}
	}
}

void ShockGraphView::DrawNodeInfo(const DAGNode* pDAGNode, const ImageInfo* ii) const
{
	const SGNode* pNode = dynamic_cast<const SGNode*>(pDAGNode);
	ASSERT(pNode);

	// Choose a color for the shock points
	leda::color c = (m_bUseColors) ?
		m_colors[pDAGNode->GetLevel() % m_colors.Size()] : leda::black;

	char cEndPt = pNode->GetEndPt();

	int t = pNode->m_nType;

	if (t != SHOCK_1 && t != SHOCK_2 && t != SHOCK_3 && t != SHOCK_4)
	{
		if (t != ROOT) // There's no info in the root
			std::cerr << "\nERROR: Unknown node type." << std::endl;

		return;
	}

	int size = pNode->m_shocks.GetSize();

	if (size == 0)
	{
		WARNING(pNode->m_shocks.GetSize() == 0, "Node has no shocks");
		return;
	}

	double x1, y1;

	for (int j = 0; j < size; j++)
	{
		const ShockInfo& s1 = pNode->m_shocks[j];

		x1 = s1.xcoord - ii->delta_x;
		y1 = (ii->reflex_y - s1.ycoord) - ii->delta_y;

		m_pWnd->draw_pixel(x1, y1, c);

		if (m_bShowLabels)
		{
			if ((cEndPt == 'a' && j == 0) || (cEndPt == 'b' && j == 0) ||
				(cEndPt != 'a' && cEndPt != 'b' && j == size / 2))
			{
				m_pWnd->draw_text(x1, y1, (const char*)pNode->GetLblForGraph(m_bShowNodeType), c);
			}
		}
	}
}

void ShockGraphView::DrawMatchingLines() const
{
	if (!HasMatchMap())
		return;

	ASSERT(m_lineColors.Size() == 10);

	const int MAX_COLOR = m_lineColors.Size() - 1;
	int color_index = 0, c = leda::black;
	double arc_x,arc_y;

	const ShockGraph* pBG1 = GetGraph(GetImageInfo1());
	const ShockGraph* pBG2 = GetGraph(GetImageInfo2());

	leda::node v;
	const SGNode* pNode1, *pNode2;

	forall_nodes(v, *pBG1)
	{
		if (m_matchMap[v].IsEmpty())
			continue;

		pNode1 = pBG1->GetSGNode(v);
		pNode2 = pBG2->GetSGNode(m_matchMap[v].node);

		if (pNode1->GetShockCount() > 0 && pNode2->GetShockCount() > 0)
		{
			const ShockInfo& s1 = pNode1->m_shocks[pNode1->GetShockCount()/2];
			const ShockInfo& s2 = pNode2->m_shocks[pNode2->GetShockCount()/2];

			/* arc will go from s1 to s2, but will pass along the way through s3,
			where s3 is midway between s1 and s2 and up a fixed amount. */
			leda_point start(s1.xcoord - GetImageInfo1()->delta_x,
				(GetImageInfo1()->reflex_y - s1.ycoord) - GetImageInfo1()->delta_y);
			leda_point end(s2.xcoord - GetImageInfo2()->delta_x,
				(GetImageInfo2()->reflex_y - s2.ycoord) - GetImageInfo2()->delta_y);

			arc_x = (start.xcoord() + end.xcoord())/2;
			arc_y = (start.ycoord() + end.ycoord())/2
				+ fabs(start.xcoord() - end.xcoord()) * (Y_OFFSET / 100.0);

			if (arc_y >= (start.ycoord() + end.ycoord())/2 + Y_OFFSET)
			{
				arc_y = (start.ycoord() + end.ycoord())/2 + Y_OFFSET;
			}

			leda_point mid(arc_x, arc_y);

			if (m_bUseColors)
			{
				c = (int)ceil(m_matchMap[v].similarity * MAX_COLOR);
				if (c < 0) c = 0;
			}

			if (m_nShowColorLines == 0 || m_nShowColorLines == c)
				m_pWnd->draw_arc(start, mid, end, m_lineColors[c]);
		}
	}
}

/*!
@brief Looks for a skeleton point in a brute force way (ie, sequentially)
*/
double ShockGraphView::FindClosestSkeletonPoint(const double& x, const double& y,
												const ImageInfo* ii,
												sg::FluxPoint& fp,
												sg::BoundaryInfo& bi) const
{
	double dist, dMinDist = -1000;

	if (ii->pDAG->GetSkeleton() != NULL)
	{
		const sg::DDSGraph* sk = ii->pDAG->GetSkeleton()->GetDDSGraph();
		const sg::DDSEdgeVect& edges = sk->getEdges();

		sg::DDSEdgeVect::const_iterator I;
		sg::FluxPointList::const_iterator II;
		sg::BoundaryInfoList::const_iterator III;

		for(I = edges.begin(); I != edges.end(); I++)
		{
			const sg::FluxPointList& fpl    = (*I)->getFluxPoints();
			const sg::BoundaryInfoList& bil = (*I)->getBoundaryInfoList();

			for (II = fpl.begin(), III = bil.begin(); III != bil.end(); II++, III++)
			{
				dist = (x - II->p.x) * (x - II->p.x) + (y - II->p.y) * (y - II->p.y);

				if (dMinDist < 0 || dist < dMinDist)
				{
					dMinDist = dist;
					fp = *II;
					bi = *III;
				}
			}
		}
	}

	return (dMinDist >= 0) ? sqrt(dMinDist):-dMinDist;
}