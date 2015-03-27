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

class Graph; //Forward Declaration of the class contained in Graph.h
class ShapeLearner; // Forward Declaration of the class contained in shapeLearner.h
class Point; //Forward Declaration of the class contained in Point.h

class Node
{
public:
	class Access {
		friend class ShapeLearner;
		static boost::shared_ptr<Node> createNode(boost::weak_ptr<Graph> _refGraph, unsigned long _index = 1, unsigned long _level = 1, unsigned long _mass = 1, unsigned long _type = 1, string _label = "1"){
			return boost::shared_ptr<Node>(new Node(_refGraph, _index, _level, _mass, _type, _label));
		}
	};	

	unsigned long getKey() const {return idNode;}

	unsigned long getIndex() const {return index;}
	void setIndex(const unsigned long _index);

	unsigned long getLevel() const {return level;}
	void setLevel(const unsigned long _level);

	unsigned long getMass() const {return mass;}
	void setMass(const unsigned long _mass);

	unsigned long getType() const {return type;}
	void setType(const unsigned long _type);

	string getLabel() const {return label;}
	void setLabel(const string& _label);

	unsigned long getPointCount() const;

	boost::weak_ptr<Graph> getParentGraph();

	vector<unsigned long> getEdgesBySource();
	vector<unsigned long> getEdgesByTarget();
	vector<unsigned long> getEdges();
	vector<unsigned long> getPoints();


	/* =========== Template function =========== */
	string getClassName() const { return "Node"; }
	/* =========== Template function =========== */

private:
	Node() {}
	Node(boost::weak_ptr<Graph> _refGraph, unsigned long _index = 1, unsigned long _level = 1, unsigned long _mass = 1, unsigned long _type = 1, string _label = "1");

	void updateInDB();
	unsigned long saveInDB();

	unsigned long						idNode;
	unsigned long						index;
	unsigned long						level;
	unsigned long						mass;
	unsigned long						type;
	string								label;
	odb::boost::lazy_weak_ptr<Graph>	refGraph;

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

#pragma db view object(Node) query("\"refGraph\" = ")
struct NodeIdViewByGraph
{
  #pragma db column("idNode")
  unsigned long id;
};

#endif // _NODE_