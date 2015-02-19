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
#ifndef __BG_NODE_H__
#define __BG_NODE_H__

#include "BGElement.h"

namespace dml {
/*!
	@brief Represents a node in a BoneGraph.

	@see BGElement, DAGNode, BoneGraph.
*/
class BGNode : public DAGNode, public BGElement
{
protected:
	virtual DAGNode* CreateObject() const
	{
		return (DAGNode*) new BGNode();
	}

public:
	BGNode(NODE_LABEL lbl = "", ElementType type = SIMPLE_BONE)
		: DAGNode(lbl), BGElement(type)
	{
	}

	BGNode& operator=(const BGNode& rhs)
	{
		DAGNode::operator=(rhs);
		BGElement::operator=(rhs);
		return *this;
	}

	void ReverseDataPoints(leda::node v);

	virtual DAGNode& operator=(const DAGNode& rhs)
	{
		const BGNode* pRhs = dynamic_cast<const BGNode*>(&rhs);
		ASSERT(pRhs);

		return operator=(*pRhs);
	}

	virtual void Clear()
	{
		DAGNode::Clear();
		BGElement::Clear();
	}

	virtual void ComputeDerivedValues()
	{
		DAGNode::ComputeDerivedValues();
		BGElement::ComputeDerivedValues();
	}

	virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;

	virtual std::istream& Read(std::istream& is)
	{
		DAGNode::Read(is);
		BGElement::Read(is);
		return is;
	}

	virtual std::ostream& Write(std::ostream& os) const
	{
		DAGNode::Write(os);
		BGElement::Write(os);
		return os;
	}

	virtual leda::color GetColorForGraph() const;
	virtual int GetShapeForGraph() const;
	virtual NODE_LABEL GetLblForGraph(bool bExtraInfo = true) const;
	virtual double GetCost() const;
};
} //namespace dml

#endif //__BG_NODE_H__
