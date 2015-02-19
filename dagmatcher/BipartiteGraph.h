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
 * $Log: BipartiteNodeGraph.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __BIPARTITE_GRAPH_H__
#define __BIPARTITE_GRAPH_H__

#include <LEDA/graph/graph.h>
#include "BasicUtils.h"

namespace dml {
typedef leda::list<leda::edge> NodeAssigmentList;
typedef leda::edge_array<double> EdgeValueArray;
typedef leda::edge_array<bool> EdgeValidityArray;

/*!
	@brief Bipartite graph with node information of type T and
	edge information of type U.

	The type T has no restrictions. The type U must act as a type
	"double". That is, it must have a constructor with a single
	double parameter, and must be "castable" to a double. The
	double value must represents the similarity between the two
	nodes in graph linked by the edge.

	Usage:
	BipartiteGraph<T, U> g;

	// Add nodes and edges to the graph

	EdgeValueArray edgeValues(g); // or .init(g) later
	NodeAssigmentList nodeAssigments;

	// fill in the values of each candidate assignment
	forall_defined(e, edgeValues)
	{
		edgeValues[e] = similarity(g(e, 0), g(e, 1));
	}

	g.SolveMaxWeightAssignment(edgeValues, &nodeAssigments);
*/
template <typename T, typename U = double> class BipartiteGraph : public leda::GRAPH<T, U>
{
public:
	typedef leda::two_tuple<double, U> AssignmentInfo;
	typedef leda::list<AssignmentInfo> AssignmentInfoList;
	typedef leda::list<leda::node> NodeSet;
	typedef leda::list_item NodeSetIterator;

protected:
	NodeSet m_setA, m_setB;

	//! Shortcut for indexing over the sets A (i==0) and B (i == 1)
	NodeSet& GetSet(int i)
	{
		ASSERT(i == 0 || i == 1);

		return (i == 0) ? m_setA : m_setB;
	}

	//! Shortcut for indexing over the sets A (i==0) and B (i == 1)
	const NodeSet& GetSet(int i) const
	{
		ASSERT(i == 0 || i == 1);
		return (i == 0) ? m_setA : m_setB;
	}

public:
	//! Provides a non-const reference to node v's information
	T& Inf(leda::node v)
	{
		return this->operator[](v);
	}

	//! Provides a non-const reference to edge e's information
	U& Inf(leda::edge e)
	{
		return this->operator[](e);
	}

	//! Number of edges with non-zero values in the bipartite graph
	int CandidateCorrespondenceCount() const
	{
		return this->number_of_edges();
	}

	//! Computes a one-to-one assigment between nodes in sets A and B
	void SolveMaxWeightAssignment(const EdgeValueArray& edgeValues,
		                          NodeAssigmentList* pNodeAssigments)
	{
		// Compute maximum weight assignment
		//MWBM_SCALE_WEIGHTS(*this, edgeValues);

		*pNodeAssigments = MAX_WEIGHT_BIPARTITE_MATCHING(*this, edgeValues);
	}

	//! Sums the values associated with each assignment
	double SumAssignmentValues(const EdgeValueArray& edgeValues,
		                    const NodeAssigmentList& nodeAssigments) const
	{
		leda::edge e;
		double sum = 0;

		forall(e, nodeAssigments)
		{
			sum += edgeValues[e];
		}

		return sum;
	}

	//! Computes a max weight assignment and sums the resulting weights
	double SolveMWAssignmentAndSumWeights(const EdgeValueArray& edgeValues)
	{
		NodeAssigmentList nal;

		SolveMaxWeightAssignment(edgeValues, &nal);

		return SumAssignmentValues(edgeValues, nal);
	}

	//! Compares two values such as to create an "descending" order
	static int CompareValuesDescend(const AssignmentInfo& a1, const AssignmentInfo& a2)
	{
		return (a1.first() > a2.first()) ? -1 : ((a1.first() < a2.first()) ? 1 : 0);
	}

	/*!
		Fills a list of (V[e], inf[e]) pairs for all e in L. This list can be used
		to sort the assignment by different criteria.

		@param bSort if true, *pAssignment is sorted by value[e]
		@return the sum of all V[e] in the list
	*/
	double PopulateAssignmentInfoList(const EdgeValueArray& V, const NodeAssigmentList& L,
		AssignmentInfoList* pAssignment)
	{
		leda::edge e;
		double sum = 0;

		pAssignment->clear();

		forall(e, L)
		{
			const double& val = V[e];

			pAssignment->push_back(AssignmentInfo(val, this->inf(e)));

			sum += val;
		}

		return sum;
	}

	//! Adds a node to the set 0 or the set 1
	void AddNode(const T& a, int i)
	{
		GetSet(i).push(new_node(a));
	}

	//! Gets the number of nodes in set 0 or set 1
	int GetNodeCount(int i) const
	{
		return GetSet(i).size();
	}

	void AddEdge(NodeSetIterator itA, NodeSetIterator itB, const U& a)
	{
		new_edge(m_setA.inf(itA), m_setB.inf(itB), a);
	}

	NodeSetIterator FirstNode(int i)
	{
		return GetSet(i).first();
	}

	NodeSetIterator NextNode(int i, NodeSetIterator it)
	{
		return GetSet(i).succ(it);
	}

	//! Retrieves the attribute of node 'v'
	const T& NodeAttribute(leda::node v) const
	{
		return this->inf(v);
	}

	const T& NodeAttribute(int i, NodeSetIterator it) const
	{
		return NodeAttribute(GetSet(i).inf(it));
	}

	//! Find the first node with attribute 'a' in set i = {0,1}
	leda::node FindNode(int i, const T& a) const
	{
		leda::node v;
		const NodeSet& s = GetSet(i);

		forall(v, s)
		{
			if (this->inf(v) == a)
				return v;
		}

		return nil;
	}

	/*!
		Only allows the node with attribute 'a0' in set 0 match the node with
		attribute 'a1' in set 1.
	*/
	void FixAssignment(const T& a0, const T& a1, EdgeValidityArray* pEdgeValidity)
	{
		leda::node v0 = FindNode(0, a0);
		ASSERT(v0 != nil);
		ASSERT(graph_of(v0) == this);

		leda::list<leda::edge> invalidEdges;
		leda::edge e;

		DBG_ONLY(int validEdgeCount = 0)

		forall_out_edges(e, v0)
		{
			if (inf(target(e)) != a1)
				(*pEdgeValidity)[e] = false;

			DBG_ONLY(else validEdgeCount++)

			//ASSERT(inf(target(e)) != a1 || (*pEdgeValidity)[e]);
		}

		ASSERT(validEdgeCount == 1);
	}

	//! Creates a candidate correspondece between every pair of nodes
	void CreateFullMap()
	{
		NodeSetIterator itA, itB;
		leda::node u;

		forall_items(itA, m_setA)
		{
			u = m_setA[itA];

			forall_items(itB, m_setB)
				new_edge(u, m_setB[itB], 1.0);
		}
	}

	//! Value associated with edge 'e' and a node in set 0 or 1
	const T& operator()(leda::edge e, int i) const
	{
		ASSERT(i == 0 || i == 1);

		return (i == 0) ? inf(source(e)) : inf(target(e));
	}

	//! Copies the graph and both sets of nodes
	void operator=(const BipartiteGraph<T, U>& rhs)
	{
		leda::GRAPH<T, U>::operator=(rhs);

		m_setA = rhs.m_setA;
		m_setB = rhs.m_setB;
	}
};
} //namespace dml

#endif //__BIPARTITE_NODE_GRAPH_H__
