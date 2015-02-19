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
#ifndef __BG_EDGE_H__
#define __BG_EDGE_H__

#include "BGElement.h"

namespace dml {
/*!
	@brief Represents an edge in a BoneGraph.

	@see BGElement, DAGEdge, BoneGraph.
*/
class BGEdge : public DAGEdge, public BGElement
{
protected:
	int m_nSrcPtIndex;    //!< index of the source point. must be given.
	int m_nTgtPtIndex;    //!< index of the target point. can be computed.
	double m_dPosition;   //!< normalized position. can be computed.
	double m_dAngle;      //!< normalized angle. can be computed.

	SmartArray<BoundaryCorner> m_corners; //!< Boundary corners associated with the edge (dbg info only)

	virtual DAGEdge* CreateObject() const
	{
		return (DAGEdge*) new BGEdge();
	}

public:
	BGEdge(int nSrcPtIdx = 0, ElementType type = LIGAMENT,
		const double& w = DEFAULT_DAG_EDGE_WEIGHT) : DAGEdge(w), BGElement(type)
	{
		m_nSrcPtIndex = nSrcPtIdx;
		m_nTgtPtIndex = 0;
		m_dPosition   = 0;
		m_dAngle      = 0;
	}

	BGEdge& operator=(const BGEdge& rhs)
	{
		DAGEdge::operator=(rhs);
		BGElement::operator=(rhs);

		m_nSrcPtIndex = rhs.m_nSrcPtIndex;
		m_nTgtPtIndex = rhs.m_nTgtPtIndex;
		m_dPosition   = rhs.m_dPosition;
		m_dAngle      = rhs.m_dAngle;

		m_corners     = rhs.m_corners;

		return *this;
	}

	virtual DAGEdge& operator=(const DAGEdge& rhs)
	{
		const BGEdge* pRhs = dynamic_cast<const BGEdge*>(&rhs);
		ASSERT(pRhs);

		return operator=(*pRhs);
	}

	virtual void Clear()
	{
		DAGEdge::Clear();
		BGElement::Clear();

		m_nSrcPtIndex = 0;
		m_nTgtPtIndex = 0;
		m_dPosition   = 0;
		m_dAngle      = 0;

		m_corners.Clear();
	}

	//! Attachment position normalized to the unity
	double Position(bool bFlipOrientation = false) const
	{
		return (bFlipOrientation) ? (1 - m_dPosition) : m_dPosition;
	}

	//! Attachment angle in radians
	double Angle(bool bFlipSides = false) const
	{
		return (bFlipSides) ? -m_dAngle : m_dAngle;
	}

	virtual void ComputeDerivedValues(leda::edge e);

	virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;

	virtual std::istream& Read(std::istream& is)
	{
		DAGEdge::Read(is);
		BGElement::Read(is);

		is.read((char*) &m_nSrcPtIndex, sizeof(m_nSrcPtIndex));
		is.read((char*) &m_nTgtPtIndex, sizeof(m_nTgtPtIndex));
		is.read((char*) &m_dPosition, sizeof(m_dPosition));
		is.read((char*) &m_dAngle, sizeof(m_dAngle));

		m_corners.Read(is);

		return is;
	}

	virtual std::ostream& Write(std::ostream& os) const
	{
		DAGEdge::Write(os);
		BGElement::Write(os);

		os.write((char*) &m_nSrcPtIndex, sizeof(m_nSrcPtIndex));
		os.write((char*) &m_nTgtPtIndex, sizeof(m_nTgtPtIndex));
		os.write((char*) &m_dPosition, sizeof(m_dPosition));
		os.write((char*) &m_dAngle, sizeof(m_dAngle));

		m_corners.Write(os);

		return os;
	}

	virtual bool HasLblForGraph() const
	{
		return true;
	}

	virtual EDGE_LABEL GetLblForGraph(bool bExtraInfo = true) const;

	//! Returns the color that is use to visualize the edge
	virtual leda::color GetColorForGraph() const
	{
		return (m_dAngle >= 0) ? leda::black : leda::red;
	}

	void ChangeSourceNode(leda::edge e, leda::node v);
	void MergeWithTargetNode(leda::edge e);

	int GetSourcePointIndex() const
	{
		return m_nSrcPtIndex;
	}

	void SetSourcePointIndex(int nSrcPtIdx)
	{
		m_nSrcPtIndex = nSrcPtIdx;
	}

	int GetTargetPointIndex() const
	{
		return m_nTgtPtIndex;
	}

	void SetTargetPointIndex(leda::edge e);

	//! Reverses the source point index. Needs the number of points in the source node
	void ReverseSourcePointIndex(int numSrcNodePts)
	{
		m_nSrcPtIndex = numSrcNodePts - m_nSrcPtIndex - 1;
	}

	//! Reverses the target point index. Needs the number of points in the target node
	void ReverseTargetPointIndex(int numTgtNodePts)
	{
		m_nTgtPtIndex = numTgtNodePts - m_nTgtPtIndex - 1;
	}

	//! Returns true if the target point is the first shock point in tgt.m_shocks
	bool FlowsFromParent() const
	{
		return (m_nTgtPtIndex == 0);
	}

	SmartArray<BoundaryCorner> GetBoundaryCorners() const
	{
		return m_corners;
	}

	void SetBoundaryCornerInfo(const BoundaryCorner& bc1, const BoundaryCorner& bc2)
	{
		ASSERT(m_corners.IsEmpty());

		m_corners.AddTail(bc1);
		m_corners.AddTail(bc2);
	}
};
} //namespace dml

#endif //__BG_EDGE_H__
