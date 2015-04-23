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
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

#define BACK_BONE_COST 1

using namespace dml;

/////////////////////////////////////////////////////////////////////////////
// BGNode class implementation

/*!
	@brief Reverses the data points and updates the source and target points
	of the edges that start or end at this node

	@param v is the leda node that contains this BGNode
*/
void BGNode::ReverseDataPoints(leda::node v)
{
	// Reverse the data points
	BGElement::ReverseDataPoints();

	// Get the container graph
	BoneGraph* pBGGraph = (BoneGraph*) graph_of(v);

	// Update the source and target point of the edges that start or end
	// at this node to reflect the data point reversal
	leda::edge e;
	BGEdge* pEdge;

	// Change the target point index of all edges arriving at the node
	forall_in_edges(e, v)
	{
		pEdge = pBGGraph->UnsafeGetBGEdge(e);
		pEdge->ReverseTargetPointIndex(NumPoints());
	}

	// Change the source point index of all edges leaving the node
	forall_out_edges(e, v)
	{
		pEdge = pBGGraph->UnsafeGetBGEdge(e);
		pEdge->ReverseSourcePointIndex(NumPoints());
	}
}

//! Returns the color that will be displayed in the graph
leda::color BGNode::GetColorForGraph() const
{
	//return SaliencyColor();
	return GetColor();
}

//! Returs the shape for the nodes in the graph (eg, circle, ellipse, etc)
int BGNode::GetShapeForGraph() const
{
	return 0; // avoid having to include graphwin.h... circle=0, ellipse = 1
}

//! Returns the node label that will be displayed in the graph
NODE_LABEL BGNode::GetLblForGraph(bool bExtraInfo) const
{
	/*if (Type() == BONE_PART_TYPE_2)
		return GetNodeLbl() + "*";
	else if (Type() == BONE_PART_TYPE_4)
		return GetNodeLbl() + "^";
	else
		return GetNodeLbl();*/

	if (bExtraInfo)
	{
		char* szBuff = new char[100];

		sprintf(szBuff, "%d:%.1f", GetDFSIndex(), GetSubtreeCost());

		return szBuff;
	}
	else
		return GetDFSLbl();
}

void BGNode::Print(std::ostream& os, bool bXMLFormat) const
{
	DAGNode::Print(os, bXMLFormat);
	BGElement::Print(os, bXMLFormat);

	// there is nothing else to do here
}

double BGNode::GetCost() const
{
	if (Type() == BACK_BONE)
		return BACK_BONE_COST;
	else
		return XORIntegral::FunctionArea(GetRadiusFunction());
}