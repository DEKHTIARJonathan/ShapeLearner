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
class GraphManager; // Forward Declaration of the class contained in graphManager.h

class Node
{
public:
	Node(Graph* _refGraph, unsigned int _index = 1, unsigned int _level = 1, unsigned int _mass = 1, unsigned int _type = 1, string _label = "1");

	unsigned long getKey() const {return idNode;}
	void setKey(const unsigned int key);

	unsigned long getIndex() const {return index;}
	void setIndex(const unsigned int _index);

	unsigned long getLevel() const {return level;}
	void setLevel(const unsigned int _level);

	unsigned long getMass() const {return mass;}
	void setMass(const unsigned int _mass);

	unsigned long getType() const {return type;}
	void setType(const unsigned int _type);

	string getLabel() const {return label;}
	void setLabel(const string& _label);

	unsigned long getPointCount() const;

	/* =========== Template function =========== */
	string getClassName() const { return "Node"; }
	/* =========== Template function =========== */

private:
	Node() {}
	unsigned long	idNode;
	unsigned int	index;
	unsigned int	level;
	unsigned int	mass;
	unsigned int	type;
	string			label;
	Graph*			refGraph;

	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(Node)
#pragma db member(Node::idNode) id auto
#pragma db member(Node::index) not_null default("1")
#pragma db member(Node::level) not_null default("1")
#pragma db member(Node::mass) not_null default("1")
#pragma db member(Node::type) not_null default("1")
#pragma db member(Node::label) not_null default("1")
#pragma db member(Node::refGraph) not_null on_delete(cascade)
#pragma db index(Node::"Node_RefGraph") method("BTREE") member(refGraph)


#endif // _NODE_