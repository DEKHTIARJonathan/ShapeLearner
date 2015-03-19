/* ************* Begin file graphManager.h ***************************************/
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
*	\file graphManager.h
*	\brief Interface publique d'acc�s au logiciel. Elle concentre les acc�s � la BDD, le G�narateur de Graph et le Graph Matcher.
*	\version 1.1
*	\author DEKHTIAR Jonathan
*/

#ifndef _GRAPH_MANAGER_H_
#define _GRAPH_MANAGER_H_

#include "allHeaders.h"
#include "dbManager.h"
using namespace std;

class ShapeLearner;
class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h
class ObjectClass;
class GraphClass;
class Graph;
class Node;
class Point;
class Edge;

class GraphManager
{
	public:

		class UserInterface {
			friend class ShapeLearner;

		private:
				// Setters pour les param�tres de la BDD.
				static void getDbCredentials(const bool dbInit = false) throw(ShapeLearnerExcept);
				static void openDatabase() throw(ShapeLearnerExcept);
				static void closeDatabase() throw(ShapeLearnerExcept);
				static void test () throw(ShapeLearnerExcept);
		};

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
					throw ShapeLearnerExcept("GraphManager::saveObject // ID", "Error : Saving operation Failed.");
			}

			template <class T> static bool updateObject(T& obj) throw (ShapeLearnerExcept){ return DatabaseManager::Interface::updateObject(obj); }

			template <class T> static bool deleteObject(T& obj) throw (ShapeLearnerExcept){ return DatabaseManager::Interface::deleteObject(obj); }

			static int getPointCountInNode (const int idNode) throw (ShapeLearnerExcept) { return DatabaseManager::Interface::getPointCountInNode (idNode); }

			template <class T, class Y> static vector<unsigned long> getForeignRelations(Y foreignKey) throw (ShapeLearnerExcept) { return DatabaseManager::Interface::getForeignRelations<T,Y>(foreignKey); }
		};

		class CommonInterface {
			friend class ObjectClass;
			friend class GraphClass;
			friend class Graph;
			friend class Node;
			friend class Edge;
			friend class Point;
			friend class GraphManager;

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
			static boost::weak_ptr<Node>			getNode(const boost::weak_ptr<Graph> _refGraph, const unsigned int _index = 1, const unsigned int _level = 1, const unsigned int _mass = 1, const unsigned int _type = 1, const string _label = "1");
			static boost::weak_ptr<Edge>			getEdge(const boost::weak_ptr<Node> _source, const boost::weak_ptr<Node> _target, const boost::weak_ptr<Graph> _refGraph, const unsigned int _weight = 1);
			static boost::weak_ptr<Graph>			getGraph(const boost::weak_ptr<GraphClass> _graphClass, const boost::weak_ptr<ObjectClass> _objectClass, const string _objectName, const unsigned int _viewNumber = 1);
		};

	private:

		// Setters pour les param�tres de la BDD.
		static void setDbName() throw(ShapeLearnerExcept);
		static void setDbUser() throw(ShapeLearnerExcept);
		static void setDbPass() throw(ShapeLearnerExcept);
		static void setDbHost() throw(ShapeLearnerExcept);
		static void setDbType() throw(ShapeLearnerExcept);
		static void setDbPort() throw(ShapeLearnerExcept);
		static void setDBInitFile() throw(ShapeLearnerExcept);

		/*! \brief dbName : Nom de la Base de Donn�es. */
		static string dbName;

		/*! \brief dbUser : Utilisateur de la base de donn�es */
		static string dbUser;

		/*! \brief dbPass : Mot de passe de la base de donn�es */
		static string dbPass;

		/*! \brief dbPort : Port de la base de donn�es */
		static unsigned int dbPort;

		/*! \brief dbHost : Adresse IP ou Hostname du serveur de BDD (localhost par d�faut). */
		static string dbHost;

		/*! \brief dbType : Type de la base de donn�es. Valeur de 1 � 3 => Voir constants.cpp */
		static unsigned int dbType;

		/*! \brief dbInit : Doit on initialiser la base de donn�es ? */
		static string dbInitFile;

		/* ************** Les Maps de stockage ***********/

		static map<unsigned long, boost::shared_ptr<Node>>		NodeMap;
		static map<unsigned long, boost::shared_ptr<Point>>		PointMap;
		static map<unsigned long, boost::shared_ptr<Edge>>		EdgeMap;
		static map<unsigned long, boost::shared_ptr<Graph>>		GraphMap;
		static map<string, boost::shared_ptr<ObjectClass>>		ObjectClassMap;
		static map<string, boost::shared_ptr<GraphClass>>		GraphClassMap;

		/* ************** DB I/O Ops *********************/

		template<class T> static string saveObjectString(T& obj) throw(ShapeLearnerExcept) {
			string rslt = DatabaseManager::Interface::saveObject(obj);
			if(!rslt.compare("")){ // Is Equal
				throw ShapeLearnerExcept("GraphManager::saveObjectString // String", "Error : Saving operation Failed.");
			}
			else
				return rslt;
		}

		template <class T, class Y> static boost::shared_ptr<T> loadObject(Y keyDB) throw (ShapeLearnerExcept){ return DatabaseManager::Interface::loadObject<T>(keyDB); }

		/* **************  No instanciation *********************/

		/*!
		* \brief Constructeur de le class GraphManager.  Il instancie �galement la classe DatabaseManager.
		*/
		GraphManager() throw(ShapeLearnerExcept);

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		GraphManager(const GraphManager&);

		/*!
		*  \brief Op�rateur =  =>> La recopie est interdite
		*/
		GraphManager& operator=(const GraphManager&);

		/*!
		*  \brief La destruction est interdite
		*/
		~GraphManager();
};

#endif //_GRAPH_MANAGER_H_