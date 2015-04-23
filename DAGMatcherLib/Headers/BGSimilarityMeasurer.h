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
*
*-----------------------------------------------------------------------*/

#ifndef __BG_SIMILARITY_MEASURER_H__
#define __BG_SIMILARITY_MEASURER_H__

#include "stdafx.h"


namespace dml {
//typedef SimilarityMeasurerT<BoneGraph> BGSimilarityMeasurer;

/*!
	Basic node distance measure for Bone Graphs.
*/
class BGSimilarityMeasurer : public SimilarityMeasurerT<BoneGraph>
{
protected:
	struct NodeParams
	{
		bool bFlipPosition;
		bool bFlipSide;
	};

	NodeParams GetNodeParams(const ParamIndices& parInds) const
	{
		NodeParams np[4] = {{false, false}, {true, false},
		                    {false, true},  {true, true}};

		ASSERT(parInds[0] >= 0 && parInds[0] <= 3);

		return np[parInds[0]];
	}

public:

	virtual double ComputeNodeSimilarity(leda::node v1, leda::node v2,
		NodeMatchInfoPtr ptrMatchInfo) const;

	virtual double ComputeNodeSimilarity(leda::node v1, leda::node v2,
		const ParamIndices& parInds) const
	{
		double simVal;

		ComputeNodeDistance(v1, v2, parInds, &simVal);

		return simVal;
	}

	virtual double ComputeNodeDistance(leda::node v1, leda::node v2,
		const ParamIndices& parInds, double* pSimilarity = NULL) const;

	virtual double ComputeEdgeSimilarity(leda::edge e1, leda::edge e2,
		const ParamIndices& parInds) const;
};

/*!
	Contextual node distance measurer for Bone Graphs.
*/
class BGContextualSimilarityMeasurer: public BGSimilarityMeasurer
{
public:
	virtual double ComputeNodeSimilarity(leda::node v1, leda::node v2,
		NodeMatchInfoPtr ptrMatchInfo) const;

	double ComputeNodeSimilarity(leda::node v1, leda::node v2,
		NodeMatchInfoPtr ptrMatchInfo, int edgeParam, int nodeParam) const;

	double MatchEdgesAndParams(BipartiteEdgeGraph& g,
		const EdgeValidityArray& edgeValidity, int paramIndex) const;
};
} //namespace dml

#endif //__BG_SIMILARITY_MEASURER_H__