/* ************* Begin file Edge.h ***************************************/
/*
** 2015 March 03
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file Edge.h
*	\brief Edge Header
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _EDGE_
#define _EDGE_

#include "allHeaders.h"
using namespace std;

class Graph; //Forward Declaration of the class contained in Graph.h
class Node; //Forward Declaration of the class contained in Node.h

class Edge
{
public:

private:
	Edge() {}

	unsigned long idEdge;
	Node* source;
	Node* target;
	Graph* refGraph;
	unsigned int weight;

	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(Edge)
#pragma db member(Edge::idEdge) id auto
#pragma db member(Edge::refGraph) not_null on_delete(cascade)
#pragma db member(Edge::source) not_null on_delete(cascade)
#pragma db member(Edge::target) not_null on_delete(cascade)
#pragma db member(Edge::weight) default("1") not_null
#pragma db index(Edge::"index_Edge_source") method("BTREE") member(source)
#pragma db index(Edge::"index_Edge_target") method("BTREE") member(target)
#pragma db index(Edge::"index_Edge_refGraph") method("BTREE") members(refGraph)

#endif // _EDGE_