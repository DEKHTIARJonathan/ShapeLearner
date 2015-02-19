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
 * $Log: BipartiteNodeGraph.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include "BipartiteNodeGraph.h"
#include "NodePairInfo.h"
#include "NodeMatchInfo.h"
#include "HelperFunctions.h"
#include "BasicUtils.h"

using namespace dml;

#ifdef _DEBUG
extern LogFile g_dagMatchingLog;
#endif

typedef BipartiteNodeGraph BNG;

/*!
	@brief Transfers the edge values in the edges of
	the bipartite graph to the edge value array, and
	sets the edge values to 1. This is what's required by
	the functions that solve the one-to-one bipartite solution.
*/
void BNG::TransferEdgeValues()
{
	leda::edge e;

	// Assign weights to edges
	m_edgeValues.init(*this);

	forall_edges(e, *this)
	{
		operator[](e)->GetPenalizedSimilarityValue(&m_edgeValues[e]);
	}
}

/*!
	@brief Creates a bipartite node graph between the nodes
	in g1 and those in g2. The edge values of the graph encode
	the attribute similarity between the nodes of g1 and g2.
*/
void BNG::Create(const DAG& g1, const DAG& g2, bool bIncludeRootNodes)
{
	NodeSetIterator itA, itB;
	leda::node u, v;
	double sim;
	NodeMatchInfoPtr ptrMatchInfo;

	// Create a graph with n1 + n2 nodes and save the nodes
	// associated with the g1 and g2 in different sets
	forall_nodes(u, g1)
		if (bIncludeRootNodes || !g1.IsRootNode(u))
			m_setA.push(new_node(u));

	forall_nodes(u, g2)
		if (bIncludeRootNodes || !g2.IsRootNode(u))
			m_setB.push(new_node(u));

	//ASSERT(m_setA.size() == g1.GetNodeCount() - 1);
	//ASSERT(m_setB.size() == g2.GetNodeCount() - 1);

	// Create a bipartite graph between g1 and g2 nodes
	forall_items(itA, m_setA)
	{
		u = m_setA[itA];

		forall_items(itB, m_setB)
		{
			v = m_setB[itB];

			// Create a new match info object
			ptrMatchInfo = new NodeMatchInfo(inf(u), g1, inf(v), g2);

			// Compute node similarity and save it into the match info object
			sim = g1.NodeSimilarity(inf(u), g2, inf(v), ptrMatchInfo);

			// Add edge only if similarity value is non zero
			if (sim > 0)
				new_edge(u, v, ptrMatchInfo);
		}
	}

	TransferEdgeValues();
}

/*!
	@brief Creates a bipartite graph using the nodes in another
	bipartite graph and excluding the given matched node pair
	and their incident edges.

	The penalties associated with the matched node pair are added
	to all edges that are incident to the ancestors and descendants
	of the matched node pair.
*/
void BNG::Create(const BipartiteNodeGraph& srcBipGraph, const NodeMatchInfoPtr& ptrNMI)
{
	NodeToNodeArray nodeMap[2]; // store links between source nodes and new nodes
	//graph_dfs_iterator descendantIt[2];
	//graph_inverted_dfs_iterator ancestorIt[2];
	leda::node_array<bool> visited[2];
	leda::node u, newNode;
	NodeSetIterator it;

	// Get the pair of nodes referred by the match info
	const NodePairInfo& npi = ptrNMI->GetNodePair();

	// Copy all nodes in the sets m_setA and m_setB of the source graph,
	// and except npi nodes, to target (this object) sets m_setA and m_setB
	for (int i = 0; i < 2; i++)
	{
		const NodeSet& srcSet = (i == 0) ? srcBipGraph.m_setA : srcBipGraph.m_setB;
		NodeSet& tgtSet       = (i == 0) ? m_setA : m_setB;

		// Initialize the node map
		nodeMap[i].init(npi.Graph(i));

		// Initialize the descendants and ancestors iterators with all nodes visited
		visited[i].init(npi.Graph(i), true);

		forall_items(it, srcSet)
		{
			u = srcBipGraph.inf(srcSet[it]);

			// Check if the node is one of the matched nodes
			if (npi.IsMatchedNode(i, u))
			{
				nodeMap[i][u] = nil;
			}
			else // it isn't
			{
				// Add the node to the corresponding target set and
				// save the m_nodeAssigments so that its edges can be added later
				newNode = new_node(u);

				tgtSet.push(newNode);
				nodeMap[i][u] = newNode;

				// The nodes that are in the set are set to 'not visited'
				visited[i][u] = false;
			}
		}
	}

	// Copy all edges that are unrelated to the npi nodes, which
	// are those with a value in nodeMap that is NOT nil
	leda::edge e;
	leda::node v;

	forall_edges(e, srcBipGraph)
	{
		u = nodeMap[0][srcBipGraph.inf(source(e))];
		v = nodeMap[1][srcBipGraph.inf(target(e))];

		if (u != nil && v != nil)
			new_edge(u, v, srcBipGraph.inf(e));
	}

	// Apply penalties to the ancestors, descendants and siblings of the matched nodes
	ApplyEdgePenalties(ptrNMI, nodeMap, visited);

	// Transfer the new edge values to the m_edgeValues array
	TransferEdgeValues();
}

/*!
	@brief Applies penalties to edge values that are related to an ancestro or
	descendant of either one of the matched nodes.

	Only ancestors/descendants that can be reached from the matched nodes by
	a path of adjacent edges are considered. This path can be formed only by
	nodes in the corresponding set of ancestors or descendants. That is, the
	nodes in the path cannot be commited nodes to the solution set.
*/
void BNG::ApplyEdgePenalties(const NodeMatchInfoPtr& ptrNMI,
							 NodeToNodeArray nodeMap[],
							 leda::node_array<bool> visited[])
{
	leda::list<leda::edge> zeroEdges;
	leda::edge_array<bool> procEdges(*this, false);
	std::vector<graph_dfs_iterator> relatives;

	leda::node u, q, m, bgNode;
	leda::edge e;
	double penCoeff;
	bool bChanged;
	unsigned int type;

	// Get the pair of nodes referred by the match info
	const NodePairInfo& npi = ptrNMI->GetNodePair();
	const int matchParamIdx = ptrNMI->GetParameterIndex();

	DBG_M_LOG("Processing ancestors and descendants\n")

	//ptrNMI->Print(std::cout << "\nProcessing node match ");

	// Process the ancestors and descendants on each set separately
	for (int i = 0; i < 2; i++)
	{
		// Get the matched node in the set i
		u = npi.Node(i);

		// Set the number of relatives to ancestor, descentant and siblings
		// with different parents (ie, 2+)
		relatives.resize(2 + npi.Graph(i).indeg(u));

		// Init the iterator of the ancestor and descendats
		relatives[0].init(u, visited[i], true); // ancestors (use inverted edges)
		relatives[1].init(u, visited[i]);       // descendants

		// Init the iterator of the descendats of each parent node (ie, siblings)
		// type 2 means siblings wrt parent 1, type 3 means siblings wrt parent 2, etc
		type = 2;

		forall_in_edges(e, u)
			relatives[type++].init(source(e)/*parent of node*/, visited[i]);

		// Process each relative
		for (type = 0; type < relatives.size(); type++)
		{
			//std::cout << "\n\nProcessing relatives type " << type << std::endl;

			// Traverse each ancestor/descendant that can be reached from
			// the matched nodes via a path of "non visited" nodes
			while ((u = relatives[type].next()) != nil)
			{
				bgNode = nodeMap[i][u];

				//std::cout << "\n\nProcessing node DFS idx " << GetDAGNodeDFSIndex(bgNode) << std::endl;

				// Process all incident edges on the "bipartite graph node" bgNode
				forall_inout_edges(e, bgNode)
				{
					//if (!((i == 0 && !procEdges[e]) || i == 1))
					//	Print(std::cout, e);

					//ASSERT((i == 0 && !procEdges[e]) || i == 1);

					// Check that e (in this bip graph) is not processed already
					if (/*i == 0 ||*/ !procEdges[e])
					{
						q = inf(source(e));
						m = inf(target(e));

						//int idx1 = npi.Graph(0).GetNodeDFSIndex(q);
						//int idx2 = npi.Graph(1).GetNodeDFSIndex(m);

						// Correct sibling issue
						/*if (type == 2 && (npi.IsDescendant(0, q) || npi.IsDescendant(1, m))
						{
							DBG_MSG1("Dealing with sibling/descendant issue")
							continue;
						}*/

						procEdges[e] = true; // mark e as processed

						if (type == 0)
							penCoeff = npi.GetAncestorPenalty(q, m);
						else if (type == 1)
							penCoeff = npi.GetDescendantPenalty(q, m);
						else
							penCoeff = npi.GetSiblingPenalty(q, m);

						ASSERT_UNIT_INTERVAL(penCoeff);

						if (penCoeff == 0)
						{
							zeroEdges.push(e);
						}
						else
						{
							// Get a non-const reference to the edge attribute
							NodeMatchInfoPtr& ptrEdgeInfo = Inf(e);

							// Update the edge info with a clone of itself
							// so that we can modify it for this graph only
							if (ptrEdgeInfo.Links() > 1)
								ptrEdgeInfo = inf(e).Clone();

							if (type == 0)
								bChanged = UpdateAncestorInfo(npi, matchParamIdx,
								           q, m, penCoeff, ptrEdgeInfo);
							else if (type == 1)
								bChanged = UpdateDescendantInfo(npi, matchParamIdx,
								           q, m, penCoeff, ptrEdgeInfo);
							else
								bChanged = false;

							// If the ancestor/descendant info change, recompute the similarity
							if (bChanged)
							{
								const DAG& g0 = ptrNMI->GetNodePair().Graph(0);
								const DAG& g1 = ptrNMI->GetNodePair().Graph(1);

								g0.NodeSimilarity(q, g1, m, ptrEdgeInfo);
							}

							ptrEdgeInfo->ApplyPenalty(penCoeff);
						}
					}
				}
			}
		}
	}

	// Remove all edges with zero value
	del_edges(zeroEdges);
}

bool BNG::UpdateAncestorInfo(const NodePairInfo& npi, int paramIdx,
							   leda::node q, leda::node m, const double& penCoeff,
							   NodeMatchInfoPtr& ptrEdgeInfo)
{
	ASSERT(q == ptrEdgeInfo->GetNodePair().Node(0));
	ASSERT(m == ptrEdgeInfo->GetNodePair().Node(1));
	ASSERT(ptrEdgeInfo.Links() == 1);

	bool bInfoChanged = false;

	leda::edge eq = npi.FindFirstEdgeFromAncestor(0, q);
	leda::edge em = npi.FindFirstEdgeFromAncestor(1, m);

	if (eq != nil && em != nil)
	{
		ptrEdgeInfo->FixOutwardEdgeMatch(eq, em);

		bInfoChanged = true; // Edge info changed
	}

	/*if (ptrEdgeInfo->GetParameterIndex() != paramIdx)
	{
		ptrEdgeInfo->SetParameterIndex(paramIdx);

		bInfoChanged = true; // Node param changed
	}*/

	return bInfoChanged;
}

bool BNG::UpdateDescendantInfo(const NodePairInfo& npi, int paramIdx,
							   leda::node q, leda::node m, const double& penCoeff,
							   NodeMatchInfoPtr& ptrEdgeInfo)
{
	ASSERT(q == ptrEdgeInfo->GetNodePair().Node(0));
	ASSERT(m == ptrEdgeInfo->GetNodePair().Node(1));
	ASSERT(ptrEdgeInfo.Links() == 1);

	bool bInfoChanged = false;

	leda::edge eq = npi.FindLastEdgeToDescendant(0, q);
	leda::edge em = npi.FindLastEdgeToDescendant(1, m);

	ptrEdgeInfo->SetClosestKnownAncestor(npi);

	if (eq != nil && em != nil)
	{
		ptrEdgeInfo->FixInwardEdgeMatch(eq, em);

		bInfoChanged = true; // Edge info changed
	}

	if (ptrEdgeInfo->GetParameterIndex() != paramIdx)
	{
		ptrEdgeInfo->SetParameterIndex(paramIdx);

		bInfoChanged = true; // Node param changed
	}

	return bInfoChanged;
}