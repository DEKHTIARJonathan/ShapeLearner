/* ************* Begin file shapeLearner.h ***************************************/
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
*	\file shapeLearner.h
*	\brief ShapeLearner header file. Central point in the software. It realizes the task distribution to the different instances of the software and make them communicate without knowing of each other.
*	\version 1.1
*	\author DEKHTIAR Jonathan
*/

#ifndef _GRAPH_MANAGER_H_
#define _GRAPH_MANAGER_H_

#include "allHeaders.h"
#include "dbManager.h"
using namespace std;

class ShapeLearner; //Forward Declaration of the class contained in shapeLearnerException.h
class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h
class ObjectClass; //Forward Declaration of the class contained in ObjectClass.h
class GraphClass; //Forward Declaration of the class contained in GraphClass.h
class Graph; //Forward Declaration of the class contained in Graph.h
class Node; //Forward Declaration of the class contained in Node.h
class Point; //Forward Declaration of the class contained in Point.h
class Edge; //Forward Declaration of the class contained in Edge.h

/*!	
*	\class ShapeLearner
*	\brief Static class, the central point of the whole architecture. It redistributes actions to the different actors.
*	The different actors doesn't need to know about each others. That way, a new handler can be added without any change whatsoever.
*	It implements different interfaces to input some actions. It allows to control the granularity access of "Who can access what".
*/
class ShapeLearner
{
	public:

		// Setters pour les paramètres de la BDD.
		static void getDbCredentials(const bool dbInit = false) throw(ShapeLearnerExcept);
		static void openDatabase() throw(ShapeLearnerExcept);
		static void closeDatabase() throw(ShapeLearnerExcept);

		class ObjectInterface {
			friend class ObjectClass;
			friend class GraphClass;
			friend class Graph;
			friend class Node;
			friend class Edge;
			friend class Point;

		private:

			/* ************** DB I/O Ops *********************/
			static string saveObject(ObjectClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}
			static string saveObject(GraphClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}

			template<class T> static unsigned long saveObject(T& obj) throw(ShapeLearnerExcept){
				unsigned long rslt = DatabaseManager::Interface::saveObject(obj);
				if (rslt != 0)
					return rslt;
				else
					throw ShapeLearnerExcept((string)__FUNCTION__ +" // Key : Unsigned Long", "Error : Saving operation Failed.");
			}

			template <class T> static bool updateObject(T& obj) throw (ShapeLearnerExcept){ return DatabaseManager::Interface::updateObject(obj); }

			static unsigned long getPointCountInNode (const unsigned long idNode) throw (ShapeLearnerExcept) { return DatabaseManager::Interface::getPointCountInNode (idNode); }

			template <class T, class Y> static vector<unsigned long> getForeignRelations(Y foreignKey) throw (ShapeLearnerExcept) { return DatabaseManager::Interface::getForeignRelations<T,Y>(foreignKey); }
		};

		class CommonInterface {
			friend class ObjectClass;
			friend class GraphClass;
			friend class Graph;
			friend class Node;
			friend class Edge;
			friend class Point;
			friend class ShapeLearner;

		private:

			/* ************** Getters *********************/
			static boost::weak_ptr<Point>			getPoint(const unsigned long keyDB) throw(ShapeLearnerExcept);
			static boost::weak_ptr<Node>			getNode(const unsigned long keyDB) throw(ShapeLearnerExcept);
			static boost::weak_ptr<Edge>			getEdge(const unsigned long keyDB) throw(ShapeLearnerExcept);
			static boost::weak_ptr<Graph>			getGraph(const unsigned long keyDB) throw(ShapeLearnerExcept);

			/* ************** Getters & Setters ***********/
			static boost::weak_ptr<GraphClass>		getGraphClass(const string& name, const bool isDirect = false, const bool isAcyclic = false) throw(ShapeLearnerExcept);
			static boost::weak_ptr<ObjectClass>		getObjectClass(const string& name) throw(ShapeLearnerExcept);

			/* ************** Setters *********************/
			static boost::weak_ptr<Point>			getPoint(const boost::weak_ptr<Node> _refNode, const boost::weak_ptr<Graph> _refGraph, const double _xCoord = 0, const double _yCoord = 0, const double _radius = 1);
			static boost::weak_ptr<Node>			getNode(const boost::weak_ptr<Graph> _refGraph, const unsigned long _index = 1, const unsigned long _level = 1, const unsigned long _mass = 1, const unsigned long _type = 1, const string _label = "1");
			static boost::weak_ptr<Edge>			getEdge(const boost::weak_ptr<Node> _source, const boost::weak_ptr<Node> _target, const boost::weak_ptr<Graph> _refGraph, const unsigned long _weight = 1);
			static boost::weak_ptr<Graph>			getGraph(const boost::weak_ptr<GraphClass> _graphClass, const boost::weak_ptr<ObjectClass> _objectClass, const string _objectName, const unsigned long _viewNumber = 1);

			/* ************** Deleters ********************/
			template <class T> static bool deleteObject(boost::weak_ptr<T> obj, bool deleteOnDB) throw (ShapeLearnerExcept){ 
				bool rslt = true;
				boost::shared_ptr<T> keepAlive;
				if(obj.expired())
					throw ShapeLearnerExcept((string)__FUNCTION__, "Error : The object doesn't exist anymore.");
				else
					keepAlive.swap(obj.lock()); // We ensure that we keep an alive version of the object.

				if(deleteOnDB)
					rslt &= DatabaseManager::Interface::deleteObject(keepAlive);

				rslt &= removeObjectFromMap(keepAlive, deleteOnDB); // If deletonOnDB == false => Desinstanciate, keep in DBn, no cascade delete // deletonOnDB == true => Delete from DB & Central Memory

				return rslt;
			}
		};

	private:
		/*! \brief dbName : Nom de la Base de Données. */
		static string dbName;

		/*! \brief dbUser : Utilisateur de la base de données */
		static string dbUser;

		/*! \brief dbPass : Mot de passe de la base de données */
		static string dbPass;

		/*! \brief dbPort : Port de la base de données */
		static unsigned int dbPort;

		/*! \brief dbHost : Adresse IP ou Hostname du serveur de BDD (localhost par défaut). */
		static string dbHost;

		/*! \brief dbInit : Doit on initialiser la base de données ? */
		static string dbInitFile;

		/* ************** Les Maps de stockage ***********/

		static map<unsigned long, boost::shared_ptr<Node>>		NodeMap;
		static map<unsigned long, boost::shared_ptr<Point>>		PointMap;
		static map<unsigned long, boost::shared_ptr<Edge>>		EdgeMap;
		static map<unsigned long, boost::shared_ptr<Graph>>		GraphMap;
		static map<string, boost::shared_ptr<ObjectClass>>		ObjectClassMap;
		static map<string, boost::shared_ptr<GraphClass>>		GraphClassMap;

		/* ******** Setters pour les paramètres de la BDD. ********/
		static void setDbName() throw(ShapeLearnerExcept);
		static void setDbUser() throw(ShapeLearnerExcept);
		static void setDbPass() throw(ShapeLearnerExcept);
		static void setDbHost() throw(ShapeLearnerExcept);
		static void setDbPort() throw(ShapeLearnerExcept);
		static void setDBInitFile() throw(ShapeLearnerExcept);

		/* ************** DB I/O Ops *********************/

		template<class T> static string saveObjectString(T& obj) throw(ShapeLearnerExcept) {
			string rslt = DatabaseManager::Interface::saveObject(obj);
			if(!rslt.compare("")){ // Is Equal
				throw ShapeLearnerExcept((string)__FUNCTION__, "Error : Saving operation Failed.");
			}
			else
				return rslt;
		}

		template <class T, class Y> static boost::shared_ptr<T> loadObject(Y keyDB) throw (ShapeLearnerExcept){ return DatabaseManager::Interface::loadObject<T>(keyDB); }

		/* ************* Memory Cleaners ****************/
		
		static bool removeObjectFromMap(boost::shared_ptr<Point> obj, bool cascade = false) throw (ShapeLearnerExcept);
		static bool removeObjectFromMap(boost::shared_ptr<Edge> obj, bool cascade = false) throw (ShapeLearnerExcept);
		static bool removeObjectFromMap(boost::shared_ptr<Node> obj, bool cascade = false) throw (ShapeLearnerExcept);
		static bool removeObjectFromMap(boost::shared_ptr<Graph> obj, bool cascade = false) throw (ShapeLearnerExcept);
		static bool removeObjectFromMap(boost::shared_ptr<GraphClass> obj, bool cascade = false) throw (ShapeLearnerExcept);
		static bool removeObjectFromMap(boost::shared_ptr<ObjectClass> obj, bool cascade = false) throw (ShapeLearnerExcept);
		
		/* **************  No instanciation *********************/

		/*!
		* \brief Constructeur de le class ShapeLearner.  Il instancie également la classe DatabaseManager.
		*/
		ShapeLearner() throw(ShapeLearnerExcept);

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		ShapeLearner(const ShapeLearner&);

		/*!
		*  \brief Opérateur =  =>> La recopie est interdite
		*/
		ShapeLearner& operator=(const ShapeLearner&);

		/*!
		*  \brief La destruction est interdite
		*/
		~ShapeLearner();
};

#endif //_GRAPH_MANAGER_H_