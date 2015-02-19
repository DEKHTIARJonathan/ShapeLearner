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
 * $Log: MatchedNodePair.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include "MatchedNodePair.h"

using namespace dml;

/*!
 	@brief Builds a sort of reduced adjacency matrix for the graph.

 	Needs to save the parents before the DAGs are splitted and
 	the links on loopy nodes are lost.
 */
void MatchedNodePair::SetParents(const DAG& g, NodeIdxMatrix& parents)
{
 	leda::node v;
 	leda::edge e;
 	int i, j;

 	parents.Resize(g.GetNodeCount());

 	forall_nodes(v, g)
 	{
 		i = g.GetNodeDFSIndex(v);
 		ASSERT(parents[i].GetSize() == 0);
 		parents[i].Resize(g.indeg(v));

 		j = 0;
 		forall_in_edges(e, v)
 			parents[i][j++] = g.GetNodeDFSIndex(g.source(e));
 	}
}

/*static*/
NodeIdxArray MatchedNodePair::GetSiblingsVector(int node, const SmartMatrix<int>& tcm,
												const NodeIdxMatrix& parents)
{
	const NodeIdxArray& vpar = parents[node];
	NodeIdxArray sibs;
	int parent;

	// Look for all the nodes that can be reached from the parents
	// of the given 'node'. i.e., the node's siblings and their descendents

	// For every parent of 'node'
	for (int i = 0; i < vpar.GetSize(); i++)
	{
		parent = vpar[i];

		// If it is the first parent with children, then we just
		// copy the vector of nodes that it reaches
		if (sibs.GetSize() == 0)
			sibs = tcm[parent];
		else // this isn't the first parent. Add its children too
		{
			const NodeIdxArray& halfSibs = tcm[parent];
			ASSERT(sibs.GetSize() == halfSibs.GetSize());

			// Do sibs |= halfSibs;
			for (int j = 0; j < halfSibs.GetSize(); j++)
				if (halfSibs[j] == 1 && sibs[j] != 1) // only modifies sibs if necessary
					sibs[j] = 1;
		}

		// we consider that a node is a sibling of itself, but the node's
		// parent isn't its sibling (obviously), so we need to correct this...
		sibs[parent] = 0;
	}

	// If the only siblinf of 'node' is itself, then is better to return
	// an empty array. Since there are only zeros and ones, we cun sum
	// all values. Is the sum is smaller or equal to 1, the only sibling
	// of 'none' is itself, and we can proceed to empty the array.
	if (sibs.Sum() <= 1)
		sibs.Clear();

	return sibs;
}

/*!
	@brief Updates the similarity matrix with the penalty terms for breaking
	sibling relations and ancestor relations.

	Let S_{Qv} the vector of sibling of node v in graph Q, which are computed from
	adjacency matrix and the trans. clos. mat. of the graph.

	S_{Qv} = OR(T_Q(A_Q(:,v), :))
	S_{Mw} = OR(T_M(A_M(:,w), :))

	The vector of penalties for nodes in M mathing non-siblings of v in Q can be computed
	by: S'_{Qv} = penalty * (1 - S_{Qv}). Similarly for S'_{Mw} = penalty * (1 - S_{Mw}).

	Then, we proceed to update the sim matrix by

	simMat(S_{Qv}, :) = simMat(S_{Qv}, :) .* S'_{Mw};
	simMat(S_{Mw}, :) = simMat(S_{Mw}, :) .* S'_{Qv};
*/
void MatchedNodePair::UpdateSimMat(SimMatrix& simMat, const double& alpha) const
{
	int rows = simMat.NRows(), cols = simMat.NCols();

	ASSERT_UNIT_INTERVAL(alpha);

	if (alpha == 1) //nothing to do
		return;

	SmartArray<int> sq, sm;
	int i, j;

	sq = GetSiblingsVector(q, *pQueryTCM, qparents);
	sm = GetSiblingsVector(m, *pModelTCM, mparents);

	rows = sq.GetSize();
	cols = sm.GetSize();

	// rows or cols will be zero when there are no siblings
	// if either q or m has no siblings, there is no point in
	// penalizing the siblings that do exist, because there is no
	// sibling relations to preserve.
	if (rows == 0 || cols == 0)
		return;

	// Update the similarity matrix
	for (i = 0; i < rows; i++)
		if (sq[i] == 1) 					// if 'v' in Q is a sibling of 'q'
			for (j = 0; j < cols; j++)		// travers its matches from M
				if (sm[j] == 0) 			// if 'w' in M isn't a sibling of 'm'
					simMat[i][j] *= alpha;

	for (j = 0; j < cols; j++)
		if (sm[j] == 1) 					// if 'v' in Q is a sibling of 'q'
			for (i = 0; i < rows; i++)		// travers its matches from M
				if (sq[i] == 0) 			// if 'w' in M isn't a sibling of 'm'
					simMat[i][j] *= alpha;
}