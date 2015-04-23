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

#ifndef __NODE_PAIR_INFO_H__
#define __NODE_PAIR_INFO_H__

#include "stdafx.h"

namespace dml {
/*!
	Represents the information asssociated with a pair of nodes, one
	in a ``query'' graph and the other in a ``model'' graph.
*/
class NodePairInfo
{
private:
	const DAG& Q;      //!< Reference to the query graph
	const DAG& M;      //!< Reference to the model graph

	leda::node q;      //!< Current query node being matched
	leda::node m;      //!< Current model node being matched

protected:

	//! Gets the penalty parameters for breaking graph relatioships
	static const DAGMatcher::MatchParams& Params()
	{
		return DAGMatcher::GetMatchParams();
	}

	//! Gets the transitive closure matrix of the query (i==0) or the model (i==1)
	/*const SmartMatrix<int>& TCM(int i) const
	{
		ASSERT(i == 0 || i == 1);

		return (i == 0) ? Q.GetTransClosMat() : M.GetTransClosMat();
	}*/

	//! Gets the DFS index of the query node (i==0) or the model node (i==1)
	/*int NodeIndex(int i) const
	{
		ASSERT(i == 0 || i == 1);

		return (i == 0) ? Q.GetNodeDFSIndex(q) : M.GetNodeDFSIndex(m);
	}*/

	//! Gets the DFS index of node v in the query (i==0) or in the model (i==1)
	/*int NodeIndex(int i, leda::node v) const
	{
		ASSERT(i == 0 || i == 1);

		return (i == 0) ? Q.GetNodeDFSIndex(v) : M.GetNodeDFSIndex(v);
	}*/

public:
	/*NodePairInfo(const DAG& query, const DAG& model)
		: Q(query), M(model)
	{
		q = nil;
		m = nil;
	}*/

	NodePairInfo(leda::node qq, const DAG& query, leda::node mm, const DAG& model)
		: Q(query), M(model)
	{
		q = qq;
		m = mm;
	}

	NodePairInfo(const NodePairInfo& rhs)
		: Q(rhs.Q), M(rhs.M)
	{
		q = rhs.q;
		m = rhs.m;
	}

	bool HasNodeMatch() const
	{
		return (q != nil && m != nil);
	}

	//! Sets the pair of query and model nodes being matched
	void SetCurrentMatchedNodes(leda::node u, leda::node v, const double& sim)
	{
		ASSERT(graph_of(u) == &Q);
		ASSERT(graph_of(v) == &M);

		q = u;
		m = v;
	}

	//! Gets the query graph (i == 0) or the model graph (i == 1)
	const DAG& Graph(int i) const
	{
		ASSERT(i == 0 || i == 1);

		return (i == 0) ? Q : M;
	}

	//! Gets the query node 'q' (i == 0) or the model node 'm' (i == 1)
	leda::node Node(int i) const
	{
		ASSERT(i == 0 || i == 1);

		return (i == 0) ? q : m;
	}

	//! Gets the level of the node in the DAG
	int GetAbsoluteNodeLevel(int i) const
	{
		return Graph(i).GetNodeLevel(Node(i));
	}

	//! Gets the relative level of v wrt u = {q, m} defined as: rel_level = level(v) - level(u)
	int GetRelativeNodeLevel(int i, leda::node v) const
	{
		return Graph(i).GetRelativeNodeLevel(Node(i), v);
	}

	//! Gets the average absolute level of both nodes
	double GetAverageNodeLevel() const
	{
		return ((GetAbsoluteNodeLevel(0) +
			GetAbsoluteNodeLevel(1)) / 2.0);
	}

	//! True if node 'v' is equal to the query node (i==0) or the model node (i==1)
	bool IsMatchedNode(int i, leda::node v) const
	{
		ASSERT_SAME_GRAPH(v, Node(i));

		return (Node(i) == v);
	}

	//! True if there is a path from node 'a' to 'q' (i==0) or 'm' (i==1)
	bool IsAncestor(int i, leda::node a) const
	{
		return Graph(i).IsAncestor(a, Node(i));
	}

	//! True if there is a path from 'q' (i==0) or 'm' (i==1) to 'd'
	bool IsDescendant(int i, leda::node d) const
	{
		return Graph(i).IsDescendant(d, Node(i));
	}

	/*!
		True if node 'v' is a child of a parent of the query
		node (i == 0) or the model node (i == 1)
	*/
	bool IsSibling(int i, leda::node v) const
	{
		return Graph(i).IsSibling(v, Node(i));
	}

	/*!
		True if there is a path from a parent of the query node (i == 0)
		or the model node (i == 1) to node 'v'
	*/
	bool IsSiblingOrSiblingDescendant(int i, leda::node v) const
	{
		return Graph(i).IsSiblingOrSiblingDescendant(v, Node(i));
	}

	//! Finds the first edge in the path that goes from 'a' to 'q' or 'm'
	leda::edge FindFirstEdgeFromAncestor(int i, leda::node a) const
	{
		return Graph(i).FindFirstEdgeInPath(a, Node(i));
	}

	//! Finds the last edge in the path that goes from 'q' or 'm' to 'd'
	leda::edge FindLastEdgeToDescendant(int i, leda::node d) const
	{
		return Graph(i).FindLastEdgeInPath(Node(i), d);
	}

	//! True iff both 'qq' and 'mm' have the same ancestor relationship with 'q' and 'm'
	bool IsAncestorRelPreserved(leda::node qq, leda::node mm) const
	{
		const bool aq = IsAncestor(0, qq);
		const bool am = IsAncestor(1, mm);

		// Check that if qq or mm are ancestors, then both nodes are ancestors
		if (aq || am)
			return (aq && am);

		// Neither is true and so both have the same type of relationship
		return true;
	}

	//! True iff both 'qq' and 'mm' have the same descendant relationship with 'q' and 'm'
	bool IsDescendantRelPreserved(leda::node qq, leda::node mm) const
	{
		// Now, we known that neither node is an ancestor. Check the opposite
		// direction in which q or m are ancestors
		const bool dq = IsDescendant(0, qq);
		const bool dm = IsDescendant(1, mm);

		// Check that if qq or mm are desendants, then both nodes are desendants
		if (dq || dm)
			return (dq && dm);

		// Neither is true and so both have the same type of relationship
		return true;
	}

	//! True iff both 'qq' and 'mm' have the same SSD relationship with 'q' and 'm'
	bool IsSiblingOrSiblingDescendantRelPreserved(leda::node qq, leda::node mm) const
	{
		const bool sq = IsSiblingOrSiblingDescendant(0, qq);
		const bool sm = IsSiblingOrSiblingDescendant(1, mm);

		// Check that if q is SSD of qq and m is SSD of mm, then both are SSD
		if (sq || sm)
			return (sq && sm);

		// Neither is true and so both have the same type of relationship
		return true;
	}

	/*double ComputePenalty(const double& pen, leda::node q_p, leda::node q_c,
		leda::node m_p, leda::node m_c) const
	{
		return (pen == 0) ? 0 : 1;
	}*/

	/*!
		@brief Gets the penalty associated with breaking the ancestor
		relation with either node 'qq' or 'mm'

		@pre{Either qq is an ancestor of q or mm is an ancestor of m}
	*/
	double  GetAncestorPenalty(leda::node qq, leda::node mm) const
	{
		ASSERT(IsAncestor(0, qq) || IsAncestor(1, mm));

		const double& pen = Params().dBreakAncestorRelPen;

		if (IsAncestorRelPreserved(qq, mm))
		{
			int relLevel0 = GetRelativeNodeLevel(0, qq);
			int relLevel1 = GetRelativeNodeLevel(1, mm);
			int diff = relLevel0 - relLevel1;

			double sigma = Params().dAncestorPathSigma;

			if (diff == 0 || sigma == 0)
				return 1; //(ie, no penalty)
			else
				return pen + (1 - pen) *
					exp(-(diff * diff) / (2 * sigma * sigma));
		}
		else
		{
			return pen;
		}
	}

	/*!
		@brief Gets the penalty associated with breaking the descendant
		relation with either node 'qq' or 'mm'

		@pre{Either qq is a descendant of q or mm is a descendant of m}
	*/
	double GetDescendantPenalty(leda::node qq, leda::node mm) const
	{
		ASSERT(IsDescendant(0, qq) || IsDescendant(1, mm));

		const double& pen = Params().dBreakDescendantRelPen;

		if (IsDescendantRelPreserved(qq, mm))
		{
			int relLevel0 = GetRelativeNodeLevel(0, qq);
			int relLevel1 = GetRelativeNodeLevel(1, mm);
			int diff = relLevel0 - relLevel1;

			double sigma = Params().dDescendantPathSigma;

			if (diff == 0 || sigma == 0)
				return 1; //(ie, no penalty)
			else
				return pen + (1 - pen) *
					exp(-(diff * diff) / (2 * sigma * sigma));
		}
		else
		{
			return pen;
		}
	}

	/*!
		@brief Gets the penalty associated with breaking the sibling
		relation with either node 'qq' or 'mm'

		@pre{Either qq is a sibling of q or mm is a sibling of m}
	*/
	double GetSiblingPenalty(leda::node qq, leda::node mm) const
	{
		ASSERT(IsSiblingOrSiblingDescendant(0, qq) ||
			   IsSiblingOrSiblingDescendant(1, mm));

		const double& pen = Params().dBreakSiblingRelPen;

		if (IsSiblingOrSiblingDescendantRelPreserved(qq, mm))
		{
			if (!IsSiblingOrSiblingDescendant(0, qq) && !IsSiblingOrSiblingDescendant(1, mm))
			{
				ASSERT(false);
				return 1;
			}

			const double& sigma = Params().dSiblingPathSigma;

			// Check that the rate of decay is positive
			ASSERT(sigma >= 0);

			int relLevel0 = GetRelativeNodeLevel(0, qq);
			int relLevel1 = GetRelativeNodeLevel(1, mm);

			//WARNING(diffLevel0 < 0 || diffLevel1 < 0, "level issue");
			//ASSERT(diffLevel0 >= 0 && diffLevel1 >= 0);

			if (relLevel0 < 0)
				relLevel0 = relLevel1; // diff will be zero
			else if (relLevel1 < 0)
				relLevel1 = relLevel0; // diff will be zero

			// The further down the node is the greater the penalty...
			//double diff = abs(relLevel0 - relLevel1);
			double diff = relLevel0 - relLevel1;

			// The penalty discount is maximum when diff is zero or
			// when no decay is requested
			if (diff == 0 || sigma == 0)
				return 1; //(ie, no penalty)
			else
				return pen + (1 - pen) *
					exp(-(diff * diff) / (2 * sigma * sigma));
		}
		else
		{
			return pen;
		}
	}

	void Print(std::ostream& os) const
	{
		os << Q.GetNodeDFSLbl(q) << " <--> " << M.GetNodeDFSLbl(m);
	}

	void Print(std::ostream& os, leda::node qq, leda::node mm, const double& pen) const
	{
		Print(os);
		os << " => " << Q.GetNodeDFSLbl(qq) << " - " << M.GetNodeDFSLbl(mm)<< " = " << pen;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// Static functions

	//! True if node 'u' is a parent of node 'v'
	static bool IsParent(leda::node u, leda::node v)
	{
		leda::edge e;

		forall_in_edges(e, v)
		{
			if (source(e) == u)
				return true;
		}

		return false;
	}

	//! True if node 'u' is a child of node 'v'
	static bool IsChild(leda::node u, leda::node v)
	{
		leda::edge e;

		forall_out_edges(e, v)
		{
			if (target(e) == u)
				return true;
		}

		return false;
	}

	/*!
		True if direction is 0 and node 'u' is parent of node 'v' or
		if direction is 1 and node 'u' is child of node 'v'
	*/
	static bool IsImmediateRelative(int direction, leda::node u, leda::node v)
	{
		ASSERT(direction == 0 || direction == 1);

		return (direction == 0) ? IsParent(u, v) : IsChild(u, v);
	}
};
} //namespace dml

#endif //__NODE_PAIR_INFO_H__