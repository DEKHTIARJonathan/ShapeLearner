/* ************* Begin file Node.h ***************************************/
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
*	\file Node.h
*	\brief Node Header
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _NODE_
#define _NODE_

#include "allHeaders.h"
using namespace std;

class Graph; //Forward Declaration of the class contained in Graph.h//

class Node
{
public:

private:
	Node() {}
	unsigned long	idNode;
	unsigned int	index;
	unsigned int	level;
	unsigned int	mass;
	unsigned int	type;
	unsigned int	pointCount;
	unsigned int	label;
	Graph*	refGraph;

	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(Node)
#pragma db member(Node::idNode) id auto
#pragma db member(Node::index) not_null default("1")
#pragma db member(Node::level) not_null default("1")
#pragma db member(Node::mass) not_null default("1")
#pragma db member(Node::type) not_null default("1")
#pragma db member(Node::pointCount) not_null default("1")
#pragma db member(Node::label) not_null default("1")
#pragma db member(Node::refGraph) not_null on_delete(cascade)
#pragma db index(Node::"Node_RefGraph") method("BTREE") member(refGraph)


#endif // _NODE_