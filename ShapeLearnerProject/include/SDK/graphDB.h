/* ************* Begin file graphDB.h ***************************************/
/*
** 2015 February 23
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file graphDB.h
*	\brief GraphDB header file. Public Interface and Central point in the software. It realizes the task distribution to the different instances of the software and make them communicate without knowing of each other.
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _GRAPH_DATABASE_H_
#define _GRAPH_DATABASE_H_

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

using namespace std;

class StandardExcept; //Forward Declaration of the class contained in StandardException.h

namespace graphDBLib{
	
	class ObjectClass; //Forward Declaration of the class contained in ObjectClass.h
	class GraphClass; //Forward Declaration of the class contained in GraphClass.h
	class Graph; //Forward Declaration of the class contained in Graph.h
	class Node; //Forward Declaration of the class contained in Node.h
	class Point; //Forward Declaration of the class contained in Point.h
	class Edge; //Forward Declaration of the class contained in Edge.h

	/*!	
	*	\class GraphDB
	*	\brief Static class, the central point of the whole architecture. It redistributes actions to the different actors.
	*	The different actors doesn't need to know about each others. That way, a new handler can be added without any change whatsoever.
	*	It implements different interfaces to input some actions. It allows to control the granularity access of "Who can access what".
	*/
	class GraphDB
	{
		public:

			/*!
			*	\fn static void openDatabase(const string& _dbUser, const string& _dbPass, const string& _dbName, const string& _dbHost, const unsigned int& _dbPort, const string& _dbInit = "") throw(StandardExcept);
			*	\brief Open the connection to the PostgreSQL database.
			*	\param _dbUser : The username used to connect to the DB.
			*	\param _dbPass : The password associated with the username.
			*	\param _dbName : The database's name.
			*	\param _dbHost : The DB's server's hostname or IP address.
			*	\param _dbPort : The DB's server's listening port.
			*	\param _dbInit : A relative path to the DB Init File, this field is not necessary. If empty => DB not initialized and assume it has been already created.
			*/
			static void openDatabase(const string& _dbUser, const string& _dbPass, const string& _dbName, const string& _dbHost, const unsigned int& _dbPort, const string& _dbInit = "") throw(StandardExcept);
			
			/*!
			*	\fn static void closeDatabase() throw(StandardExcept);
			*	\brief Close the connection to the PostgreSQL database.
			*/
			static void closeDatabase() throw(StandardExcept);


			/*!	
			*	\class GraphDB::CommonInterface
			*	\brief Static subclass, communication interface for all Objects inside the library.
			*	The implemented methods inside this interface are susceptible to be used by all the objects.
			*/
			class CommonInterface {
				public:

				/* ************** Getters *********************/
				/*!
				*	\fn static boost::weak_ptr<Point> getPoint(const unsigned long keyDB) throw(StandardExcept);
				*	\brief Static method returning a weak_ptr<Point>, if the object isn't instanciated yet in the application memory, it loads it from the DB. If the keyDB doesn't exist an exception is raised.
				*	\param keyDB : The object's key we want to load from application memory/DB.
				*/
				static boost::weak_ptr<Point>			getPoint(const unsigned long keyDB) throw(StandardExcept);

				/*!
				*	\fn static boost::weak_ptr<Node> getNode(const unsigned long keyDB) throw(StandardExcept);
				*	\brief Static method returning a weak_ptr<Node>, if the object isn't instanciated yet in the application memory, it loads it from the DB. If the keyDB doesn't exist an exception is raised.
				*	\param keyDB : The object's key we want to load from application memory/DB.
				*/
				static boost::weak_ptr<Node>			getNode(const unsigned long keyDB) throw(StandardExcept);

				/*!
				*	\fn static boost::weak_ptr<Edge> getEdge(const unsigned long keyDB) throw(StandardExcept);
				*	\brief Static method returning a weak_ptr<Edge>, if the object isn't instanciated yet in the application memory, it loads it from the DB. If the keyDB doesn't exist an exception is raised.
				*	\param keyDB : The object's key we want to load from application memory/DB.
				*/
				static boost::weak_ptr<Edge>			getEdge(const unsigned long keyDB) throw(StandardExcept);

				/*!
				*	\fn static boost::weak_ptr<Graph> getGraph(const unsigned long keyDB) throw(StandardExcept);
				*	\brief Static method returning a weak_ptr<Graph>, if the object isn't instanciated yet in the application memory, it loads it from the DB. If the keyDB doesn't exist an exception is raised.
				*	\param keyDB : The object's key we want to load from application memory/DB.
				*/
				static boost::weak_ptr<Graph>			getGraph(const unsigned long keyDB) throw(StandardExcept);

				/* ************** Getters & Setters ***********/
				/*!
				*	\fn static boost::weak_ptr<GraphClass> getGraphClass(const string& name, const bool isDirect = false, const bool isAcyclic = false) throw(StandardExcept);
				*	\brief Static method returning a weak_ptr<GraphClass>, if the object isn't instanciated yet in the application memory, it loads it from the DB.
				*	If the key "name" doesn't exist, the object is created in the DB with the optional parameters as member values and then returned as a weak_ptr<GraphClass>.
				*	\param name : The object's key we want to load from application memory/DB. If the object doesn't exist yet, it will be the new object key.
				*	\param isDirect: Only usefull if the objet doesn't exist in the DB yet. Is the graph type a direct graph : true / false.
				*	\param isAcyclic: Only useful if the objet doesn't exist in the DB yet. Is the graph type an acyclic graph : true / false.
				*/
				static boost::weak_ptr<GraphClass>		getGraphClass(const string& name, const bool isDirect = false, const bool isAcyclic = false) throw(StandardExcept);

				/*!
				*	\fn static boost::weak_ptr<ObjectClass> getObjectClass(const string& name) throw(StandardExcept);
				*	\brief Static method returning a weak_ptr<ObjectClass>, if the object isn't instanciated yet in the application memory, it loads it from the DB.
				*	If the key "name" doesn't exist, the object is created in the DB and then returned as a weak_ptr<ObjectClass>.
				*	\param name : The object's key we want to load from application memory/DB. If the object doesn't exist yet, it will be the new object key.
				*/
				static boost::weak_ptr<ObjectClass>		getObjectClass(const string& name) throw(StandardExcept);

				/* ************** Setters *********************/
				/*!
				*	\fn static boost::weak_ptr<Point> getPoint(const boost::weak_ptr<Node> _refNode, const boost::weak_ptr<Graph> _refGraph, const double _xCoord = 0, const double _yCoord = 0, const double _radius = 1);
				*	\brief Static method returning a weak_ptr<Point>. Instanciantes a new Object of type Point and persists it in the DB. The members values are set by the different arguments.
				*	\param _refNode : What Node is the new Point linked to ?
				*	\param _refGraph : What Graph is the new Point linked to ?
				*/
				static boost::weak_ptr<Point>			getPoint(const boost::weak_ptr<Node> _refNode, const boost::weak_ptr<Graph> _refGraph);

				/*!
				*	\fn static boost::weak_ptr<Node> getNode(const boost::weak_ptr<Graph> _refGraph);
				*	\brief Static method returning a weak_ptr<Node>. Instanciantes a new Object of type Node and persists it in the DB. The members values are set by the different arguments.
				*	\param _refGraph : What Graph is the new Node linked to ?
				*/
				static boost::weak_ptr<Node>			getNode(const boost::weak_ptr<Graph> _refGraph);

				/*!
				*	\fn static boost::weak_ptr<Edge> getEdge(const boost::weak_ptr<Node> _source, const boost::weak_ptr<Node> _target, const boost::weak_ptr<Graph> _refGraph, const unsigned long _weight = 1);
				*	\brief Static method returning a weak_ptr<Edge>. Instanciantes a new Object of type Edge and persists it in the DB. The members values are set by the different arguments.
				*	\param _source : What is the "source Node" of the Edge ?
				*	\param _target : What is the "target Node" of the Edge ?
				*	\param _refGraph : What Graph is the new Edge linked to ?
				*/
				static boost::weak_ptr<Edge>			getEdge(const boost::weak_ptr<Node> _source, const boost::weak_ptr<Node> _target, const boost::weak_ptr<Graph> _refGraph);

				/*!
				*	\fn static boost::weak_ptr<Graph> getGraph(const boost::weak_ptr<GraphClass> _graphClass, const boost::weak_ptr<ObjectClass> _objectClass, const string _objectName);
				*	\brief Static method returning a weak_ptr<Graph>. Instanciantes a new Object of type Graph and persists it in the DB. The members values are set by the different arguments.
				*	\param _graphClass : What is the "GraphClass" of the Graph ?
				*	\param _objectClass : What is the "ObjectClass" of the Graph ?
				*	\param _objectName : What is the "objectName" of the Graph ?
				*/
				static boost::weak_ptr<Graph>			getGraph(const boost::weak_ptr<GraphClass> _graphClass, const boost::weak_ptr<ObjectClass> _objectClass, const string _objectName);

			};
		
	};
	
}

#endif //_GRAPH_DATABASE_H_