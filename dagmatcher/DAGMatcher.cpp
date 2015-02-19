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

#include "DAGMatcher.h"
#include "DAG.h"
#include "HelperFunctions.h"
#include "BasicUtils.h"

using namespace dml;

#ifdef _DEBUG
LogFile g_dagMatchingLog("dagMatching.log", &DAG::s_bDbgMode);
#endif

/////////////////////////////////////////////////////////////////////////////
// DAGMatcher::MatchParams class implementation

DAGMatcher::MatchParams DAGMatcher::s_matchParams;

//! Initializes all matching parameter values to zero
DAGMatcher::MatchParams::MatchParams()
{
	memset(this, 0, sizeof(*this));
}

//! Prints all matching parameter values
void DAGMatcher::MatchParams::Print(std::ostream& os) const
{
	PRINT_OPEN(os, nMatchingAlgorithm);
	PRINT_CLOSE(os, nNodeSimilarityFunction);

	PRINT_OPEN(os, dTSVSimWeight);
	PRINT(os, dSimilMassWeight);
	PRINT(os, dRelMassWeight);
	PRINT_CLOSE(os, dDiffAttachPosWeight);

	PRINT_OPEN(os, dBreakAncestorRelPen);
	PRINT(os, dBreakDescendantRelPen);
	PRINT(os, dBreakSiblingRelPen);

	PRINT(os, dAncestorPathSigma);
	PRINT(os, dDescendantPathSigma);
	PRINT_CLOSE(os, dSiblingPathSigma);

	PRINT_OPEN(os, nPreserveAncestorRel);
	PRINT(os, nUseNewVoteWeightFunc);
	PRINT_CLOSE(os, nUseMOOVC);
}

/////////////////////////////////////////////////////////////////////////////
// DAGMatcher class implementation

/*//! Virtual destructor
DAGMatcher::~DAGMatcher()
{
	delete m_pSimilarityMeasurer;
}

double DAGMatcher::NodeAttributeSimilarity(const DAG& g1, leda::node u, const DAG& g2,
										   leda::node v, NodeMatchInfoPtr ptrMatchInfo) const
{
	return m_pSimilarityMeasurer->ComputeSimilarity(g1, u, g2, v, ptrMatchInfo);
}*/

/*!
	@brief Computes the structural similarity between node u in g1 and node v in g2

	The formula used is:

	\[ \fraq{|tsv_1 - tsv_2|}{\mbox{max} (|tsv_1|, (|tsv_2|)} \]

	If both terms are zero, the similarity returned is 1.
	If the numerator is grater than or equal to the numerator,
	the TSV similarity is 0.
*/
double DAGMatcher::NodeTSVSimilarity(leda::node u, leda::node v) const
{
	const TSV& tsv1 = m_pDag1->GetNodeTSV(u);
	const TSV& tsv2 = m_pDag2->GetNodeTSV(v);

	double n1 = m_pDag1->GetNodeTSVNorm(u);
	double n2 = m_pDag2->GetNodeTSVNorm(v);

	if (n1 == 0 && n2 == 0)
		return 1;

	TSV diff = tsv1 - tsv2;
	double diffNorm = diff.Norm2();

	double max = MAX(n1, n2);

	// max should be a normalization factor to ensure
	// the the result would be between zero and 1. However,
	// it is possible that diffNorm is grater than max, and so
	// we need to handle this case somehow. Clearly, a better solution
	// would be to use a better normalization factor.
	if (diffNorm > max)
		return 0;

	return 1 - diffNorm / max;
}

/*!
	@brief Computes the attribute and structural similarity between
		   node u in g1 and node v in g2
*/
double DAGMatcher::NodeSimilarity(leda::node u, leda::node v,
								  NodeMatchInfoPtr ptrMatchInfo) const
{
	double dNodeSim, dEigenSim, dSimilarity;

	if (!ptrMatchInfo.IsNull())
	{
		if (!ptrMatchInfo->HasTopologicalSimilarity())
			ptrMatchInfo->SetTopologicalSimilarity(NodeTSVSimilarity(u, v));

		dEigenSim = ptrMatchInfo->GetTopologicalSimilarity();
	}
	else
	{
		dEigenSim = NodeTSVSimilarity(u, v);
	}

	ASSERT_VALID_NUM(dEigenSim);
	ASSERT_UNIT_INTERVAL(dEigenSim);

	// Get the node attribute similarity
	dNodeSim = NodeAttributeSimilarity(u, v, ptrMatchInfo);

	ASSERT_VALID_NUM(dNodeSim);
	//ASSERT(dNodeSim >= 0);

	// Set the overall similarity value
	if (dNodeSim > 0.0)
	{
		dSimilarity = s_matchParams.dTSVSimWeight * dEigenSim
			+ (1 - s_matchParams.dTSVSimWeight) * dNodeSim;
	}
	else
	{
		dSimilarity = 0.0;
		WARNING(dSimilarity < -0.1, "Node similarity is smaller than zero");
	}

	// Print some info (only applied if in dbg model)
	DBG_M_LOG_ONLY(g_dagMatchingLog.Print("Node similarity %s - %s: "
		"node sim %f, tsv sim %f (%f), sim %f\n\n",
		(CSTR) m_pDag1->GetNodeDFSLbl(u), (CSTR) m_pDag2->GetNodeDFSLbl(v),
		dNodeSim, dEigenSim, s_matchParams.dTSVSimWeight, dSimilarity))

	ASSERT_VALID_NUM(dSimilarity);
	ASSERT(dSimilarity >= 0);

	if (!ptrMatchInfo.IsNull())
		ptrMatchInfo->SetSimilarityValue(dSimilarity);

	return dSimilarity;
}