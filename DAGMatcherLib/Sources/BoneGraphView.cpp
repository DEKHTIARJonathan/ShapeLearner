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
 * $Log: BoneGraphView.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

// matching window parms
#define Y_OFFSET 10

using namespace dml;

BoneGraphView::BoneGraphView()
{
	m_pPlotView = NULL;

	m_pSelectedBGElements[0] = NULL;
	m_pSelectedBGElements[1] = NULL;

	m_pBGElementsToWarp[0] = NULL;
	m_pBGElementsToWarp[1] = NULL;

	m_bShowBGSpokes = false;
	m_bDrawCorners = false;
	m_bUseMinCorners = false;
	m_bShowBGAxis = false;
	m_bShowGapFunctions = false;

	m_nWarpIndex = -1;
	m_nCurrentWarpIndex = m_nWarpIndex;

	m_lastCornerPtShown.set(-1, -1);

	// Add colors to the color array
	m_colors.Add(0, 0, 0);       // black
	m_colors.Add(255, 157, 159); // pastel X
	m_colors.Add(255, 113, 81);  // pastel orange
	m_colors.Add(157, 255, 178); // pastel green
	m_colors.Add(178, 168, 255); // pastel purple
	m_colors.Add(157, 240, 255); // pastel cyan
	m_colors.Add(255, 213, 136); // "sand" yellow

	m_colors.Add(204, 102, 51);  // brown

	m_colors.Add(255, 0, 0);     // red
	m_colors.Add(0, 255, 0);     // green
	m_colors.Add(0, 0, 255);     // blue

	m_colors.Add(255, 113, 81);  // orange 1
	m_colors.Add(113, 255, 81);  // ?
	m_colors.Add(81, 113, 255);  // ?

	m_colors.Add(146, 0, 63);    // ?
	m_colors.Add(0, 146, 63);    // green  2
	m_colors.Add(0, 63, 146);    // ?

	m_colors.Add(255, 128, 0);   // orange 2
	m_colors.Add(0, 128, 255);   // ?
	m_colors.Add(128, 255, 0);   // ?

	m_selectionColors.Add(255, 128, 0); // orange
	m_selectionColors.Add(0, 146, 63);  // green
}

bool BoneGraphView::OnKeyPress(int val)
{
	if (DAGView::OnKeyPress(val))
		return true;

	switch (val)
	{
		case 'i':
			m_bShowGapFunctions = !m_bShowGapFunctions;
			return true;
		case 's':
			m_bShowBGSpokes = !m_bShowBGSpokes;
			return true;
		case 'v':
		case 'w':
			m_bDrawCorners = !m_bDrawCorners;
			m_bUseMinCorners = (val == 'v');

			if (m_bDrawCorners && !m_bShowEdgeInfo)
				m_bShowEdgeInfo = true; // must be switched on

			return true;
		case 'x':
			m_bShowBGAxis = !m_bShowBGAxis;
			return true;
		case '#':
			m_nWarpIndex++;

			if (m_nWarpIndex > 2)
				m_nWarpIndex = -1;

			return true;
	};

	return false;
}

void BoneGraphView::ShowWindowCommandHelp() const
{
	DAGView::ShowWindowCommandHelp();

	std::cout << "\n Bone Graph Specific Commands:\n"
		"\t i\tShow Interpolated boundary and skeletal functions\n"
		"\t s\tShow Spokes from bone points to boundary points\n"
		"\t v\tShow concaVe corners (short arms)\n"
		"\t w\tShow concaVe corners (long arms)\n"
		"\t x\tShow bone graph aXis\n" << std::endl;
}

void BoneGraphView::OnMouseMove(const leda::point& mousePt, const ImageInfo* ii)
{
	if (m_bShowPointData)
	{
		ShockPointInfo spi;

		if (FindClosestShockPoint(mousePt.xcoord(), mousePt.ycoord(), ii, &spi) < 2
			&& spi.sp.pt != m_pWnd->m_lastSkelPtShown)
		{
			m_pWnd->m_lastSkelPtShown = spi.sp.pt;

			std::cout << "\nBone Point: " << spi.sp.pt
				<< "\nBoundary info: " << spi.sp.bndryInfo
				<< std::endl;
		}
	}

	// If corners are drawn, check whether the mouse is hovering over one of them
	// If it's so, display the info associated with the corner
	if (m_bDrawCorners)
	{
		SmartArray<BoundaryCorner> corners;
		sg::Point cornerPt;
		leda::edge e;
		int i;

		forall_edges(e, *ii->pDAG)
		{
			const BGEdge* pEdge = dynamic_cast<const BGEdge*>((const DAGEdge*)ii->pDAG->GetEdge(e));
			ASSERT(pEdge);

			corners = pEdge->GetBoundaryCorners();

			for (i = 0; i < corners.Size(); i++)
			{
				if (corners[i].IsValid())
				{
					cornerPt = corners[i].m_pt;

					if (cornerPt.sqDist(mousePt.xcoord(), mousePt.ycoord()) < 4
						&& m_lastCornerPtShown != cornerPt)
					{
						m_lastCornerPtShown = cornerPt;

						std::cout << "\nCorner point: " << cornerPt <<
							", angle: " << acos(corners[i].m_cosAngle) * (180 / M_PI) <<
							", left arm: " << corners[i].m_leftArm <<
							" (" << corners[i].m_leftArm.norm() << " pixels), " <<
							"right arm: " << corners[i].m_rightArm <<
							" (" << corners[i].m_rightArm.norm() << " pixels)" << std::endl;
					}
				}
			}
		}
	}
}

void BoneGraphView::OnMouseButtonRelease(int val, const leda::point& pt, const ImageInfo* ii)
{
	ShockPointInfo spi;
	const BGElement* pBGElem = NULL;
	double dist = FindClosestShockPoint(pt.xcoord(), pt.ycoord(), ii, &spi);
	bool bCloseEnough = (dist < 5 || dist <= spi.sp.radius);

	if (bCloseEnough && spi.parentNode != nil)
		pBGElem = GetBGNode(ii, spi.parentNode);
	else if (bCloseEnough && spi.parentEdge != nil)
		pBGElem = GetBGEdge(ii, spi.parentEdge);

	if (pBGElem)
	{
		int imageSide = (ii == GetImageInfo1()) ? 0 : 1;
		bool bReselected;

		// Select the set of points the image
		if (m_pSelectedBGElements[imageSide] != pBGElem)
		{
			m_pSelectedBGElements[imageSide] = pBGElem;
			bReselected = false;

			// Redraw BGView to highlight the selected bone/edge points
			Redraw();
		}
		else
		{
			bReselected = true;
		}

		bool bReverse = false;

		// Show plot window
		if (m_pPlotView == NULL)
		{
			ShockPoint maxSPVals = FindMaxShockPointValues();

			m_pPlotView = new PlotView(maxSPVals.axisDist, maxSPVals.radius,
				"Radius Function", m_selectionColors, this);
		}
		else if (bReselected)
		{
			bReverse = !m_pPlotView->GetReverseStatus(imageSide);
		}

		// Make sure that the plot window is visible
		if (!m_pPlotView->IsVisible())
			m_pPlotView->Display();

		// Set the revers status in the plot windows so that it knows
		m_pPlotView->SetReverseStatus(imageSide, bReverse);

		// Set the datapoints and radius function
		m_pPlotView->SetDataPoints(imageSide, pBGElem->GetVelocityRadiusArray(bReverse),
			pBGElem->GetRadiusFunction(bReverse));
	}
}

/*!
	@brief Draw skeletal point coordinates and labels.
*/
void BoneGraphView::DrawBGElementInfo(const BGElement* pBGElem, const ImageInfo* ii,
									  const char* szLabel, leda::color c,
									  int width /*= -1*/) const
{
	leda::point pt;
	leda::list<leda_point> pts;
	const int nPtCount = pBGElem->m_shocks.GetSize();

	// Check whether we are drawing any of the selected elements. If we do,
	// highligh it using its selection color and a thicker pen
	if (IsSelected(pBGElem, &c))
		width = 2;

	if (width != -1)
		width = m_pWnd->set_line_width(width);

	// Draw bone points
	for (int i = 0; i < nPtCount; i++)
	{
		const ShockPoint& bp = pBGElem->m_shocks[i];

		pt = ReMapPt(bp.pt, ii);

		pts.push(pt);

		if (m_bShowLabels && i == nPtCount / 2)
			m_pWnd->draw_text(pt, szLabel, c);

		if (m_bShowBGSpokes)
		{
			m_pWnd->draw_segment(pt, ReMapPt(bp.BndryPt1(), ii), leda::red);
			m_pWnd->draw_segment(pt, ReMapPt(bp.BndryPt2(), ii), leda::orange);
		}
	}

	if (m_bDrawContinuousSkeleton && pts.size() > 1)
	{
		// We need to duplicate the second point so that the first point
		// gets drawn
		pts.push_back(ReMapPt(pBGElem->m_shocks[1].pt, ii));
		m_pWnd->draw_polyline(pts, c);
	}
	else
	{
		leda::list_item it;

		forall_items(it, pts)
			m_pWnd->draw_pixel(pts[it], c);
	}

	if (m_bShowBGAxis)
		DrawPolyLine(pBGElem->m_axisFunction, ii); //DrawPolyBezier(pBGElem->m_axisFunction, ii);

	if (width != -1)
		m_pWnd->set_line_width(width);
}

/*!
	Draws the disks of each shock point
*/
void BoneGraphView::DrawShockDisks(const BGElement* pBGElem, const ImageInfo* ii,
								   leda::color disc_color) const
{
	double x1, y1;
	int nPtCount = pBGElem->m_shocks.GetSize();

	//leda::color disc_color = pBGElem->SaliencyColor();

	for (int i = 0; i < nPtCount; i++)
	{
		const ShockPoint& bp = pBGElem->m_shocks[i];

		x1 = bp.pt.x - ii->delta_x;
		y1 = (ii->reflex_y - bp.pt.y) - ii->delta_y;

		m_pWnd->draw_disc(x1, y1, bp.radius, disc_color);
	}
}

void BoneGraphView::DrawNodeOrderedInfo(const DAGNode* pDAGNode, const ImageInfo* ii) const
{
	const BGNode* pNode = dynamic_cast<const BGNode*>(pDAGNode);
	ASSERT(pNode);

	DrawShockDisks(pNode, ii, m_colors[pNode->GetLevel() % m_colors.Size()]);
}

void BoneGraphView::DrawNodeInfo(const DAGNode* pDAGNode, const ImageInfo* ii) const
{
	const BGNode* pNode = dynamic_cast<const BGNode*>(pDAGNode);
	ASSERT(pNode);

	NODE_LABEL lbl = pNode->GetLblForGraph(m_bShowNodeType);

	leda::color col;

	if (pNode->Type() == BGElement::ROOT_BONE)
	{
		col = (m_bUseColors) ? leda::yellow : leda::black;
	}
	else if (pNode->Type() == BGElement::CORNER_BONE)
	{
		col = (m_bUseColors) ? leda::red : leda::black;
	}
	else
	{
		col = leda::black;
	}

	DrawBGElementInfo(pNode, ii, lbl, col);
}

void BoneGraphView::DrawEdgeInfo(const DAGEdge* pDAGEdge, const ImageInfo* ii) const
{
	const BGEdge* pEdge = dynamic_cast<const BGEdge*>(pDAGEdge);
	ASSERT(pEdge);

	EDGE_LABEL lbl = pEdge->GetLblForGraph(false);

	SmartArray<BoundaryCorner> corners = pEdge->GetBoundaryCorners();
	leda::color c;
	int i, originalCornersCount = 0, mergedCornersCount = 0;

	// The first two corners are the original coners. The next corners
	// are the merged coners. Count valid ones only.
	for (i = 0; i < corners.Size(); i++)
	{
		if (corners[i].IsValid())
		{
			if (i < 2)
				originalCornersCount++;
			else
				mergedCornersCount++;
		}
	}

	if (mergedCornersCount > 0)
		c = leda::color(51, 204, 102); // green
	else if (originalCornersCount == 1)
		c = leda::color(102, 153, 255); // semi-ligature (blue) or 0, 204, 255 (sky blue)
	else if (originalCornersCount == 2)
		c = leda::red;                  // full ligature (red)
	else
		c = leda::yellow;

	/*if (m_bDrawDisks)
	{
		DrawShockDisks(pEdge, ii, pEdge->SaliencyColor());
	}*/

	DrawBGElementInfo(pEdge, ii, lbl, c);

	// If requested, draw the concave corners associated with each ligature segment
	if (m_bDrawCorners)
	{
		const double radius = 1;
		sg::Point p;
		leda::point wp;
		int w;

		for (i = 0; i < corners.Size(); i++)
		{
			if (corners[i].IsValid())
			{
				p = corners[i].m_pt;
				wp = ReMapPt(p, ii);

				m_pWnd->draw_circle(wp, radius, c);

				w = m_pWnd->set_line_width(2);

				if (m_bUseMinCorners)
				{
					m_pWnd->draw_segment(wp, ReMapPt(corners[i].m_minLeftArm + p, ii), c);
					m_pWnd->draw_segment(wp, ReMapPt(corners[i].m_minRightArm + p, ii), c);
				}
				else
				{
					m_pWnd->draw_segment(wp, ReMapPt(corners[i].m_leftArm + p, ii), c);
					m_pWnd->draw_segment(wp, ReMapPt(corners[i].m_rightArm + p, ii), c);
				}

				m_pWnd->set_line_width(w);
			}
		}
	}
}

void BoneGraphView::DrawMatchingLines() const
{
	if (!HasMatchMap())
		return;

	//leda::color color[] = {leda::yellow, , leda::grey1, leda::grey2, leda::grey3,
	//					  leda::black, , leda::red, leda::blue, leda::green};
	leda::color color[] = {leda::grey1, leda::grey2, leda::grey3, leda::black, leda::brown,
						  leda::pink, leda::red, leda::violet, leda::blue, leda::green};

	const int NCOLORS = sizeof(color) / sizeof(color[0]);

	int color_index = 0, c = leda::black;
	double arc_x, arc_y;

	const BoneGraph* pBG1 = GetGraph(GetImageInfo1());
	const BoneGraph* pBG2 = GetGraph(GetImageInfo2());

	leda::node v;
	const BGNode* pNode1, *pNode2;
	char szSimVal[20];

	const char* szEdgeParam0[4] = {"", "FP", "FS", "FPS"};
	const char* szEdgeParam12[4] = {"", "*", "**"};
	const char* szEdgeParam34[4] = {"", "i", "ii"};

	forall_nodes(v, *pBG1)
	{
		if (m_matchMap[v].IsEmpty())
			continue;

		pNode1 = pBG1->GetBGNode(v);
		pNode2 = pBG2->GetBGNode(m_matchMap[v].node);

		if (pNode1->NumPoints() > 0 && pNode2->NumPoints() > 0)
		{
			const POINT& s1 = pNode1->m_shocks[pNode1->NumPoints() / 2].pt;
			const POINT& s2 = pNode2->m_shocks[pNode2->NumPoints() / 2].pt;

			// arc will go from s1 to s2, but will pass along the way through s3,
			//   where s3 is midway between s1 and s2 and up a fixed amount.
			leda::point start = ReMapPt(s1, GetImageInfo1());
			leda::point end   = ReMapPt(s2, GetImageInfo2());

			arc_x = (start.xcoord() + end.xcoord()) / 2;
			arc_y = (start.ycoord() + end.ycoord()) / 2 +
				fabs(start.xcoord() - end.xcoord()) * (Y_OFFSET / 100.0);

			if (arc_y >= (start.ycoord() + end.ycoord())/2 + Y_OFFSET)
			{
				arc_y = (start.ycoord() + end.ycoord())/2 + Y_OFFSET;
			}

			leda_point mid(arc_x, arc_y);

			if (m_bUseColors)
			{
				ASSERT_UNIT_INTERVAL(m_matchMap[v].similarity);

				c = (int)ceil(m_matchMap[v].similarity * NCOLORS - 1);

				if (c < 0)
					c = 0;
			}

			if (m_nShowColorLines == 0 || (m_nShowColorLines - 1) == c)
			{
				m_pWnd->draw_arc(start, mid, end, color[c]);

				bool bHasParams = (m_matchMap[v].params.size() > 0);

				if (m_nShowColorLines != 0)
				{
					sprintf(szSimVal, "%.3f (%d)", m_matchMap[v].similarity,
						(bHasParams) ? m_matchMap[v].params[0] : -1);

					m_pWnd->draw_ctext(mid, szSimVal, color[c]);
				}
				else if (bHasParams)
				{
					int flip, skip, invert;

					flip = m_matchMap[v].params[0];

					ASSERT(flip >= 0 && flip <= 3);

					if (m_matchMap[v].params.size() >= 5)
					{
						skip   = m_matchMap[v].params[1] + m_matchMap[v].params[2];
						invert = m_matchMap[v].params[3] + m_matchMap[v].params[4];

						ASSERT(skip >= 0 && skip <= 2);
						ASSERT(invert >= 0 && invert <= 2);
					}
					else
					{
						ShowError("not enouph matching params");
						skip   = 0;
						invert = 0;
					}

					sprintf(szSimVal, "%s %s %s", szEdgeParam0[flip],
						szEdgeParam12[skip], szEdgeParam34[invert]);

					m_pWnd->draw_ctext(mid, szSimVal, color[c]);
				}
			}
		}
	}
}

/*!
	@brief Draw BG specific attributes after all the generic
	DAG attributes and the node and edges of the BG are drawn.
*/
void BoneGraphView::DrawDAGInfo(const ImageInfo* ii) const
{
	// Call base funcion first (the one with param 'ii')
	DAGView::DrawDAGInfo(ii);

	if (m_bShowGapFunctions)
	{
		const BoneGraph* pBG = dynamic_cast<const BoneGraph*>(ii->pDAG);

		DrawPolyBezier(pBG->m_skeletalGaps, ii);

		// Draw bone points
		int i, j;
		leda::point pt;
		//leda::color colors[3] = {leda::red, leda::green, leda::blue};//{leda::black, leda::blue, leda::brown};
		leda::color c = leda::red;

		for (i = 0; i < pBG->m_boundaryGaps.Size(); i++)
		{
			if (m_nShowPartID > 0 && m_nShowPartID - 1 != i)
				continue;

			//c = colors[i % 3];

			for (j = 0; j < pBG->m_boundaryGaps[i].Size(); j++)
			{
				pt = ReMapPt(pBG->m_boundaryGaps[i][j], ii);
				m_pWnd->draw_pixel(pt, c);

				//m_pWnd->draw_disc(pt, 1, colors[j % 3]);
			}
		}
	}

	if (m_bDrawDisks)
	{
		double area;//, xnum = 0, x_den = 0, y_num = 0, y_den = 0;
		leda::node v;
		int nPtCount;
		const BGNode* pNode;
		POINT shapeNum(0,0);
		double shapeDen = 0;

		forall_nodes(v, *ii->pDAG)
		{
			POINT num(0,0);
			double den = 0;

			pNode = GetBGNode(ii, v);
			nPtCount = pNode->m_shocks.GetSize();

			for (int i = 0; i < nPtCount; i++)
			{
				const ShockPoint& bp = pNode->m_shocks[i];

				area = (M_PI * bp.radius * bp.radius);

				num += bp.pt * area;
				den += area;
				shapeNum += bp.pt * area;
				shapeDen += area;
			}

			if (den > 0)
			{
				leda::point pt = ReMapPt(num / den, ii);
				m_pWnd->draw_disc(pt, 3, leda::blue);
			}
		}

		if (shapeDen > 0)
		{
			leda::point pt = ReMapPt(shapeNum / shapeDen, ii);
			m_pWnd->draw_disc(pt, 3, leda::red);
		}
	}
}

/*!
	@brief Searches for the closest point to (x,y) in the given array of shock points
*/
void BoneGraphView::FindClosestShockPoint(const double& x, const double& y,
										  const ShockPoints& pts,
										  double* pMinSqDist,
										  ShockPoint* pClosestPt) const
{
	double sqDist;
	int closestPtIdx = -1;

	ASSERT(!pts.IsEmpty());

	for (int i = 0; i < pts.Size(); i++)
	{
		sqDist = pts[i].pt.SqDist(x, y);

		if (i == 0 || sqDist < *pMinSqDist)
		{
			*pMinSqDist = sqDist;
			closestPtIdx = i;
		}
	}

	*pClosestPt = pts[closestPtIdx];
}

/*!
	@brief Searches for the closest point to (x,y) among all shock points
	in every bone none and bone edge (if visible)
*/
double BoneGraphView::FindClosestShockPoint(const double& x, const double& y,
											const ImageInfo* ii,
											ShockPointInfo* pSPI) const
{
	double sqDist, minSqDist = -1000;
	ShockPoint sp;
	leda::node v;

	pSPI->parentNode = nil;
	pSPI->parentEdge = nil;

	forall_nodes(v, *ii->pDAG)
	{
		const BGNode* pNode = GetBGNode(ii, v);

		if (!pNode->m_shocks.IsEmpty())
		{
			FindClosestShockPoint(x, y, pNode->m_shocks, &sqDist, &sp);

			if (minSqDist < 0 || sqDist < minSqDist)
			{
				minSqDist = sqDist;
				pSPI->sp = sp;
				pSPI->parentNode = v;
			}
		}
	}

	if (m_bShowEdgeInfo)
	{
		leda::edge e;

		forall_edges(e, *ii->pDAG)
		{
			const BGEdge* pEdge = GetBGEdge(ii, e);

			if (!pEdge->m_shocks.IsEmpty())
			{
				FindClosestShockPoint(x, y, pEdge->m_shocks, &sqDist, &sp);

				if (minSqDist < 0 || sqDist < minSqDist)
				{
					minSqDist = sqDist;
					pSPI->sp = sp;
					pSPI->parentEdge = e;
					pSPI->parentNode = nil; // min it's not on a node
				}
			}
		}
	}

	return (minSqDist >= 0) ? sqrt(minSqDist):-minSqDist;
}

/*!
	@brief
*/
ShockPoint BoneGraphView::FindMaxShockPointValues() const
{
	ShockPoint sp;
	leda::node v;
	int i;

	sp.axisDist = 0;
	sp.radius = 0;

	const ImageInfo* iiArray[] = {GetImageInfo1(), GetImageInfo2()};

	for (int iiIdx = 0; iiIdx < 2; iiIdx++)
	{
		const ImageInfo* ii = iiArray[iiIdx];

		forall_nodes(v, *ii->pDAG)
		{
			const BGNode* pNode = GetBGNode(ii, v);
			const ShockPoints& pts = pNode->GetShockPoints();

			for (i = 0; i < pts.Size(); i++)
			{
				if (pts[i].axisDist > sp.axisDist)
					sp.axisDist = pts[i].axisDist;

				if (pts[i].radius > sp.radius)
					sp.radius = pts[i].radius;
			}
		}

		leda::edge e;

		forall_edges(e, *ii->pDAG)
		{
			const BGEdge* pEdge = GetBGEdge(ii, e);
			const ShockPoints& pts = pEdge->GetShockPoints();

			for (i = 0; i < pts.Size(); i++)
			{
				if (pts[i].axisDist > sp.axisDist)
					sp.axisDist = pts[i].axisDist;

				if (pts[i].radius > sp.radius)
					sp.radius = pts[i].radius;
			}
		}
	}

	return sp;
}

/*!
	@brief
*/
void BoneGraphView::WarpBGElements(const BGElement* pElem0, const BGElement* pElem1)
{
	ASSERT(pElem0 != NULL && pElem0 != NULL);

	// Save the pointers of the elements being warped
	m_pBGElementsToWarp[0] = pElem0;
	m_pBGElementsToWarp[1] = pElem1;
	m_nCurrentWarpIndex = m_nWarpIndex;

	// Copy the 3 point coordinates contained in each shock point
	const int numShockPts = 2;
	const int numPtsPerShockPt = 3;
	const int numPts = numShockPts * numPtsPerShockPt;

	int ptIdx0[numShockPts] = {0, pElem0->NumPoints() - 1};
	int ptIdx1[numShockPts] = {0, pElem1->NumPoints() - 1};

	//POINT srcPts[numPts], tgtPts[numPts];
	int i, j, shockPtSubIdx;

	m_srcPts.Resize(numPts);
	m_tgtPts.Resize(numPts);

	const bool bFlipAll = (m_nWarpIndex == numShockPts);
	const int subIdxComp = numPtsPerShockPt - 1;
	int subIdx0;
	bool bFlip;

	for (i = 0, j = 0; i < numShockPts; i++)
	{
		bFlip = (i == m_nWarpIndex || bFlipAll);

		for (shockPtSubIdx = 0; shockPtSubIdx < numPtsPerShockPt; shockPtSubIdx++)
		{
			subIdx0 = (bFlip) ? subIdxComp - shockPtSubIdx : shockPtSubIdx;

			pElem0->CopyPoint(ptIdx0[i], subIdx0, &m_srcPts[j]);
			pElem1->CopyPoint(ptIdx1[i], shockPtSubIdx, &m_tgtPts[j]);
			j++;
		}
	}

	// Form the m x n matrix A
	const int m = numPts * 2; // two rows per point
	const int n = 9;          // the number of columns os always 9

	NEWMAT::Matrix A(m, n);

	for (j = 0, i = 0; j < numPts; j++)
	{
		const POINT& p = m_srcPts[j];
		const POINT& q = m_tgtPts[j];

		//DBG_PRINT2(p, q)

		A.Row(++i) << p.x << p.y << 1 << 0 << 0 << 0
			<< -q.x * p.x << -q.x * p.y << -q.x;

		A.Row(++i) << 0 << 0 << 0 << p.x << p.y << 1
			<< -q.y * p.x << -q.y * p.y << -q.y;
	}

	// Compute the singular value decomposition of A
	NEWMAT::Matrix U(m, n), V(n, n);
	NEWMAT::DiagonalMatrix D(n);

	NEWMAT::SVD(A, D, U, V);

	// The column of V corresponding to the smallest singular value in D will be the
	// solution to H * P = 0. With 8 degrees of freedom, the 9th column should be
	// zero. However, due to noise H * P will likely be only close to zero.
	// Also, reshape the solution vector V(:,9) to get the homography matrix.
	NEWMAT::Matrix H = V.Column(9).AsMatrix(3, 3);

	// Normalize H using the fact that H(3,3) should be equal to 1
	H /= H(3, 3);

	//DBG_LINE
	//DBG_PRINT1(H)

	// Create a new BGElement with the warped points
	m_ptrWarpedBGElement = new BGElement(*m_pBGElementsToWarp[0]);

	//m_ptrWarpedBGElement->WarpShockPoints(H);
}

/*!
	@brief
*/
void BoneGraphView::DrawWarpedShape()
{
	// Make sure that there are two elements selected
	if (m_pSelectedBGElements[0] == NULL || m_pSelectedBGElements[1] == NULL)
		return;

	return;

	// If the selected elements are different from the warped ones, a (re-)warp is needed
	if (m_pSelectedBGElements[0] != m_pBGElementsToWarp[0] ||
		m_pSelectedBGElements[1] != m_pBGElementsToWarp[1] ||
		m_nCurrentWarpIndex != m_nWarpIndex)
	{
		// Reset the warp index if what changed are the actual points
		if (m_nCurrentWarpIndex == m_nWarpIndex)
			m_nWarpIndex = -1;

		WarpBGElements(m_pSelectedBGElements[0], m_pSelectedBGElements[1]);
	}

	//DrawBGElementInfo(m_ptrWarpedBGElement, GetImageInfo2(), "", leda::violet, 2);

	double mid_x, mid_y, y_offset;
	leda::point start, end;

	for (int i = 0; i < m_srcPts.Size(); i++)
	{
		start = ReMapPt(m_srcPts[i], GetImageInfo1());
		end   = ReMapPt(m_tgtPts[i], GetImageInfo2());

		m_pWnd->draw_disc(start, 1, leda::blue);
		m_pWnd->draw_disc(end, 1, leda::red);

		mid_x = (start.xcoord() + end.xcoord()) / 2;
		mid_y = (start.ycoord() + end.ycoord()) / 2;
		y_offset = fabs(start.xcoord() - end.xcoord()) * (Y_OFFSET / 100.0);

		//if (mid_y + y_offset < mid_y + Y_OFFSET)
			mid_y += y_offset;
		//else
		//	mid_y += Y_OFFSET;

		m_pWnd->draw_arc(start, leda::point(mid_x, mid_y), end, leda::blue);
	}
}