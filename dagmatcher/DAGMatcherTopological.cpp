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
//#include <list>
#include "DAGMatcherTopological.h"
#include "DAG.h"
#include "BipartiteGraph.h"
#include "BoneGraph.h" // temporary solution
#include "HelperFunctions.h"
#include "BasicUtils.h"

using namespace dml;

#ifdef _DEBUG
extern LogFile g_dagMatchingLog;
#endif

typedef BipartiteGraph<leda::node, NodeAssignmentPtr> NodeAssignmentGraph;
typedef BipartiteGraph<leda::edge, NodeAssignmentPtr> EdgeAssignmentGraph;

inline double AvgNodeLevel(const NodeAssignmentPtr& ptrNA)
{
	return (ptrNA->Graph(0)->GetNodeLevel(ptrNA->Node(0)) +
		ptrNA->Graph(1)->GetNodeLevel(ptrNA->Node(1))) / 2.0;
}

const BGNode* GetBGNode(leda::node v, const DAG& g)
{
	try {
		const BoneGraph& bg = dynamic_cast<const BoneGraph&>(g);

		return bg.GetBGNode(v);
	}
    //catch(std::bad_cast) {
    catch(...) {
        ShowError("Can't cast to BoneGraph\n");
		return NULL;
    }
}

double TotalLength(const DAG& g)
{
	leda::node v;
	double totLen = 0;

	forall_nodes(v, g)
	{
		totLen += GetBGNode(v, g)->Length();
	}

	return totLen;
}

double MaxLength(const DAG& g)
{
	leda::node v;
	double len, maxLen = 0;

	forall_nodes(v, g)
	{
		len = GetBGNode(v, g)->Length();

		if (len > maxLen)
			maxLen = len;
	}

	return maxLen;
}

/*!
	@brief

	Note: g1 is assumed to be the query graph and g2 the model graph.
*/
double DAGMatcherTopological::Match(const DAG& g0, const DAG& g1)
{
	NodeAssignmentPtr ptrNodeAss;
	double dist, minDist = -1;
	leda::node v0, v1;
	ParamIndices parInds(1);

	ASSERT(g0.GetDAGCost() > 0 && g1.GetDAGCost() > 0);

	const bool bIncludeRootNodes = true;

	m_ptrRootAssignment = NULL;

	// Set member variables so that the graph can be accessed from any member functions
	// And by the similarity measurer object
	InitGraphs(&g0, &g1);

	// Create a graph with n1 + n2 nodes and save the nodes
	// associated with the g1 and g2 in different sets
	forall_nodes(v0, g0)
	{
		if (!bIncludeRootNodes && g0.IsRootNode(v0))
			continue;

		if (g0.GetNodeLevel(v0) != 1)
			continue;

		forall_nodes(v1, g1)
		{
			if (!bIncludeRootNodes && g1.IsRootNode(v1))
				continue;

			if (g1.GetNodeLevel(v1) != 1)
				continue;

			// Try all possible combinations of parameter values
			for(parInds[0] = 0; parInds[0] < 4; parInds[0]++)
			{
				// Create a new match info object
				ptrNodeAss = new NodeAssignment(v0, v1, parInds, nil, nil);

				// Compute node similarity and save it into the match info object
				dist = GraphDistance(ptrNodeAss);

				if (m_ptrRootAssignment.IsNull() || dist < minDist)
				{
					m_ptrRootAssignment = ptrNodeAss;
					minDist = dist;
				}
				else if (dist == minDist &&
					AvgNodeLevel(ptrNodeAss) < AvgNodeLevel(m_ptrRootAssignment))
				{
					m_ptrRootAssignment = ptrNodeAss;
				}
			}
		}
	}

	ASSERT(minDist >= 0);

	return GetGraphSimilarity();
}

double DAGMatcherTopological::GraphDistance(NodeAssignmentPtr ptrNodeAss) //const
{
	ComputeRootedTreeDistance(ptrNodeAss);

	double parentDist = ComputeParentsDistance(ptrNodeAss);

	return ptrNodeAss->Distance() + parentDist;
}

void DAGMatcherTopological::ComputeRootedTreeDistance(NodeAssignmentPtr ptrNodeAss) //const
{
	double nodeAttDist = ComputeNodeDistance(ptrNodeAss);

	// Create BipGraph G. Exclude edges whose target is any of the known nodes
	EdgeAssignmentGraph g;
	leda::node v;
	leda::edge e;

	for (int setId = 0; setId <= 1; setId++)
	{
		v = ptrNodeAss->Node(setId);

		forall_adj_edges(e, v)
		{
			if (!ptrNodeAss->IsGivenChildAssignment(setId, target(e)))
			{
				g.AddNode(e, setId);
			}
		}
	}

	//DBG_PRINT2(g.number_of_nodes(), g.number_of_edges())
	const int nodeCount = g.number_of_nodes();

	EdgeAssignmentGraph::NodeSetIterator it0, it1;
	NodeAssignmentPtr ptrChildNodeAss;
	double edgeSim, maxCost, assDist, maxAssDist = 0;
	leda::node v0, v1;
	leda::edge e0, e1;

	ParamIndices edgeParInds(3, 0), nodeParInds(3, 0);

	edgeParInds[0] = ptrNodeAss->ParamIndex(0);

	//if (ptrNodeAss->ParamIndex(0) == 2 || ptrNodeAss->ParamIndex(0) == 3)
	//	nodeParInds[0] = 2;

	for (it0 = g.FirstNode(0); it0 != nil; it0 = g.NextNode(0, it0))
	{
		e0 = g.NodeAttribute(0, it0);
		v0 = target(e0);

		for (it1 = g.FirstNode(1); it1 != nil; it1 = g.NextNode(1, it1))
		{
			e1 = g.NodeAttribute(1, it1);
			v1 = target(e1);

			edgeSim = ComputeEdgeSimilarity(e0, e1, edgeParInds);

			if (edgeSim > 0)
			{
				ptrChildNodeAss = new NodeAssignment(v0, v1, nodeParInds, nil, nil);

				//ptrChildNodeAss->SetParentNodeAssignment(ptrNodeAss);

				if (!ptrChildNodeAss->HasDistance())
				{
					ComputeRootedTreeDistance(ptrChildNodeAss);
				}

				maxCost = ptrChildNodeAss->Graph(0)->GetSubtreeCost(v0) +
					ptrChildNodeAss->Graph(1)->GetSubtreeCost(v1);

				// Convex sum of match and missmatch distances
				assDist = edgeSim * ptrChildNodeAss->Distance() +
					(1 - edgeSim) * maxCost;

				ptrChildNodeAss->SetWeightedDistance(assDist);

				//ptrChildNodeAss->SetInEdgeSimilarity(edgeSim);
				ptrChildNodeAss->SetNodeSimilarity(ptrChildNodeAss->NodeSimilarity() * edgeSim);

				g.AddEdge(it0, it1, ptrChildNodeAss);

				if (assDist > maxAssDist)
					maxAssDist = assDist;
			}
		}
	}

	NodeAssigmentList nodeAssigments;
	EdgeValueArray edgeValues(g);

	//DBG_PRINT2(g.number_of_nodes(), g.number_of_edges())
	maxAssDist++;

	forall_edges(e, g)
	{
		edgeValues[e] = maxAssDist - g.inf(e)->WeightedDistance();
	}

	g.SolveMaxWeightAssignment(edgeValues, &nodeAssigments);

	ASSERT(g.number_of_edges() == 0 || nodeAssigments.size() > 0);

	leda::node_array<bool> matchedNodes(g, false);

	// Init the sum of child distances to zero
	double childDist = 0;

	// Add children assignments to ptrNodeAss and sum their distances
	forall(e, nodeAssigments)
	{
		matchedNodes[source(e)] = true;
		matchedNodes[target(e)] = true;

		ptrNodeAss->AddChildAssignment(g.inf(e));

		childDist += g.inf(e)->WeightedDistance();
	}

	// Add the weighted distance of the given child assignment (if there is one)
	if (ptrNodeAss->HasGivenChildAssignment())
		childDist += ptrNodeAss->GetGivenChildAssignmentWeightedDistance();

	leda::edge dagEdge;
	leda::node dagNode;
	const DAG* pDag;

	forall_nodes(v, g)
	{
		if (!matchedNodes[v])
		{
			dagEdge = g.inf(v);
			dagNode = target(dagEdge);

			pDag = (const DAG*) graph_of(dagEdge);

			childDist += pDag->GetSubtreeCost(dagNode);
		}
	}

	ptrNodeAss->SetDistance(nodeAttDist + childDist);
}

double DAGMatcherTopological::ComputeParentsDistance(NodeAssignmentPtr ptrChildNodeAss) //const
{
	EdgeAssignmentGraph g;
	leda::node v;
	leda::edge e;

	for (int setId = 0; setId <= 1; setId++)
	{
		v = ptrChildNodeAss->Node(setId);

		forall_in_edges(e, v)
			g.AddNode(e, setId);
	}

	const double node0ParentCost =
		ptrChildNodeAss->Graph(0)->GetSubtreeComplementCost(ptrChildNodeAss->Node(0));

	const double node1ParentCost =
		ptrChildNodeAss->Graph(1)->GetSubtreeComplementCost(ptrChildNodeAss->Node(1));

	const double maxCost = node0ParentCost + node1ParentCost;

	const double maxChildAssCost =
		ptrChildNodeAss->Graph(0)->GetSubtreeCost(ptrChildNodeAss->Node(0)) +
		ptrChildNodeAss->Graph(1)->GetSubtreeCost(ptrChildNodeAss->Node(1));

	EdgeAssignmentGraph::NodeSetIterator it0, it1;
	NodeAssignmentPtr ptrNodeAss;
	double edgeSim, childAssDist, weightedDist, parComplement;
	leda::node v0, v1;
	leda::edge e0, e1;

	// Determine parIdx from ptrChildNodeAss
	//int parIdx = 0; //ptrChildNodeAss->ParamIndex();
	ParamIndices parInds(3, 0);

	for (it0 = g.FirstNode(0); it0 != nil; it0 = g.NextNode(0, it0))
	{
		e0 = g.NodeAttribute(0, it0);
		v0 = source(e0);

		for (it1 = g.FirstNode(1); it1 != nil; it1 = g.NextNode(1, it1))
		{
			e1 = g.NodeAttribute(1, it1);
			v1 = source(e1);

			edgeSim = ComputeEdgeSimilarity(e0, e1, parInds);

			if (edgeSim > 0)
			{
				ptrNodeAss = new NodeAssignment(v0, v1, parInds, nil, nil);

				// Convex sum of match and missmatch distances
				childAssDist = edgeSim * ptrChildNodeAss->Distance() +
					(1 - edgeSim) * maxChildAssCost;

				ptrNodeAss->SetGivenChildNodeAssignment(ptrChildNodeAss, childAssDist);

				ComputeRootedTreeDistance(ptrNodeAss);

				parComplement = ptrNodeAss->Distance() - childAssDist +
					ComputeParentsDistance(ptrNodeAss);

				weightedDist = edgeSim * parComplement + (1 - edgeSim) * maxCost;

				ptrNodeAss->SetWeightedDistance(weightedDist);

				g.AddEdge(it0, it1, ptrNodeAss);
			}
		}
	}

	double minDist = 0;

	if (g.number_of_edges() > 0)
	{
		leda::edge minEdge = nil;
		double dist;

		forall_edges(e, g)
		{
			dist = g.inf(e)->WeightedDistance();

			if (minEdge == nil || dist < minDist)
			{
				minEdge = e;
				minDist = dist;
			}
		}

		ptrChildNodeAss->AddParentAssignment(g.inf(minEdge));
	}
	else
	{
		if (!ptrChildNodeAss->IsRoot(0))
			minDist += node0ParentCost;

		if (!ptrChildNodeAss->IsRoot(1))
			minDist += node1ParentCost;
	}

	return minDist;
}

void DAGMatcherTopological::GetNodeMap(NodeMatchMap& nodeMap) const
{
	const DAG& dag0 = *m_ptrRootAssignment->Graph(0);

	nodeMap.init(dag0);

	m_ptrRootAssignment->CopyAssignment(nodeMap);
	m_ptrRootAssignment->CopyChildAssignments(nodeMap);
	m_ptrRootAssignment->CopyParentAssignments(nodeMap);

	/*leda::node v;
	double cost0, cost1, costSum;
	const DAG& dag1 = *m_ptrRootAssignment->Graph(1);

	forall_nodes(v, dag0)
	{
		NodeMatchCodomain& nmc = nodeMap[v];

		if (!nmc.IsEmpty())
		{
			cost0 = dag0.GetSubtreeCost(v);
			cost1 = dag1.GetSubtreeCost(nmc.node);
			costSum = cost0 + cost1;

			if (costSum > 0)
				nmc.similarity /= costSum;
			else
				nmc.similarity = 0;

			ASSERT_UNIT_INTERVAL(nmc.similarity);

			nmc.similarity = 1 - nmc.similarity;
			//nmc.similarity = 1;
		}
	}*/
}

/*!
*/
double DAGMatcherTopological::GetGraphSimilarity() const
{
	NodeMatchMap nodeMap;

	const DAG& dag0 = *m_ptrRootAssignment->Graph(0);

	nodeMap.init(dag0);

	m_ptrRootAssignment->CopyAssignment(nodeMap);
	m_ptrRootAssignment->CopyChildAssignments(nodeMap);
	m_ptrRootAssignment->CopyParentAssignments(nodeMap);

	leda::node v0;
	double nodeSim = 0;

	const DAG& dag1 = *m_ptrRootAssignment->Graph(1);

	const double maxLen0 = MaxLength(dag0);
	const double maxLen1 = MaxLength(dag1);
	double nodeLen0, nodeLen1;
	double nodeSim0 = 0, nodeSim1 = 0;

	forall_nodes(v0, dag0)
	{
		NodeMatchCodomain& nmc = nodeMap[v0];

		if (!nmc.IsEmpty())
		{
			nodeLen0 = GetBGNode(v0, dag0)->Length();
			nodeSim0 += nmc.similarity * nodeLen0;

			nodeLen1 = GetBGNode(nmc.node, dag1)->Length();
			nodeSim1 += nmc.similarity * nodeLen1;

			nodeSim += nmc.similarity;
		}
	}

	//double graphSim0 = nodeSim0 / TotalLength(dag0);
	//double graphSim1 = nodeSim1 / TotalLength(dag1);
	int N0 = dag0.GetNodeCount();
	int N1 = dag1.GetNodeCount();

	ASSERT(N0 > 0);
	ASSERT(N1 > 0);

	double graphSim0 = nodeSim / N0;
	double graphSim1 = nodeSim / N1;

	ASSERT_UNIT_INTERVAL(graphSim0);
	ASSERT_UNIT_INTERVAL(graphSim1);

	//return (graphSim0 + graphSim1) / 2;
	return MIN(graphSim0, graphSim1);
}