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

#ifndef _GRAPH_FACTORY_H_
#define _GRAPH_FACTORY_H_

#include "allHeaders.h"
using namespace std;

class GraphManagerExcept; //Forward Declaration of the class contained in graphManagerException.h
class DatabaseManager; //Forward Declaration of the class contained in dbManager.h

class GraphManager
{
	public:

		// Setters pour les param�tres de la BDD.
		static void getDbCredentials() throw(GraphManagerExcept);

		/* **************  Singleton *********************/

		/**
		*	\brief M�thode static d�truisant le singleton et d�truisant en cascade tous les objets en dessous de lui.
		*/
		static void	destroy();

		
		/**
		*	\brief M�thode static initialisant le singleton et instanciant la Base de Donn�es selon les param�tres qui lui sont fournis.
		*/
		static GraphManager& openManager();

		/* **************  Command Line Parsing *********************/

		/**
		*	\brief M�thode static r�alisant l'analyse des arguments fournis en ligne de commande.
		*	\return argc : Argument fourni par la fonction main().
		*	\return argv : Argument fourni par la fonction main().
		*/
		static void parseCommandLine(int argc, char **argv) throw(GraphManagerExcept);

		

	
	private:

		// Setters pour les param�tres de la BDD.
		static void setDbName() throw(GraphManagerExcept);
		static void setDbUser() throw(GraphManagerExcept);
		static void setDbPass() throw(GraphManagerExcept);
		static void setDbHost() throw(GraphManagerExcept);
		static void setDbType() throw(GraphManagerExcept);
		static void setDbPort() throw(GraphManagerExcept);

		/*! \brief dBManager : R�f�rence vers l'unique instance du DatabaseManager en m�moire. */
		DatabaseManager& dBManager;

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
		static bool dbInit;

		/*! \brief s_inst : Unique instance du GraphManager en m�moire.*/
		static GraphManager* s_inst;

		

		/* **************  Singleton *********************/

		/*!
		* \brief Constructeur de le class GraphManager.  Il instancie �galement la classe DatabaseManager.
		*/
		GraphManager() throw(GraphManagerExcept);

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		GraphManager(const GraphManager& gm);

		/*!
		*  \brief Op�rateur =  =>> La recopie est interdite
		*/
		GraphManager& operator=(const GraphManager& gm);

		/*!
		*  \brief La destruction est interdite
		*/
		~GraphManager();
};


#endif //_GRAPH_FACTORY_H_