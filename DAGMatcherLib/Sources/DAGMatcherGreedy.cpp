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

#include "DAGMatcherGreedy.h"
#include "DAG.h"
#include "HelperFunctions.h"
#include "BasicUtils.h"

using namespace dml;

#ifdef _DEBUG
extern LogFile g_dagMatchingLog;
#endif

/*!
	@brief Computes the similarity between two dags.

	The similarity is computed by matching image's and model's nodes and
	then normalizing the number of matched nodes according to the order of
	the model graph.
*/
double DAGMatcherGreedy::Match(const DAG& query, const DAG& model)
{
// 	See who's being matched:
// 	DBG_VAL(model.GetObjName())
// 	DBG_VAL(model.GetViewNumber())

	leda::node q, m;
	int i, j;

	int n1 = query.GetNodeCount();
	int n2 = model.GetNodeCount();
	int min = MIN(n1, n2);

	// Create a similarity matrinx and a correspondence matrix
	SmartMatrix<double> simMat;

#ifdef _DEBUG
	SmartMatrix<double> dbgMat;
#endif

	InitGraphs(&query, &model);

	simMat.Resize(n1, n2, true);

	// Populate the similarity matrix
	forall_nodes(q, query)
	{
		forall_nodes(m, model)
		{
			i = query.GetNodeDFSIndex(q);
			j = model.GetNodeDFSIndex(m);

			simMat[i][j] = query.NodeSimilarity(q, model, m);
		}
	}

#ifdef _DEBUG
	if (DAG::IsDbgMode())
	{
    	g_dagMatchingLog.Print("\nQuery: %s\nModel: %s\n\n",
			(CSTR) query.GetDAGLbl(), (CSTR) model.GetDAGLbl());

		simMat.Print(g_dagMatchingLog, 6, 3, true);
		g_dagMatchingLog.Print("\n\n");
		s_matchParams.Print(g_dagMatchingLog);
		dbgMat = simMat;
	}
#endif

	// Make sure that the nodeMap's are empty
	m_nodeMap1.clear();
	m_nodeMap2.clear();
	m_nodeSimMap1.clear();

	// DAG::Match() does not preserve the contents of its parameters so
	// we need to make a copy of the graphs.
	DAGPtr cpQuery = query.CreateObject();
	DAGPtr cpModel = model.CreateObject();

	MatchedNodePair matNodes(query, model); // graphs use to apply penalties

	double dMatchedNodes = Match(*cpQuery = query, *cpModel = model,
		simMat, &matNodes);

	/*DAGNodePtrMap nodeMapQ, nodeMapM;
	double newSim = DAG::Match(query, model, nodeMapQ, nodeMapM);

	DBG_MSG2("New similarity: ", newSim)*/

#ifdef _DEBUG
	if (DAG::IsDbgMode())
	{
		for (int i = 0; i < simMat.NRows(); i++)
			for (int j = 0; j < simMat.NCols(); j++)
				if (simMat[i][j] - dbgMat[i][j] != 0.0)
				{
					if (simMat[i][j] != 0.0)
						dbgMat[i][j] = simMat[i][j] - dbgMat[i][j];
				}
				else
					dbgMat[i][j] = 0.0;

		dbgMat.Print(g_dagMatchingLog, 6, 3, true);
	}
#endif

	FillNodeMap(query, model);

	return (dMatchedNodes / n1 + dMatchedNodes / n2) / 2.0;
	//return dMatchedNodes / n1;
}

/*!
	@brief The main matching algorithm for various DAG's.

	It is a static function that receives two DAG's as input
	and computes a value representing their similarity, as well as
	a list of corresponding nodes in the two DAG's.  Uses two
	node functions, <node_similarity> and <nodes_are_related>.

	@see NodeSimilarity AreNodesRelated ComputeBipartiteGraph
*/
double DAGMatcherGreedy::Match(DAG& g1, DAG& g2, SimMatrix& simMat,
							   MatchedNodePair* pMatchedPair)
{
	int n1 = g1.GetNodeCount();
	int n2 = g2.GetNodeCount();

	// Edges should be directed from a smaller to a larger set.
	// but s_rwm and swm are set assuming g1 is the query!!!!! so...
	/*if (n1 > n2)
	{
		DBG_MSG("SWAPPING...");
		ASSERT(false); //transpose simMat
		if (pMatchedPair)
			pMatchedPair->SwapNodes();

		return Match(g2, g1, simMat, m_nodeMap2, m_nodeMap1, pMatchedPair);
	}*/

	// Check that both graphs are of non-zero size
	if (n1 * n2 == 0)
		return 0;

	// Check that both sizes are >1, since if one of them
	// is of size one, it has been matched (unless very first call).
	if (((n1 == 1) || (n2 == 1)) && m_nodeMap1.size() > 0)
		return 0;

	// Create a bipartite graph between A and B nodes and
	// check that the number of edges is non-zero.
	LEDA_GRAPH<leda::node, double> G; // graph of meta-nodes.

	leda::edge max_edge = ComputeBipartiteGraph(G, g1, g2, simMat, pMatchedPair);

	if (max_edge == nil)
		return 0;

	// Take the endpoints of the max weight edge
	leda::node maxG1Node, maxG2Node;
	leda::node_array<leda::node>& nodes = G.node_data();

	maxG1Node = nodes[G.source(max_edge)];
	maxG2Node = nodes[G.target(max_edge)];

	// Append the found nodes to the matching dict.
	// mark one with another's global number.
	DAGNodePtr maxG1NodePtr = g1.GetNode(maxG1Node);
	DAGNodePtr maxG2NodePtr = g2.GetNode(maxG2Node);

	m_nodeMap1[maxG1NodePtr] = maxG2NodePtr;
	m_nodeMap2[maxG2NodePtr] = maxG1NodePtr;

	//MatchedNodePair matNodes(maxG1Node, g1, maxG2Node, g2);

	// Get the DFS index BEFORE spliting the graphs
	int idxG1Node = g1.GetNodeDFSIndex(maxG1Node);
	int idxG2Node = g2.GetNodeDFSIndex(maxG2Node);

	// Compute rooted subgraphs and remainders
	// We want to leave the splitting node in both subgraphs
	DAGPtr sg1 = g1.SplitSubGraph(maxG1Node);
	DAGPtr sg2 = g2.SplitSubGraph(maxG2Node);

	double max_weight = simMat[idxG1Node][idxG2Node];

	////////////////////////////////////////////////////////////////
	//For display purposes, save the similarity assigned to the node
	//DAGNode* pNode = (DAGNode*)(const DAGNode*)maxG2NodePtr;
	//pNode->SetSimilarity(max_weight);
	m_nodeSimMap1[maxG1NodePtr] = max_weight;

	/*DBG_STREAM(((const DAGNode*)maxG1NodePtr)->GetNodeLbl() << " <--> "
		<< ((const DAGNode*)maxG2NodePtr)->GetNodeLbl() << " = "
		<< max_weight << "\n")*/
	////////////////////////////////////////////////////////////////

	// Recurse on the root-matched subtree first...
	pMatchedPair->SetEmpty();
	max_weight += Match(*sg1, *sg2, simMat, pMatchedPair);

	// ...set the info needed to update the similarity matrix ...
	pMatchedPair->SetNodes(idxG1Node, idxG2Node);

	if (g1.GetNodeCount() > 0 && g2.GetNodeCount() > 0)
	{
		// ...update sim matrix to avoid breaking hierarchical constraints...
		pMatchedPair->UpdateSimMat(simMat, s_matchParams.dBreakSiblingRelPen);

		// ...and then recurse on the complement graph
		max_weight += Match(g1, g2, simMat, pMatchedPair);
	}

	return max_weight;
}

/*!
	@brief Static helper function for DAG::Match().

	It creates a bipartite graph between g1 and g2's nodes,
	as long as they weren't previously matched, in which edges are directed
	from a smaller to a larger set.

	It the solution to the max weight max card bipartite matching and returns
	the largest weight edge from the matching.

	@param pMatchedPair Pointer to the last matched node pair or null if there is none.

	@return largest weight edge from the MWMCB matching.
*/
leda::edge DAGMatcherGreedy::ComputeBipartiteGraph(LEDA_GRAPH<leda::node, double>& G,
												   const DAG& g1, const DAG& g2,
												   SimMatrix& simMat,
												   MatchedNodePair* pMatchedPair) const
{
	//g1 is the query graph and g2 is the model graph
	leda::node u, v, g1Node, g2Node;

	leda::list<leda::node> A, B;
	leda::list_item lia, lib;

	// Create a graph with n1 + n2 nodes
	forall_nodes(u, g1)
		if (pMatchedPair->GetQueryNode() != g1.GetNodeDFSIndex(u))
			A.push(G.new_node(u));

	forall_nodes(u, g2)
		if (pMatchedPair->GetModelNode() != g2.GetNodeDFSIndex(u))
			B.push(G.new_node(u));

	leda::node_array<leda::node>& nodes = G.node_data();

	// Create a bipartite graph between A and B nodes.
	// do not establish edges between nodes that have been matched.
	for (lia = A.first(); lia != nil; lia = A.succ(lia))
	{
		// pick node from A
		u = A[lia];
		g1Node = nodes[u];

		if (m_nodeMap1.defined(g1.GetNode(g1Node)))
			continue;	// loopy node that has been matched

		for (lib = B.first(); lib != nil; lib = B.succ(lib))
		{
			// pick node from B
			v = B[lib];
			g2Node = nodes[v];

			if (m_nodeMap2.defined(g2.GetNode(g2Node)))
				continue;	// loopy node that has been matched

			// establish an edge in proper direction if needed
			//if (g1.AreNodesRelated(g1Node, g2, g2Node))
			if (simMat[g1.GetNodeDFSIndex(g1Node)][g2.GetNodeDFSIndex(g2Node)] > 0.0)
				G.new_edge(u, v, 1.0);
		}
	}

	//DBG_MSG1("Number of edges in Bipartite graph: ", G.number_of_edges());

	if (G.number_of_edges() == 0)
		return nil;

	// Assign weights to edges
	leda::edge_array<double> edgeCosts(G);
	leda::edge e;
	int i, j;

	ASSERT_UNIT_INTERVAL(s_matchParams.dBreakSiblingRelPen);

	forall_edges(e, G)
	{
		g1Node = nodes[G.source(e)];
		g2Node = nodes[G.target(e)];

		i = g1.GetNodeDFSIndex(g1Node);
		j = g2.GetNodeDFSIndex(g2Node);

		if (GetMatchParams().nPreserveAncestorRel)
		{
			if (!pMatchedPair->IsEmpty() && !pMatchedPair->AncestorRelPreserved(i, j))
				simMat[i][j] = 0;
		}

		edgeCosts[e] = simMat[i][j];
	}

	/*GraphWin gw(G);
	gw.display();
	gw.place_into_win();
	gw.edit();*/

	// Solve the max weight max card bipartite matching

#ifdef USE_TEMPLATE_BASED_MWBM_FUNCTION
	scale_weights(G, edgeCosts, 3.0);
	leda::list<leda::edge> tmch = MAX_WEIGHT_BIPARTITE_MATCHING_T(G, edgeCosts);
#else
	MWBM_SCALE_WEIGHTS(G, edgeCosts);
	leda::list<leda::edge> tmch = MAX_WEIGHT_BIPARTITE_MATCHING(G, edgeCosts);
#endif //USE_TEMPLATE_BASED_MWBM_FUNCTION

	// Find the largest weight edge from the matching
	leda::list_item lie;
	double    sim, curr_weight, max_weight = -9999999;
	leda::edge curr_edge, max_edge = nil;

	const double g1CMass = g1.GetCumulativeMass();  //cumulative mass for query
	const double g2CMass = g2.GetCumulativeMass();  //cumulative mass for model

	double g1RelMass, g2RelMass;

	// Only for debug
	leda::node a = nil, b = nil, c = nil, d = nil;
	double x,y, maxsim = -1;
	int m1, m2;
	// end debug

	const double& smw = s_matchParams.dSimilMassWeight;
	ASSERT_UNIT_INTERVAL(smw);

	/*
		Note that node masses are relative to the original graph the belong to
		so the relative mass should be according to the size of that graph or to the root
		of the graph the belong to.
	*/
	forall_items(lie, tmch)
	{
		curr_edge = tmch[lie];

		g1Node = nodes[G.source(curr_edge)];
		g2Node = nodes[G.target(curr_edge)];

		g1RelMass = g1.GetNodeMass(g1Node) / g1CMass;
		g2RelMass = g2.GetNodeMass(g2Node) / g2CMass;

		ASSERT_UNIT_INTERVAL(g1RelMass);
		ASSERT_UNIT_INTERVAL(g2RelMass);

		sim = edgeCosts[curr_edge];

		// Compute a convex sum of similarity and node mass
		curr_weight = smw * sim + (1 - smw) * MAX(g1RelMass, g2RelMass);

		ASSERT_UNIT_INTERVAL(curr_weight);

		if (DAG::IsDbgMode() && sim > maxsim)
		{
			maxsim = sim;
			c = g1Node;
			d = g2Node;
		}

		if (curr_weight > max_weight)
		{
			max_weight = curr_weight;
			max_edge = curr_edge;

			// only for debug
			if (DAG::IsDbgMode())
			{
    			a = g1Node;
    			b = g2Node;
    			x = edgeCosts[curr_edge];
    			y = MAX(g1RelMass, g2RelMass);
    			m1 = g1.GetNodeMass(g1Node);
    			m2 = g2.GetNodeMass(g2Node);
    		}
		}
	}

#ifdef _DEBUG
	if (DAG::IsDbgMode() && max_edge != nil)
	{
		g_dagMatchingLog.Print("\nQuery node: %s (%d)", (CSTR) g1.GetNodeLbl(a), g1.GetNodeDFSIndex(a));
		g_dagMatchingLog.Print("\nModel node: %s (%d)", (CSTR) g2.GetNodeLbl(b), g2.GetNodeDFSIndex(b));

    	if (a != c || b != d)
		{
			g_dagMatchingLog.Print("\nTrue max Q node: %s (%d)", (CSTR) g1.GetNodeLbl(c), g1.GetNodeDFSIndex(c));
			g_dagMatchingLog.Print("\nTrue max M node: %s (%d)", (CSTR) g2.GetNodeLbl(d), g2.GetNodeDFSIndex(d));
    	}

   		g_dagMatchingLog << "\nM1: " << m1 << "\nM2: " << m2;
   		g_dagMatchingLog << "\nSim: " << x << "\nRel Mass: " << y << "\nWSim: " << max_weight << "\n\n";
	}
#endif

	return max_edge;
}

/*leda::node DAGMatcherGreedy::WeightNodeSimilarity(const double& sim, leda::node v) const
{
	// The cumulative mass goes from 2N - 1 for the star graph
	// to N(N+1)/2 for a path graph.
	// We can convert the relative mass of a node to a value between 0 and 1
	// by the cumulative distribution function for a continuous uniform distribution.
	// (x - a) / (b - a)

	double relMass = GetNodeMass(v) / double(nCumulativeMass); // relative mass for the node
	int n = GetNodeCount();

	double w = (relMass - 2 * n + 1) / ((n * n) / 2 - 1.5 * n + 1);

	return .9 * sim + .1 * w;
}*/

/*!
	Translate the node pointer map into a leda node map with
	similarity values
*/
void DAGMatcherGreedy::FillNodeMap(const DAG& query, const DAG& model)
{
	leda::d_array<DAGNodePtr, leda::node> modelMap;
	DAGNodePtr ptrQueryNode, ptrModelNode;
	leda::node q, m;
	double sim, totalSim = 0;

	m_nodeMatchMap.init(query);

	// Fill in a map from model node pointers to leda model nodes
	forall_nodes(m, model)
	{
		ptrModelNode = model.GetNode(m);

		if (m_nodeMap2.defined(ptrModelNode))
			modelMap[ptrModelNode] = m;
	}

	// Now map the query leda nodes to the model leda nodes
	forall_nodes(q, query)
	{
		ptrQueryNode = query.GetNode(q);

		if (m_nodeMap1.defined(ptrQueryNode))
		{
			ptrModelNode = m_nodeMap1[ptrQueryNode];
			m = modelMap[ptrModelNode];

			ASSERT(m_nodeSimMap1.defined(ptrQueryNode));

			sim = m_nodeSimMap1[ptrQueryNode];

			//DBG_PRINT1(sim)

			totalSim += sim;

			m_nodeMatchMap[q].Set(m, sim);
		}
	}
	//DBG_PRINT1(totalSim)
}