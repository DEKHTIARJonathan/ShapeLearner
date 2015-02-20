/* ************* Begin file dbManager.h ***************************************/
/*
** 2015 February 18
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
 * \file dbManager.h
 * \brief Interface entre le logiciel et la base de donn�es. Elle est la seule � r�aliser des acc�s � la BDD
 * \version	1.0
 * \author DEKHTIAR Jonathan
 */

#ifndef DatabaseManager_H
#define DatabaseManager_H

#include "allHeaders.h"
using namespace std;

class DBException; //Forward Declaration of the class contained in dbexception.h

class DatabaseManager
{

	public:

		/* **************** Getters *********************** */

		/*!
		*  \brief Renvoie le path du fichier SQLite de la base de donn�e
		*/
		const QString getPath() const;
		const QString getName() const;

		/* ****************** Setters ********************* */

		/* **************** DB REQUESTS ******************* */

		/* **************** Retrievers ******************** */

		/*!
		*	\brief Renvoie la liste de tous les idGraphs de la BDD.
		*	\return Renvoie une liste d'entier => std::list<unsigned int>.
		*/
		list<unsigned int> getAllidGraphs() const;  /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief Renvoie le type du Graph dont l'id est pass� en argument
		*	\return Renvoie une QString contenant le type du graph.
		*	\param idGraph : id du graph dont on veut connaitre le type.
		*/
		QString getGraphType(const unsigned int idGraph) const; /* ==================================== � impl�menter ===================================== */

		/* **************** Inserters ********************** */
		
		/*!
		*	\brief Ins�re un noeud dans la BDD.
		*	\return Renvoie un Unsigned Int repr�sentant l'ID du Node.
		*	\param idGraph : id du Graph dans lequel on l'ins�re.
		*/
		unsigned int insertNode(const unsigned int idGraph) const; /* ==================================== � impl�menter ===================================== */

		/* **************** Deleters *********************** */

		/*!
		*	\brief On r�initialise la DB � l'�tat initial, on la vide compl�tement : Nodes + Graphs + Edges + Points + ObjectClass
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*/
		bool flushDB () const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les entr�es de la table ObjectClass.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*/
		bool deleteAllObjectClass() const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les Graphs. Tous les Nodes, les Points et les Edges de la BDD sont supprim�s en cascade. Les tables Nodes, Graphs, Edges, et Points sont donc enti�rement vid�es.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*/
		bool deleteAllGraphs() const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD le Graph identifi�. Tous les Nodes, les Points et les Edges associ�s � ce Graph sont supprim�s en cascade.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idGraph : id du Graph � supprimer
		*/
		bool deleteGraph(unsigned int const idGraph) const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les Nodes existants. Tous les Points et Edges sont supprim�s en cascade.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*/
		bool deleteAllNodes() const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD le Node identifi�. Les points et Edges associ�s � ce Node sont supprim�s en cascade.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idNode : id du Node � supprimer
		*/
		bool deleteNode(unsigned int const idNode) const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les Nodes du Graph identifi�. Tous les Points et Edges associ�s � ce Node sont supprim�s en cascade.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idGraph : id du Graph dans lequel on veut supprimer tous les noeuds.
		*/
		bool deleteAllNodeInGraph(unsigned int const idGraph) const; /* ==================================== � impl�menter ===================================== */
		
		/*!
		*	\brief On supprime de la BDD le Point identifi�.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idPoint : id du Point � supprimer
		*/
		bool deletePoint (unsigned int const idPoint) const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les points.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*/
		bool deleteAllPoints () const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les points associ�s au Node identifi�.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idNode : id du Node dans lequel on veut supprimer tous les Points.
		*/
		bool deleteAllPointsInNode (unsigned int const idNode) const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les points associ�s au Graph identifi�.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idGraph : id du Node � supprimer
		*/
		bool deleteAllPointsInGraph (unsigned int const idGraph) const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les Edges.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*/
		bool deleteAllEdges () const; /* ==================================== � impl�menter ===================================== */
		
		/*!
		*	\brief On supprime de la BDD l'Edge identifi�e.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idEdge : id de l'Edge � supprimer
		*/
		bool deleteEdge (unsigned int const idEdge) const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les Edges associ�es au Graph identifi�.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idGraph : id du Graph dans lequel on veut supprimer toutes les Edges.
		*/
		bool deleteAllEdgesInGraph (unsigned int const idGraph) const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les Edges connect�es en entr�e ou en sortie vers un Node sp�cifique.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idNode : id du Node que l'on veut isoler.
		*/
		bool deleteAllEdgesConnected2Node (unsigned int const idNode) const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les Edges connect�es o� source = Node identifi�.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idNode : id du Node dont on veut couper tous les Edges sortantes.
		*/
		bool deleteAllEdgeFromNode (unsigned int const idNode) const; /* ==================================== � impl�menter ===================================== */

		/*!
		*	\brief  On supprime de la BDD toutes les Edges connect�es o� target = Node identifi�.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idNode : id du Node dont on veut couper tous les Edges entrantes.
		*/
		bool deleteAllEdgeToNode (unsigned int const idNode) const; /* ==================================== � impl�menter ===================================== */

		/* *************** Updaters ********************/

		/*!
		*	\brief Update (=sauvegarde les modifications) dans la BDD d'un Node.
		*	\return Renvoie un bool sur la r�ussite de l'op�ration.
		*	\param idNode : Le Node que l'on veut modifier dans la BDD
		*/
		bool updateNode (const unsigned int idNode)  const; /* ==================================== � impl�menter ===================================== */

		/* ********* AssocBuilders // AssocRemovers **********/


		/* *************** Fillers ********************/
	

		/* ***************  Singleton *********************/
		

		/*!
		*	\brief M�thode static renvoyant une r�f�rence sur le singleton DatabaseManager.
		*	\param dbName : Nom de la Base de Donn�es.
		*	\param dbPath : Chemin du dossier contenant la base SQLite
		*	\param user : Utilisateur de la base de donn�es
		*	\param pass : Mot de passe de la base de donn�es
		*	\param hostname : Adresse du serveur de BDD (localhost d'office pour SQLite).
		*/
		static DatabaseManager&	getInstance(QString dbName = "temp.shape", QString dbPath = QDir::currentPath(), QString user = "", QString pass = "", QString hostname = "localhost");

		/*!
		*  \brief M�thode static d�truisant le singleton
		*/
		static void	destroy();

	private:

		/* ****************** Attributs ********************/

		QSqlDatabase *database;
		QString dbPath;
		static DatabaseManager *s_inst;	// Contient le singleton s'il est instanci�

		/* **************** DB Requests ********************/
		/*!
		*  \brief Initialise les tables de la DB et la structure compl�te du mod�le relationnel de la BDD.
		*/
		bool initDB();

		/*!
		*  \brief �x�cute une query pass�e en argument, renvoie un bool sur la r�ussite de l'op�ration
		*  \param query : la query que l'on veut �x�cuter
		*/
		bool query(const QString& query) const throw(DBException);

		/*!
		*  \brief Renvoie le dernier ID ins�r� dans la BDD sur toutes les Tables.
		*/
		unsigned int getLastID() const throw(DBException);

		/* ******************** Escaper ********************/
		/*!
		*  \brief Escape une query de mani�re � limiter les injections SQL
		*  \param s : la query que l'on cherche � escape
		*/
		QString escape(QString s) const;

		/*!
		*  \brief Renvoie une QString avec la premi�re lettre en majuscule et les autres en minuscule : "Exemple"
		*  \param str : la string que l'on veut capitalize
		*/
		QString capitalize(QString str) const;

		/* **************** Deleters ***********************/


		/* **************** Inserters **********************/


		/* **************** Retrievers **********************/


		/* **************  Singleton *********************/
		/*!
		*	\brief Constructeur de la classe DatabaseManager, sa d�claration en priv� emp�che toute instanciation depuis l'ext�rieur.
		*	\param dbName : Nom de la Base de Donn�es.
		*	\param dbPath : Chemin du dossier contenant la base SQLite
		*	\param user : Utilisateur de la base de donn�es
		*	\param pass : Mot de passe de la base de donn�es
		*	\param hostname : Adresse du serveur de BDD (localhost d'office pour SQLite).
		*/
		DatabaseManager(const QString &dbName, const QString &dbPath, const QString &user, const QString &pass, const QString &hostname) throw(DBException);

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		DatabaseManager(const DatabaseManager& nm);

		/*!
		*  \brief Op�rateur =  =>> La recopie est interdite
		*/
		DatabaseManager& operator=(const DatabaseManager& n);

		/*!
		*  \brief La destruction est interdite
		*/
		~DatabaseManager();
};

#endif // DatabaseManager_H
