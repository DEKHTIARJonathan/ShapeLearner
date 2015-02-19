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
#include "DAG.h"
#include "NodeAssignment.h"
#include "BasicUtils.h"

using namespace dml;

void PrintTabs(std::ostream& os, int numTabs)
{
	for (int i = 0; i < numTabs; i++)
		os << char(9);
}

void NodeAssignment::Print(std::ostream& os, int numTabs) const
{
	const DAG& dag0 = *Graph(0);
	const DAG& dag1 = *Graph(1);

	leda::node v0 = Node(0);
	leda::node v1 = Node(1);

	//const DAGNodePtr ptrNode0 = Graph(0)->GetNode(Node(0));
	//const DAGNodePtr ptrNode1 = Graph(1)->GetNode(Node(1));

	PrintTabs(os, numTabs);

	os << dag0.GetNodeDFSIndex(v0) << " <=> " << dag1.GetNodeDFSIndex(v1) << ": ";

	os << " distance = " << m_distance << ", weighted distance = "
		<< m_weightedDistance << ", param = " << ParamIndex(0);

	os << " [cost0=" << dag0.GetNodeCost(v0)
		<< ", stCost0=" << dag0.GetSubtreeCost(v0)
		<< ", stCompCost0=" << dag0.GetSubtreeComplementCost(v0)
		<< ", cost1=" << dag1.GetNodeCost(v1)
		<< ", stCost1=" << dag1.GetSubtreeCost(v1)
		<< ", stCompCost1=" << dag1.GetSubtreeComplementCost(v1)
		<< ", given wd=" << ((HasGivenChildAssignment()) ? GetGivenChildAssignmentWeightedDistance() : 0)
		<< "]" << std::endl;

	PrintTabs(os, numTabs);

	os << "Parent assignments:\n";

	for (unsigned int i = 0; i < m_parentAssignments.size(); i++)
		m_parentAssignments[i]->Print(os, numTabs + 1);

	PrintTabs(os, numTabs);

	os << "Child assignments:\n";

	for (unsigned int i = 0; i < m_childAssignments.size(); i++)
		 m_childAssignments[i]->Print(os, numTabs + 1);
}