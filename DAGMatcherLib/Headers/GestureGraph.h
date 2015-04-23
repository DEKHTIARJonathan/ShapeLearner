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
 * $Log: GestureGraph.h,v $
 * Revision 1.1  2002/05/21 16:52:20  dmac
 * New file added.
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef __GESTURE_GRAPH_H__
#define __GESTURE_GRAPH_H__

#include "stdafx.h"

namespace dml {
/*!
	\brief Represents a gesture graph consisting of <GGNode *>'s.

	\see DAG, DAGNode, GGNode.
*/
class GestureGraph : public DAG
{
	SmartArray<int> hierarchyLevels; 	 			//!< Vector of hierarchy levels for the vertices
	SmartMatrix<int> hierarchicalMatrix; 			//!< Matrix of the hierarchy of the graph
	SmartMatrix<GGRelation> relationMatrix;			//!< Matrix of the relations
	int m_nRootNodeCount;			                //!< Number of roots in the graph

	static DAGMatcher* s_pDAGMatcher;               //!< Matching algorithm for gesture graphs

public:
	GestureGraph() { m_nRootNodeCount = 0; }

	const GGRelation& GetRelation(int v1, int v2) const
	{
		return relationMatrix[v1 - 2][v2 - 1];
	}

	void BuildMatrices(Matrix& euclideanDistance, Matrix& bearing);
	void BuildNodeHistograms(const Matrix& euclideanDistance, const Matrix& bearing);
	bool Read(String strFileName);

	// DAG virtual functions
	virtual DAG& operator=(const DAG& rhs);
	virtual void Clear();

	// DAG pure virtual functions
	virtual DAGMatcher* GetMatchingAlgorithm() const { return s_pDAGMatcher; }

	virtual void SetMatchingAlgorithm();
	virtual bool AreNodesRelated(leda_node g1Node, const DAG& g2, leda_node g2Node) const;
	virtual DAG* CreateObject() const;
	virtual DAGNodePtr CreateNodeObject(NODE_LABEL lbl) const;
	virtual DAGNodePtr ReadNode(std::istream& is) const;
	virtual String ClassName() const;
	virtual int NodeType(leda_node v) const { return 0; }

	virtual const SkeletalGraph* GetSkeleton() const { return NULL; }

	virtual std::istream& Read(std::istream& is, bool bOnlyDataForMatching = false);
	virtual std::ostream& Write(std::ostream& os) const;
	virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;

	const GGNode* GetGGNode(leda_node v) const { return (const GGNode*)(const DAGNode*)GetNode(v); }
	GGNode* UnsafeGetGGNode(leda_node v) { return (GGNode*)GetGGNode(v); }
};
} //namespace dml

#endif //__GESTURE_GRAPH_H__
