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
 * $Log: DAGView.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include <stdio.h>
#include "VisualDAG.h"
#include "DAGView.h"

// DAGView window params
#define SW_LEFT_MARGIN    50
#define SW_RIGHT_MARGIN   50
#define SW_TOP_MARGIN     50
#define SW_BOTTOM_MARGIN  50
#define SW_CENTER_MARGIN  20
#define SW_WIN_WIDTH      800

//! Not used anymore. Defines the margin used when showing mouse coordinates
#define COORD_MARGIN 0

#define ENTER_KEY 1
#define ESC_KEY 2
#define SPACE_KEY 32
#define BACKSPACE_KEY 0

#define SHAPE_COLOR leda::color(195, 217, 255) //leda_grey1

using namespace std;
using namespace dml;
using namespace leda;

//////////////////////////////////////////////////////////////////////////////////////////////////
// DAGView

DAGView::DAGView()
{
	m_pWnd = NULL;

	m_bShowLabels = true;
	m_bShowNodeType = false;
	m_nShowColorLines = 0;
	m_nShowPartID = 0;
	m_bShowCoords = true;
	m_bShowLinApprox = false;
	m_bShowBezApprox = false;
	m_bShowSkeletalGraph = false;
	m_nShowRays = 0; // 0, 1, or 2
	m_bShowPointData = false;
	m_bDrawDisks = false;
	m_bShowEdgeInfo = false;
	m_bShowNodeInfo = true;
	m_bUseColors = true;
	m_bTopToBottomOrder = true;
	m_nShowPtCoords = 0;
	m_bDrawContinuousSkeleton = true;
	m_bShowContour = true;
	m_bKeyboardInputMode = false;
	m_bShowMatchingLines = true;
	m_nForegroundColor = 1;
	m_bDrawTable = false;

	//m_pImageInfo1 = m_pImageInfo2 = NULL;
	m_imageInfoMat.Clear();
}

DAGView::~DAGView()
{
	delete m_pWnd;
}

/*!
	@brief Selects the image that contains the given window coordinate.
*/
const DAGView::ImageInfo* DAGView::SelectImageInfo(const double& x, const double& y) const
{
	/*double centerPt = SW_LEFT_MARGIN + GetImageInfo1()->pDAG->Width()
		+ SW_CENTER_MARGIN / 2.0;

	return (x <= centerPt) ? GetImageInfo1() : GetImageInfo2();*/

	double left_offset = SW_LEFT_MARGIN;
	int col;

	ASSERT(m_imageInfoMat.Size() > 0);

	for (col = 0; col < m_imageInfoMat.NCols() - 1; col++)
	{
		left_offset += m_colWidths[col] + SW_CENTER_MARGIN / 2.0;

		if (x <= left_offset)
			break;
	}

	double top_offset = SW_TOP_MARGIN;
	int row;

	for (row = 0; row < m_imageInfoMat.NRows() - 1; row++)
	{
		top_offset += m_rowHeights[row] + SW_CENTER_MARGIN / 2.0;

		if (y <= top_offset)
			break;
	}

	return &m_imageInfoMat[row][col];
}

/*!
	@brief Creates a window to display the information stored in the DAG.

	In order to position the window on the top right, set x and y to -1.

	Trick: We reuse the same create function that shows two DAGs side by side,
	but make on of them empty.
*/
bool DAGView::Create(const DAG* pDAG, int x, int y)
{
	DAG* pDummyDAG = pDAG->CreateObject(); // creates empty object

	bool bRetVal = Create(pDAG, pDummyDAG, x, y);

	delete pDummyDAG;

	return bRetVal;
}

/*!
	@brief Creates a window to display the information stored in two DAG side by side.

	Call SetMatchMap() to assign and display node correspondeces between the two DAGs.
*/
bool DAGView::Create(const DAG* pDAG1, const DAG* pDAG2, int xpos, int ypos)
{
	SmartMatrix<const DAG*> dagMat(1, 2);

	dagMat[0][0] = pDAG1;
	dagMat[0][1] = pDAG2;

	return Create(dagMat, xpos, ypos);
}

/*!
	@brief Creates a window to display the information stored in two DAG side by side.

	In order to position the window on the top right, set xpos and ypos to -1.

	@return false if the 'Esc'-key-press is the last event and true otherwise.
*/
bool DAGView::Create(SmartMatrix<const DAG*> dagMat, int xpos, int ypos)
{
	int i, j;

	m_rowHeights.Resize(dagMat.NRows(), true);
	m_colWidths.Resize(dagMat.NCols(), true);

	// Find the max width and height of each row and column, respectively
	for (i = 0; i < dagMat.NRows(); i++)
	{
		for (j = 0; j < dagMat.NCols(); j++)
		{
			ASSERT(dagMat[i][j] != NULL);

			if (dagMat[i][j]->Width() > m_colWidths[j])
				m_colWidths[j] = dagMat[i][j]->Width();

			if (dagMat[i][j]->Height() > m_rowHeights[i])
				m_rowHeights[i] = dagMat[i][j]->Height();
		}
	}

	// Create the image info structure for each DAG
	double left_offset, bottom_offset;

	m_imageInfoMat.Resize(dagMat.NRows(), dagMat.NCols());

	bottom_offset = SW_BOTTOM_MARGIN;

	for (i = 0; i < dagMat.NRows(); i++)
	{
		left_offset = SW_LEFT_MARGIN;

		for (j = 0; j < dagMat.NCols(); j++)
		{
			// Get a reference to a CELL in the image info matrix
			ImageInfo& ii = m_imageInfoMat[i][j];

			ii.pDAG = dagMat[i][j];

			ii.delta_x  = ii.pDAG->xmin() - left_offset;
			ii.delta_y  = ii.pDAG->ymin() - bottom_offset;
			ii.reflex_y = ii.pDAG->ymax() + ii.pDAG->ymin();

			left_offset += m_colWidths[j] + SW_CENTER_MARGIN;
		}

		bottom_offset += m_rowHeights[i] + SW_CENTER_MARGIN;
	}

	double xmax = m_colWidths.Sum() + SW_LEFT_MARGIN + SW_RIGHT_MARGIN +
		(dagMat.NCols() - 1) * SW_CENTER_MARGIN;

	double ymax = m_rowHeights.Sum() + SW_BOTTOM_MARGIN + SW_TOP_MARGIN +
		(dagMat.NRows() - 1) * SW_CENTER_MARGIN;

	int pixelWidth = (xmax <= 1024) ? int(xmax) : 1024;
	int pixelHeight = (ymax <= 768) ? int(ymax) : 768;

	// Create a window of the desired size
	m_pWnd = new Wnd(pixelWidth, pixelHeight, MakeWinTitle(), this);

	m_pWnd->init(0, xmax, 0);

	//m_pWnd->set_show_coordinates(m_bShowCoords);

	return Display(xpos, ypos);
}

dml::String DAGView::MakeWinTitle()
{
	char szWinTitle[100];

	//char szWinTitle[100] = {"Press 'h' for help"};
	int vn11, vn12, vn21, vn22;

	if (m_imageInfoMat.NRows() == 1 && m_imageInfoMat.NCols() == 2)
	{
		const DAG* pDAG1 = m_imageInfoMat[0][0].pDAG;
		const DAG* pDAG2 = m_imageInfoMat[0][1].pDAG;

		if (pDAG1->Width() > 0 && pDAG2->Width() > 0)
		{
			pDAG1->GetViewNumbers(&vn11, &vn12);
			pDAG2->GetViewNumbers(&vn21, &vn22);

			sprintf(szWinTitle, "%s %d %d vs %s %d %d",
				(const char*)pDAG1->GetObjName(), vn11, vn12,
				(const char*)pDAG2->GetObjName(), vn21, vn22);
		}
		else if (pDAG1->Width() > 0)
		{
			pDAG1->GetViewNumbers(&vn11, &vn12);

			sprintf(szWinTitle, "%s %d %d",
				(const char*)pDAG1->GetObjName(), vn11, vn12);
		}
		else if (pDAG2->Width() > 0)
		{
			pDAG2->GetViewNumbers(&vn21, &vn22);

			sprintf(szWinTitle, "%s %d %d",
				(const char*)pDAG2->GetObjName(), vn21, vn22);
		}
		else
		{
			strcpy(szWinTitle, "Press 'h' for help");
		}
	}
	else
	{
		sprintf(szWinTitle, "Table of %d x %d shapes",
				m_imageInfoMat.NRows(), m_imageInfoMat.NCols());
	}

	return dml::String(szWinTitle);
}

/*!
	Displays the DAGView window
*/
bool DAGView::Display(int xpos, int ypos)
{
	ASSERT(m_pWnd);

	if (xpos >= 0 && ypos >= 0)
		m_pWnd->display(xpos, ypos);
	else
		m_pWnd->display();

	m_pWnd->redraw();

	int val = 0; // ie, not 'Esc' key value, which is 2
	double x, y;
	int winevent;
	bool bReadNextEvent = true;
	bool bRetVal = true;

	while (bReadNextEvent)
	{
		winevent = m_pWnd->read_event(val, x, y);

		if (winevent == leda::key_press_event)
		{
			//cout << "Key is " << val << endl;

			if (val >= 'A' && val <= 'Z')
				val -= 'A' - 'a';

			bReadNextEvent = OnKeyPress(val);

			if (bReadNextEvent && !m_bKeyboardInputMode)
				Redraw();
		}
		else if (winevent == leda::button_release_event)
		{
			const ImageInfo* ii = SelectImageInfo(x, y);
			leda::point pt = MapBackPt(x, y, ii);

			OnMouseButtonRelease(val, pt, ii);
		}
		else if (winevent == leda_exposure_event)
			m_pWnd->reset_clipping();
		else if (winevent == leda_destroy_event)
			break;
	}

	// If the last even was the 'Esc' key, return false
	return (winevent == leda::key_press_event && val == 2) ? false : true;
}

bool DAGView::OnKeyPress(int val)
{
	if (m_bKeyboardInputMode && val != ENTER_KEY && val != ESC_KEY)
	{
		if (isdigit(val) || val == SPACE_KEY || val == '.' || val == ',')
		{
			AppendToWindowText(leda::string((char)val));

			// Get rid of commas before inserting into key's buffer
			char keyVal = (val == ',') ? ' ' : (char)val;

			// Add key to the buffer
			m_streamInputedKeys << keyVal;
		}
		else if (val == BACKSPACE_KEY)
		{
			BackspaceWindowText();
			m_streamInputedKeys.seekp(-1, ios_base::end);
		}
		else
		{
			cout << char(7)
				<< "Invalid input key. Must be a number or a space." << endl;
		}

		return true;
	}

	// See if key is a valid non-digit character
	switch (val)
	{
		case 'a':
			VisualDAG::ShowGraph(*(DAG*)GetImageInfo1()->pDAG);
			return true; // @TODO clone graph
		case 'b':
			if (GetImageInfo2()->pDAG->GetNodeCount() > 0)
				VisualDAG::ShowGraph(*(DAG*)GetImageInfo2()->pDAG);

			return true; // @TODO clone graph
		case 'c':
			m_bShowCoords = !m_bShowCoords;
			return true;
		case 'd':
			m_bShowPointData = !m_bShowPointData;
			return true;
		case 'e':
			m_bShowEdgeInfo = !m_bShowEdgeInfo;
			return true;
		case 'f':
			m_nForegroundColor++;
			if (m_nForegroundColor > 2) m_nForegroundColor = 0;
			return true;
		case 'g':
			if (GetImageInfo2()->pDAG->GetNodeCount() > 0)
			{
				VisualDAG::ShowGraphs(*(DAG*)GetImageInfo1()->pDAG,
					*(DAG*)GetImageInfo2()->pDAG, m_matchMap, m_colors);
			}
			else
			{
				VisualDAG::ShowGraph(*(DAG*)GetImageInfo1()->pDAG);
			}

			return true;
		case 'h':
			ShowWindowCommandHelp();
			return true;
		case 'k':
			m_bDrawDisks = !m_bDrawDisks;
			return true;
		case 'l':
			m_bShowLabels = !m_bShowLabels;
			return true;
		case 'm':
			m_bShowMatchingLines = !m_bShowMatchingLines;
			return true;
		case 'n':
			m_bShowLinApprox = !m_bShowLinApprox;
			return true;
		case 'o':
			m_bUseColors = !m_bUseColors;
			return true;
		case 'p':
			m_bShowNodeInfo = !m_bShowNodeInfo;
			return true;
		case 'r':
			m_pWnd->reset_clipping();
			return true;
		case 't':
			m_bShowNodeType = !m_bShowNodeType;
			return true;
		case 'u':
			m_bDrawContinuousSkeleton = !m_bDrawContinuousSkeleton;
			return true;
		case 'y':
			m_nShowRays++; if (m_nShowRays > 2) m_nShowRays = 0;
			return true;
		case 'z':
			m_bShowBezApprox = !m_bShowBezApprox;
			return true;
		case '^':
			m_bTopToBottomOrder = !m_bTopToBottomOrder;
			return true;
		case '.':
			m_nShowPtCoords++; if (m_nShowPtCoords > 3) m_nShowPtCoords = 0;
			return true;
		case '-':
			m_bShowContour = !m_bShowContour;
			return true;
		case '*':
			m_bShowSkeletalGraph = !m_bShowSkeletalGraph;
			return true;
		case '#':
			m_bDrawTable = !m_bDrawTable;
			if (m_bDrawTable) SetBlackShapeMode();
			return true;
	}

	// See if it is the start or stop for keyboard input
	if (val == ENTER_KEY)
	{
		m_bKeyboardInputMode = !m_bKeyboardInputMode;

		if (m_bKeyboardInputMode)
		{
			ClearInputedKeys();
			SetWindowText(leda::string("> "));
		}
		else
			ProcessInputedKeys();

		return true;
	}

	// See if the key is a number
	if (isdigit(val))
	{
		if (IsDualView())
			m_nShowColorLines = val - '0';
		else if (m_nShowPartID >= 9 && val == '9')
			m_nShowPartID++; // keep moving up
		else
			m_nShowPartID = val - '0';

		return true;
	}

	return false; // unknown character. let the derived class handle this
}

void DAGView::ShowWindowCommandHelp() const
{
	cout << "\tKey\tCommand\n"
		    "\t---     -------------------------------------------------------\n"
		    "\t a\tShow only the graph of leftmost shape\n"
			"\t b\tShow only the graph of rightmost shape (if there is one)\n"
			"\t c\tShow Coordinaes\n"
			"\t d\tShow skeleton point Data (on terminal window)\n"
			"\t e\tShow Edge information\n"
			"\t f\tChanges the foreground color of shapes {none, l-blue, black}\n"
			"\t g\tShow all Graphs\n"
			"\t h\tShow this Help\n"
			"\t k\tDraw flux point disKs\n"
			"\t l\tShow node Labels\n"
			"\t m\tShow Matching lines between shape parts\n"
			"\t n\tShow liNear skeleton approximation\n"
			"\t o\tShow cOlors\n"
			"\t p\tShow 'processed' skeletal Points\n"
			"\t r\tRedraw window\n"
			"\t t\tShow node Types\n"
			"\t u\tToggles skeleton drawing {continUous, discrete}\n"
			"\t y\tShow shock point raYs (none, endpoints only, all)\n"
			"\t z\tShow cubic BeZier skeleton approximation\n"
			"\t ^\tToggles disk drawing order {top-to-bottom, bottom-to-top}\n"
			"\t .\tToggles show endpoint coordinates {none, junctions, terminals, all}\n"
			"\t -\tShow shape contour\n"
			"\t *\tShow 'original' skeletal points\n"
			"\t ENTER\tStart/stop reading coordinates from keyboard\n"
			"\t0-9\tShow node similarity = 9: 1--0.9, 8: 0.9--0.8, ... 0: all\n"
			"\t  \tAny other key closes the window\n" << endl;
}

/*static*/ void DAGView::Wnd::OnShowCoord(leda::window* pBaseWnd, double x, double y)
{
    Wnd* pWnd = dynamic_cast<Wnd*>(pBaseWnd);
    DAGView* pView = pWnd->GetDAGView();
	const ImageInfo* ii = pView->SelectImageInfo(x, y);

	leda::point pt = pView->MapBackPt(x, y, ii);

    if (pView->m_bShowCoords)
    {
    	char szTxt[100];
    	sprintf(szTxt, "%.1f %.1f", pt.xcoord(), pt.ycoord());

		double x0 = pWnd->xmin();
		double y0 = pWnd->ymax();

		leda::text_mode m = pWnd->set_text_mode(leda::opaque);

		pWnd->draw_text(x0 + COORD_MARGIN, y0 - COORD_MARGIN,
			"                           "); // clears old text with 'white' string

    	pWnd->draw_text(x0 + COORD_MARGIN, y0 - COORD_MARGIN, szTxt);

		pWnd->set_text_mode(m);
    }

	// Can virtual OnMouseMove() so that the derived classes have a chance to respond to the mouse
	pView->OnMouseMove(pt, ii);
}

/*!
	@brief Handles the window's draw event
*/
/*static*/ void DAGView::Wnd::OnDraw(leda::window* pBaseWnd)
{
	Wnd* pWnd = dynamic_cast<Wnd*>(pBaseWnd);
	ASSERT(pWnd);

	DAGView* pView = pWnd->GetDAGView();

	pView->DrawDAGInfo();

	if (pView->AreMatchingLinesVisible())
		pView->DrawMatchingLines();

	pView->DrawWarpedShape();

	// Draw selected points
	for (int i = 0; i < pView->m_inputCoords.Size(); i++)
	{
		pWnd->draw_disc(pView->m_inputCoords[i], 1, leda::orange);
	}

	pView->DrawTable();
}

/*!
	@brief Handles the window's destroy event
*/
/*static*/ void DAGView::Wnd::OnDelete(leda::window* pWnd)
{
	dynamic_cast<Wnd*>(pWnd)->GetDAGView()->OnWindowClose(pWnd);
}

void DAGView::ProcessInputedKeys()
{
	POINT pt;

	while (!m_streamInputedKeys.eof())
	{
		m_streamInputedKeys >> pt.x;

		if (!m_streamInputedKeys.eof())
		{
			m_streamInputedKeys >> pt.y;
			m_inputCoords.AddTail(ReMapPt(pt, GetImageInfo1()));
		}
		else
			cout << "Incomplete input coordinates." << endl;
	}
}

/*!
	Draw the contour and skeleton represented in the skeleton graph object.
	OLD: It gives priority to the boundary data points returnd by GetBoundaryPts()
	over those in the DDSkeleton.
	New: draws ONLY the DDSkeleton info
*/
bool DAGView::DrawSkeletonGraphInfo(const ImageInfo* ii, bool bFillShape) const
{
	// Always check this first!
	if (ii->pDAG->GetSkeleton() == NULL)
		return false;

	// Draw shape contour stored in the skeleton graph object
	const sg::DDSGraph* sk = ii->pDAG->GetSkeleton()->GetDDSGraph();

	if (sk != NULL && m_bShowContour)
	{
		leda::list<leda::point> pts;
		ShapeBoundary bndry(sk);
		unsigned int numPts = bndry.Size();

		// Remap all pts wrt current window scaling and store them in a list
		for (unsigned int i = 0; i < numPts; i++)
			pts.push(ReMapPt(bndry[i], ii));

		if (bFillShape)
		{
			leda::color fgcol[] = {leda::black, SHAPE_COLOR, leda::white};

			ASSERT(m_nForegroundColor >= 0 && m_nForegroundColor <= 2);

			m_pWnd->draw_filled_polygon(pts, m_nForegroundColor[fgcol]);
		}

		m_pWnd->draw_polygon(pts);
	}

	// Draw the all the spokes first
	if (m_nShowRays)
	{
		const SkelBranch* pBranch;
		leda::color c(51, 204, 102); // green (or pink: 243, 169, 161)

		forall_const_branches(pBranch, sk->getEdges())
		{
			const sg::FluxPointList& fpl = pBranch->getFluxPoints();
			const sg::BoundaryInfoList& bil = pBranch->getBoundaryInfoList();

			for (unsigned int i = 0; i < bil.size(); i++)
			{
				// If m_nShowRays == 2, show external rays only
				if (m_nShowRays == 2 && !(i == 0 || i + 1 == bil.size()))
					continue;

				m_pWnd->draw_segment(ReMapPt(fpl[i].p, ii),
					ReMapPt(bil[i].first.pt, ii), c);

				m_pWnd->draw_segment(ReMapPt(fpl[i].p, ii),
					ReMapPt(bil[i].second.pt, ii), c);
			}
		}
	}

	// Draw the skeletal points AFTER the spokes
	if (m_bShowSkeletalGraph)
	{
		const SkelBranch* pBranch;
		double area, sumArea = 0;
		sg::Point sumWeightedArea(0, 0);

		forall_const_branches(pBranch, sk->getEdges())
		{
			const sg::FluxPointList& fpl = pBranch->getFluxPoints();

			for (unsigned int i = 0; i < fpl.size(); i++)
			{
				m_pWnd->draw_pixel(ReMapPt(fpl[i].p, ii), leda::black);

				area = M_PI * fpl[i].val * fpl[i].val;
				sumWeightedArea += fpl[i].p * area;
				sumArea += area;
			}
		}

		if (sumArea > 0)
		{
			leda::point pt = ReMapPt(sumWeightedArea / sumArea, ii);
			m_pWnd->draw_disc(pt, 3, leda::green);
			//DBG_PRINT1(sumWeightedArea / sumArea)
		}
	}

	// Show the piecewise linear approximation of the medial axis
	if (m_bShowLinApprox)
	{
		const SmartArray<EuclideanLineSegmentArray> skelSegments =
			ii->pDAG->GetSkeleton()->GetLinearApproximation();

		for (int i = 0; i < skelSegments.GetSize(); i++)
			DrawPolyLine(skelSegments[i], ii);
	}

	// Show the piecewise cubic Bezier approximation of the medial axis
	if (m_bShowBezApprox)
	{
		const SmartArray<BezierSegmentArray> skelSegments =
			ii->pDAG->GetSkeleton()->GetBezierApproximation();

		for (int i = 0; i < skelSegments.GetSize(); i++)
			DrawPolyBezier(skelSegments[i], ii);
	}

	// Show endpoint coordinates. If m_nShowPtCoords equals 1, show
	// only the coords of junction points. If equals 2, show only the coords
	// of terminal points. And if equals 3, show all coords
	if (m_nShowPtCoords > 0 && sk != NULL)
	{
		const SkelEndpt* pEndpt;
		leda::point pt;
		char szCoords[100];

		forall_const_endpts(pEndpt, sk->getNodes())
		{
			if ((m_nShowPtCoords == 1 && pEndpt->degree() > 1) ||
				(m_nShowPtCoords == 2 && pEndpt->degree() <= 1) ||
				(m_nShowPtCoords == 3))
			{
				pt = ReMapPt(pEndpt->fp.p, ii);
				//sprintf(szCoords, "(%.1f,%.1f)", pEndpt->fp.p.x, pEndpt->fp.p.y);
				sprintf(szCoords, "(%d,%d)", (int)pEndpt->fp.p.x, (int)pEndpt->fp.p.y);
				m_pWnd->draw_disc(pt, 1, leda::black);
				m_pWnd->draw_text(pt, szCoords, leda::black);
			}
		}
	}

	return true;
}

/*!
	@brief Draws the info of each node and edge in the DAG on top of the
	skeleton graph information displayed by DrawSkeletonGraphInfo()
*/
void DAGView::DrawDAGInfo() const
{
	leda::node v;
	leda::edge e;
	int r, c;

	m_pWnd->start_buffering();

	m_pWnd->clear(leda_white);

	for (r = 0; r < m_imageInfoMat.NRows(); r++)
	{
		for (c = 0; c < m_imageInfoMat.NCols(); c++)
		{
			const ImageInfo* ii = &m_imageInfoMat[r][c];

    		DrawSkeletonGraphInfo(ii, m_bUseColors); // it fills the boundary polygon

			const DAG& dag = *(ii->pDAG);

			if (m_bDrawDisks)
			{
				DrawOrderedDAGInfo(dag, ii);

				// Redraw the countour because it may have been occluded by the disks
				DrawSkeletonGraphInfo(ii, false);

				m_pWnd->flush_buffer();
			}

			if (m_bShowEdgeInfo)
			{
				forall_edges(e, dag)
					DrawEdgeInfo(dag.GetEdge(e), ii);
			}

			if (m_bShowNodeInfo)
			{
				forall_nodes(v, dag)
					DrawNodeInfo(dag.GetNode(v), ii);
			}

			// Finish by drawing the global attributes of the DAG
			DrawDAGInfo(ii);
		}
	}

	m_pWnd->flush_buffer();
	m_pWnd->stop_buffering();
}

/*!
	Draws information in reverse DFS order. ie, lower
	level nodes are drawn first. Use to draw "only" the info that
	depends on the edge directions.
*/
void DAGView::DrawOrderedDAGInfo(const DAG& dag, const ImageInfo* ii) const
{
	leda::list<leda::node> nodes;
	leda::node v;

	if (TOPSORT(dag, nodes))
	{
		if (m_bTopToBottomOrder)
			nodes.reverse();

		forall(v, nodes)
			DrawNodeOrderedInfo(dag.GetNode(v), ii);
	}
	else
	{
		cout << "\nError: graph is not planar." << endl;
	}
}

/*!
	@brief Draws the array of line segments with given color and width.
*/
void DAGView::DrawPolyLine(const EuclideanLineSegmentArray& segments,
						   const ImageInfo* ii, leda::color c, int width) const
{
	int oldWidth = m_pWnd->set_line_width(width);

	for (int j = 0; j < segments.GetSize(); j++)
		m_pWnd->draw_segment(ReMapPt(segments[j].GetP0(), ii),
			ReMapPt(segments[j].GetP1(), ii), c);

	m_pWnd->set_line_width(oldWidth);
}

/*!
	@brief Draws the array of cubic bezier segments with given color and width.
*/
void DAGView::DrawPolyBezier(const BezierSegmentArray& segments, const ImageInfo* ii,
					leda::color c, int width, int ptsPerLine /* = 100 */) const
{
	// Draw piecewise cubic Bezier approximation
	int oldWidth = m_pWnd->set_line_width(width);

	for (int j = 0; j < segments.GetSize(); j++)
	{
		leda::list<leda::point> ctrlPts;

		ctrlPts.push_back(ReMapPt(segments[j].GetP0(), ii));
		ctrlPts.push_back(ReMapPt(segments[j].GetP1(), ii));
		ctrlPts.push_back(ReMapPt(segments[j].GetP2(), ii));
		ctrlPts.push_back(ReMapPt(segments[j].GetP3(), ii));

		m_pWnd->draw_bezier(ctrlPts, ptsPerLine, c);
	}

	m_pWnd->set_line_width(oldWidth);
}

/*!
	Draws a table with a cell per each displayed shape
*/
void DAGView::DrawTable()
{
	if (m_bDrawTable)
	{
		const double xmin = SW_LEFT_MARGIN - SW_CENTER_MARGIN / 2.0;
		const double xmax = xmin + m_colWidths.Sum() + SW_CENTER_MARGIN * m_colWidths.Size();
		const double ymin = SW_BOTTOM_MARGIN - SW_CENTER_MARGIN / 2.0;
		const double ymax = ymin + m_rowHeights.Sum() + SW_CENTER_MARGIN * m_rowHeights.Size();
		double dx = 0, dy = 0;
		int i;

		for (i = 0; i < m_rowHeights.Size(); i++)
		{
			m_pWnd->draw_segment(xmin, ymin + dy, xmax, ymin + dy, leda::black);

			dy += m_rowHeights[i] + SW_CENTER_MARGIN;
		}

		m_pWnd->draw_segment(xmin, ymin + dy, xmax, ymin + dy, leda::black);

		for (i = 0; i < m_colWidths.Size(); i++)
		{
			m_pWnd->draw_segment(xmin + dx, ymin, xmin + dx, ymax, leda::black);

			dx += m_colWidths[i] + SW_CENTER_MARGIN;
		}

		m_pWnd->draw_segment(xmin + dx, ymin, xmin + dx, ymax, leda::black);
	}
}