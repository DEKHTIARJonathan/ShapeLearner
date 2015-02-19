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
#include <algorithm>
#include "BGSimilarityMeasurer.h"
#include "XORIntegral.h"
#include "HelperFunctions.h"
#include "MathUtils.h"

#define MINDIST 0
#define MAXDIST 1

#define LAST_PARAM_INDEX 3

using namespace std;
using namespace dml;
using namespace leda;

#ifdef _DEBUG
extern LogFile g_dagMatchingLog;
#endif

typedef BGContextualSimilarityMeasurer BGCSM;

/*!
	@brief Computes node distance by taking into account the
	local context of the node.

	Parameter ambiguity: the side and position of the child nodes
	are specified up to the direction and side parameter of the node.
	If the node has no parent, this pair of parameters can take any value.
	However, if any of the nodes has a parent, the values of these parameters
	muts be consistant with its parent.

	Evidence: if a parent node has a known correspondence, then there
	must also be a known inward edge assignment. Similarly,
	if a child node has a known correspondence, it means that
	there is a known outward edge assignment.
*/
double BGCSM::ComputeNodeSimilarity(leda::node v1, leda::node v2,
									NodeMatchInfoPtr ptrMatchInfo) const
{
	if (m_pG1->GetNodeLevel(v1) == 1 || m_pG2->GetNodeLevel(v2) == 1)
	{
		double sim, bestSim = 0;
		int nodeparam, edgeParam;

		for (edgeParam = 0; edgeParam < 4; edgeParam++)
		{
			// If parameterization of position flips, so does the
			// ordering of skeletal points
			nodeparam = (edgeParam == 0 || edgeParam == 2) ? 0 : 1;

			sim = ComputeNodeSimilarity(v1, v2, ptrMatchInfo,
				edgeParam, nodeparam);

			if (sim > bestSim)
			{
				bestSim = sim;
				ptrMatchInfo->SetParameterIndex(edgeParam);
			}
		}

		return bestSim;
	}
	else
	{
		ptrMatchInfo->SetParameterIndex(0);

		return ComputeNodeSimilarity(v1, v2, ptrMatchInfo, 0, 0);
	}
}

double BGCSM::ComputeNodeSimilarity(leda::node v1, leda::node v2,
									NodeMatchInfoPtr ptrMatchInfo,
									int edgeParam, int nodeParam) const
{
	ASSERT(!ptrMatchInfo.IsNull());

	// Make sure that the dummy roots only match each other
	if (m_pG1->NodeType(v1) == BGNode::BACK_BONE ||
		m_pG2->NodeType(v2) == BGNode::BACK_BONE)
	{
		return (m_pG1->NodeType(v1) == m_pG2->NodeType(v2)) ? 1 : 0;
	}

	double edgeSim;

	if (DAGMatcher::GetMatchParams().nCompareEdges)
	{
		// Sum the max of in degrees for both nodes
		double numEdges1   = m_pG1->indeg(v1);
		double numEdges2   = m_pG2->indeg(v2);
		double maxNumEdges = MAX(numEdges1, numEdges2);

		// Sum the max of out degrees for both nodes
		numEdges1   = m_pG1->outdeg(v1);
		numEdges2   = m_pG2->outdeg(v2);
		maxNumEdges += MAX(numEdges1, numEdges2);

		//Solve MWBM problems
		double inEdgeSimSum = MatchEdgesAndParams(*ptrMatchInfo->m_ptrInwardEdgeMap,
			ptrMatchInfo->m_validInwardEdge, 0);

		double outEdgeSimSum = MatchEdgesAndParams(*ptrMatchInfo->m_ptrOutwardEdgeMap,
			ptrMatchInfo->m_validOutwardEdge, edgeParam);

		// Compute edge simialrity
		edgeSim = (inEdgeSimSum + outEdgeSimSum) / maxNumEdges;

		//edgeSim = 0.3 + 0.7 * edgeSim;
	}
	else
	{
		edgeSim = 1;
	}

	ptrMatchInfo->SetEdgeSimilarity(edgeSim);

	// Compute node similarity
	double nodeSim;

	if (!ptrMatchInfo->HasNodeSimilarity(nodeParam))
	{
		ParamIndices parInds(1, nodeParam);

		ComputeNodeDistance(v1, v2, parInds, &nodeSim);
		//nodeSim = BGSimilarityMeasurer::ComputeNodeSimilarity(v1, v2, parInds);

		ptrMatchInfo->SetNodeSimilarity(nodeParam, nodeSim);
	}
	else
	{
		nodeSim = ptrMatchInfo->GetNodeSimilarity(nodeParam);
	}

	return (edgeSim * nodeSim);
}

/*!
	Finds the set of parameters that maximize the edge similarity that
	results from solving the edge assignment problem
*/
double BGCSM::MatchEdgesAndParams(BipartiteEdgeGraph& g,
								  const EdgeValidityArray& edgeValidity,
								  int paramIndex) const
{
	if (g.number_of_edges() == 0)
	{
		return 0;
	}
	else if (g.number_of_edges() == 1)
	{
		leda::edge e = g.first_edge();
		ParamIndices parInds(1, paramIndex);

		return (edgeValidity[e]) ?
			ComputeEdgeSimilarity(g(e, 0), g(e, 1), parInds) : 0;
	}
	else
	{
		EdgeValueArray edgeValues(g);
		leda::edge e;

		ParamIndices parInds(1, paramIndex);

		// Set the values of each candidate mapping in the bipartite graph
		forall_edges(e, g)
		{
			edgeValues[e] = (edgeValidity[e]) ?
				ComputeEdgeSimilarity(g(e, 0), g(e, 1), parInds) : 0;
		}

		// Get the sum of weights in the max weight assignment
		return g.SolveMWAssignmentAndSumWeights(edgeValues);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//

double BGSimilarityMeasurer::ComputeNodeSimilarity(leda::node v1, leda::node v2,
												   NodeMatchInfoPtr ptrMatchInfo) const
{
	double similarity, bestSim = -1;
	ParamIndices parInds(1);

	//parInds[0] = ptrMatchInfo->GetParameterIndex();

	for (parInds[0] = 0; parInds[0] < 2; parInds[0]++)
	{
		ComputeNodeDistance(v1, v2, parInds, &similarity);

		ASSERT_UNIT_INTERVAL(similarity);

		if (similarity > bestSim)
			bestSim = similarity;
	}

	return bestSim;
}

/*!
	@brief
*/
double BGSimilarityMeasurer::ComputeNodeDistance(leda::node v1, leda::node v2,
												 const ParamIndices& parInds,
												 double* pSimilarity) const
{
	if (!DAGMatcher::GetMatchParams().nCompareNodes)
	{
		*pSimilarity = 1;
		return 0;
	}

	const BGNode* pNode1 = m_pG1->GetBGNode(v1);
	const BGNode* pNode2 = m_pG2->GetBGNode(v2);
	double areaDiff;

	bool bCornerBone1 = (pNode1->Type() == BGElement::CORNER_BONE);
	bool bCornerBone2 = (pNode2->Type() == BGElement::CORNER_BONE);

	if (pNode1->Type() == BGElement::BACK_BONE ||
		pNode2->Type() == BGElement::BACK_BONE)
	{
		if (pNode1->Type() != BGElement::BACK_BONE)
		{
			areaDiff = XORIntegral::FunctionArea(pNode1->GetRadiusFunction());

			if (pSimilarity)
				*pSimilarity = 0;
		}
		else if (pNode2->Type() != BGElement::BACK_BONE)
		{
			areaDiff = XORIntegral::FunctionArea(pNode2->GetRadiusFunction());

			if (pSimilarity)
				*pSimilarity = 0;
		}
		else
		{
			areaDiff = 0;

			if (pSimilarity)
				*pSimilarity = 1;
		}
	}
	else if (bCornerBone1 != bCornerBone2)
	{
		areaDiff = XORIntegral::FunctionArea(pNode1->GetRadiusFunction()) +
				XORIntegral::FunctionArea(pNode2->GetRadiusFunction());

		if (pSimilarity)
			*pSimilarity = 0;
	}
	else
	{
		const NodeParams params = GetNodeParams(parInds);

		XORIntegral xi(pNode1->GetRadiusFunction(params.bFlipPosition),
			pNode2->GetRadiusFunction());

		areaDiff = xi.XORArea();

		*pSimilarity = xi.FunctionSimilarity(DAGMatcher::GetMatchParams().dSlopeSigma);

		if (params.bFlipPosition)
		{
			XORIntegral xi2(pNode1->GetRadiusFunction(),
				pNode2->GetRadiusFunction(true));

			double sim2 = xi2.FunctionSimilarity(DAGMatcher::GetMatchParams().dSlopeSigma);

			if (*pSimilarity < sim2)
			{
				*pSimilarity = sim2;
				areaDiff = xi2.XORArea();
			}
		}

		// Correct rounding errors
		if (*pSimilarity < 0 && *pSimilarity > -0.00001)
			*pSimilarity = 0;
		else if (*pSimilarity > 1 && *pSimilarity < 1.00001)
			*pSimilarity = 1;

		ASSERT_UNIT_INTERVAL(*pSimilarity);

		/*if (bCornerBone1)
		{
			ASSERT(bCornerBone2);

			if (pSimilarity)
				*pSimilarity *= 0.5;
		}*/
	}

	return areaDiff;
}

/*!
	@brief
*/
double BGSimilarityMeasurer::ComputeEdgeSimilarity(leda::edge e1, leda::edge e2,
												   const ParamIndices& parInds) const
{
	if (!DAGMatcher::GetMatchParams().nCompareEdges)
	{
		return 1;
	}

	const BGEdge* pEdge1 = m_pG1->GetBGEdge(e1);
	const BGEdge* pEdge2 = m_pG2->GetBGEdge(e2);

	const NodeParams params = GetNodeParams(parInds);

	double angle1 = pEdge1->Angle(params.bFlipSide);
	double angle2 = pEdge2->Angle();

	double pos1 = pEdge1->Position(params.bFlipPosition);
	double pos2 = pEdge2->Position();

	// Set the angle and position of skipped edges
	if (parInds.size() >= 3 && parInds[1] == 1)
	{
		// the edge is the out edge of a skipped node with indeg 1.
		// we really want to compare that pos of the inedge of the parent node
		leda::node v = source(e1);
		ASSERT(indeg(v) == 1);

		leda::edge ee = m_pG1->first_in_edge(v); //skipped edge

		angle1 = m_pG1->GetBGEdge(ee)->Angle(params.bFlipSide);
		pos1   = m_pG1->GetBGEdge(ee)->Position(params.bFlipPosition);
	}

	if (parInds.size() >= 3 && parInds[2] == 1)
	{
		leda::node v = source(e2); //skipped node
		ASSERT(indeg(v) == 1);

		leda::edge ee = m_pG2->first_in_edge(v); //skipped edge

		angle2 = m_pG2->GetBGEdge(ee)->Angle();
		pos2   = m_pG2->GetBGEdge(ee)->Position();
	}

	// Set the angle and position of inverted edges
	if (parInds.size() >= 5 && parInds[3] == 1)
	{
		angle1 = 0;
		pos1 = 1;
	}

	if (parInds.size() >= 5 && parInds[4] == 1)
	{
		angle2 = 0;
		pos2 = 1;
	}

	double angleSimilarity;

	if (angle1 != 0 && angle2 != 0 && SIGN(angle1) != SIGN(angle2))
		angleSimilarity = DAGMatcher::GetMatchParams().dBGWrongSidePen;
	else
		angleSimilarity = 1;

	double positionSimilarity;
	const double& sigma = DAGMatcher::GetMatchParams().dBGPositionSigma;

	if (sigma == 0)
	{
		positionSimilarity = 1 - fabs(pos1 - pos2);
	}
	else
	{
		double posDiff = pos1 - pos2;

		positionSimilarity = exp(-(posDiff * posDiff) / (2 * sigma * sigma));
	}

	return angleSimilarity * positionSimilarity;
}