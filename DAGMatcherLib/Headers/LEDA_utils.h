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

#ifndef __LEDA_UTILS_H__
#define __LEDA_UTILS_H__

#include "stdafx.h"



//! Asserts that the nodes or edges A and B belong to the same graph
#define ASSERT_SAME_GRAPH(A, B) ASSERT(graph_of(A) == graph_of(B))

//! Asserts that the node/edge A belongs to the graph pointed by G
#define ASSERT_GRAPH1(G, A) ASSERT(graph_of(A) == G)

//! Asserts that the nodes/edges A and B belong to the graph pointed by G
#define ASSERT_GRAPH2(G, A, B) ASSERT(graph_of(A) == G && graph_of(B) == G)

/*!
	Iterator that follows a depth first search order when
	traversing a graph (ie, preorder traversal).
*/
class graph_dfs_iterator
{
protected:
	const leda::graph* m_pGraph;
	leda::list<leda::edge> m_path;
	leda::node_array<bool> m_visited;
	leda::edge m_currentEdge;
	bool m_bInverted;

protected:
	leda::edge first_edge(const leda::node& v)
	{
		if (m_bInverted)
			return m_pGraph->first_in_edge(v);
		else
			return m_pGraph->first_adj_edge(v);
	}

	leda::edge next_edge(const leda::edge& e)
	{
		if (m_bInverted)
			return m_pGraph->in_succ(e);
		else
			return m_pGraph->adj_succ(e);
	}

	leda::node target_node(const leda::edge& e)
	{
		if (m_bInverted)
			return source(e);
		else
			return target(e);
	}

	int number_of_children(const leda::node& v)
	{
		if (m_bInverted)
			return m_pGraph->indeg(v);
		else
			return m_pGraph->outdeg(v);
	}

public:
	//! Constructs an un-initialized iterator
	graph_dfs_iterator()
	{
		m_pGraph = NULL;
		m_currentEdge = nil;
		m_bInverted = false;
	}

	/*!
		Constructs an iterator with a starting node and an optional
		direction parameter where false indicates depth first and
		true indicates hight first (ie, edge directions are inverted).
	*/
	graph_dfs_iterator(leda::node v, bool inverted = false)
	{
		init(v, inverted);
	}

	/*!
		Initializes the iterator with a starting node and an optional
		direction parameter where false indicates depth first (default)
		and true indicates hight first (inverted edge directions).
	*/
	void init(leda::node v, bool inverted = false)
	{
		m_pGraph = leda::graph_of(v);
		m_bInverted = inverted;
		m_visited.init(*m_pGraph, false);

		m_currentEdge = first_edge(v); // depends on "inverted" status
	}

	/*!
		Initializes the iterator with a starting node, an array of visited nodes
		in the graph, and an optional direction parameter where false indicates depth
		first (default) and true indicates hight first (inverted edge directions).
	*/
	void init(leda::node v, const leda::node_array<bool>& visited, bool inverted = false)
	{
		m_pGraph = leda::graph_of(v);
		m_bInverted = inverted;
		m_visited = visited;

		m_currentEdge = first_edge(v);  // depends on "inverted" status
	}

	//! Returns true of a call to next() returns a valid node
	bool has_next()
	{
		return (m_currentEdge != nil);
	}

	//! Sets the 'visited' status of node 'v'
	void set_visited_status(leda::node v, bool bStatus)
	{
		m_visited[v] = bStatus;
	}

	//! Sets the 'visited' status of all nodes
	void set_visited_status(const leda::node_array<bool>& status)
	{
		m_visited = status;
	}

	//! Returns the next node wrt a DFS order that is not yet visited
	leda::node next()
	{
		if (m_currentEdge == nil)
			return nil;

		// Get the target node to return
		leda::node v = target_node(m_currentEdge);

		// Get a reference to the 'visited' variable of 'v'
		bool& visited = m_visited[v];

		// Move to the next sibling edge (may be nil)...
		m_currentEdge = next_edge(m_currentEdge);

		// but see if its there are child nodes to visit first...
		if (number_of_children(v) > 0 && !visited)
		{
			// Save the next sibling for later (if there is one)
			if (m_currentEdge != nil)
				m_path.push_back(m_currentEdge);

			// Move to the first child node
			m_currentEdge = first_edge(v);
		}
		else if (m_currentEdge == nil && !m_path.empty())
		{
			// There is no sibling, but we can still backtrack
			m_currentEdge = m_path.pop_back();
		}

		// If the node was already visited, get the next one
		if (visited)
			return next();

		// Mark the node as visited and return it
		visited = true; // 'visited' is a reference variable

		return v;
	}
};

/*!
	@brief Finds the edge with source node src and target node tgt
*/
/*leda::edge FindEdge(const leda::node& src, const leda::node& tgt)
{
	leda::edge e;

	forall_out_edges(e, src)
		if (target(e) == tgt)
			return e;

	return nil;
}*/

#endif //__LEDA_UTILS_H__
