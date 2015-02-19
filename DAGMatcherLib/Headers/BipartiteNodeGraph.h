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
 * $Log: BipartiteNodeGraph.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __BIPARTITE_NODE_GRAPH_H__
#define __BIPARTITE_NODE_GRAPH_H__

#include <LEDA/core/p_queue.h>

#include "DAG.h"
#include "BipartiteGraph.h"
#include "NodeMatchInfo.h"

namespace dml {
/*!
	@brief A bipartite graph in which the node attributes
	are themselves references to nodes of other graphs.

	Each partition A and B correspondes to a set of nodes in
	the bipartite graph whose attributes are nodes in the
	same DAG (ie, either g1 or g2).

	Each edge e of the bipartite graph correspond to the similarity
	value between the "attributes of the attribute" of source(e) and
	the "attributes of the attribute" of target(e).
*/
class BipartiteNodeGraph : public BipartiteGraph<leda::node, NodeMatchInfoPtr>
{
public:
	typedef BipartiteGraph<leda::node, NodeMatchInfoPtr> BASE_CLASS;

	typedef BASE_CLASS::AssignmentInfo AssignmentInfo;
	typedef BASE_CLASS::AssignmentInfoList AssignmentInfoList;

	enum SORT_TYPE {DO_NOT_SORT = 0, SORT_BY_SIMILARITY,
		SORT_BY_CERTAINTY_AND_SIMILARITY};

protected:
	EdgeValueArray m_edgeValues;        //!< Permanent storage of edge values

	void ApplyEdgePenalties(const NodeMatchInfoPtr& ptrNMI, NodeToNodeArray nodeMap[],
		leda::node_array<bool> visited[]);

	bool UpdateAncestorInfo(const NodePairInfo& npi, int paramIdx, leda::node q,
		leda::node m, const double& penCoeff, NodeMatchInfoPtr& ptrEdgeInfo);

	bool UpdateDescendantInfo(const NodePairInfo& npi, int paramIdx, leda::node q,
		leda::node m, const double& penCoeff, NodeMatchInfoPtr& ptrEdgeInfo);

	void TransferEdgeValues();

	int GetDAGNodeDFSIndex(leda::node v) const
	{
		leda::node imageNode = inf(v);
		const DAG* pDAG = dynamic_cast<const DAG*>(graph_of(imageNode));

		return pDAG->GetNodeDFSIndex(imageNode);
	}

public:
	BipartiteNodeGraph()
	{
		// needed default constructor. there is nothing to do here.
	}

	BipartiteNodeGraph(const DAG& g1, const DAG& g2, bool bIncludeRootNodes)
	{
		Create(g1, g2, bIncludeRootNodes);
	}

	BipartiteNodeGraph(const BipartiteNodeGraph& src, const NodeMatchInfoPtr& ptrNMI)
	{
		Create(src, ptrNMI);
	}

	void Create(const DAG& g1, const DAG& g2, bool bIncludeRootNodes);
	void Create(const BipartiteNodeGraph& src, const NodeMatchInfoPtr& ptrNMI);

	//! Sorts the given list of node assignments
	void SortAssignment(AssignmentInfoList* pList, SORT_TYPE type)
	{
		if (type == SORT_BY_SIMILARITY)
		{
			pList->sort(CompareSimilarityDescend);
		}
		else if (type == SORT_BY_CERTAINTY_AND_SIMILARITY)
		{
			pList->sort(CompareCertaintyAndSimilarityDescend);
		}
	}

	/*!
		Solves the max weight assignment problem, fills the list of node
		assignments, and sorts it by 'sortType' in descending order.

		@return the sum of assignment values
	*/
	double SolveMaxWeightAssignment(AssignmentInfoList* pAssignmentInfo,
		                            SORT_TYPE sortType)
	{
		NodeAssigmentList L;
		double n;

		BASE_CLASS::SolveMaxWeightAssignment(m_edgeValues, &L);

		n = PopulateAssignmentInfoList(m_edgeValues, L, pAssignmentInfo);

		SortAssignment(pAssignmentInfo, sortType);

		double sum = 0;
		AssignmentInfo ai;

		forall(ai, *pAssignmentInfo)
			sum += ai.first();

		return sum;
	}

	void GetNonZeroAssignments(AssignmentInfoList* pAssignmentInfo, SORT_TYPE sortType)
	{
		leda::edge e;
		double w;

		pAssignmentInfo->clear();

		forall_edges(e, *this)
		{
			w = m_edgeValues[e];

			if (w > 0)
				pAssignmentInfo->push_back(AssignmentInfo(w, inf(e)));
		}

		SortAssignment(pAssignmentInfo, sortType);
	}

	//! Retrieves the similarity value associated with edge 'e'
	const double& SimilarityValue(leda::edge e) const
	{
		return m_edgeValues[e];
	}

	//! Compares two values such as to create an "descending" order
	static int CompareSimilarityDescend(
		const AssignmentInfo& a1, const AssignmentInfo& a2)
	{
		return BASE_CLASS::CompareValuesDescend(a1, a2);
	}

	//! Compares two values such as to create an "descending" order
	static int CompareCertaintyAndSimilarityDescend(
		const AssignmentInfo& a1, const AssignmentInfo& a2)
	{
		return NodeMatchInfo::CompareCertaintyAndSimilarityDescend(
			a1.second(), a2.second());
	}

	//! Prints the assigment information stored in edge 'e'
	void Print(std::ostream& os, leda::edge e) const
	{
		os << GetDAGNodeDFSIndex(source(e)) << "--"
			<< GetDAGNodeDFSIndex(target(e)) << ": ";

		inf(e)->Print(os);

		os << std::endl;
	}
};
} //namespace dml

#endif //__BIPARTITE_NODE_GRAPH_H__
