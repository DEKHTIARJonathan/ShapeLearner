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

#include "stdafx.h"

using namespace dml;

#ifdef _DEBUG
extern LogFile g_dagMatchingLog;
#endif

int g_counter;

/*!
	@brief

	Note: g1 is assumed to be the query graph and g2 the model graph.
*/
double DAGMatcherOptimal::Match(const DAG& g1, const DAG& g2)
{
	SolutionQueue sq;

	g_counter = 0;

	m_bIsSolSetFull = false;

	//g_dagMatchingLog.precision(50);
	DBG_M_LOG("Matching " << g1.GetDAGLbl() << " against " << g2.GetDAGLbl())
	DBG_M_LOG_ONLY(DAGMatcher::GetMatchParams().Print(g_dagMatchingLog))

	// Set the type of sorting that we used for the node assignments
	//m_sortType = BipartiteNodeGraph::SORT_BY_SIMILARITY;
	//m_sortType = BipartiteNodeGraph::SORT_BY_CERTAINTY_AND_SIMILARITY;
	//m_sortType = nNodeAssignSortType

	InitGraphs(&g1, &g2);

	// Start by setting the best set as the empty set
	m_ptrBestSet = new SolutionSet(g1, g2, false /*ie, don't include root nodes*/);

	const int N0 = g1.GetNodeCount() - 1;
	const int N1 = g2.GetNodeCount() - 1;

	//const double maxSimVal = MAX(N0,N1);

	//const double maxSimVal = m_ptrBestSet->GetAverageNodeCount();

	DBG_M_LOG_ONLY(m_ptrBestSet->PrintCandidateAssignments(g_dagMatchingLog))

	// Solve the full node correspondece problem
	m_ptrBestSet->AssignOneToOneCorrespondences(GetSortType());

	// Insert the set to the queue so that it can be retrieved later
	//sq.insert(m_ptrBestSet->TotalSimilarityEstimate(), m_ptrBestSet);
	sq.append(m_ptrBestSet);

	// Update m_ptrBestSet with the true best set
	FindBestNodeAssigment(sq);

	DBG_M_LOG("Matches in order\n")
	DBG_M_LOG_ONLY(m_ptrBestSet->Print(g_dagMatchingLog))

	//m_ptrBestSet->Print(std::cout);
	//std::cout << " iter count " << g_counter << std::flush;

	//const double maxSimVal = MAX(N0,N1);

	const double maxSimVal = ComputeNormalizationFactor(g1, g2);

	double graphSim = m_ptrBestSet->TotalSimilarity() / maxSimVal;

	ASSERT_UNIT_INTERVAL(graphSim);

	return graphSim;
}

/*!
	@brief Performs an A* search for the best one-to-one node assigment between
	the the two graphs associated with the solution sets in the queue.

	The algorithm starts by retrieving the solution set with highest
	"total similarity estimate" from the queue andprocessing it if such similarity
	value is higher than the partial similarity of the best set seen.

	Note: the "best set" is the solution set that has the highest "partial
	similarity" value among all solution sets seen. Since such similarity is not
	an estimate, it represents a conservative value of the final similarity,
	while the total similarity estimate is an optimistic similarity.

	@param sq queue of solution sets to process order by prority
	@param m_ptrBestSet shared pointer to the known set with highest partial similarity

	@return m_ptrBestSet a new solution set with higher partial similarity (if there is any)
*/
void DAGMatcherOptimal::FindBestNodeAssigment(SolutionQueue& sq)
{
	if (sq.empty())
		return;

	DBG_M_LOG("Begin processing a solution set\n")

	g_counter++;

	SolutionSetPtr ptrSet = sq.pop();

	const SolutionSet::NodeAssignments& nodeAssigList = ptrSet->GetNodeAssignments();
	SolutionSet::NodeAssignment nodeAssig;
	SolutionSetPtr ptrNewSet;
	int localBranching = 0;

	double weight;

	//DBG_M_LOG_ONLY(ptrSet->PrintAssignments(g_dagMatchingLog))

	// Iterate through all uncommitted correspondences in the set (there may be none)
	// The correspondences are sorted by decreasing similarity value
	forall(nodeAssig, nodeAssigList)
	{
		nodeAssig.second()->GetPenalizedSimilarityValue(&weight);

		if (weight == 0)
			break;

		// Create a new solution set with the current node correspondece in it
		ptrNewSet = new SolutionSet(ptrSet, nodeAssig.second());

		// Reassign the node correspondences by accounting for the
		// constraints imposed by the commited correspondece
		if (DAGMatcher::GetMatchParams().nUseMWBMHeuristic)
			ptrNewSet->AssignOneToOneCorrespondences(GetSortType());
		else
			ptrNewSet->AssignNonZeroCorrespondences(GetSortType());

		// If the new set has a higher partial similarity,
		// it becomes the new "gold standard" used to prune the search
		if (ptrNewSet->PartialSimilarity() >= m_ptrBestSet->PartialSimilarity())
			m_ptrBestSet = ptrNewSet;

		sq.append(ptrNewSet);

		localBranching++;

		if (sq.size() >= GetMatchParams().nMaxNumSolSets)
		{
			m_bIsSolSetFull = true;
			break;
		}

		if (localBranching >= GetMatchParams().nMaxSolSetsPerIter)
			break;
	}

	// Process the local queue first (ie, do a depth search) in order
	// to improve the best partial similarity known
	FindBestNodeAssigment(sq);
}

double GetBGNodeSaliency(leda::node v, const DAG& g, const double& maxLength)
{
	try {
		const BoneGraph& bg = dynamic_cast<const BoneGraph&>(g);

		return MIN(1, bg.GetBGNode(v)->Length() / maxLength);
	}
    //catch(std::bad_cast) {
    catch(...) {
        ShowError("Can't cast to BoneGraph\n");
		return NULL;
    }

	//return GetBGNode(v, g)->Length() / 20;
}

double TotalSaliency(const DAG& g, const double& maxLength)
{
	leda::node v;
	double totSal = 0;

	forall_nodes(v, g)
	{
		totSal += GetBGNodeSaliency(v, g, maxLength);
	}

	return totSal;
}

double DAGMatcherOptimal::ComputeNormalizationFactor(const DAG& g1, const DAG& g2)
{
	const double maxLength = s_matchParams.dSaliencyParam;

	double totSal1 = TotalSaliency(g1, maxLength);
	double totSal2 = TotalSaliency(g2, maxLength);

	double matSal1 = 0;
	double matSal2 = 0;

	int n = 0;

	NodeMatchMap nodeMap;

	GetNodeMap(nodeMap);

	leda::node v;

	forall_nodes(v, g1)
	{
		if (!nodeMap[v].IsEmpty())
		{
			matSal1 = GetBGNodeSaliency(v, g1, maxLength);
			matSal2 = GetBGNodeSaliency(nodeMap[v].node, g2, maxLength);
			n++;
		}
	}

	double unmatchedSal = totSal1 + totSal2 - matSal1 - matSal2;

	return n + unmatchedSal;
}