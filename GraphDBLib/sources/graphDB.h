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

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::graphDB.h  - this should happen just once per project.\n")
#endif

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
class StandardExcept; //Forward Declaration of the class contained in StandardException.h

namespace graphDBLib {
	class DatabaseManager; //Forward Declaration of the class contained in StandardException.h
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
			*	\class GraphDB::ObjectInterface
			*	\brief Static subclass, communication interface for all the DBObjects.
			*/
			class ObjectInterface {
				friend class ObjectClass;
				friend class GraphClass;
				friend class Graph;
				friend class Node;
				friend class Edge;
				friend class Point;

			private:

				/* ************** DB I/O Ops *********************/
				/*!
				*	\fn static string saveObject(ObjectClass& obj) throw(StandardExcept);
				*	\brief Static method returning the key on success and an empty string on fail while saving an ObjectClass to the DB.
				*	\param obj : The Object we want to persist in DB.
				*/
				static string saveObject(ObjectClass& obj) throw(StandardExcept) {return saveObjectString(obj);}

				/*!
				*	\fn static string saveObject(GraphClass& obj) throw(StandardExcept);
				*	\brief Static method returning the key on success and an empty string on fail while saving an GraphClass to the DB.
				*	\param obj : The Object we want to persist in DB.
				*/
				static string saveObject(GraphClass& obj) throw(StandardExcept) {return saveObjectString(obj);}

				/*!
				*	\fn template<class T> static unsigned long saveObject(T& obj) throw(StandardExcept);
				*	\brief Template static method returning the key on success and an empty string on fail while saving an Edge/Point/Graph/Node to the DB.
				*	\param obj : The Object we want to persist in DB.
				*/
				template<class T> static unsigned long saveObject(T& obj) throw(StandardExcept){
					unsigned long rslt = DatabaseManager::Interface::saveObject(obj);
					if (rslt != 0)
						return rslt;
					else
						throw StandardExcept((string)__FUNCTION__ +" // Key : Unsigned Long", "Error : Saving operation Failed.");
				}

				/*!
				*	\fn template <class T> static bool updateObject(T& obj) throw (StandardExcept)
				*	\brief Template static method returning a boolean on the success of the update of a Database Object.
				*	\param obj : The Point we want to update in the DB.
				*/
				template <class T> static bool updateObject(T& obj) throw (StandardExcept){ return DatabaseManager::Interface::updateObject(obj); }

				/*!
				*	\fn static unsigned long getPointCountInNode (const unsigned long idNode) throw (StandardExcept);
				*	\brief Static method returning the count of Point which have for foreign key the Node given in argument.
				*	\param idNode : The Node we want to look up.
				*/
				static unsigned long getPointCountInNode (const unsigned long idNode) throw (StandardExcept) { return DatabaseManager::Interface::getPointCountInNode (idNode); }

				/*!
				*	\fn template <class T, class Y> static vector<unsigned long> getForeignRelations(Y foreignKey) throw (StandardExcept);
				*	\brief Template static method returning a vector of keys of object of type T and having the foreign key of type Y passed in argument.
				*	\param foreignKey : Of type String or Unsigned Long in our case. The foreign key we are looking for.
				*/
				template <class T, class Y> static vector<unsigned long> getForeignRelations(Y foreignKey) throw (StandardExcept) { return DatabaseManager::Interface::getForeignRelations<T,Y>(foreignKey); }
			};

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
				*	\param _xCoord : What is the "x" Coordinate of the Point ?
				*	\param _yCoord : What is the "y" Coordinate of the Point ?
				*	\param _radius : What is the "radius" of the Point ?
				*/
				static boost::weak_ptr<Point>			getPoint(const boost::weak_ptr<Node> _refNode, const boost::weak_ptr<Graph> _refGraph, const double _xCoord = 0, const double _yCoord = 0, const double _radius = 1);

				/*!
				*	\fn static boost::weak_ptr<Node> getNode(const boost::weak_ptr<Graph> _refGraph, const unsigned long _index = 1, const unsigned long _level = 1, const unsigned long _mass = 1, const unsigned long _type = 1, const string _label = "1");
				*	\brief Static method returning a weak_ptr<Node>. Instanciantes a new Object of type Node and persists it in the DB. The members values are set by the different arguments.
				*	\param _refGraph : What Graph is the new Node linked to ?
				*	\param _index : What is the "index" of the Node ?
				*	\param _level : What is the "level" of the Node ?
				*	\param _mass : What is the "mass"  of the Node ?
				*	\param _type : What is the "type"  of the Node ?
				*	\param _label : What is the "label"  of the Node ?
				*/
				static boost::weak_ptr<Node>			getNode(const boost::weak_ptr<Graph> _refGraph, const unsigned long _index = 1, const unsigned long _level = 1, const unsigned long _mass = 1, const unsigned long _type = 1, const string _label = "1");

				/*!
				*	\fn static boost::weak_ptr<Edge> getEdge(const boost::weak_ptr<Node> _source, const boost::weak_ptr<Node> _target, const boost::weak_ptr<Graph> _refGraph, const unsigned long _weight = 1);
				*	\brief Static method returning a weak_ptr<Edge>. Instanciantes a new Object of type Edge and persists it in the DB. The members values are set by the different arguments.
				*	\param _source : What is the "source Node" of the Edge ?
				*	\param _target : What is the "target Node" of the Edge ?
				*	\param _refGraph : What Graph is the new Edge linked to ?
				*	\param _weight : What is the "weight" of the Edge ?
				*/
				static boost::weak_ptr<Edge>			getEdge(const boost::weak_ptr<Node> _source, const boost::weak_ptr<Node> _target, const boost::weak_ptr<Graph> _refGraph, const unsigned long _weight = 1);

				/*!
				*	\fn static boost::weak_ptr<Graph> getGraph(const boost::weak_ptr<GraphClass> _graphClass, const boost::weak_ptr<ObjectClass> _objectClass, const string _objectName);
				*	\brief Static method returning a weak_ptr<Graph>. Instanciantes a new Object of type Graph and persists it in the DB. The members values are set by the different arguments.
				*	\param _graphClass : What is the "GraphClass" of the Graph ?
				*	\param _objectClass : What is the "ObjectClass" of the Graph ?
				*	\param _objectName : What is the "objectName" of the Graph ?
				*/
				static boost::weak_ptr<Graph>			getGraph(const boost::weak_ptr<GraphClass> _graphClass, const boost::weak_ptr<ObjectClass> _objectClass, const string _objectName);

				/* ************** Deleters ********************/
				/*!
				*	\fn template <class T> static bool deleteObject(boost::weak_ptr<T> obj, bool deleteOnDB);
				*	\brief Template static method returning a boolean on the success of the suppression operation. The object is removed from the application's memory and optionally also from the database.
				*	\param obj : The object we want to delete from the memory.
				*	\param deleteOnDB : A boolean to indicate if we want to delete the object also in the Database.
				*/
				template <class T> static bool deleteObject(boost::weak_ptr<T> obj, bool deleteOnDB) throw (StandardExcept){
					bool rslt = true;
					boost::shared_ptr<T> keepAlive;
					if(obj.expired())
						throw StandardExcept((string)__FUNCTION__, "Error : The object doesn't exist anymore.");
					else
						keepAlive.swap(obj.lock()); // We ensure that we keep an alive version of the object.

					if(deleteOnDB)
						rslt &= DatabaseManager::Interface::deleteObject(keepAlive);

					rslt &= removeObjectFromMap(keepAlive, deleteOnDB); // If deletonOnDB == false => Desinstanciate, keep in DBn, no cascade delete // deletonOnDB == true => Delete from DB & Central Memory

					return rslt;
				}
			};

		private:
			/* ************** Les Maps de stockage ***********/
			/*!
			*	\brief NodeMap : A MAP containing every Nodes in the application using shared_ptr<Node>.
			*	The MAP's key is the Key of the object in the DB.
			*/
			static map<unsigned long, boost::shared_ptr<Node>>		NodeMap;

			/*!
			*	\brief PointMap : A MAP containing every Points in the application using shared_ptr<Point>.
			*	The MAP's key is the Key of the object in the DB.
			*/
			static map<unsigned long, boost::shared_ptr<Point>>		PointMap;

			/*!
			*	\brief EdgeMap : A MAP containing every Edges in the application using shared_ptr<Edge>.
			*	The MAP's key is the Key of the object in the DB.
			*/
			static map<unsigned long, boost::shared_ptr<Edge>>		EdgeMap;

			/*!
			*	\brief GraphMap : A MAP containing every Graphs in the application using shared_ptr<Graph>.
			*	The MAP's key is the Key of the object in the DB.
			*/
			static map<unsigned long, boost::shared_ptr<Graph>>		GraphMap;

			/*!
			*	\brief ObjectClassMap : A MAP containing every ObjectClass in the application using shared_ptr<ObjectClass>.
			*	The MAP's key is the Key of the object in the DB.
			*/
			static map<string, boost::shared_ptr<ObjectClass>>		ObjectClassMap;

			/*!
			*	\brief GraphClassMap : A MAP containing every GraphClass in the application using shared_ptr<GraphClass>.
			*	The MAP's key is the Key of the object in the DB.
			*/
			static map<string, boost::shared_ptr<GraphClass>>		GraphClassMap;

			/* ************** DB I/O Ops *********************/
			/*!
			*	\fn template<class T> static string saveObjectString(T& obj)  throw(StandardExcept);
			*	\brief  Static method to insert an object in the DB which have a primary key as string.
			*	\param obj : The object we want to insert in the DB.
			*/
			template<class T> static string saveObjectString(T& obj) throw(StandardExcept) {
				string rslt = DatabaseManager::Interface::saveObject(obj);
				if(!rslt.compare("")){ // Is Equal
					throw StandardExcept((string)__FUNCTION__, "Error : Saving operation Failed.");
				}
				else
					return rslt;
			}

			/*!
			*	\fn template <class T, class Y> static boost::shared_ptr<T> loadObject(Y keyDB) throw (StandardExcept);
			*	\brief Template of a static method returning a shared_ptr to the DBObject just loaded. It only treats objects with primary key as string.
			*	\param keyDB The object's key of the record we want to retrieve.
			*/
			template <class T, class Y> static boost::shared_ptr<T> loadObject(Y keyDB) throw (StandardExcept){ return DatabaseManager::Interface::loadObject<T>(keyDB); }

			/* ************* Memory Cleaners ****************/
			/*!
			*	\fn static bool removeObjectFromMap(boost::shared_ptr<Point> obj, bool cascade = false) throw (StandardExcept);
			*	\brief Static method returning a boolean on the success of the deletion of a Point in the application memory. The "cascade" parameter is useless but kept present to keep the syntax persistant.
			*	\param obj : The Point we want to remove from the application's memory.
			*	\param cascade : useless here, value doesn't matter.
			*/
			static bool removeObjectFromMap(boost::shared_ptr<Point> obj, bool cascade = false) throw (StandardExcept);

			/*!
			*	\fn static bool removeObjectFromMap(boost::shared_ptr<Edge> obj, bool cascade = false) throw (StandardExcept);
			*	\brief Static method returning a boolean on the success of the deletion of an Edge in the application memory. The "cascade" parameter is useless but kept present to keep the syntax persistant.
			*	\param obj : The Edge we want to remove from the application's memory.
			*	\param cascade : useless here, value doesn't matter.
			*/
			static bool removeObjectFromMap(boost::shared_ptr<Edge> obj, bool cascade = false) throw (StandardExcept);

			/*!
			*	\fn static bool removeObjectFromMap(boost::shared_ptr<Node> obj, bool cascade = false) throw (StandardExcept);
			*	\brief Static method returning a boolean on the success of the deletion of a Node in the application memory. If cascade is set to true, then all the Points and Edges connected to this Node are also removed.
			*	\param obj : The Node we want to remove from the application's memory.
			*	\param cascade : If cascade is set to true, then all the Points and Edges connected to this Node are also removed.
			*/
			static bool removeObjectFromMap(boost::shared_ptr<Node> obj, bool cascade = false) throw (StandardExcept);

			/*!
			*	\fn static bool removeObjectFromMap(boost::shared_ptr<Graph> obj, bool cascade = false) throw (StandardExcept);
			*	\brief Static method returning a boolean on the success of the deletion of a Graph in the application memory. If cascade is set to true, then all the Points, Edges and Nodes connected to this Graph are also removed.
			*	\param obj : The Graph we want to remove from the application's memory.
			*	\param cascade : If cascade is set to true, then all the Points, Edges and Nodes connected to this Graph are also removed.
			*/
			static bool removeObjectFromMap(boost::shared_ptr<Graph> obj, bool cascade = false) throw (StandardExcept);

			/*!
			*	\fn static bool removeObjectFromMap(boost::shared_ptr<GraphClass> obj, bool cascade = false) throw (StandardExcept);
			*	\brief Static method returning a boolean on the success of the deletion of a GraphClass in the application memory. If cascade is set to true, then all the Points, Edges, Nodes and Graphs connected to this GraphClass are also removed.
			*	\param obj : The GraphClass we want to remove from the application's memory.
			*	\param cascade : If cascade is set to true, then all the Points, Edges, Nodes and Graphs connected to this GraphClass are also removed.
			*/
			static bool removeObjectFromMap(boost::shared_ptr<GraphClass> obj, bool cascade = false) throw (StandardExcept);

			/*!
			*	\fn static bool removeObjectFromMap(boost::shared_ptr<ObjectClass> obj, bool cascade = false) throw (StandardExcept);
			*	\brief Static method returning a boolean on the success of the deletion of a ObjectClass in the application memory. If cascade is set to true, then all the Points, Edges, Nodes and Graphs connected to this ObjectClass are also removed.
			*	\param obj : The ObjectClass we want to remove from the application's memory.
			*	\param cascade : If cascade is set to true, then all the Points, Edges, Nodes and Graphs connected to this ObjectClass are also removed.
			*/
			static bool removeObjectFromMap(boost::shared_ptr<ObjectClass> obj, bool cascade = false) throw (StandardExcept);

			/* **************  No instanciation *********************/

			/*!
			*	\brief Constructeur de le class GraphDB.  Il instancie également la classe DatabaseManager.
			*/
			GraphDB() throw(StandardExcept);

			/*!
			*	\brief Constructeur de recopie => La recopie est interdite
			*/
			GraphDB(const GraphDB&);

			/*!
			*	\brief Opérateur =  =>> La recopie est interdite
			*/
			GraphDB& operator=(const GraphDB&);

			/*!
			*	\brief La destruction est interdite
			*/
			~GraphDB();
	};
}

#endif //_GRAPH_DATABASE_H_