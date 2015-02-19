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
 * $Log: DAGView.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef _DAG_VIEW_H_
#define _DAG_VIEW_H_

#include <LEDA/graphics/window.h>
#include "LEDA_window_utils.h"
#include "DAG.h"
#include "DDSGraphProject.h"

namespace dml {
/*!
	@brief This class groups all the operations needed to visualize a DAG object.
*/
class DAGView
{
public:
	struct Wnd : public leda::window
	{
		DAGView* m_pParent;
		POINT m_lastSkelPtShown;

		static void OnDraw(leda::window* pWnd);
		static void OnShowCoord(leda::window* pBaseWnd, double x, double y);
		static void OnDelete(leda::window* pWnd);

		Wnd(int w, int h, const char* label, DAGView* pParent) : leda::window(w, h, label)
		{
			m_pParent = pParent;
			set_redraw(Wnd::OnDraw);
			set_show_coord_handler(Wnd::OnShowCoord);
			set_window_delete_handler(Wnd::OnDelete);
			m_lastSkelPtShown.x = -1;
			m_lastSkelPtShown.y = -1;
		}

		DAGView* GetDAGView() { return m_pParent; }
	};

	struct ImageInfo
	{
		const DAG* pDAG;
		double delta_x;
		double delta_y;
		double reflex_y;

		ImageInfo() { pDAG = NULL; }

		//! Note: DAG* p is not constant because we may modify the DAG during visualization
		ImageInfo(const DAG* p, double dx, double dy, double ry) : pDAG(p)
		{
			delta_x = dx;
			delta_y = dy;
			reflex_y = ry;
		}
		// WARNING: in acyc mode we need to clone the DAG and then delete it
		// To clone the dag we need to make sure that we also clone the skeleton in it
		//~ImageInfo() { delete pDAG; }
	};

	friend struct Wnd;

protected:
	SmartMatrix<ImageInfo> m_imageInfoMat;

	SmartArray<double> m_rowHeights;
	SmartArray<double> m_colWidths;

	NodeMatchMap m_matchMap;
	Wnd*		 m_pWnd;
	ColorArray   m_colors;
	ColorArray   m_lineColors;

	bool		m_bShowLabels;
	bool		m_bShowNodeType;
	int			m_nShowColorLines; //!< 0=all lines. 1, 2, ... only given color
	int         m_nShowPartID;     //!< id of the part to show (0 = all parts)
	bool 		m_bShowCoords;
	bool        m_bShowSkeletalGraph; //! if true, shows the original skelatal pts
	int 		m_nShowRays;       //!< 0=no rays, 1=all rays, 2=rays at endpoints
	bool		m_bShowLinApprox;
	bool		m_bShowBezApprox;
	bool		m_bShowPointData;
	bool        m_bDrawDisks;
	bool        m_bShowEdgeInfo;
	bool        m_bShowNodeInfo;
	bool        m_bUseColors;
	bool        m_bTopToBottomOrder;
	int         m_nShowPtCoords;
	bool        m_bDrawContinuousSkeleton;
	bool        m_bShowContour;
	bool        m_bShowMatchingLines;
	int         m_nForegroundColor;
	bool        m_bDrawTable;

	bool        m_bKeyboardInputMode; //! if true, reads and stores each key pressed

	leda::string m_strWindowText;
	std::stringstream m_streamInputedKeys;
	SmartArray<leda::point> m_inputCoords;

public:
	DAGView();
	~DAGView();

	bool Create(const DAG* pDAG, int x = 0, int y = 0);
	bool Create(const DAG* pDAG1, const DAG* pDAG2, int x = 0, int y = 0);
	bool Create(SmartMatrix<const DAG*> dagMat, int xpos = 0, int ypos = 0);

	dml::String MakeWinTitle();
	bool Display(int xpos, int ypos);

	//! Redraws the child window
	void Redraw()
	{
		m_pWnd->redraw();
	}

	//! Maps a figure coordinate to its correspoinding window coordinate
	leda::point ReMapPt(const double& x, const double& y, const ImageInfo* ii) const
	{
		//const double s = (m_pWnd->scale() > 1) ? (1 / m_pWnd->scale()) : 1;
		return leda_point(x - ii->delta_x, ii->reflex_y - y - ii->delta_y /* * s */);
	}

	//! Maps a window coordinate to its correspoinding figure coordinate
	leda::point DAGView::MapBackPt(const double& x, const double& y, const ImageInfo* ii) const
	{
		// const double s = (m_pWnd->scale() > 1) ? (1 / m_pWnd->scale()) : 1;
		return leda_point(x + ii->delta_x, ii->reflex_y - y - ii->delta_y /* * s */);
	}

	//! Maps a figure coordinate to its correspoinding window coordinate
	leda::point ReMapPt(const dml::POINT& a, const ImageInfo* ii) const
	{
		return ReMapPt(a.x, a.y, ii);
	}

	//! Maps a figure coordinate to its correspoinding window coordinate
	leda::point ReMapPt(const sg::Point& a, const ImageInfo* ii) const
	{
		return ReMapPt(a.x, a.y, ii);
	}

	//! Maps a figure coordinate to its correspoinding window coordinate
	leda::point ReMapPt(const leda::point& a, const ImageInfo* ii) const
	{
		return ReMapPt(a.xcoord(), a.ycoord(), ii);
	}

	void SetBlackShapeMode(bool bDrawTable = false)
	{
		m_nForegroundColor = 0;
		m_bShowSkeletalGraph = false;
		m_nShowRays = 0;
		m_bShowLabels = false;
		m_bShowEdgeInfo = false;
		m_bShowNodeInfo = false;
		m_bShowCoords = false;
		m_bDrawTable = bDrawTable;
	}

	//! returns true of the windows shows two DAGs and false otherwise
	bool IsDualView() const
	{
		return (m_imageInfoMat.Size() == 2 &&
			m_imageInfoMat[0][0].pDAG->Width() > 0 &&
			m_imageInfoMat[0][1].pDAG->Width() > 0);
	}

	bool AreMatchingLinesVisible() const
	{
		return m_bShowMatchingLines;
	}

	void DrawPolyLine(const EuclideanLineSegmentArray& segments, const ImageInfo* ii,
		leda::color c = leda::black, int width = 2) const;

	void DrawPolyBezier(const BezierSegmentArray& segments, const ImageInfo* ii,
		leda::color c = leda::blue, int width = 2, int ptsPerLine = 100) const;

	// Virtual functions. Their implementation in this base class is quite generic
	virtual bool OnKeyPress(int val);
	virtual void ShowWindowCommandHelp() const;

	virtual void OnWindowClose(leda::window* pWnd)
	{
		pWnd->close();
	}

	virtual void OnMouseMove(const leda::point& pt, const ImageInfo* ii)
	{
		/* nothing to fo here*/
	}

	virtual void OnMouseButtonRelease(int val, const leda::point& pt, const ImageInfo* ii)
	{
		/* nothing to fo here*/
	}

	//virtual void DrawSilhouette(const ImageInfo* ii) const;
	virtual bool DrawSkeletonGraphInfo(const ImageInfo* ii, bool bFillShape = true) const;
	virtual void DrawDAGInfo() const;
	virtual void DrawOrderedDAGInfo(const DAG& dag, const ImageInfo* ii) const;

	//! Draw the global attributes of a DAG
	virtual void DrawDAGInfo(const ImageInfo* ii) const
	{
		// nothing to do here
	}

	//! This function is pure virtual because dependes entirely on the derived class
	virtual void DrawNodeInfo(const DAGNode* pDAGNode, const ImageInfo* ii) const = 0;

	//! The default behavious of this function is to do nothing
	virtual void DrawEdgeInfo(const DAGEdge* pDAGEdge, const ImageInfo* ii) const
	{
		//do nothing
	}

	//! The default behavious of this function is to do nothing
	virtual void DrawNodeOrderedInfo(const DAGNode* pDAGNode, const ImageInfo* ii) const
	{
		// do nothing
	}

	//! This function is pure virtual, but could be ported back to this class
	virtual void DrawMatchingLines() const = 0;

	//! Draws the result of warping the query shape to match the model silhouette
	virtual void DrawWarpedShape()
	{
		// do nothing
	}

	//! Draws the current text in m_strWindowText
	void DrawWindowText()
	{
		// make room for the display of mouse coordinates. Find the bottom
		// y-coord of the display of a generic pair of coordinates
		double y = m_pWnd->text_box(m_pWnd->xmin(), m_pWnd->xmax(),
			m_pWnd->ymax(), "x,y", false);

		// Set the text mode as oppaque
		leda::text_mode m = m_pWnd->set_text_mode(leda::opaque);

		// Show the text bellow that coordinate
		m_pWnd->text_box(m_pWnd->xmin(), m_pWnd->xmax(), y, m_strWindowText);

		// Restore the original text mode
		m_pWnd->set_text_mode(m);
	}

	void DrawTable();

	//! Sets the text of the window and draws it
	void SetWindowText(leda::string str)
	{
		m_strWindowText = str;
		DrawWindowText();
	}

	//! Appends the text to the window text and draws the updated text
	void AppendToWindowText(leda::string str)
	{
		m_strWindowText += str;
		DrawWindowText();
	}

	//! Clears the text associated with the window
	void ClearWindowText()
	{
		m_strWindowText = "";
	}

	//! Deletes the last character of the window text
	void BackspaceWindowText()
	{
		if (!m_strWindowText.empty())
		{
			leda::string str = m_strWindowText.head(m_strWindowText.length() - 1);
			m_strWindowText = str + "..";
			DrawWindowText();
			m_strWindowText = str;
		}
	}

	void ClearInputedKeys()
	{
		m_streamInputedKeys.clear();
	}

	virtual void ProcessInputedKeys();

	void SetMatchMap(const NodeMatchMap& matchMap)
	{
		m_matchMap = matchMap;
	}

	bool HasMatchMap() const
	{
		return (m_matchMap.size() > 0);
	}

	//const ImageInfo* GetImageInfo1() const { return m_pImageInfo1; }
	//const ImageInfo* GetImageInfo2() const { return m_pImageInfo2; }

	const ImageInfo* GetImageInfo1() const { return &m_imageInfoMat[0][0]; }
	const ImageInfo* GetImageInfo2() const { return &m_imageInfoMat[0][1]; }

	const ImageInfo* SelectImageInfo(const double& x, const double& y) const;
};
} // namespace dml

#endif //_DAG_VIEW_H_
