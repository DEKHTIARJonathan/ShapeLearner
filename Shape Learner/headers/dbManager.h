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
 * \brief Interface entre le logiciel et la base de données. Elle est la seule à réaliser des accès à la BDD
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
		*  \brief Renvoie le path du fichier SQLite de la base de donnée
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
		list<unsigned int> getAllidGraphs() const;  /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief Renvoie le type du Graph dont l'id est passé en argument
		*	\return Renvoie une QString contenant le type du graph.
		*	\param idGraph : id du graph dont on veut connaitre le type.
		*/
		QString getGraphType(const unsigned int idGraph) const; /* ==================================== à implémenter ===================================== */

		/* **************** Inserters ********************** */
		
		/*!
		*	\brief Insère un noeud dans la BDD.
		*	\return Renvoie un Unsigned Int représentant l'ID du Node.
		*	\param idGraph : id du Graph dans lequel on l'insère.
		*/
		unsigned int insertNode(const unsigned int idGraph) const; /* ==================================== à implémenter ===================================== */

		/* **************** Deleters *********************** */

		/*!
		*	\brief On réinitialise la DB à l'état initial, on la vide complètement : Nodes + Graphs + Edges + Points + ObjectClass
		*	\return Renvoie un bool sur la réussite de l'opération.
		*/
		bool flushDB () const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les entrées de la table ObjectClass.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*/
		bool deleteAllObjectClass() const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les Graphs. Tous les Nodes, les Points et les Edges de la BDD sont supprimés en cascade. Les tables Nodes, Graphs, Edges, et Points sont donc entièrement vidées.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*/
		bool deleteAllGraphs() const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD le Graph identifié. Tous les Nodes, les Points et les Edges associés à ce Graph sont supprimés en cascade.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idGraph : id du Graph à supprimer
		*/
		bool deleteGraph(unsigned int const idGraph) const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les Nodes existants. Tous les Points et Edges sont supprimés en cascade.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*/
		bool deleteAllNodes() const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD le Node identifié. Les points et Edges associés à ce Node sont supprimés en cascade.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idNode : id du Node à supprimer
		*/
		bool deleteNode(unsigned int const idNode) const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les Nodes du Graph identifié. Tous les Points et Edges associés à ce Node sont supprimés en cascade.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idGraph : id du Graph dans lequel on veut supprimer tous les noeuds.
		*/
		bool deleteAllNodeInGraph(unsigned int const idGraph) const; /* ==================================== à implémenter ===================================== */
		
		/*!
		*	\brief On supprime de la BDD le Point identifié.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idPoint : id du Point à supprimer
		*/
		bool deletePoint (unsigned int const idPoint) const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les points.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*/
		bool deleteAllPoints () const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les points associés au Node identifié.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idNode : id du Node dans lequel on veut supprimer tous les Points.
		*/
		bool deleteAllPointsInNode (unsigned int const idNode) const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD tous les points associés au Graph identifié.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idGraph : id du Node à supprimer
		*/
		bool deleteAllPointsInGraph (unsigned int const idGraph) const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les Edges.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*/
		bool deleteAllEdges () const; /* ==================================== à implémenter ===================================== */
		
		/*!
		*	\brief On supprime de la BDD l'Edge identifiée.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idEdge : id de l'Edge à supprimer
		*/
		bool deleteEdge (unsigned int const idEdge) const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les Edges associées au Graph identifié.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idGraph : id du Graph dans lequel on veut supprimer toutes les Edges.
		*/
		bool deleteAllEdgesInGraph (unsigned int const idGraph) const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les Edges connectées en entrée ou en sortie vers un Node spécifique.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idNode : id du Node que l'on veut isoler.
		*/
		bool deleteAllEdgesConnected2Node (unsigned int const idNode) const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief On supprime de la BDD toutes les Edges connectées où source = Node identifié.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idNode : id du Node dont on veut couper tous les Edges sortantes.
		*/
		bool deleteAllEdgeFromNode (unsigned int const idNode) const; /* ==================================== à implémenter ===================================== */

		/*!
		*	\brief  On supprime de la BDD toutes les Edges connectées où target = Node identifié.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idNode : id du Node dont on veut couper tous les Edges entrantes.
		*/
		bool deleteAllEdgeToNode (unsigned int const idNode) const; /* ==================================== à implémenter ===================================== */

		/* *************** Updaters ********************/

		/*!
		*	\brief Update (=sauvegarde les modifications) dans la BDD d'un Node.
		*	\return Renvoie un bool sur la réussite de l'opération.
		*	\param idNode : Le Node que l'on veut modifier dans la BDD
		*/
		bool updateNode (const unsigned int idNode)  const; /* ==================================== à implémenter ===================================== */

		/* ********* AssocBuilders // AssocRemovers **********/


		/* *************** Fillers ********************/
	

		/* ***************  Singleton *********************/
		

		/*!
		*	\brief Méthode static renvoyant une référence sur le singleton DatabaseManager.
		*	\param dbName : Nom de la Base de Données.
		*	\param dbPath : Chemin du dossier contenant la base SQLite
		*	\param user : Utilisateur de la base de données
		*	\param pass : Mot de passe de la base de données
		*	\param hostname : Adresse du serveur de BDD (localhost d'office pour SQLite).
		*/
		static DatabaseManager&	getInstance(QString dbName = "temp.shape", QString dbPath = QDir::currentPath(), QString user = "", QString pass = "", QString hostname = "localhost");

		/*!
		*  \brief Méthode static détruisant le singleton
		*/
		static void	destroy();

	private:

		/* ****************** Attributs ********************/

		QSqlDatabase *database;
		QString dbPath;
		static DatabaseManager *s_inst;	// Contient le singleton s'il est instancié

		/* **************** DB Requests ********************/
		/*!
		*  \brief Initialise les tables de la DB et la structure complète du modèle relationnel de la BDD.
		*/
		bool initDB();

		/*!
		*  \brief Éxécute une query passée en argument, renvoie un bool sur la réussite de l'opération
		*  \param query : la query que l'on veut éxécuter
		*/
		bool query(const QString& query) const throw(DBException);

		/*!
		*  \brief Renvoie le dernier ID inséré dans la BDD sur toutes les Tables.
		*/
		unsigned int getLastID() const throw(DBException);

		/* ******************** Escaper ********************/
		/*!
		*  \brief Escape une query de manière à limiter les injections SQL
		*  \param s : la query que l'on cherche à escape
		*/
		QString escape(QString s) const;

		/*!
		*  \brief Renvoie une QString avec la première lettre en majuscule et les autres en minuscule : "Exemple"
		*  \param str : la string que l'on veut capitalize
		*/
		QString capitalize(QString str) const;

		/* **************** Deleters ***********************/


		/* **************** Inserters **********************/


		/* **************** Retrievers **********************/


		/* **************  Singleton *********************/
		/*!
		*	\brief Constructeur de la classe DatabaseManager, sa déclaration en privé empêche toute instanciation depuis l'extérieur.
		*	\param dbName : Nom de la Base de Données.
		*	\param dbPath : Chemin du dossier contenant la base SQLite
		*	\param user : Utilisateur de la base de données
		*	\param pass : Mot de passe de la base de données
		*	\param hostname : Adresse du serveur de BDD (localhost d'office pour SQLite).
		*/
		DatabaseManager(const QString &dbName, const QString &dbPath, const QString &user, const QString &pass, const QString &hostname) throw(DBException);

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		DatabaseManager(const DatabaseManager& nm);

		/*!
		*  \brief Opérateur =  =>> La recopie est interdite
		*/
		DatabaseManager& operator=(const DatabaseManager& n);

		/*!
		*  \brief La destruction est interdite
		*/
		~DatabaseManager();
};

#endif // DatabaseManager_H
