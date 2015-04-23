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

#ifndef __DAG_MATCHER_ADAPTIVE_H__
#define __DAG_MATCHER_ADAPTIVE_H__

#include "stdafx.h"

namespace dml {
/*!
	Generic class for matching DAGs.
*/
class DAGMatcherAdaptive : public DAGMatcher
{
	struct EdgePair
	{
		leda::edge e0, e1;

		void Clear()
		{
			e0 = nil;
			e1 = nil;
		}
	};

	typedef std::vector<EdgePair> SkipEdges;

	NodeAssignmentPtr m_ptrRootAssignment;

	std::vector<leda::node> m_roots0;
	std::vector<leda::node> m_roots1;

protected:

	double ComputeRootedTreeSimilarity(NodeAssignmentPtr ptrNodeAss,
		const SkipEdges& skipEdges, const double& similarity) const;

	void ComputeRootedTreeSimilarity(NodeAssignmentPtr ptrNodeAss) const;

	double ComputeNodeSimilarity(NodeAssignmentPtr ptrNodeAss) const
	{
		return m_pSimilarityMeasurer->ComputeNodeSimilarity(
			ptrNodeAss->Node(0), ptrNodeAss->Node(1),
			ptrNodeAss->GetParamIndices());
	}

	double ComputeEdgeSimilarity(leda::edge e0, leda::edge e1,
		const ParamIndices& parInds) const
	{
		return m_pSimilarityMeasurer->ComputeEdgeSimilarity(e0, e1, parInds);
	}

	double GetGraphSimilarity() const;

public:
	DAGMatcherAdaptive(SimilarityMeasurer* pNodeDistMeasurer)
		: DAGMatcher(pNodeDistMeasurer)
	{
	}

	virtual ~DAGMatcherAdaptive()
	{
		// Must call the destructor of the base class
		//DAGMatcher::~DAGMatcher();
		delete m_pSimilarityMeasurer;
	}

	virtual void Clear()
	{
		DAGMatcher::Clear();

		m_roots0.clear();
		m_roots1.clear();

		m_ptrRootAssignment = NULL;
	}

	virtual double Match(const DAG& g1, const DAG& g2);

	//! Gets a one-to-one map from g1 nodes to g2 nodes
	virtual void GetNodeMap(NodeMatchMap& nodeMap) const;
};
} //namespace dml

#endif //__DAG_MATCHER_ADAPTIVE_H__
