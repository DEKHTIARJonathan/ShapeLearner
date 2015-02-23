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
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _GRAPH_FACTORY_H_
#define _GRAPH_FACTORY_H_

#include "allHeaders.h"
using namespace std;

class GraphManagerExcept; //Forward Declaration of the class contained in graphManagerException.h
class DatabaseManager; //Forward Declaration of the class contained in dbManager.h

class GraphManager
{
	public:
		// Setters pour les paramètres de la BDD.
		static void setDbName() throw(GraphManagerExcept);
		static void setDbPath() throw(GraphManagerExcept);
		static void setDbUser() throw(GraphManagerExcept);
		static void setDbPass() throw(GraphManagerExcept);
		static void setDbHost() throw(GraphManagerExcept);

		/* **************  Singleton *********************/

		/**
		*  \brief Méthode static détruisant le singleton et détruisant en cascade tous les objets en dessous de lui.
		*/
		static void	destroy();

		/**
		*  \brief Méthode static initialisant le singleton et instanciant la Base de Données selon les paramètres qui lui sont fournis.
		*/
		static GraphManager& openManager();
	
	private:

		/*! \brief dBManager : Référence vers l'unique instance du DatabaseManager en mémoire. */
		DatabaseManager& dBManager;

		/*! \brief dbName : Nom de la Base de Données. */
		static QString dbName;

		/*! \brief dbPath : Chemin du dossier contenant la base SQLite */
		static QString dbPath;

		/*! \brief user : Utilisateur de la base de données */
		static QString user;

		/*! \brief pass : Mot de passe de la base de données */
		static QString pass;

		/*! \brief hostname : Adresse du serveur de BDD (localhost d'office pour SQLite). */
		static QString hostname;

		/*! \brief s_inst : Unique instance du GraphManager en mémoire.*/
		static GraphManager* s_inst;

		/* **************  Singleton *********************/

		/*!
		* \brief Constructeur de le class GraphManager.  Il instancie également la classe DatabaseManager.
		*/
		GraphManager() throw(GraphManagerExcept);

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


#endif //_GRAPH_FACTORY_H_