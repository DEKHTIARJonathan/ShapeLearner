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
 * $Log: LEDA_graphwin_utils.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __LEDA_GRAPHWIN_UTILS_H__
#define __LEDA_GRAPHWIN_UTILS_H__

#include <vector>
#include <LEDA/graphics/graphwin.h>

/*!
	@brief Positions every node of the given draft randomly
	in the unit square.

	This function should be in the LEDA library, but it isn't.

	Call graphwin::place_into_win() to scale the unit square positions
	to the current graphwin
*/
void LayoutGraphRandomly(leda::GraphWin& gw)
{
	// Get the underlying target graph...
	leda::graph& targetGraph = gw.get_graph();

	// ...and find random positions for each node...
	leda::graph randomGraph;
	leda::node_array<double> xpos(randomGraph), ypos(randomGraph);

	// ...by creating a random graph embeded in the unit square...
	random_planar_graph(randomGraph, xpos, ypos, targetGraph.number_of_nodes());
	const leda::list<leda::node> nodeList = randomGraph.all_nodes();

	// ...and copying those coordinates to the target graph
	leda::node v, w;
	leda::list_item it = nodeList.first();

	forall_nodes(v, targetGraph)
	{
		w = nodeList.inf(it);
		gw.set_position(v, leda::point(xpos[w], ypos[w]));
		it = nodeList.succ(it);
	}

	// Fit the graph to the window
	gw.place_into_win();
}

#endif //__LEDA_GRAPHWIN_UTILS_H__
