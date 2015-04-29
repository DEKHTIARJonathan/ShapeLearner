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

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include "StandardException.h"
#include "SDK/GraphClass.h"
#include "SDK/ObjectClass.h"
#include "SDK/Graph.h"
#include "SDK/Node.h"
#include "SDK/Point.h"
#include "SDK/Edge.h"


// ODB Classes
#include <odb/core.hxx>
#include <odb/callback.hxx>
#include <odb/boost/lazy-ptr.hxx>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/schema-catalog.hxx>

using namespace std;

namespace graphDBLib{

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
			static boost::shared_ptr<Edge> createEdge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph){
				return boost::shared_ptr<Edge>(new Edge(_source, _target, _refGraph));
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
		Edge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph);

	};

}

#endif // _EDGE_