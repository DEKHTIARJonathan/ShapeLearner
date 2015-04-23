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
 * $Log: GGNode.h,v $
 * Revision 1.2  2002/06/14 23:13:03  dmac
 * Matching algorithm updated.
 *
 * Revision 1.1  2002/05/21 16:52:19  dmac
 * New file added.
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/
#ifndef __GG_NODE_H__
#define __GG_NODE_H__

#include "DAGNode.h"

namespace dml {
/*!
	\brief Represents a gesture graph relation

	\see DAGNode, GestureGraph, GGNode.

*/
class GGRelation
{
public:
	int m_nType;
	double m_dScaleRatio;
	double m_dOrient;
	double m_dDist1;
	double m_dDist2;
	double m_dBearing;

public:

	GGRelation() { memset(this, 0, sizeof(*this)); }

	GGRelation(const GGRelation& rhs) { operator=(rhs); }

	GGRelation& operator=(const GGRelation& rhs)
	{
		memcpy(this, &rhs, sizeof(*this));
		return *this;
	}

	std::istream& Read(std::istream& is);
	std::ostream& Write(std::ostream& os) const;
	void Print(std::ostream& os, bool bXMLFormat = false) const;

	friend std::istream& operator>>(std::istream& is, GGRelation& rel);
	friend std::ostream& operator<<(std::ostream& os, const GGRelation& rel);
};

/*!
	\brief Represents a node of a gesture graph.

	\see DAGNode, GestureGraph.

*/
class GGNode : public DAGNode
{
public:
	int m_nType;
	double m_dSign;
	double m_dScale;
	int m_nXPos;
	int m_nYPos;
	double m_dA;
	double m_dB;
	double m_dOrientation;
	double m_dAnisotropy;
	double m_dUnkField;

	int m_nNodeIndex;

	SmartArray<double> m_xCoords;
	SmartArray<double> m_weights;
	SmartArray<double> m_scales;

	virtual DAGNode* CreateObject() const;

// Operations
  public:

	GGNode(NODE_LABEL lbl = "") : DAGNode(lbl) { }
	GGNode(const GGNode &sb) { GGNode::operator=(sb); }

	virtual void Clear();
	virtual DAGNode& operator=(const DAGNode& rhs);
	virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;
	virtual std::istream& Read(std::istream& is);
	virtual std::ostream& Write(std::ostream& os) const;
};
} //namespace dml

#endif //__GG_NODE_H__
