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
*	\brief Interface publique d'accès au logiciel. Elle concentre les accès à la BDD, le Génarateur de Graph et le Graph Matcher.
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
				// Setters pour les paramètres de la BDD.
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

			static unsigned int execCountRequest( const string& query ) { return DatabaseManager::Interface::execCountRequest(query); }
		};	
	
	private:

		// Setters pour les paramètres de la BDD.
		static void setDbName() throw(ShapeLearnerExcept);
		static void setDbUser() throw(ShapeLearnerExcept);
		static void setDbPass() throw(ShapeLearnerExcept);
		static void setDbHost() throw(ShapeLearnerExcept);
		static void setDbType() throw(ShapeLearnerExcept);
		static void setDbPort() throw(ShapeLearnerExcept);
		static void setDBInitFile() throw(ShapeLearnerExcept);

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

		/*! \brief dbType : Type de la base de données. Valeur de 1 à 3 => Voir constants.cpp */
		static unsigned int dbType;

		/*! \brief dbInit : Doit on initialiser la base de données ? */
		static string dbInitFile;

		/* ************** DB I/O Ops *********************/

		template<class T> static string saveObjectString(T& obj) throw(ShapeLearnerExcept) {
			string rslt = DatabaseManager::Interface::saveObject(obj);
			if(!rslt.compare("")){ // Is Equal
				throw ShapeLearnerExcept("GraphManager::saveObjectString // String", "Error : Saving operation Failed.");
			}
			else
				return rslt;
		}

		/* **************  No instanciation *********************/

		/*!
		* \brief Constructeur de le class GraphManager.  Il instancie également la classe DatabaseManager.
		*/
		GraphManager() throw(ShapeLearnerExcept);

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		GraphManager(const GraphManager&);

		/*!
		*  \brief Opérateur =  =>> La recopie est interdite
		*/
		GraphManager& operator=(const GraphManager&);

		/*!
		*  \brief La destruction est interdite
		*/
		~GraphManager();
};


#endif //_GRAPH_MANAGER_H_