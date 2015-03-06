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
using namespace std;

class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h
class DatabaseManager; //Forward Declaration of the class contained in dbManager.h
class ObjectClass;
class GraphClass;
class Graph;
class Node;
class Point;
class Edge;

class GraphManager
{
	public:

		// Setters pour les paramètres de la BDD.
		static void getDbCredentials() throw(ShapeLearnerExcept);

		/* ************** DB I/O Ops *********************/

		string saveObject(ObjectClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}
		string saveObject(GraphClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}

		template<class T>
		unsigned long saveObject(T& obj) throw(ShapeLearnerExcept){ 
			unsigned long rslt = dBManager.saveObject(obj); 
			if (rslt != 0)
				return rslt;
			else 
				throw ShapeLearnerExcept("GraphManager::saveObject // ID", "Error : Saving operation Failed.");
		}		
		
		template <class T>
		bool updateObject(T& obj) throw (ShapeLearnerExcept){ return dBManager.updateObject(obj); }

		template <class T>
		bool deleteObject(T& obj) throw (ShapeLearnerExcept){ return dBManager.deleteObject(obj); }


		/* **************  Singleton *********************/

		/**
		*	\brief Méthode static détruisant le singleton et détruisant en cascade tous les objets en dessous de lui.
		*/
		static void	destroy();

		
		/**
		*	\brief Méthode static initialisant le singleton et instanciant la Base de Données selon les paramètres qui lui sont fournis.
		*/
		static GraphManager& openManager();

		/* **************  Command Line Parsing *********************/

		/**
		*	\brief Méthode static réalisant l'analyse des arguments fournis en ligne de commande.
		*	\return argc : Argument fourni par la fonction main().
		*	\return argv : Argument fourni par la fonction main().
		*/
		static void parseCommandLine(int argc, char **argv) throw(ShapeLearnerExcept);

		void test () throw(ShapeLearnerExcept);
	
	private:

		// Setters pour les paramètres de la BDD.
		static void setDbName() throw(ShapeLearnerExcept);
		static void setDbUser() throw(ShapeLearnerExcept);
		static void setDbPass() throw(ShapeLearnerExcept);
		static void setDbHost() throw(ShapeLearnerExcept);
		static void setDbType() throw(ShapeLearnerExcept);
		static void setDbPort() throw(ShapeLearnerExcept);

		/*! \brief dBManager : Référence vers l'unique instance du DatabaseManager en mémoire. */
		DatabaseManager& dBManager;

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
		static bool dbInit;

		/*! \brief s_inst : Unique instance du GraphManager en mémoire.*/
		static GraphManager* s_inst;

		/* ************** DB I/O Ops *********************/

		template<class T>
		string saveObjectString(T& obj) throw(ShapeLearnerExcept) {
			string rslt = dBManager.saveObject(obj);
			if(!rslt.compare("")){ // Is Equal
				throw ShapeLearnerExcept("GraphManager::saveObjectString // String", "Error : Saving operation Failed.");
			}
			else
				return rslt;
		}

		

		/* **************  Singleton *********************/

		/*!
		* \brief Constructeur de le class GraphManager.  Il instancie également la classe DatabaseManager.
		*/
		GraphManager() throw(ShapeLearnerExcept);

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		GraphManager(const GraphManager& gm);

		/*!
		*  \brief Opérateur =  =>> La recopie est interdite
		*/
		GraphManager& operator=(const GraphManager& gm);

		/*!
		*  \brief La destruction est interdite
		*/
		~GraphManager();
};


#endif //_GRAPH_MANAGER_H_