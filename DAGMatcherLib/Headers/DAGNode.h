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
 * $Log: DAGNode.h,v $
 * Revision 1.3  2002/06/10 18:39:14  dmac
 * MAss and dfs index attributes added.
 *
 * Revision 1.2  2002/05/16 22:49:28  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef __DAGNODE_H__
#define __DAGNODE_H__

#include "stdafx.h"

//! Default node label
#define NODE_LBL_DEFAULT_VAL "DLV"

//! Maximum buffer size for DFS node index labels
#define MAX_INDEX_DIGIT 5

namespace dml {
typedef dml::String NODE_LABEL;

/*!
	\brief This class represents a node of an object of class DAG.

	DAGNode is an abstract class intended to provide the basic functionality of
	a DAG node and to specify which functions must be overloaded (the pure virtual ones).

	At this moment, many functions that could belong to the DAGNode class, such as
	NodeAttributeDistance() and so on, are implemented in the DAG class. The reason for this
	is that some of these functions require information about the neighbours of the
	node, and that information is contained in the DAG class. However, if a node had
	a member variable of type leda_node that links it to the associated node in the graph,
	all the above functions could be implemented in the DAGNode class. Remember that given
	a leda_node v, we can retrieve the graph it belongs to by calling graph_of(v).
	A drawback of this approach would be that we introduce some risk of inconsistency
	because when we move a node from one graph to another, we need to change the
	value of its reference to the leda_node accordingly.

	/see DAG and DAGNodePtr.
*/
class DAGNode
{
	NODE_LABEL			nodeLbl;		//!< Node's label.
	TSV					childrenTSV;	//!< Adj. node's eigen labels.
	double				eigenLbl;		//!< Node's eigen label.

	// Derived values
	double				tsvNorm;		//!< Node TSV's norm
	int					nMass;			//!< Node mass
	int					nLevel;			//!< Node level in the hierarchical structure
	int					nDFSIndex;		//!< Index relative to a depth-first search
	double              subtreeCost;    //!< Cost of the subtree rooted at the node

	leda::color         color;          //!< Display color. It's not saved.

public:
	typedef int label_type;

	DAGNode(NODE_LABEL lbl = NODE_LBL_DEFAULT_VAL);

	DAGNode(const DAGNode& rhs) { DAGNode::operator=(rhs); }

	// Pure virtual functions
	virtual DAGNode* CreateObject() const = 0;	//!< This function is required by the SmartPtr class.

	// Virtual functions
	virtual ~DAGNode() { }					//!< Necessary virtual destructor.
	virtual DAGNode& operator=(const DAGNode& rhs);
	virtual void Clear();
	virtual std::istream& Read(std::istream& is);
	virtual std::ostream& Write(std::ostream& os) const;
	virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;

	// Functions to display the node info in the screen
	virtual NODE_LABEL GetLblForGraph(bool bExtraInfo = true) const;
	virtual leda::color GetColorForGraph() const;
	virtual int GetShapeForGraph() const;

	virtual void ComputeDerivedValues();

	//! Gets the cost of the node (default is 1)
	virtual double GetCost() const
	{
		return 1;
	}

	const double& GetSubtreeCost() const
	{
		return subtreeCost;
	}

	void SetSubtreeCost(const double& stCost)
	{
		subtreeCost = stCost;
	}

	/// Similarity function for two nodes.
	//virtual double operator-(const DAGNode& node) = 0;

	// Non-virtual functions

	void Set(NODE_LABEL nl, double el)		{ nodeLbl = nl; eigenLbl = el; }

	void SetNodeLbl(NODE_LABEL lbl)			{ nodeLbl = lbl; }
	NODE_LABEL GetNodeLbl() const			{ return nodeLbl; }

	void SetEigenLbl(double lbl)			{ eigenLbl = lbl; }
	double GetEigenLbl() const				{ return eigenLbl; }

	void SetMass(int m)						{ nMass = m; }
	void SetLevel(int l)					{ nLevel = l; }
	void SetDFSIndex(int i)				   	{ nDFSIndex = i; }

	//! Gets the color associated with the node
	leda::color GetColor() const            { return color; }

	//! Sets the color associated with the node
	void SetColor(leda::color col)          { color = col; }

	//! Sets the node's TSV assosiated with its adjacent nodes
	void SetChildrenTSV(const TSV& tsv) { childrenTSV = tsv; }

	TSV GetTSV(int nDim) const;
	const TSV& GetTSV()	const		{ return childrenTSV; }
	TSV& GetTSV()					{ return childrenTSV; }

	double GetTSVNorm()	const		{ return tsvNorm; }
	int GetMass() const				{ return nMass; }
	int GetLevel() const			{ return nLevel; }
	int GetDFSIndex() const			{ return nDFSIndex; }
	NODE_LABEL GetDFSLbl() const	{ return String("", nDFSIndex); }

	friend std::ostream& operator<<(std::ostream &os, const DAGNode& node)
	{
		return node.Write(os);
	}

	friend std::istream& operator>>(std::istream &is, DAGNode& node)
	{
		return node.Read(is);
	}
};
} //namespace dml

#endif //__DAGNODE_H__
