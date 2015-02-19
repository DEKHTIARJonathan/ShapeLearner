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
 *-----------------------------------------------------------------------*/

#ifndef __NODE_ASSIGNMENT_H__
#define __NODE_ASSIGNMENT_H__

#include <vector>
#include "SharedPtr.h"
#include "DAGMatcher.h"
//#include "DAG.h"

namespace dml {
class DAG;

// Define a shared pointer type for NodeMatchInfo objects
class NodeAssignment;
typedef SharedPtr<NodeAssignment> NodeAssignmentPtr; //!< shared pointer for node assignments

/*!
	Represents the information asssociated with an assignment between
	a pair of nodes, one of which is from a ``query'' graph and
	the other is from a ``model'' graph.
*/
class NodeAssignment
{
	double m_distance;    //!< Absolute difference between the trees rooted at the nodes

	double m_nodeSimilarity;  //!< Normalized similarity between the nodes
	double m_subtreeSimilaritySum;

	double m_weightedDistance; //!< Distance weigthed by edge similarity

	//double m_inEdgeSimilarity; //!< Similarity of in edge (if more than one, uses average)

	leda::node m_v0;        //!< Node in graph 0
	leda::node m_v1;        //!< Node in graph 1

	ParamIndices m_paramIndices;  //!< Indices of parameter combinations used to compute similarity

	leda::edge m_inEdge0;   //!< In edge connecting mathed parent node in graph 0
	leda::edge m_inEdge1;   //!< In edge connecting mathed parent node in graph 1

	//const NodeAssignment* m_pGivenParentAssignment;
	const NodeAssignment* m_pGivenChildAssignment;

	double m_givenChildAssignmentWeightedDistance;

	std::vector<NodeAssignmentPtr> m_parentAssignments;
	std::vector<NodeAssignmentPtr> m_childAssignments;

public:
	NodeAssignment(leda::node v0, leda::node v1, const ParamIndices& parInds,
		leda::edge e0, leda::edge e1)
	{
		m_v0 = v0;
		m_v1 = v1;

		m_paramIndices = parInds;

		m_inEdge0 = e0;
		m_inEdge1 = e1;

		//m_pGivenParentAssignment = NULL;
		m_pGivenChildAssignment  = NULL;

		m_parentAssignments.reserve(MAX(indeg(v0), indeg(v1)));
		m_childAssignments.reserve(MAX(outdeg(v0), outdeg(v1)));

		m_distance = -1;
		m_weightedDistance = -1;
		m_givenChildAssignmentWeightedDistance = -1;

		m_nodeSimilarity = -1;
		m_subtreeSimilaritySum = -1;
	}

	void operator=(const NodeAssignment& rhs)
	{
		m_distance = rhs.m_distance;

		m_nodeSimilarity = rhs.m_nodeSimilarity;

		m_subtreeSimilaritySum = rhs.m_subtreeSimilaritySum;

		m_weightedDistance = rhs.m_weightedDistance;

		m_v0 = rhs.m_v0;
		m_v1 = rhs.m_v1;

		m_paramIndices = rhs.m_paramIndices;

		m_inEdge0 = rhs.m_inEdge0;
		m_inEdge1 = rhs.m_inEdge1;

		m_pGivenChildAssignment = rhs.m_pGivenChildAssignment;

		m_givenChildAssignmentWeightedDistance = rhs.m_givenChildAssignmentWeightedDistance;

		m_parentAssignments = rhs.m_parentAssignments;
		m_childAssignments  = rhs.m_childAssignments;
	}

	/*void SetParentNodeAssignment(const NodeAssignment* pParentAssignment)
	{
		m_pGivenParentAssignment = pParentAssignment;
	}*/

	void SetGivenChildNodeAssignment(const NodeAssignment* pChildAssignment,
		const double& weightedDist)
	{
		m_pGivenChildAssignment = pChildAssignment;
		m_givenChildAssignmentWeightedDistance = weightedDist;
	}

	const ParamIndices& GetParamIndices() const
	{
		return m_paramIndices;
	}

	int ParamIndex(int i) const
	{
		return m_paramIndices[i];
	}

	bool HasDistance() const
	{
		return m_distance >= 0;
	}

	double Distance() const
	{
		ASSERT(m_distance >= 0);

		return m_distance;
	}

	double WeightedDistance() const
	{
		ASSERT(m_weightedDistance >= 0);

		return m_weightedDistance;
	}

	double NodeSimilarity() const
	{
		ASSERT_UNIT_INTERVAL(m_nodeSimilarity);

		return m_nodeSimilarity;
	}

	double SubtreeSimilaritySum() const
	{
		ASSERT(m_subtreeSimilaritySum >= 0);

		return m_subtreeSimilaritySum;
	}

	/*double InEdgeSimilarity() const
	{
		ASSERT_UNIT_INTERVAL(m_inEdgeSimilarity);

		return m_inEdgeSimilarity;
	}*/

	void SetDistance(const double& dist)
	{
		ASSERT(dist >= 0);

		m_distance = dist;
	}

	void SetWeightedDistance(const double& dist)
	{
		ASSERT(dist >= 0);

		m_weightedDistance = dist;
	}

	void SetNodeSimilarity(const double& sim)
	{
		ASSERT_UNIT_INTERVAL(sim);

		m_nodeSimilarity = sim;
	}

	void SetSubtreeSimilaritySum(const double& sim)
	{
		ASSERT(sim >= 0);

		m_subtreeSimilaritySum = sim;
	}

	/*void SetInEdgeSimilarity(const double& edgeSim)
	{
		ASSERT_UNIT_INTERVAL(edgeSim);

		if (m_inEdgeSimilarity >= 0)
			m_inEdgeSimilarity = (m_inEdgeSimilarity + edgeSim) / 2;
		else
			m_inEdgeSimilarity = edgeSim;
	}*/

	const DAG* Graph(int i) const
	{
		ASSERT(i == 0 || i == 1);

		if (i == 0)
			return (const DAG*) graph_of(m_v0);
		else
			return (const DAG*) graph_of(m_v1);
	}

	leda::node Node(int i) const
	{
		ASSERT(i == 0 || i == 1);

		return (i == 0) ? m_v0 : m_v1;
	}

	leda::edge InEdge(int i) const
	{
		ASSERT(i == 0 || i == 1);

		return (i == 0) ? m_inEdge0 : m_inEdge1;
	}

	bool IsRoot(int i) const
	{
		ASSERT(i == 0 || i == 1);

		int deg = (i == 0) ? indeg(m_v0) : indeg(m_v1);

		return (deg == 0);
	}

	bool IsGivenChildAssignment(int i, leda::node v) const
	{
		if (m_pGivenChildAssignment && m_pGivenChildAssignment->Node(i) == v)
			return true;
		else
			return false;
	}

	bool HasGivenChildAssignment() const
	{
		return (m_pGivenChildAssignment != NULL);
	}

	//! Returns the weighted distance of the child assignment
	double GetGivenChildAssignmentWeightedDistance() const
	{
		ASSERT(HasGivenChildAssignment());
		ASSERT(m_givenChildAssignmentWeightedDistance >= 0);

		return m_givenChildAssignmentWeightedDistance;
	}

	bool HasParentAssignments() const
	{
		return !m_parentAssignments.empty();
	}

	bool HasChildAssignments() const
	{
		return !m_childAssignments.empty();
	}

	void AddParentAssignment(const NodeAssignmentPtr& ptrNA)
	{
		m_parentAssignments.push_back(ptrNA);
	}

	void AddChildAssignment(const NodeAssignmentPtr& ptrNA)
	{
		m_childAssignments.push_back(ptrNA);
	}

	void CopyAssignment(NodeMatchMap& nodeMap) const
	{
		ASSERT(nodeMap[Node(0)].node == nil);

		nodeMap[Node(0)].Set(Node(1), NodeSimilarity(), ParamIndices());
	}

	void CopyParentAssignments(NodeMatchMap& nodeMap) const
	{
		for (unsigned int i = 0; i < m_parentAssignments.size(); i++)
		{
			const NodeAssignmentPtr& ptrNA = m_parentAssignments[i];

			ptrNA->CopyAssignment(nodeMap);

			ptrNA->CopyChildAssignments(nodeMap);

			ptrNA->CopyParentAssignments(nodeMap);
		}
	}

	void CopyChildAssignments(NodeMatchMap& nodeMap) const
	{
		for (unsigned int i = 0; i < m_childAssignments.size(); i++)
		{
			const NodeAssignmentPtr& ptrNA = m_childAssignments[i];

			ptrNA->CopyAssignment(nodeMap);

			ptrNA->CopyChildAssignments(nodeMap);
		}
	}

	void Print(std::ostream& os = std::cout, int numTabs = 0) const;

	friend std::ostream& operator<<(std::ostream &os, const NodeAssignment& na)
	{
		return (os << na.m_weightedDistance);
	}

	friend std::istream& operator>>(std::istream &is, NodeAssignment& na)
	{
		return (is >> na.m_weightedDistance);
	}
};
} //namespace dml

#endif //__NODE_ASSIGNMENT_H__
