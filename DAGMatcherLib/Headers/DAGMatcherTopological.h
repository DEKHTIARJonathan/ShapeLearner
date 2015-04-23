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

#ifndef __DAG_MATCHER_TOPOLOGICAL_H__
#define __DAG_MATCHER_TOPOLOGICAL_H__


namespace dml {
/*!
	Generic class for matching DAGs.
*/
class DAGMatcherTopological : public DAGMatcher
{
	NodeAssignmentPtr m_ptrRootAssignment;

protected:

	double ComputeParentsDistance(NodeAssignmentPtr ptrChildNodeAss); //const;

	void ComputeRootedTreeDistance(NodeAssignmentPtr ptrNodeAss);// const;

	double GraphDistance(NodeAssignmentPtr ptrNodeAss);// const;

	double ComputeNodeDistance(NodeAssignmentPtr ptrNodeAss) const
	{
		double similarity, distance;

		distance = m_pSimilarityMeasurer->ComputeNodeDistance(
			ptrNodeAss->Node(0), ptrNodeAss->Node(1),
			ptrNodeAss->GetParamIndices(), &similarity);

		ptrNodeAss->SetNodeSimilarity(similarity);

		return distance;
	}

	double ComputeEdgeSimilarity(leda::edge e0, leda::edge e1,
		const ParamIndices& parInds) const
	{
		return m_pSimilarityMeasurer->ComputeEdgeSimilarity(e0, e1, parInds);
	}

	double GetGraphSimilarity() const;

	inline NodeAssignmentPtr GetNodeAssignment(leda::node v0, leda::node v1,
		const ParamIndices& parInds);

public:
	DAGMatcherTopological(SimilarityMeasurer* pNodeDistMeasurer)
		: DAGMatcher(pNodeDistMeasurer)
	{
	}

	virtual ~DAGMatcherTopological()
	{
		// Must call the destructor of the base class
		//DAGMatcher::~DAGMatcher();
		delete m_pSimilarityMeasurer;
	}

	virtual double Match(const DAG& g1, const DAG& g2);

	//! Gets a one-to-one map from g1 nodes to g2 nodes
	virtual void GetNodeMap(NodeMatchMap& nodeMap) const;
};
} //namespace dml

#endif //__DAG_MATCHER_TOPOLOGICAL_H__
