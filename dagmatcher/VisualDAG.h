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
 *-----------------------------------------------------------------------*/

#ifndef __VISUAL_DAG_H__
#define __VISUAL_DAG_H__

#include <LEDA/graphics/graphwin.h>

#include "LEDA_window_utils.h"

#include "DAGMatcher.h"

namespace dml {
class VisualDAG : public leda::GraphWin
{
	struct GraphInfo
	{
		SharedPtr<DAG> ptrG;
		leda::node rootNode;
		NodeIndexMap map;
		int extraTopMargin;
	};

public:
	enum VISUALIZATION {HIERARCHICAL, GEOMETRICAL};
	enum LABEL_TYPE {EMPTY_LABEL, MINIMAL_LABEL, FULL_LABEL};

protected:
	/*SharedPtr<DAG> m_ptrG1, m_ptrG2;
	NodeIndexMap m_map1, m_map2;
	leda::node m_rootNode1, m_rootNode2;
	int m_extraTopMargin;*/
	GraphInfo m_graphInfo1, m_graphInfo2;

	VISUALIZATION m_visualization;
	LABEL_TYPE m_edgeLabelType;
	LABEL_TYPE m_nodeLabelType;

public:
	VisualDAG(leda::graph& G, int w, int h, const char* win_label="")
		: leda::GraphWin(G, w, h, win_label)
	{
		SetDefaultValues();
	}

	void SetGraphInfo(int n, SharedPtr<DAG> ptrG, leda::node rootNode,
		const NodeIndexMap& map, int extraTopMargin)
	{
		GraphInfo& gi = (n == 1) ? m_graphInfo1 : m_graphInfo2;

		gi.ptrG = ptrG;
		gi.rootNode = rootNode;
		gi.map = map;
		gi.extraTopMargin = extraTopMargin;
	}

	void SetDefaultValues()
	{
		m_visualization = HIERARCHICAL;
		m_edgeLabelType = MINIMAL_LABEL;
		m_nodeLabelType = MINIMAL_LABEL;
	}

	LABEL_TYPE GetEdgeLabelType() const
	{
		return m_edgeLabelType;
	}

	void ChageVisualization(VISUALIZATION vis);
	void ChageEdgeLabels(LABEL_TYPE lblType);

	static void ShowGraph(DAG& g, bool bEditMode = true);

	static void ShowGraphs(DAG& g1, DAG& g2, const NodeMatchMap& matchMap,
		const ColorArray& colors, bool bEditMode = true);
};
} // namespace dml

#endif //__VISUAL_DAG_H__
