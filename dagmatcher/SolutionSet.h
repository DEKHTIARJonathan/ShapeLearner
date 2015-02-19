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
 * $Log: SolutionSet.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __SOLUTION_SET_H__
#define __SOLUTION_SET_H__

#include <LEDA/core/queue.h>
#include <LEDA/core/sortseq.h>

#include "BipartiteNodeGraph.h"
#include "NodePairInfo.h" // needed by FillNodeMap

namespace dml {
class SolutionSet;

typedef SharedPtr<SolutionSet> SolutionSetPtr;

//typedef leda::sortseq<double, SolutionSetPtr> SolutionQueue;

typedef leda::queue<SolutionSetPtr> SolutionQueue;

typedef leda::seq_item SolutionQueueItem;

/*!
	@brief It is a set that stores one-to-one assignemt correspondeces
	between the nodes of two DAGs. It represents the current state of
	a solution associated with some iteration of the DAG matching algorithm.
*/
class SolutionSet
{
public:
	typedef BipartiteNodeGraph::AssignmentInfo NodeAssignment;
	typedef BipartiteNodeGraph::AssignmentInfoList NodeAssignments;

protected:
	const SolutionSetPtr m_ptrParent;     //!< previous state of the solution set

	NodeMatchInfoPtr m_ptrNodeMatchInfo;  //!< Last node pair added to the set

	double m_nodeMatchSimilarity;         //!< Similarity value of the node match

	BipartiteNodeGraph m_bipGraph;        //!< Graph of candidate correspondences

	NodeAssignments m_nodeAssigments;     //!< One-to-one assignments of nodes

	double m_partialSimilarity;           //!< Similarity of nodes in the set

	double m_nodeSimilarityEstimate;      //!< Similarity estimate of nodes not in the set

public:
	SolutionSet(const DAG& g1, const DAG& g2, bool bIncludeRootNodes)
		: m_bipGraph(g1, g2, bIncludeRootNodes)
	{
		m_partialSimilarity = 0;
	}

	SolutionSet(const SolutionSetPtr ptrParent, NodeMatchInfoPtr ptrMatchInfo)
		: m_ptrParent(ptrParent), m_ptrNodeMatchInfo(ptrMatchInfo)
	{
		// Set the similarity value of the node match added to the set
		m_ptrNodeMatchInfo->GetPenalizedSimilarityValue(&m_nodeMatchSimilarity);

		// Set the partial similarity by adding the edgevalue to the
		// partial similarity of the parent set
		m_partialSimilarity = m_ptrParent->PartialSimilarity() + m_nodeMatchSimilarity;

		// Create a new bipartite graph that excludes the given node match
		m_bipGraph.Create(ptrParent->m_bipGraph, m_ptrNodeMatchInfo);

		// We need to solve the bipartite graph to get an estimate
		// of the unknown similarity. Simply init to known invalid value now
		m_nodeSimilarityEstimate = -1;
	}

	//! Gets the average number of nodes on each set in the bipartite graph
	double GetAverageNodeCount() const
	{
		return AVG(m_bipGraph.GetNodeCount(0), m_bipGraph.GetNodeCount(1));
	}

	//! Assigns one-to-one node correspondences between nodes in the two graphs
	void AssignOneToOneCorrespondences(BipartiteNodeGraph::SORT_TYPE sortType)
	{
		m_nodeSimilarityEstimate = m_bipGraph.SolveMaxWeightAssignment(
			&m_nodeAssigments, sortType);
	}

	//! Sort all nonzero node correspondences between nodes in the two graphs
	void AssignNonZeroCorrespondences(BipartiteNodeGraph::SORT_TYPE sortType)
	{
		m_bipGraph.GetNonZeroAssignments(&m_nodeAssigments, sortType);
	}

	const NodeAssignments& GetNodeAssignments() const
	{
		return m_nodeAssigments;
	}

	//! True if the set is the top set
	bool IsEmpty() const
	{
		return m_ptrParent.IsNull();
	}

	//! Similarity of the last node match added to the set
	const double& LastMatchSimilarity() const
	{
		ASSERT(!m_ptrNodeMatchInfo.IsNull());

		return m_nodeMatchSimilarity;
	}

	//! Similarity from the biginning and up to the current state
	double PartialSimilarity() const
	{
		return m_partialSimilarity;
	}

	/*!
		Estimate of the remaining similarity up to the state
		in which all nodes are matched
	*/
	double SimilarityEstimateToCompletion() const
	{
		ASSERT(m_nodeSimilarityEstimate >= 0);

		return m_nodeSimilarityEstimate;
	}

	//! Sum of partial similarity and the estimate of similarity to completion
	double TotalSimilarityEstimate() const
	{
		return PartialSimilarity() + SimilarityEstimateToCompletion();
	}

	/*!
		The total similarity is simple the partial similarity of a solution
		set that has no more candidate correspondeces to evaluate
	*/
	double TotalSimilarity() const
	{
		//ASSERT(m_bipGraph.CandidateCorrespondenceCount() == 0);

		return m_partialSimilarity;
	}

	/*!
		Populates the given map with the node matching information
		in the set
	*/
	void FillNodeMap(NodeMatchMap* pNodeMap) const
	{
		leda::node q, m;
		double sim;

		const SolutionSet* pSet = this;

		pNodeMap->init(m_ptrNodeMatchInfo->GetNodePair().Graph(0));

		while (!pSet->m_ptrNodeMatchInfo.IsNull())
		{
			q   = pSet->m_ptrNodeMatchInfo->GetNodePair().Node(0);
			m   = pSet->m_ptrNodeMatchInfo->GetNodePair().Node(1);
			sim = pSet->m_nodeMatchSimilarity;

			(*pNodeMap)[q].Set(m, sim);

			pSet = pSet->m_ptrParent;
		}
	}

	void GetNodeMatchList(leda::list<NodeMatchInfoPtr>* pList) const
	{
		const SolutionSet* pSet = this;

		pList->clear();

		while (!pSet->IsEmpty())
		{
			pList->push_back(pSet->m_ptrNodeMatchInfo);
			pSet = pSet->m_ptrParent;
		}

		pList->reverse();
	}

	std::ostream& Print(std::ostream& os) const
	{
		leda::list<NodeMatchInfoPtr> L;
		NodeMatchInfoPtr ptrNMI;

		GetNodeMatchList(&L);

		os << "\n{";

		forall(ptrNMI, L)
		{
			os << "\n";
			ptrNMI->Print(os);
			os << "\n";
		}

		return os << "}\n";
	}

	void PrintAssignments(std::ostream& os) const
	{
		NodeAssignment nodeAssig;
		double sum = 0;

		forall(nodeAssig, m_nodeAssigments)
		{
			sum += nodeAssig.first();

			os << "(" << nodeAssig.first() << ", ";

			nodeAssig.second()->Print(os);

			os << ")\n";
		}

		os << "Total is " << sum << "\n";
	}

	void PrintCandidateAssignments(std::ostream& os) const
	{
		leda::edge e;
		leda::node lastSrc = nil;

		os << "\nBeginning of candidate assignments\n";

		forall_edges(e, m_bipGraph)
		{
			if (source(e) != lastSrc)
				os << "\n";

			m_bipGraph.Print(os, e);

			lastSrc = source(e);
		}

		os << "\nEnd of candidate assignments\n\n";
	}
};
} //namespace dml

#endif //__SOLUTION_SET_H__