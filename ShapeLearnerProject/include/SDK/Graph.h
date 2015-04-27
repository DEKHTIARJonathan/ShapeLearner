/* ************* Begin file Graph.h ***************************************/
/*
** 2015 March 02
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file Graph.h
*	\brief Graph Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _GRAPH_
#define _GRAPH_

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

	class GraphClass; //Forward Declaration of the class contained in GraphClass.h
	class ObjectClass; //Forward Declaration of the class contained in ObjectClass.h
	class GraphDB; // Forward Declaration of the class contained in graphDB.h


	/*!	
	*	\class GraphClass
	*	\brief Part of the Graph Data Model. It is the center piece of the modeL. It has one ObjectClass and one GraphClass. It is composed of many Nodes, Edges, Points...
	*/
	class Graph
	{
	public:
		/*!	
		*	\class GraphClass::Access
		*	\brief Limit instantiation only to GraphDB. Static subclass which role is only to execute its unique static method.
		*/
		class Access {
			friend class GraphDB;
			static boost::shared_ptr<Graph> createGraph(boost::weak_ptr<GraphClass> _graphClass, boost::weak_ptr<ObjectClass> _objectClass, string const _objectName, unsigned long const _viewNumber = 1){
				return boost::shared_ptr<Graph>(new Graph(_graphClass, _objectClass, _objectName, _viewNumber));
			}
		};	

		unsigned long getKey() const {return idGraph;}

		string getObjectName() const {return objectName;}
		void setObjectName(const string& _objectName);

		unsigned long getView() const {return viewNumber;}
		void setView(const unsigned long _viewNumber);

		boost::weak_ptr<GraphClass> getParentGraphClass();
		boost::weak_ptr<ObjectClass> getParentObjectClass();

		vector<unsigned long> getNodes();
		vector<unsigned long> getEdges();
		vector<unsigned long> getPoints();

		/* =========== Template function =========== */
		string getClassName() const { return "Graph"; }
		/* =========== Template function =========== */

	private:
		unsigned long idGraph;
		string objectName; // The name of the image file.
		unsigned long viewNumber;

		odb::boost::lazy_weak_ptr<GraphClass> refGraphClass;
		odb::boost::lazy_weak_ptr<ObjectClass> refObjectClass;
		
		/*!
		*	\brief  Classical constructor needed to let ODB load objects from DB.
		*/
		Graph() {}
		Graph(boost::weak_ptr<GraphClass> _graphClass, boost::weak_ptr<ObjectClass> _objectClass, string const _objectName, unsigned long const _viewNumber = 1);

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
		*	\brief Friendship required in order to let ODB manage the object.
		*/
		friend class odb::access;
	};

	#pragma db value(std::string) type("VARCHAR(255)")
	#pragma db object(Graph)
	#pragma db member(Graph::idGraph) id auto
	#pragma db member(Graph::refGraphClass) not_null on_delete(cascade)
	#pragma db member(Graph::refObjectClass) not_null on_delete(cascade)
	#pragma db member(Graph::objectName) not_null
	#pragma db member(Graph::viewNumber)  default("1") not_null
	#pragma db index(Graph::"index_Graph_graphClass") method("BTREE") member(refGraphClass)
	#pragma db index(Graph::"index_Graph_objectClass") method("BTREE") member(refObjectClass)
	#pragma db index(Graph::"index_Graph_objectName") unique method("BTREE") member(objectName)

	#pragma db view object(Graph) query("\"refGraphClass\" = ")
	struct GraphIdViewByGraphClass
	{
	  #pragma db column("idGraph")
	  unsigned long id;
	};

	#pragma db view object(Graph) query("\"refObjectClass\" = ")
	struct GraphIdViewByObjectClass
	{
	  #pragma db column("idGraph")
	  unsigned long id;
	};

}

#endif // _GRAPH_