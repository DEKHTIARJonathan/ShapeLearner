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
*-----------------------------------------------------------------------*/

#ifndef __DAG_MATCHER_H__
#define __DAG_MATCHER_H__

#include "stdafx.h"
#include "SimilarityMeasurer.h"

namespace dml {
//! The codomain of a NodeMatchMap. It's a node and a similarity value.
struct NodeMatchCodomain
{
	leda::node node;
	double similarity;
	ParamIndices params;

	NodeMatchCodomain() { node = nil; }

	void Set(leda::node v, const double& sim)
	{
		node = v;
		similarity = sim;
		params.clear();
	}

	void Set(leda::node v, const double& sim, const ParamIndices& par)
	{
		node = v;
		similarity = sim;
		params = par;
	}

	bool IsEmpty() const { return (node == nil); }
};

typedef leda::node_array<NodeMatchCodomain> NodeMatchMap;

/*!
	Generic class for matching DAGs.
*/
class DAGMatcher
{
public:
	struct MatchParams
	{
		int nMatchingAlgorithm;      //!< Matching algorithm ID
		int nNodeSimilarityFunction; //!< Node distance function ID
		double dTSVSimWeight;        //!< TSV similarity weight
		double dSimilMassWeight;     //!< Similarity vs node's relative mass weight
		double dRelMassWeight;       //!< Relative mass weight for query vs model
		double dDiffAttachPosWeight; //!< Relative importance of attachment position differences

		double dBreakAncestorRelPen;   //!< Breaking ancestor relationship penalty
		double dBreakDescendantRelPen; //!< Breaking descendant relationship penalty
		double dBreakSiblingRelPen;    //!< Breaking siblings relationship penalty

		double dAncestorPathSigma;     //!< Rate of decay of ancestor relationship penalty
		double dDescendantPathSigma;   //!< Rate of decay of descendant relationship penalty
		double dSiblingPathSigma;      //!< Rate of decay of siblings relationship penalty

		int nCompareNodes;         //!< Measure node similarity
		int nCompareEdges;         //!< Measure edge similarity
		int nDisableNodeSkipping;  //!< Forbids the skipping of nodes when matching

		double dCertaintyParam;     //!< Parameter in the equation of node match certainty

		int nNodeAssignSortType;    //!< Type of sorting for node assignments

		int nPreserveAncestorRel;    //!< Preserve the ancestor relations when matching

		int nUseMWBMHeuristic;      //!< Use or not MWBM before selecting correspondences

		int nMaxNumSolSets;          //!< Maximum number of solutions sets
		int nMaxSolSetsPerIter;      //!< Maximum number of new solutions sets per iteration

		double dBGWrongSidePen;      //!< Wrong side penalty for bone graphs
		double dBGPositionSigma;     //!< Wrong side penalty for bone graphs

		double dSaliencyParam;       //!< Used to compute node saliency
		double dSlopeSigma;          //!< Slope sigma for bone graphs

		int nExtreme;                //!< Load all DAGs to memory before matching

		//Indexing params
		int nUseNewVoteWeightFunc;  //!< Indexing: use new vote weighting function
		int nUseMOOVC;              //!< Indexing: use multiple one-to-one vote counting

		MatchParams();
		void Print(std::ostream& os) const;
	};

protected:
	static MatchParams s_matchParams;

	const DAG* m_pDag1;
	const DAG* m_pDag2;

	SimilarityMeasurer* m_pSimilarityMeasurer;

public:
	const DAG& G1() const { return *m_pDag1; }
	const DAG& G2() const { return *m_pDag2; }

public:
	DAGMatcher(SimilarityMeasurer* pNodeDistMeasurer)
	{
		m_pSimilarityMeasurer = pNodeDistMeasurer;
	}

	//! This virtual destructor must be called from all derived classes
	virtual ~DAGMatcher()
	{
		delete m_pSimilarityMeasurer;
	}

	virtual void InitGraphs(const DAG* pDag1, const DAG* pDag2)
	{
		m_pDag1 = pDag1;
		m_pDag2 = pDag2;

		m_pSimilarityMeasurer->Init(pDag1, pDag2);
	}

	virtual void Clear()
	{
		m_pDag1 = NULL;
		m_pDag2 = NULL;
	}

	virtual double Match(const DAG& g1, const DAG& g2) = 0;

	//! Gets a one-to-one map from g1 nodes to g2 nodes
	virtual void GetNodeMap(NodeMatchMap& nodeMap) const = 0;

	//! Computes the attribute distance between nodes u and v
	double NodeAttributeSimilarity(leda::node u, leda::node v,
		NodeMatchInfoPtr ptrMatchInfo) const
	{
		return m_pSimilarityMeasurer->ComputeNodeSimilarity(u, v, ptrMatchInfo);
	}

	double NodeTSVSimilarity(leda::node u, leda::node v) const;

	double NodeSimilarity(leda::node u, leda::node v,
		NodeMatchInfoPtr ptrMatchInfo) const;

	static void SetMatchParams(const MatchParams& matchParams)
	{
		s_matchParams = matchParams;
	}

	static const MatchParams& GetMatchParams()
	{
		return s_matchParams;
	}
};
} //namespace dml

#endif //__DAG_MATCHER_H__
