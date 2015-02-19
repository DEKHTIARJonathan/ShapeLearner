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
 * $Log: VisualDAG.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include <LEDA/graphics/graphwin.h>
#include "VisualDAG.h"
#include "ShockGraph.h"

#include <stdio.h>

// Graph window params
#define GW_LEFT_MARGIN    50
#define GW_RIGHT_MARGIN   50
#define GW_TOP_MARGIN     50
#define GW_BOTTOM_MARGIN  50
#define NODE_WIDTH		  25 //54
#define NODE_HEIGHT		  25
#define NODE_FONT_SIZE	  11
#define EDGE_FONT_SIZE	  9

#define HGAP			(NODE_WIDTH / 4)
#define VGAP			(NODE_HEIGHT * 2)

using namespace std;
using namespace dml;
using namespace leda;

/* Useful piece of help on leda colors...

	Colors can be created from rgb-values, from names in a color data base (X11),
	or from the 16 integer constants (enumeration in \<LEDA/incl/impl/x\_window.h\>)
	$black$, $white$, $red$, $green$, $blue$, $yellow$, $violet$, $orange$;
	$cyan$, $brown$, $pink$, $green2$, $blue2$, $grey1$, $grey2$, $grey3$.
*/

void RecomputeTSV(leda_GraphWin& gw, leda_node v)
{
	DAG& dag = dynamic_cast<DAG&>(gw.get_graph());

	dag.ComputeTSVs(dag.GetFirstRootNode());
	dag.PrintTSVs();
}

void OnRecomputeTSV(leda_GraphWin& gw, const leda_point& pt)
{
	double x = pt.xcoord(), y = pt.ycoord();
	leda_list<leda_node> nodes = gw.get_nodes_in_area(x, y, x, y);

	RecomputeTSV(gw, nodes.pop());
}

void OnDisplayNodeEigenSum(leda_GraphWin& gw, const leda_point& pt)
{
	const DAG& dag = dynamic_cast<DAG&>(gw.get_graph());
	double x = pt.xcoord(), y = pt.ycoord();
	leda_node v = gw.get_nodes_in_area(x, y, x, y).pop();

	if (v != nil)
	{
		double n = dag.GetNode(v)->GetEigenLbl();
		char sz[50];
		sprintf(sz, "Eigenvalue Sum = %.4f.", n);
		gw.message(sz);
	}
}

void OnDisplayNodeInfo(leda_GraphWin& gw, const leda_point& pt)
{
	const DAG& dag = dynamic_cast<DAG&>(gw.get_graph());
	double x = pt.xcoord(), y = pt.ycoord();
	leda_node v = gw.get_nodes_in_area(x, y, x, y).pop();

	if (v != nil)
	{
		double n = dag.GetNode(v)->GetEigenLbl();
		char sz[500];

		const ShockGraph* pSG = dynamic_cast<const ShockGraph*>(&dag);

		if (pSG) {
			sprintf(sz, "%s %d: DFS index: %d, EigenSum: %.4f, Shock Pts: %d",
				(const char*)pSG->GetObjName(),
				pSG->GetViewNumber(),
				pSG->GetNodeDFSIndex(v),
				n,
				pSG->NodeLength(v));

			cout << endl; pSG->GetSGNode(v)->Print();
			cout << endl; pSG->GetSGNode(v)->m_shocks.Print();
		}
		else
		{
			//sprintf(sz, "Label:%s\nEigenSum: %.4f.", (const char*)dag.GetDAGLbl(), n);
			strcpy(sz, "See terminal window...");
			dag.GetNode(v)->Print();
		}

		gw.message(sz);
	}
}

void OnShowAdjMatrix(leda_GraphWin& gw, const leda_point& pt)
{
	DAG& dag = dynamic_cast<DAG&>(gw.get_graph());

	dag.PrintAdjMatrix();
}

void OnNewNode(leda_GraphWin& gw, leda_node v)
{
	DAG& dag = dynamic_cast<DAG&>(gw.get_graph());

	dag[v] = dag.CreateNodeObject((const char*)gw.get_label(v));
	RecomputeTSV(gw, v);
}

void OnNewEdge(leda_GraphWin& gw, leda_edge e)
{
	DAG& dag = dynamic_cast<DAG&>(gw.get_graph());

	dag[e]->SetWeight(DEFAULT_DAG_EDGE_WEIGHT);
	RecomputeTSV(gw, dag.source(e));
}

void OnDelNode(leda_GraphWin& gw)
{
	RecomputeTSV(gw, nil);
}

void OnDelEdge(leda_GraphWin& gw)
{
	RecomputeTSV(gw, nil);
}

void OnHierarchicalVisualization(leda_GraphWin& gw)
{
	((VisualDAG&) gw).ChageVisualization(VisualDAG::HIERARCHICAL);
}

void OnGeometricalVisualization(leda_GraphWin& gw)
{
	((VisualDAG&) gw).ChageVisualization(VisualDAG::GEOMETRICAL);
}

void OnEmptyEdgeLabels(leda_GraphWin& gw)
{
	((VisualDAG&) gw).ChageEdgeLabels(VisualDAG::EMPTY_LABEL);
}

void OnMinimalEdgeLabels(leda_GraphWin& gw)
{
	((VisualDAG&) gw).ChageEdgeLabels(VisualDAG::MINIMAL_LABEL);
}

void OnFullEdgeLabels(leda_GraphWin& gw)
{
	((VisualDAG&) gw).ChageEdgeLabels(VisualDAG::FULL_LABEL);
}

int CompNumOfLeaves(const DAG& g, leda_node v, NodeIndexMap& nodeIdxMap)
{
	if (!nodeIdxMap.defined(v))
	{
		leda_node u;
		int n = 0;

		if (g.outdeg(v) == 0)
			n = 1;
		else
			forall_adj_nodes(u, v)
				n += CompNumOfLeaves(g, u, nodeIdxMap);

		return nodeIdxMap[v] = n;
	}

	return nodeIdxMap[v];
}

/*!
	@brief Recursive function that sets the position of a node and all its children

	Parents position are set to be above and at the center of all their child nodes.

	@param nodeIdxMap stores the number of leaves associated with eacg node
*/
double SetNodePos(leda_GraphWin& gw, DAG& g, leda_node v, dml::POINT top_left, NodeIndexMap& nodeIdxMap)
{
	leda_node u;
	int num_lea = nodeIdxMap[v]; // number of leaves for node v

	double w = num_lea * NODE_WIDTH + (num_lea - 1) * HGAP;

	leda_point pos(top_left.x + w / 2, top_left.y);

	gw.set_position(v, pos);

	top_left.y -= NODE_HEIGHT + VGAP;

	forall_adj_nodes(u, v)
	{
		if (indeg(u) <= 1 || g.GetNodeLevel(u) == g.GetNodeLevel(v) + 1)
			top_left.x += SetNodePos(gw, g, u, top_left, nodeIdxMap) + HGAP;
	}

	return w;
}

void ComputeDAGDimensions(const DAG& g, int* pWinWidth, int* pWinHeight,
						  NodeIndexMap* pMap, leda::node* pRootNode)
{
	leda_node v, r = g.GetFirstRootNode();

	// Compute graph's dimensions. Diameter and height
	int nMaxHeight = 0;

	forall_nodes(v, g)
		if (g.GetNodeLevel(v) > nMaxHeight)
			nMaxHeight = g.GetNodeLevel(v);

	int nLeaves = CompNumOfLeaves(g, r, *pMap);

	// Set window's dimensions
	*pWinWidth = NODE_WIDTH * nLeaves + HGAP * (nLeaves - 1)
		+ GW_LEFT_MARGIN + GW_RIGHT_MARGIN;

	*pWinHeight = NODE_HEIGHT * (nMaxHeight + 1) + VGAP * nMaxHeight
		+ GW_TOP_MARGIN + GW_BOTTOM_MARGIN;

	*pRootNode = r;
}
//leda::GraphWin
void DrawGraph(VisualDAG& gw, DAG& g, leda::node rootNode,
				  NodeIndexMap& map, int nExtraTopMargin = 0)
{
	leda::node v;

	// Set node positions
	dml::POINT top_left;

	top_left.x = GW_LEFT_MARGIN;
	top_left.y = gw.get_ymax() - GW_TOP_MARGIN - NODE_HEIGHT / 2 - nExtraTopMargin;

	SetNodePos(gw, g, rootNode, top_left, map);

	bool bEmptyLbl = (gw.GetEdgeLabelType() == VisualDAG::EMPTY_LABEL);
	bool bFullLbl  = (gw.GetEdgeLabelType() == VisualDAG::FULL_LABEL);

	// Set node attributes
	forall_nodes(v, g)
	{
		const DAGNodePtr node = g.GetNode(v);

		gw.set_label(v, (const char*)node->GetLblForGraph(bFullLbl));
		gw.set_shape(v, (leda_gw_node_shape)node->GetShapeForGraph());
		gw.set_color(v, node->GetColorForGraph());
	}

	//Set edge attributes
	leda::edge e;

	forall_edges(e, g)
	{
		const DAGEdgePtr edge = g.GetEdge(e);

		if (edge->HasLblForGraph())
		{
			if (bEmptyLbl)
				gw.set_user_label(e, "");
			else
				gw.set_user_label(e, (const char*)edge->GetLblForGraph(bFullLbl));

			gw.set_label_type(e, leda::user_label);
			gw.set_color(e, edge->GetColorForGraph());
		}
	}
}

void VisualDAG::ChageVisualization(VISUALIZATION vis)
{
	if (vis == GEOMETRICAL)
	{
		DBG_MSG1("Geometrical")
	}
	else
	{
		DBG_MSG1("Hierarchical")
	}

	m_visualization = vis;
}

void VisualDAG::ChageEdgeLabels(LABEL_TYPE lblType)
{
	m_edgeLabelType = lblType;

	DrawGraph(*this, *m_graphInfo1.ptrG, m_graphInfo1.rootNode,
		m_graphInfo1.map, m_graphInfo1.extraTopMargin);

	DrawGraph(*this, *m_graphInfo2.ptrG, m_graphInfo2.rootNode,
		m_graphInfo2.map, m_graphInfo2.extraTopMargin);
}

void SetGlobalProperties(leda::GraphWin& gw)
{
	// Set global params
	gw.set_node_shape(leda_ellipse_node, false);
	gw.set_node_width(NODE_WIDTH, true);
	gw.set_node_height(NODE_HEIGHT, true);

	gw.set_node_label_font(leda_roman_font, NODE_FONT_SIZE);
	gw.set_edge_label_font(leda_roman_font, EDGE_FONT_SIZE);
	gw.enable_label_box(false);
	gw.set_show_status(false);

	gw.add_node_menu("Recompute TSV", OnRecomputeTSV);
	gw.add_node_menu("Display EigenSum", OnDisplayNodeEigenSum);
	gw.add_node_menu("Display node info", OnDisplayNodeInfo);
	gw.add_node_menu("Show adj matrix", OnShowAdjMatrix);
	gw.set_new_node_handler(OnNewNode);
	gw.set_new_edge_handler(OnNewEdge);
	gw.set_del_node_handler(OnDelNode);
	gw.set_del_edge_handler(OnDelEdge);

	gw.set_default_menu(M_FILE | M_EDIT | M_WINDOW | M_OPTIONS | M_DONE);

	int menu_id0 = gw.add_menu("&Visualization");

	int menu_id1 = gw.add_menu("&Graph Topology", menu_id0);
	gw.add_simple_call(OnHierarchicalVisualization, "&Hierarchical", menu_id1);
	gw.add_simple_call(OnGeometricalVisualization, "&Geometrical", menu_id1);

	int menu_id2 = gw.add_menu("&Edge Labels", menu_id0);
	gw.add_simple_call(OnEmptyEdgeLabels, "&Empty", menu_id2);
	gw.add_simple_call(OnMinimalEdgeLabels, "&Minimal", menu_id2);
	gw.add_simple_call(OnFullEdgeLabels, "&Full", menu_id2);
}

void SetMatchingNodeColors(DAG& g1, DAG& g2, const NodeMatchMap& map,
						   const ColorArray& colors)
{
	leda::node v;
	leda::color c;
	int i;

	ASSERT(colors.Size() > 0);

	// Init all node colors in g2 to "unmatched"
	forall_nodes(v, g2)
		g2.SetNodeColor(v, leda::white);

	forall_nodes(v, g1)
	{
		if (map[v].IsEmpty())
		{
			g1.SetNodeColor(v, leda::white);
		}
		else
		{
			i = g1.GetNodeDFSIndex(v);
			c = colors[i % colors.Size()];

			g1.SetNodeColor(v, c);
			g2.SetNodeColor(map[v].node, c);
		}
	}
}

void VisualDAG::ShowGraph(DAG& g, bool bEditMode)
{
	NodeIndexMap map;
	int nWinWidth, nWinHeight;
	leda::node rootNode;

	// Create window
	ComputeDAGDimensions(g, &nWinWidth, &nWinHeight, &map, &rootNode);

	VisualDAG gw(g, nWinWidth, nWinHeight, "DAG Visualization");
	//leda::GraphWin& gw = *this;

	gw.win_init(0, nWinWidth, 0);

	DrawGraph(gw, g, rootNode, map);

	SetGlobalProperties(gw);

	char szMsg[100];
	sprintf(szMsg, "%s %d", (const char*)g.GetObjName(), g.GetViewNumber());

	//gw.place_into_win();
	gw.display();
	gw.message(szMsg);

	if (bEditMode)
		gw.edit();
	else
		gw.wait();
}

void VisualDAG::ShowGraphs(DAG& g1, DAG& g2, const NodeMatchMap& matchMap,
					 const ColorArray& colors, bool bEditMode)
{
	NodeIndexMap map1, map2;
	int nWinWidth1, nWinWidth2, nWinHeight1, nWinHeight2;
	leda::node rootNode1, rootNode2;

	// Set matching node colors in both graphs
	if (matchMap.size() > 0)
		SetMatchingNodeColors(g1, g2, matchMap, colors);

	// Clone the graphs
	/*DAGPtr*/SharedPtr<DAG> ptrG1 = g1.CreateObject();
	/*DAGPtr*/SharedPtr<DAG> ptrG2 = g2.CreateObject();

	*ptrG1 = g1;
	*ptrG2 = g2;

	// Create window
	ComputeDAGDimensions(*ptrG1, &nWinWidth1, &nWinHeight1, &map1, &rootNode1);
	ComputeDAGDimensions(*ptrG2, &nWinWidth2, &nWinHeight2, &map2, &rootNode2);

	// Merge graphs!!!
	ptrG1->join(*ptrG2);

	int w = MAX(nWinWidth1, nWinWidth2);
	int h = nWinHeight1 + nWinHeight2 - GW_TOP_MARGIN;

	VisualDAG gw(*ptrG1, w, h, "Multi DAG Visualization");

	gw.win_init(0, w, 0);

	// Arrange the position of the nodes in the given graph AND map
	// ptrG1 is a merge of the given g1 and g2, but map refers to g1 only
	DrawGraph(gw, *ptrG1, rootNode1, map1);
	DrawGraph(gw, *ptrG2, rootNode2, map2, nWinHeight1 - GW_TOP_MARGIN);

	// Save info so that we can re-draw the graphs later
	gw.SetGraphInfo(1, ptrG1, rootNode1, map1, 0);
	gw.SetGraphInfo(2, ptrG2, rootNode2, map2, nWinHeight1 - GW_TOP_MARGIN);

	SetGlobalProperties(gw);

	char szMsg[200];

	sprintf(szMsg, "%s %d -- %s %d",
		(const char*)g1.GetObjName(), g1.GetViewNumber(),
		(const char*)g2.GetObjName(), g2.GetViewNumber());

	gw.display();
	gw.message(szMsg);

	if (bEditMode)
		gw.edit();
	else
		gw.wait();
}