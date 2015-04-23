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

#ifndef __MATCHED_NODE_PAIR_H__
#define __MATCHED_NODE_PAIR_H__

#include "stdafx.h"

namespace dml {
typedef SmartMatrix<double> SimMatrix;
typedef SmartArray<int> NodeIdxArray;
typedef SmartArray<NodeIdxArray> NodeIdxMatrix;

/*!
	Represents a node correspondance between a node in a query
	graph and a node in a model graph. It also has access to the
	ancestor and descendant information associated with each node.

	This class is used by the DAGMatcherGreedy algorithm, which is
	based on recursively splitting the query and model graphs. Since
	such an operation destroys the ancestor and descendant information
	the MatchedNodePair class precomputes the info that it needs to
	in order to answer topologigal questions later.
*/
class MatchedNodePair
{
	int q;                               //!< DFS index of query node
	int m;                               //!< DFS index of model node

	const SmartMatrix<int>* pQueryTCM;   //!< Query's transitive closure matrix
	const SmartMatrix<int>* pModelTCM;   //!< Models's transitive closure matrix

	NodeIdxMatrix qparents;              //!< Adjacency matrix for query graph
	NodeIdxMatrix mparents;              //!< Adjacency matrix for model graph

private:
	static bool IsAncestor(int u, int v, const SmartMatrix<int>& tcm)
	{
		return tcm[u][v] != 0;
	}

	static bool IsSibling(int u, int v, const SmartMatrix<int>& tcm,
		const NodeIdxMatrix& parents)
	{
		const NodeIdxArray& vpar = parents[v];

		for (int i = 0; i < vpar.GetSize(); i++)
			if (IsAncestor(vpar[i], u, tcm))
				return true;

		return false;
	}

	static NodeIdxArray GetSiblingsVector(int v, const SmartMatrix<int>& tcm,
		const NodeIdxMatrix& parents);

public:
	MatchedNodePair(const DAG& query, const DAG& model)
	{
		pQueryTCM = &query.GetTransClosMat();
		pModelTCM = &model.GetTransClosMat();

		SetParents(query, qparents);
		SetParents(model, mparents);

		SetEmpty();
	}

	void SetEmpty() { q = m = -1; }

	bool IsEmpty() const { return (q == -1 && m == -1); }

	void SetNodes(int v1, int v2)
	{
		q = v1;
		m = v2;
	}

	void SetParents(const DAG& g, NodeIdxMatrix& parents);

	int GetQueryNode() const { return q; }
	int GetModelNode() const { return m; }

	bool AncestorRelPreserved(int qq, int mm) const
	{
		bool aq = IsAncestor(qq, q, *pQueryTCM), am = IsAncestor(mm, m, *pModelTCM);
		return (aq && am) || (!aq && !am);
	}

	bool SiblingRelPreserved(int qq, int mm) const
	{
		bool sq = IsSibling(qq, q, *pQueryTCM, qparents);
		bool sm = IsSibling(mm, m, *pModelTCM, mparents);

		return (sq && sm) || (!sq && !sm);
	}

	void UpdateSimMat(SimMatrix& simMat, const double& alpha) const;
};
} //namespace dml

#endif //__MATCHED_NODE_PAIR_H__