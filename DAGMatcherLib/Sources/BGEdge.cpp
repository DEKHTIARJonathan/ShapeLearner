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

using namespace dml;

/////////////////////////////////////////////////////////////////////////////
// BGEdge class implementation

/*!
	@brief Sets the taget point of the attachment. The closest point in
	the target node to the source point is seen as the target point.

	The leda edge parameter is needed to retrieve the source and target
	node points associated with the edge. This leda egde should be the container
	of the BGEdge, ie, inf(e) == this.
*/
void BGEdge::SetTargetPointIndex(leda::edge e)
{
	const BoneGraph* pBGGraph = (const BoneGraph*) graph_of(e);

	const BGNode* pSrcNode = pBGGraph->GetBGNode(source(e));
	const BGNode* pTgtNode = pBGGraph->GetBGNode(target(e));

	// If either node is empty, the default value for the target point is zero
	if (pSrcNode->IsEmpty() || pTgtNode->IsEmpty())
	{
		m_nTgtPtIndex = 0;
	}
	else
	{
		const ShockPoint& sp = pSrcNode->GetShockPoint(m_nSrcPtIndex);

		double d0 = sp.pt.SqDist(pTgtNode->FirstShockPoint().pt);
		double dN = sp.pt.SqDist(pTgtNode->LastShockPoint().pt);

		// We either have end-to-end or end-to-side attachments, so the
		// target point must be and endpoint
		//@TODO Handle side-to-side cases

		m_nTgtPtIndex = (d0 <= dN) ? 0 : pTgtNode->GetLastPointPosition();
	}
}

/*!
	@brief Sets the attachment position with the source point as
	the normalized distance along the axis of the source node.
*/
void BGEdge::ComputeDerivedValues(leda::edge e)
{
	DAGEdge::ComputeDerivedValues(e);
	BGElement::ComputeDerivedValues();

	const BoneGraph* pBGGraph = (const BoneGraph*) graph_of(e);
	const BGNode* pSrcNode = pBGGraph->GetBGNode(source(e));

	if (pSrcNode->IsEmpty())
	{
		m_dPosition = 0;
		m_dAngle    = 0;
	}
	else if (pSrcNode->NumPoints() == 1)
	{
		m_dPosition = 1;
		m_dAngle    = 0;
	}
	else
	{
		// Compute normalized position
		const ShockPoint& sp0 = pSrcNode->GetShockPoint(m_nSrcPtIndex);

		ASSERT(pSrcNode->Length() > 1);

		if (sp0.axisDist == 0)
			m_dPosition = 0; // make sure that it is exactly 0.0
		else if (sp0.axisDist + 1 == pSrcNode->Length())
			m_dPosition = 1; // make sure it is exactly 1.0
		else
			m_dPosition = sp0.axisDist / (pSrcNode->Length() - 1);

		// Compute angle between the target attachment pt and the tangent at the source pt
		const BGNode* pTgtNode = pBGGraph->GetBGNode(target(e));
		const ShockPoint& sp1 = pTgtNode->FindClosestPoint(sp0.pt,
			(m_nTgtPtIndex == 0), sp0.radius * sp0.radius);

		//pTgtNode->GetShockPoint(m_nTgtPtIndex);

		if (sp1.pt == sp0.pt)
		{
			m_dAngle = (m_nSrcPtIndex == 0) ? 0 : M_PI;
		}
		else
		{
			POINT tanVec = pSrcNode->ComputeTangent(m_nSrcPtIndex);
			POINT tgtVec = sp1.pt - sp0.pt;

			ASSERT(tanVec.SqNorm() > 0);

			m_dAngle = SignedVectorAngle(tanVec.x, tanVec.y, tgtVec.x, tgtVec.y);

			ASSERT(m_dAngle >= -M_PI && m_dAngle <= M_PI);

			// Here we have the sign of the angle that is correct.
			// The actual angle may not be, as it should be in |[0,PI/2]|
			// If the agle is greater than that, there should be another skeletal
			// point that forms a smaller angle. Ie, we see this as an object angle.

			// We can easily conver the angles that are too large to object angles by
			if (m_dAngle > M_PI_2)
			{
				m_dAngle = M_PI - m_dAngle;
			}
			else if (m_dAngle < -M_PI_2)
			{
				m_dAngle = -M_PI - m_dAngle;
			}
		}
	}

	ASSERT_VALID_NUM(m_dPosition);
	ASSERT_VALID_NUM(m_dAngle);
}

EDGE_LABEL BGEdge::GetLblForGraph(bool bExtraInfo /*=true*/) const
{
	char lbl[100];

	if (bExtraInfo)
		sprintf(lbl, "(%1.1f,%1.1f)", m_dPosition, m_dAngle);
	else if (fabs(m_dPosition) == 1 || m_dPosition == 0)
		sprintf(lbl, "%1.0f", m_dPosition);  //ie, (1, -1, or 0)
	else
		sprintf(lbl, ".%1.0f", 10 * m_dPosition); //ie, .5, .7, etc

	return lbl;
}

void BGEdge::Print(std::ostream& os, bool bXMLFormat) const
{
	DAGEdge::Print(os, bXMLFormat);
	BGElement::Print(os, bXMLFormat);

	if (bXMLFormat)
	{
		XML_PRINT(os, "sourcePointIndex", m_nSrcPtIndex);
		XML_PRINT(os, "targetPointIndex", m_nTgtPtIndex);
		XML_PRINT(os, "position", m_dPosition);
		XML_PRINT(os, "angle", m_dAngle);
	}
	else
	{
		PRINT_OPEN(os, m_nSrcPtIndex);
		PRINT(os, m_nTgtPtIndex);
		PRINT(os, m_dPosition);
		PRINT_CLOSE(os, m_dAngle);
	}
}

void BGEdge::ChangeSourceNode(leda::edge e, leda::node v)
{
	//const BoneGraph* pBGGraph = (const BoneGraph*) graph_of(e);
	//pBGGraph->move_edge(e, v, source(e));

	graph_of(e)->move_edge(e, v, source(e));

	SetTargetPointIndex(e);
	ComputeDerivedValues(e);
}

void BGEdge::MergeWithTargetNode(leda::edge e)
{
	if (m_shocks.IsEmpty())
		return; // there is nothing to do

	BoneGraph* pBGGraph = (BoneGraph*) graph_of(e);
	BGNode* pTgtNode = pBGGraph->UnsafeGetBGNode(target(e));

	// We assume that the point flow from parent to child, so
	// the last point of the edge can be either the first or last
	// point of the target node (which can have more than one parent)

	if (m_nTgtPtIndex == 0)
	{
		double distFF = FirstShockPoint().pt.SqDist(pTgtNode->FirstShockPoint().pt);
		double distLF = LastShockPoint().pt.SqDist(pTgtNode->FirstShockPoint().pt);

		ASSERT(distLF < 2 && distLF <= distFF);

		pTgtNode->m_shocks.AddHead(m_shocks);
	}
	else
	{
		double distLL = LastShockPoint().pt.SqDist(pTgtNode->LastShockPoint().pt);
		double distFL = FirstShockPoint().pt.SqDist(pTgtNode->LastShockPoint().pt);

		ASSERT(distFL < 2 && distFL <= distLL);

		pTgtNode->m_shocks.AddTail(m_shocks);
	}

	m_shocks.Clear();

	pTgtNode->ComputeDerivedValues();
}