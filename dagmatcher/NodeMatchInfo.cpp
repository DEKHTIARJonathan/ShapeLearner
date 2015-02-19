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

#include "DAG.h"
#include "NodePairInfo.h"
#include "NodeMatchInfo.h"

using namespace dml;

NodeMatchInfo::NodeMatchInfo(leda::node q, const DAG& Q, leda::node m, const DAG& M)
{
	InitializeParams();

	m_pNodePair = new NodePairInfo(q, Q, m, M);

	m_distToAncestor[0] = Q.GetNodeLevel(q);
	m_distToAncestor[1] = M.GetNodeLevel(m);

	InitializeEdgeMaps(q, m);
}

NodeMatchInfo::~NodeMatchInfo()
{
	delete m_pNodePair;
}

const double& NodeMatchInfo::GetCertaintyParam() const
{
	return DAGMatcher::GetMatchParams().dCertaintyParam;
}

/*!
	@brief Assignment operator.
*/
NodeMatchInfo& NodeMatchInfo::operator=(const NodeMatchInfo& rhs)
{
	m_similarity     = rhs.m_similarity;
	m_penalty        = rhs.m_penalty;

	m_nodeSimilarity[0] = rhs.m_nodeSimilarity[0];
	m_nodeSimilarity[1] = rhs.m_nodeSimilarity[1];

	m_edgeSimilarity = rhs.m_edgeSimilarity;
	m_topoSimilarity = rhs.m_topoSimilarity;

	m_paramIndex = rhs.m_paramIndex;

	m_distToAncestor[0] = rhs.m_distToAncestor[0];
	m_distToAncestor[1] = rhs.m_distToAncestor[1];

	m_pNodePair = new NodePairInfo(*rhs.m_pNodePair);

	// Share the bipartite graphs...
	m_ptrInwardEdgeMap  = rhs.m_ptrInwardEdgeMap;
	m_ptrOutwardEdgeMap = rhs.m_ptrOutwardEdgeMap;

	// ...but copy the edge validity, which may change
	m_validInwardEdge  = rhs.m_validInwardEdge;
	m_validOutwardEdge = rhs.m_validOutwardEdge;

	return *this;
}

/*!
	Creates a bipartite graph in which the nodes refer to the
	outward edges in the pair of nodes being compared
*/
void NodeMatchInfo::InitializeEdgeMaps(leda::node v1, leda::node v2)
{
	leda::edge e;

	/////////////////////////////////////////////////////////////////////
	// Add inward edges...
	m_ptrInwardEdgeMap  = new BipartiteEdgeGraph;

	forall_in_edges(e, v1)
		m_ptrInwardEdgeMap->AddNode(e, 0); // add the edge to the set 0

	forall_in_edges(e, v2)
		m_ptrInwardEdgeMap->AddNode(e, 1); // add the edge to the set 0

	// ... and map every node in set 1 to every node in set 2
	m_ptrInwardEdgeMap->CreateFullMap();

	// Set all inward edges as 'valid'
	m_validInwardEdge.init(*m_ptrInwardEdgeMap, true);

	/////////////////////////////////////////////////////////////////////
	// Add outward edges...
	m_ptrOutwardEdgeMap = new BipartiteEdgeGraph;

	forall_out_edges(e, v1)
		m_ptrOutwardEdgeMap->AddNode(e, 0); // add the edge to the set 0

	forall_out_edges(e, v2)
		m_ptrOutwardEdgeMap->AddNode(e, 1); // add the edge to the set 1

	// ... and map every node in set 1 to every node in set 2
	m_ptrOutwardEdgeMap->CreateFullMap();

	// Set all outward edges as 'valid'
	m_validOutwardEdge.init(*m_ptrOutwardEdgeMap, true);

	// Set the saliency of each edge in each graph
}

void NodeMatchInfo::SetClosestKnownAncestor(const NodePairInfo& npi)
{
	for (int i = 0; i < 2; i++)
	{
		if (m_pNodePair->IsAncestor(i, npi.Node(i)))
		{
			int oldDist = m_distToAncestor[i];

			m_distToAncestor[i] = -m_pNodePair->GetRelativeNodeLevel(i, npi.Node(i));

			ASSERT(m_distToAncestor[i] > 0);
			//ASSERT(old >= m_distToAncestor[i]);

			//WARNING(oldDist < m_distToAncestor[i], "dist ancestor issue");

			if (oldDist < m_distToAncestor[i])
				m_distToAncestor[i] = oldDist;
		}
	}
}

void NodeMatchInfo::Print(std::ostream& os) const
{
	if (m_pNodePair)
	{
		m_pNodePair->Print(os);
		os << ": ";
	}

	double certainty, penSim, cerAndPenSim;

	GetCertaintyValue(&certainty);
	GetPenalizedSimilarityValue(&penSim);
	GetCertaintyAndSimilarityValue(&cerAndPenSim);

	PRINT(os, penSim);
	PRINT(os, cerAndPenSim);
	PRINT(os, certainty);

	PRINT2(os, "param", m_paramIndex);
	PRINT2(os, "pen", m_penalty);
	PRINT2(os, "sim", m_similarity);
	PRINT2(os, "nodeSim", m_nodeSimilarity);
	PRINT2(os, "edgeSim", m_edgeSimilarity);
	PRINT2(os, "topoSim", m_topoSimilarity);
}

/*static*/
int NodeMatchInfo::CompareCertaintyAndSimilarityDescend(
		const NodeMatchInfo& a1, const NodeMatchInfo& a2)
{
	double v1, v2;

	v1 = a1.GetNodePair().GetAverageNodeLevel();
	v2 = a2.GetNodePair().GetAverageNodeLevel();

	return (v1 > v2) ? 1 : ((v1 < v2) ? -1 : 0);
}