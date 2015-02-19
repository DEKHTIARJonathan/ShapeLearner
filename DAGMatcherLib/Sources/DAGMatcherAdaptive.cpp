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
#include "DAGMatcherAdaptive.h"
#include "DAG.h"
#include "BipartiteGraph.h"
#include "BoneGraph.h" // temporary solution
#include "HelperFunctions.h"
#include "BasicUtils.h"

#define NUM_MATCH_PARAMS 5

using namespace dml;

#ifdef _DEBUG
extern LogFile g_dagMatchingLog;
#endif

struct EdgeAndParams
{
	leda::edge edge;
	int skip;
	int invert;

	EdgeAndParams(leda::edge e = nil, int s = 0, int i = 0)
	{
		edge = e;
		skip = s;
		invert = i;
	}

	friend std::ostream& operator<<(std::ostream &os, const EdgeAndParams& ep)
	{
		return (os << ep.edge << ep.skip << ep.invert);
	}

	friend std::istream& operator>>(std::istream &is, EdgeAndParams& ep)
	{
		return (is >> ep.edge >> ep.skip >> ep.invert);
	}
};

//typedef BipartiteGraph<leda::edge, NodeAssignmentPtr> EdgeAssignmentGraph;

typedef BipartiteGraph<EdgeAndParams, NodeAssignmentPtr> EdgeAssignmentGraph;

leda::edge GetSkipEdge(const DAG* pDAG, leda::node v, int endPtIdx)
{
	const BoneGraph* pBG = (const BoneGraph*) pDAG;
	const BGEdge* pEdge;

	double minLength;
	leda::edge e, ee, minEdge = nil;
	leda::node w;
	bool bValid;

	forall_out_edges(e, v)
	{
		pEdge = pBG->GetBGEdge(e);

		if (pEdge->IsEmpty() || pEdge->Position() != endPtIdx)
		//if (pEdge->IsEmpty() || fabs(pEdge->Position() - endPtIdx) > 0.15)
			continue;

		w = target(e); // node to skip

		if (outdeg(w) == 0 || indeg(w) != 1)
			continue;

		bValid = true;

		forall_out_edges(ee, w)
		{
			if (pBG->GetBGEdge(ee)->IsEmpty())
			{
				bValid = false;
				break;
			}
		}

		if (!bValid)
			continue;

		if (minEdge == nil || pBG->GetBGNode(w)->Length() < minLength)
		{
			minLength = pBG->GetBGNode(w)->Length();
			minEdge = e;
		}
	}

	return minEdge;
}

bool IsCornerBone(const BoneGraph* pBG, leda::node v)
{
	if (outdeg(v) > 0 || indeg(v) != 1 || pBG->GetNodeLevel(v) < 2)
		return false;

	leda::edge e = pBG->first_in_edge(v);

	if (!pBG->GetBGEdge(e)->IsEmpty())
		return false;

	const BGNode* pNode = pBG->GetBGNode(v);

	if (pNode->GetRadiusFunction().Size() != 1)
		return false;

	if (pNode->GetRadiusFunction().GetHead().m >= 0)
		return false;

	if (pNode->Length() >= 2 * pNode->FirstShockPoint().radius)
		return false;

	return true;
}

double NodeCount(const DAG& g)
{
	leda::node v;
	double numNodes = 0;

	const BoneGraph& bg = dynamic_cast<const BoneGraph&>(g);

	forall_nodes(v, g)
	{
		if (!IsCornerBone(&bg, v))
			numNodes++;
		else
			numNodes += 0.5;
	}

	return numNodes;
}

/*!
	@brief

	Note: g1 is assumed to be the query graph and g2 the model graph.
*/
double DAGMatcherAdaptive::Match(const DAG& g0, const DAG& g1)
{
	leda::node v0, v1;

	// Ensure that the matched has no old data
	Clear();

	// Set member variables so that the graph can be accessed from any member functions
	// And by the similarity measurer object
	InitGraphs(&g0, &g1);

	const int rootLevel = 1;

	forall_nodes(v0, g0)
	{
		if (g0.GetNodeLevel(v0) == rootLevel)
			m_roots0.push_back(v0);
	}

	forall_nodes(v1, g1)
	{
		if (g1.GetNodeLevel(v1) == rootLevel)
			m_roots1.push_back(v1);
	}

	double simSum, maxSimSum = -1;
	ParamIndices parInds(NUM_MATCH_PARAMS, 0);
	NodeAssignmentPtr ptrNodeAss;

	for (unsigned int i = 0; i < m_roots0.size(); i++)
	{
		v0 = m_roots0[i];

		for (unsigned int j = 0; j < m_roots1.size(); j++)
		{
			v1 = m_roots1[j];

			// Try all possible combinations of parameter values
			for(parInds[0] = 0; parInds[0] < 4; parInds[0]++)
			{
				// Create a new match info object
				ptrNodeAss = new NodeAssignment(v0, v1, parInds,
					g0.first_in_edge(v0), g1.first_in_edge(v1));

				// Compute node similarity and save it into the match info object
				ComputeRootedTreeSimilarity(ptrNodeAss);

				simSum = ptrNodeAss->SubtreeSimilaritySum();

				if (simSum > maxSimSum)
				{
					m_ptrRootAssignment = ptrNodeAss;
					maxSimSum = simSum;
				}
			}
		}
	}

	ASSERT(maxSimSum >= 0);

	const int N0 = g0.GetNodeCount() - 1;
	const int N1 = g1.GetNodeCount() - 1;

	//const double N0 = NodeCount(g0) - 1;
	//const double N1 = NodeCount(g1) - 1;

	ASSERT(N0 > 0 && N1 > 0);

	double graphSim0 = maxSimSum / N0;
	double graphSim1 = maxSimSum / N1;

	ASSERT_UNIT_INTERVAL(graphSim0);
	ASSERT_UNIT_INTERVAL(graphSim1);

	return MIN(graphSim0, graphSim1);

	//return GetGraphSimilarity();
}

void DAGMatcherAdaptive::ComputeRootedTreeSimilarity(NodeAssignmentPtr ptrNodeAss) const
{
	SkipEdges skipEdges(2);
	leda::edge skipEdge0, skipEdge1;
	double childDist, bestChildDist;
	NodeAssignmentPtr ptrBestNodeAss; // starts as null pointer

	int dfsIdx0 = ptrNodeAss->Graph(0)->GetNodeDFSIndex(ptrNodeAss->Node(0));
	int dfsIdx1 = ptrNodeAss->Graph(1)->GetNodeDFSIndex(ptrNodeAss->Node(1));

	double nodeSimilarity;

	nodeSimilarity = ComputeNodeSimilarity(ptrNodeAss);

	ASSERT_UNIT_INTERVAL(nodeSimilarity);

	skipEdges[0].Clear();
	skipEdges[1].Clear();

	bestChildDist = ComputeRootedTreeSimilarity(ptrNodeAss, skipEdges, nodeSimilarity);

	if (s_matchParams.nDisableNodeSkipping)
		return; // we are done

	for (int i = 0; i < 2; i++)
	{
		skipEdge0 = GetSkipEdge(ptrNodeAss->Graph(i), ptrNodeAss->Node(i), 0);
		skipEdge1 = GetSkipEdge(ptrNodeAss->Graph(i), ptrNodeAss->Node(i), 1);

		for (int skip0 = 0; skip0 < 2; skip0++)
		{
			if (skip0 && skipEdge0 == nil)
				continue;

			skipEdges[i].e0 = (skip0) ? skipEdge0 : nil;

			for (int skip1 = 0; skip1 < 2; skip1++)
			{
				if ((skip1 && skipEdge1 == nil) || (!skip0 && !skip1))
					continue;

				skipEdges[i].e1 = (skip1) ? skipEdge1 : nil;

				NodeAssignmentPtr ptrAuxNodeAss = new NodeAssignment(ptrNodeAss->Node(0),
					ptrNodeAss->Node(1), ptrNodeAss->GetParamIndices(),
					ptrNodeAss->InEdge(0), ptrNodeAss->InEdge(1));

				childDist = ComputeRootedTreeSimilarity(ptrAuxNodeAss, skipEdges, nodeSimilarity);

				if (childDist > bestChildDist)
				{
					bestChildDist = childDist;
					ptrBestNodeAss = ptrAuxNodeAss;
				}
			}
		}

		// Clean up the edge values
		skipEdges[i].Clear();
	}

	if (!ptrBestNodeAss.IsNull())
		*ptrNodeAss = *ptrBestNodeAss;
}

double DAGMatcherAdaptive::ComputeRootedTreeSimilarity(NodeAssignmentPtr ptrNodeAss,
												   const SkipEdges& skipEdges,
												   const double& rootSimilarity) const
{
	// Create BipGraph G. Exclude edges whose target is any of the known nodes
	EdgeAssignmentGraph g;
	leda::node v;
	leda::edge e;

	for (int setId = 0; setId <= 1; setId++)
	{
		v = ptrNodeAss->Node(setId);

		forall_out_edges(e, v)
		{
			// When the subtree is inverted, the inEdge can be
			// an out edge, which must be ignored
			if (e == ptrNodeAss->InEdge(setId))
				continue;

			if (skipEdges[setId].e0 == e || skipEdges[setId].e1 == e)
			{
				leda::edge ee;

				forall_out_edges(ee, target(e))
				{
					g.AddNode(EdgeAndParams(ee, 1, 0), setId);
				}
			}
			else
			{
				g.AddNode(EdgeAndParams(e, 0, 0), setId);
			}
		}

		forall_in_edges(e, v)
		{
			if (e != ptrNodeAss->InEdge(setId) &&
				!ptrNodeAss->Graph(setId)->IsRootNode(source(e)))
			{
				g.AddNode(EdgeAndParams(e, 0, 1), setId);
			}
		}
	}

	EdgeAssignmentGraph::NodeSetIterator it0, it1;
	NodeAssignmentPtr ptrChildNodeAss;
	leda::node v0, v1;
	leda::edge e0, e1;
	EdgeAndParams ep0, ep1;
	double edgeSim;

	ParamIndices edgeParInds(NUM_MATCH_PARAMS), nodeParInds;

	// Propagate firt param from parent (only happens from level 1 to level 2)
	edgeParInds[0] = ptrNodeAss->ParamIndex(0);

	for (it0 = g.FirstNode(0); it0 != nil; it0 = g.NextNode(0, it0))
	{
		ep0 = g.NodeAttribute(0, it0);
		e0 = ep0.edge;

		v0 = (ep0.invert) ? source(e0) : target(e0);

		edgeParInds[1] = ep0.skip;
		edgeParInds[3] = ep0.invert;

		for (it1 = g.FirstNode(1); it1 != nil; it1 = g.NextNode(1, it1))
		{
			ep1 = g.NodeAttribute(1, it1);
			e1 = ep1.edge;

			v1 = (ep1.invert) ? source(e1) : target(e1);

			edgeParInds[2] = ep1.skip;
			edgeParInds[4] = ep1.invert;

			edgeSim = ComputeEdgeSimilarity(e0, e1, edgeParInds);

			if (edgeSim > 0)
			{
				nodeParInds = edgeParInds;
				nodeParInds[0] = 0;

				ptrChildNodeAss = new NodeAssignment(v0, v1, nodeParInds, e0, e1);

				ComputeRootedTreeSimilarity(ptrChildNodeAss);

				ptrChildNodeAss->SetSubtreeSimilaritySum(
					ptrChildNodeAss->SubtreeSimilaritySum() * edgeSim);

				g.AddEdge(it0, it1, ptrChildNodeAss);
			}
		}
	}

	NodeAssigmentList nodeAssigments;
	EdgeValueArray edgeValues(g);

	forall_edges(e, g)
	{
		edgeValues[e] = g.inf(e)->SubtreeSimilaritySum();
	}

	g.SolveMaxWeightAssignment(edgeValues, &nodeAssigments);

	ASSERT(g.number_of_edges() == 0 || nodeAssigments.size() > 0);

	// Init the sum of child distances to zero
	double childSimSum = 0;

	// Add children assignments to ptrNodeAss and sum their distances
	forall(e, nodeAssigments)
	{
		ptrNodeAss->AddChildAssignment(g.inf(e));

		childSimSum += g.inf(e)->SubtreeSimilaritySum();
	}

	// Make sure that the node match has the similarity assigned
	ptrNodeAss->SetNodeSimilarity(rootSimilarity);

	ptrNodeAss->SetSubtreeSimilaritySum(rootSimilarity + childSimSum);

	return ptrNodeAss->SubtreeSimilaritySum();
}

void DAGMatcherAdaptive::GetNodeMap(NodeMatchMap& nodeMap) const
{
	const DAG& dag0 = *m_ptrRootAssignment->Graph(0);

	nodeMap.init(dag0);

	m_ptrRootAssignment->CopyAssignment(nodeMap);
	m_ptrRootAssignment->CopyChildAssignments(nodeMap);
}

/*!
*/
double DAGMatcherAdaptive::GetGraphSimilarity() const
{
	NodeMatchMap nodeMap;

	const DAG& dag0 = *m_ptrRootAssignment->Graph(0);

	nodeMap.init(dag0);

	m_ptrRootAssignment->CopyAssignment(nodeMap);
	m_ptrRootAssignment->CopyChildAssignments(nodeMap);

	leda::node v0;
	double nodeSim = 0;
	double simBoost;

	const DAG& dag1 = *m_ptrRootAssignment->Graph(1);

	forall_nodes(v0, dag0)
	{
		NodeMatchCodomain& nmc = nodeMap[v0];

		if (!nmc.IsEmpty())
		{
			ASSERT(nmc.params.size() >= 3);

			simBoost = 1 + (nmc.params[1] + nmc.params[2]) / 2.0;

			nodeSim += nmc.similarity * simBoost;
		}
	}

	int N0 = dag0.GetNodeCount() - 1;
	int N1 = dag1.GetNodeCount() - 1;

	ASSERT(N0 > 0);
	ASSERT(N1 > 0);

	double graphSim0 = nodeSim / N0;
	double graphSim1 = nodeSim / N1;

	ASSERT_UNIT_INTERVAL(graphSim0);
	ASSERT_UNIT_INTERVAL(graphSim1);

	return MIN(graphSim0, graphSim1);
}