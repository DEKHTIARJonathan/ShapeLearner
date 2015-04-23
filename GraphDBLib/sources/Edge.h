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
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _EDGE_
#define _EDGE_

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::Edge.h  - this should happen just once per project.\n")
	#include "stdafx.h"
	#include "allHeaders.h"
#else
	#include "odbHeaders.h"
#endif

using namespace std;

class Graph; //Forward Declaration of the class contained in Graph.h
class Node; //Forward Declaration of the class contained in Node.h
class GraphDB; // Forward Declaration of the class contained in graphDB.h

/*!	
*	\class GraphClass
*	\brief Part of the Graph Data Model. One of the main components of the model, it links 2 nodes in the same graph.
*/
class Edge
{
public:
	/*!	
	*	\class Edge::Access
	*	\brief Limit instantiation only to GraphDB. Static subclass which role is only to execute its unique static method.
	*/
	class Access {
		friend class GraphDB;
		static boost::shared_ptr<Edge> createEdge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph, unsigned long _weight = 1){
			return boost::shared_ptr<Edge>(new Edge(_source, _target, _refGraph, _weight));
		}
	};		

	unsigned long getKey() const {return idEdge;}

	unsigned long getWeight() const {return weight;}
	void setWeight(const unsigned long _weight);

	/* =========== Template function =========== */
	string getClassName() const { return "Edge"; }
	/* =========== Template function =========== */

private:
	unsigned long idEdge;
	unsigned long weight;

	odb::boost::lazy_weak_ptr<Node> source;
	odb::boost::lazy_weak_ptr<Node> target;
	odb::boost::lazy_weak_ptr<Graph> refGraph;
	
	/*!
	*	\brief  Classical constructor needed to let ODB load objects from DB.
	*/
	Edge() {}
	Edge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph, unsigned long _weight = 1);

	/*!
	*	\fn void updateInDB();
	*	\brief Update the object in the database.
	*/
	void updateInDB();

	/*!
	*	\fn unsigned long saveInDB();
	*	\brief Persist the object in the database.
	*/
	unsigned long saveInDB();


	/*!
	*	\fn void checkCorrectness(odb::callback_event e, odb::database&) const throw(StandardExcept);
	*	\brief C++ Trigger Launched on each DB Operation on this object.
	*/
	void checkCorrectness(odb::callback_event e, odb::database&) const throw(StandardExcept);

	/*!
	*	\brief Friendship required in order to let ODB manage the object.
	*/
	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(Edge) callback(checkCorrectness)
#pragma db member(Edge::idEdge) id auto
#pragma db member(Edge::refGraph) not_null on_delete(cascade)
#pragma db member(Edge::source) not_null on_delete(cascade)
#pragma db member(Edge::target) not_null on_delete(cascade)
#pragma db member(Edge::weight) default("1") not_null
#pragma db index(Edge::"index_Edge_source") method("BTREE") member(source)
#pragma db index(Edge::"index_Edge_target") method("BTREE") member(target)
#pragma db index(Edge::"index_Edge_link") unique method("BTREE") members(source, target)
#pragma db index(Edge::"index_Edge_refGraph") method("BTREE") member(refGraph)

#pragma db view object(Edge) query("\"source\" = ")
struct EdgeIdViewBySource
{
  #pragma db column("idEdge")
  unsigned long id;
};

#pragma db view object(Edge) query("\"target\" = ")
struct EdgeIdViewByTarget
{
  #pragma db column("idEdge")
  unsigned long id;
};

#pragma db view object(Edge) query("\"refGraph\" = ")
struct EdgeIdViewByGraph
{
  #pragma db column("idEdge")
  unsigned long id;
};

#endif // _EDGE_