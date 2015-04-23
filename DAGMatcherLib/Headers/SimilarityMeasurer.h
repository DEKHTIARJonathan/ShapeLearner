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
* $Log: SimilarityMeasurer.h,v $
*
*-----------------------------------------------------------------------*/

#ifndef __SIMILARITY_MEASURER_H__
#define __SIMILARITY_MEASURER_H__

#include "stdafx.h"
#include "NodeMatchInfo.h"

namespace dml {
typedef std::vector<int> ParamIndices;

/*!
	Abstract class for measuring the attribute distance between
	two nodes of two different DAGs.
*/
class SimilarityMeasurer
{
public:
	virtual ~SimilarityMeasurer() {}

	virtual void Init(const DAG* pDag1, const DAG* pDag2) = 0;

	virtual double ComputeNodeSimilarity(leda::node v1, leda::node v2,
		NodeMatchInfoPtr ptrMatchInfo) const = 0;

	virtual double ComputeNodeSimilarity(leda::node v1, leda::node v2,
		const ParamIndices& parInds) const = 0;

	virtual double ComputeNodeDistance(leda::node v1, leda::node v2,
		const ParamIndices& parInds, double* pSimilarity = NULL) const = 0;

	virtual double ComputeEdgeSimilarity(leda::edge e1, leda::edge e2,
		const ParamIndices& parInds) const = 0;
};
} //namespace dml

#endif //__SIMILARITY_MEASURER_H__
