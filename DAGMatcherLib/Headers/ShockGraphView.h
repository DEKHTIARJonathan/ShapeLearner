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
 * $Log: ShockGraphView.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef _SHOCK_GRAPH_VIEW_H_
#define _SHOCK_GRAPH_VIEW_H_

#include "DAGView.h"
#include "ShockGraph.h"

namespace dml {
/*!
	@brief This class groups all the operations needed to visualize a ShockGraph object.
*/
class ShockGraphView : public DAGView
{
public:

	ShockGraphView();

	double FindClosestSkeletonPoint(const double& x, const double& y,
		const ImageInfo* ii, sg::FluxPoint& fp, sg::BoundaryInfo& bi) const;

	virtual void DrawNodeInfo(const DAGNode* pDAGNode, const ImageInfo* ii) const;
	virtual void DrawMatchingLines() const;
	virtual void OnMouseMove(const leda::point& pt, const ImageInfo* ii);

	const ShockGraph* GetGraph(const ImageInfo* ii) const
	{
		return dynamic_cast<const ShockGraph*>(ii->pDAG);
	}
};
} // namespace dml

#endif //_SHOCK_GRAPH_VIEW_H_
