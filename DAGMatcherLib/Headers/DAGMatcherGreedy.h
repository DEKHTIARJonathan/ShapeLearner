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

#ifndef __DAG_MATCHER_GREEDY_H__
#define __DAG_MATCHER_GREEDY_H__

#include "stdafx.h"

namespace dml {
/*!
	Greedy DAG matching algorithm.
*/
class DAGMatcherGreedy : public DAGMatcher
{
	DAGNodePtrMap m_nodeMap1; //!< One-to-one map from g1 nodes to g2 nodes
	DAGNodePtrMap m_nodeMap2; //!< One-to-one map from g2 nodes to g1 nodes

	DAGNodeSimilarityMap m_nodeSimMap1; //!< node similarity from query to model

	NodeMatchMap m_nodeMatchMap; //!< One-to-one map from g1 nodes to g2 nodes

protected:
	leda::edge ComputeBipartiteGraph(leda::GRAPH<leda::node, double>& G,
		const DAG& g1, const DAG& g2, SimMatrix& simMat,
		MatchedNodePair* pMatchedPair) const;

	double Match(DAG& g1, DAG& g2, SimMatrix& simMat,
		MatchedNodePair* pMatchedPair);

	void FillNodeMap(const DAG& query, const DAG& model);

public:
	DAGMatcherGreedy(SimilarityMeasurer* pNodeDistMeasurer)
		: DAGMatcher(pNodeDistMeasurer)
	{
	}

	virtual ~DAGMatcherGreedy()
	{
		// Must call the destructor of the base class
		//DAGMatcher::~DAGMatcher();
		delete m_pSimilarityMeasurer;
	}

	virtual double Match(const DAG& g1, const DAG& g2);

	//! Gets a one-to-one map from g1 nodes to g2 nodes
	virtual void GetNodeMap(NodeMatchMap& nodeMap) const
	{
		nodeMap = m_nodeMatchMap;
	}
};
} //namespace dml

#endif //__DAG_MATCHER_GREEDY_H__
