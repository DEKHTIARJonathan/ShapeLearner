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

#ifndef __NODE_MATCH_INFO_H__
#define __NODE_MATCH_INFO_H__

#include "stdafx.h"

namespace dml {
class DAG;
class NodePairInfo;

// Define a shared pointer type for NodeMatchInfo objects
class NodeMatchInfo;
typedef SharedPtr<NodeMatchInfo> NodeMatchInfoPtr; //!< shared pointer for node assignments

//! Bipartite graph class of DAG edges
typedef BipartiteGraph<leda::edge> BipartiteEdgeGraph;

/*!
	Represents the information asssociated with an assignment between
	a pair of nodes, one of which is from a ``query'' graph and
	the other is from a ``model'' graph.
*/
class NodeMatchInfo
{
	typedef SharedPtr<BipartiteEdgeGraph> EdgeMapPtr;

	NodePairInfo* m_pNodePair;

	double m_similarity; //!< Similarity value of the match
	double m_penalty;    //!< Cumulative penalty term on the node

	double m_nodeSimilarity[2]; //!< Similarity of node attributes
	double m_edgeSimilarity; //!< Similarity of edge attributes
	double m_topoSimilarity; //!< Similarity of graph topology

	int m_paramIndex;        //!< Index of parameter used to compute similarity (-1 if none)

	int m_distToAncestor[2]; //!< Distance to the closest "known" ancestors of each node

public:
	EdgeMapPtr m_ptrInwardEdgeMap;  //!< Potential inward edge correspondences
	EdgeMapPtr m_ptrOutwardEdgeMap; //!< Potential outward edge correspondences

	EdgeValidityArray m_validInwardEdge;
	EdgeValidityArray m_validOutwardEdge;

protected:
	void InitializeParams()
	{
		m_similarity = -1;

		m_nodeSimilarity[0] = -1;
		m_nodeSimilarity[1] = -1;

		m_edgeSimilarity = -1;
		m_topoSimilarity = -1;

		m_paramIndex = -1;

		m_distToAncestor[0] = -1;
		m_distToAncestor[1] = -1;

		m_pNodePair = NULL;

		m_penalty = 1; // ie, no penalty
	}

	void InitializeEdgeMaps(leda::node v1, leda::node v2);
	const double& GetCertaintyParam() const;

public:

	NodeMatchInfo() { InitializeParams(); }

	NodeMatchInfo(leda::node q, const DAG& Q, leda::node m, const DAG& M);

	~NodeMatchInfo();

	NodeMatchInfo& operator=(const NodeMatchInfo& rhs);

	bool HasNodeSimilarity(int parIdx) const
	{
		ASSERT(parIdx >= 0 && parIdx <= 1);

		return (m_nodeSimilarity[parIdx] != -1);
	}

	bool HasEdgeSimilarity() const        { return (m_edgeSimilarity != -1); }
	bool HasTopologicalSimilarity() const { return (m_topoSimilarity != -1); }

	//! Gets the pair of nodes referred by the match
	const NodePairInfo& GetNodePair() const
	{
		ASSERT(m_pNodePair != NULL);

		return *m_pNodePair;
	}

	void SetClosestKnownAncestor(const NodePairInfo& npi);

	//! Fixes the inward edge correspondece eq <--> em
	void FixInwardEdgeMatch(leda::edge eq, leda::edge em)
	{
		m_ptrInwardEdgeMap->FixAssignment(eq, em, &m_validInwardEdge);
	}

	//! Fixes the outward edge correspondece eq <--> em
	void FixOutwardEdgeMatch(leda::edge eq, leda::edge em)
	{
		m_ptrOutwardEdgeMap->FixAssignment(eq, em, &m_validOutwardEdge);
	}

	int GetParameterIndex() const
	{
		return m_paramIndex;
	}

	void SetParameterIndex(int idx)
	{
		m_paramIndex = idx;
	}

	//! Gets the certainty value associated with the node match
	void GetCertaintyValue(double* pCertainty) const
	{
		const double& p = GetCertaintyParam();

		//ASSERT(s_certaintyParam >= 0)

		if (p == 0)
			*pCertainty = 1;
		else
			*pCertainty = exp(-MAX(m_distToAncestor[0], m_distToAncestor[1]) / p);
	}

	//! Gets the similarity value associated with the node match
	void GetPenalizedSimilarityValue(double* pPenSim) const
	{
		*pPenSim = m_similarity * m_penalty;
	}

	void GetCertaintyAndSimilarityValue(double* pCSVal) const
	{
		double sim;

		GetPenalizedSimilarityValue(&sim);

		GetCertaintyValue(pCSVal);

		*pCSVal *= sim;
	}

	//! Sets the similarity value associated with the node match
	void SetSimilarityValue(const double& simValue)
	{
		m_similarity = simValue;
	}

	//! Multiplies the similarity value by the given penalty term
	void ApplyPenalty(const double& penCoeff)
	{
		m_penalty *= penCoeff;
	}

	//! Gets the similarity between node attributes
	const double& GetNodeSimilarity(int parIdx) const
	{
		ASSERT(parIdx >= 0 && parIdx <= 1);

		return m_nodeSimilarity[parIdx];
	}

	//! Gets the similarity between node attributes
	void SetNodeSimilarity(int parIdx, const double& simValue)
	{
		ASSERT(parIdx >= 0 && parIdx <= 1);

		m_nodeSimilarity[parIdx] = simValue;
	}

	//! Gets the similarity between edge attributes
	const double& GetEdgeSimilarity() const
	{
		return m_edgeSimilarity;
	}

	//! Gets the similarity between edge attributes
	void SetEdgeSimilarity(const double& simValue)
	{
		m_edgeSimilarity = simValue;
	}

	//! Gets the similarity between topological attributes
	const double& GetTopologicalSimilarity() const
	{
		return m_topoSimilarity;
	}

	//! Gets the similarity between topological attributes
	void SetTopologicalSimilarity(const double& simValue)
	{
		m_topoSimilarity = simValue;
	}

	void Print(std::ostream& os) const;

	friend std::ostream& operator<<(std::ostream &os, const NodeMatchInfo& nmi)
	{
		return (os << nmi.m_similarity);
	}

	friend std::istream& operator>>(std::istream &is, NodeMatchInfo& nmi)
	{
		return (is >> nmi.m_similarity);
	}

	//! Compares the similarity values such as to create a "descending" order
	static int CompareCertaintyAndSimilarityDescend(
		const NodeMatchInfo& a1, const NodeMatchInfo& a2);

	/*static int CompareCertaintyAndSimilarityDescend(
		const NodeMatchInfo& a1, const NodeMatchInfo& a2)
	{
		double v1, v2;

		a1.GetCertaintyAndSimilarityValue(&v1);
		a2.GetCertaintyAndSimilarityValue(&v2);

		return (v1 > v2) ? -1 : ((v1 < v2) ? 1 : 0);
	}*/
};
} //namespace dml

#endif //__NODE_MATCH_INFO_H__