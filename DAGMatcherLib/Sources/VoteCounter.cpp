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
 * $Log: VoteCounter.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include <LEDA/core/tuple.h>
#include <LEDA/graph/graph_alg.h>
#include <LEDA/graph/node_array.h>

#include "VoteCounter.h"

#ifdef USE_TEMPLATE_BASED_MWBM_FUNCTION
#include <LEDA/graph/templates/mwb_matching.h>
using namespace std; // Fixes an error in the next LEDA header scale_weights.h line 35
#include <LEDA/graph/scale_weights.h>
#else
#include <LEDA/graph/mwb_matching.h>
#endif

//#include "ShockGraph.h" // needed for ShowGraph()
//#include "VisualDAG.h"  // needed for ShowGraph()

//#include <LEDA/templates/mwb_matching.t>

#define MAX_MODEL_NODE_COUNT 1024

using namespace dml;
using namespace MOOVC;
using namespace leda;

SmartArray<leda_node> PartitionBins::modelNodeMap(MAX_MODEL_NODE_COUNT);

/*!
	@brief Symmetric function that determines whether the range queries
	centered at pt1 and pt2 overlap.
*/
bool QueryRange::DoPtsOverlap(const TSV pt1, const TSV pt2) const
{
	ASSERT(GetSize() == pt1.GetSize());
	ASSERT(GetSize() == pt2.GetSize());

	double min1, max1, min2, max2;

	for (int i = 0; i < GetSize(); i++)
	{
		GetDimBounds(pt1[i], i, min1, max1);
		GetDimBounds(pt2[i], i, min2, max2);

		if (max1 < min2 || min1 > max2)
			return false;
	}

	return true;
}

void PartitionBins::CreateVoteGraph(LEDA_GRAPH<int, double>& g) const
{
	leda_node q;
	int queryNode, modelNode;
	double weight;
	leda_pq_item it;

	ASSERT(g.empty());

	// Init the map of model nodes
	modelNodeMap.Set(nil);

	// Add the model nodes and the edge weights to the graph of node votes
	for (queryNode = 0; queryNode < GetNodeCount(); queryNode++)
	{
		if (!bins[queryNode].empty())
		{
			// Add the query node to the graph of node votes
			q = g.new_node(queryNode);

			forall_items(it, bins[queryNode])
			{
				modelNode = bins[queryNode].inf(it);
				weight = bins[queryNode].prio(it);

                ASSERT(modelNode >= 0);

				if (modelNodeMap[modelNode] == nil)
					modelNodeMap[modelNode] = g.new_node(modelNode);

				g.new_edge(q, modelNodeMap[modelNode], weight);
			}
		}
	}
}

double PartitionBins::CompPartitionVote() const
{
	LEDA_GRAPH<int, double> g;
	leda_edge e;
	leda_list_item it;

	// Quick test
	if (GetCumulativeVote() == 0.0)
		return 0;

	// Since there have been some votes, we must do work now...
	CreateVoteGraph(g);

	leda_edge_array<double> edgeCosts(g);

	// Gather the weigths in an array and let the weights in the graph be equal to 1.
	forall_edges(e, g)
	{
		edgeCosts[e] = g.inf(e);
		g[e] = 1.0;
	}

#ifdef USE_TEMPLATE_BASED_MWBM_FUNCTION
	scale_weights(g, edgeCosts, 3.0);
	leda_list<leda_edge> voteMapping = MAX_WEIGHT_BIPARTITE_MATCHING_T(g, edgeCosts);
#else
	MWBM_SCALE_WEIGHTS(g, edgeCosts);
	leda_list<leda_edge> voteMapping = MAX_WEIGHT_BIPARTITE_MATCHING(g, edgeCosts);
#endif //USE_TEMPLATE_BASED_MWBM_FUNCTION

	double partitionVote = 0.0;

	// Sum up the votes in the mapping
	forall_items(it, voteMapping)
	{
		partitionVote += edgeCosts[voteMapping[it]];
		//cerr << g.inf(source(voteMapping[it])) << "=>" << g.inf(target(voteMapping[it])) << endl;
	}

	ASSERT(partitionVote >= 0.0 && partitionVote <= 1.0);

	return partitionVote;
}

void ModelBins::AddPartitionVotes(Partition& part, const DAGPtr ptrQueryDag, DAGDatabase& modelDB,
	const QueryRange& range, double dQueryNodeRelMass)
{
	VoteBalance balance(modelDB.GetMaxTSVDimension(), dQueryNodeRelMass);
	SmartArray<DAGSearchRecordEx> closestModelNodes;
	int idxNode, idxModel, i;
	VOTE vote;
	//QueryRange rangeCheck(range.GetDimDelta(), modelDB.GetMaxTSVDimension());

	// Set size (and, implicitly, bounds) for each partition bin
	for (idxModel = 0; idxModel < modelDB.GetDAGCount(); idxModel++)
		partBins[idxModel].SetPartitionSize(part.NodeCount());

	// Do NN-search for each node in the partition
	for (idxNode = 0; idxNode < part.NodeCount(); idxNode++)
	{
		vote.queryNode = part[idxNode];
		balance.SetQueryNode(ptrQueryDag, vote.queryNode);

		closestModelNodes = modelDB.GetIndexDB().GetClosest(*ptrQueryDag, vote.queryNode, range.GetDimDelta());

		// Add each node's vote to the partition bin
		// of the corresponding model
		for (i = 0; i < closestModelNodes.GetSize(); i++)
		{
			const DAGSearchRecordEx& modelNodeInfo = closestModelNodes[i];
			//ASSERT(rangeCheck.DoPtsOverlap(balance.GetQueryNodeTSV(), modelNodeInfo.nodeTSV));

			if (vote.dWeight = balance.Weight(modelNodeInfo))
			{
    			vote.nModelGraph = modelNodeInfo.id;
    			vote.nModelNode = modelNodeInfo.node;

    			partBins[vote.nModelGraph].AddVote(idxNode, vote);
		   }
		}
	}

	// Add up the total vote for each model's partition
	for (idxModel = 0; idxModel < modelDB.GetDAGCount(); idxModel++)
	{
		AddVote(idxModel, partBins[idxModel].CompPartitionVote());
	}
}

SmartArray<VOTE> ModelBins::GetRankedVotes() const
{
	// Make sure that there are some valid models to sort
	if (nModelsAboveMinVote == 0)
		return SmartArray<VOTE>(); // nothing to do, return am empty array

	// There is at least one model to consider
	SmartArray<VOTE> sortedVotes(nModelsAboveMinVote);

	for (int i = 0, j = 0; i < modelVotes.GetSize(); i++)
	{
		if (modelVotes[i] >= dMinTotalModelVote)
		{
			VOTE& vote = sortedVotes[j++];

			vote.dWeight = modelVotes[i];
			vote.nModelGraph = i;
		}
	}

	sortedVotes.Sort(VOTE::Compare);

	return sortedVotes;
}

/*!
	@brief Any pair of points that can potentially vote for the same model node
	is said to overlap. The overlapping relation may or may not be symmetric. If it is,
	the problem of finding pairs of overlapping nodes is a bit simplified.

	Since now we are working with a query range based on percentages at each dimension,
	we have a non symmetric overlapping relation. The following code should work for any
	kind of relation.
*/
DAGPtr MOOVC::ComputeOverlappingNodeGraph(const DAGPtr ptrQueryDag, const QueryRange& r)
{
	leda_node u, v;

	DAGPtr ptrDag = ptrQueryDag->CreateObject();

	// Copy the graph info but not its nodes or edges. Simply do:
	*ptrDag = *ptrQueryDag;
	ptrDag->del_all_nodes(); //this will later re-index the nodes

	// Create a graph with the nodes in the query graph. Note that
	// leaf nodes don't vote, so we must not include them in the new graph
	forall_nodes(u, *ptrQueryDag)
		if (ptrQueryDag->GetNodeMass(u) > 1)
			ptrDag->NewNode(ptrQueryDag->GetNode(u));

	// We can speed up things a bit by computing all the TSVs with the right
	// dimension only once.
	SmartArray<TSV> tsvs(ptrDag->GetNodeCount());
	int tsvdim = r.GetSize();
	TSV pt1, pt2;

	forall_nodes(u, *ptrDag)
		tsvs[ptrDag->GetNodeIndex(u)] = ptrDag->GetNodeTSV(u, tsvdim);

	// Find the edges of the overlap graph. Since the overlapping relation is symmetric,
	// we only need to do N! comparisons.
	for (u = ptrDag->first_node(); u != nil; u = ptrDag->succ_node(u))
	{
		pt1 = tsvs[ptrDag->GetNodeIndex(u)];

		for (v = ptrDag->succ_node(u); v != nil; v = ptrDag->succ_node(v))
		{
			pt2 = tsvs[ptrDag->GetNodeIndex(v)];

			if (r.DoPtsOverlap(pt1, pt2))
				ptrDag->NewEdge(u, v);
		}
	}

	//ShowGraph(*ptrDag);

	return ptrDag;
}

PartitionArray MOOVC::FindPartitions(const DAGPtr ptrOverlapDag)
{
	leda_node v;

	// Solve the connected components problem
	leda_node_array<int> compnum(*ptrOverlapDag);
	int c = COMPONENTS(*ptrOverlapDag, compnum);

	// Create the partitions
	PartitionArray parts(ptrOverlapDag);
	parts.SetSize(c);

	forall_nodes(v, *ptrOverlapDag)
		parts.AddElement(compnum[v], v);

	return parts;
}

NNVoteArray dml::ComputeVotesForModelGraphs(const DAGPtr ptrQueryDag, DAGDatabase& modelDB,
	const QueryRange& range, double dQueryNodeRelMass, double dMinSimilarity)
{
	  ModelBins modelBins(modelDB.GetDAGCount(), dMinSimilarity);
	  DAGPtr ptrOverlapGraph = ComputeOverlappingNodeGraph(ptrQueryDag, range);
	  PartitionArray partitions = FindPartitions(ptrOverlapGraph);

	  for (int i = 0; i < partitions.GetSize(); i++)
		  modelBins.AddPartitionVotes(partitions[i], ptrOverlapGraph, modelDB, range, dQueryNodeRelMass);

	  return modelBins.GetRankedVotes();
}