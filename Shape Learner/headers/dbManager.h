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

#ifndef DatabaseManager_H
#define DatabaseManager_H

#include "allHeaders.h"
using namespace std;

/**
 * \file dbManager.h
 * \brief Interface entre le logiciel et la base de donn�es. Elle est la seule � r�aliser des acc�s � la BDD
 * \version	1.0
 * \author DEKHTIAR Jonathan
 */

class DBException;

class DatabaseManager
{

	public:

		/* **************** Getters *********************** */

		/*!
		*  \brief Renvoie le path du fichier SQLite de la base de donn�e
		*/
		const QString getpath() const;

		/* ****************** Setters ********************* */

		/* **************** DB REQUESTS ******************* */

		/* **************** Retrievers ******************** */

		/**
		* \brief renvoie la liste de tous les idDAGs de la BDD.
		*/
		list<int> getAllidDAGs() const;

		/*!
		*  \brief renvoie une QString contenant le type du DAG dont l'id est pass� en argument
		*
		*  \param idDAG : id du DAG
		*/
		QString getDAGType(const unsigned int idDAG); // Renvoie le type d'une DAG

		/* **************** Deleters *********************** */

		/*!
		*  \brief on r�initialise la DB, on la vide compl�tement : Node + DAGs
		*/
		bool flushDB () const; // On vide compl�tement la base de donn�es


		/* *************** Updaters ********************/

		/*!
		*  \brief on update (=sauvegarde les modifs) dans la BDD un noeud.
		*
		*  \param idNode : Le Node que l'on veut modifier dans la BDD
		*/
		bool updateNode (const int idNode)  const;

		/* ********* AssocBuilders // AssocRemovers **********/


		/* *************** Fillers ********************/
	

		/* ***************  Singleton *********************/
		/*!
		*  \brief M�thode static renvoyant une r�f�rence sur le singleton DatabaseManager
		*
		*  Constructeur de la classe DatabaseManager
		*
		*	\param path : chemin du fichier de la base SQLite
		*  \param user : utilisateur de la base de donn�es
		*  \param pass : mot de passe de la base de donn�es
		*/
		static DatabaseManager&	getInstance(QString path = QDir::currentPath() +"/temp.shape", QString user = "", QString pass = "");

		/*!
		*  \brief M�thode static d�truisant le singleton
		*/
		static void	destroy();

	private:

		/* ****************** Attributs ********************/

		QSqlDatabase *database;
		QString dbpath;

		/* **************** DB Requests ********************/
		/*!
		*  \brief Initialise les tables de la DB et la structure compl�te du mod�le relationnel de la BDD.
		*/
		bool initDB();

		/*!
		*  \brief �x�cute une query pass�e en argument, renvoie un bool sur la r�ussite de l'op�ration
		*  \param query : la query que l'on veut �x�cuter
		*/
		bool query(const QString& query) const; // Execute une query en SQL

		/*!
		*  \brief Renvoie le dernier ID ins�r� dans la table Node.
		*/
		unsigned int getLastID() const; // Retourne le rowid du de la derniere requete d'insertion effectu�e.

		/* ******************** Escaper ********************/
		/*!
		*  \brief Escape une query de mani�re � limiter les injections SQL
		*
		*  \param s : la query que l'on cherche � escape
		*/
		QString escape(QString s) const;

		/*!
		*  \brief Renvoie une QString avec la premi�re lettre en majuscule et les autres en minuscule : "Exemple"
		*
		*  \param str : la string que l'on veut capitalize
		*/
		QString capitalize(QString str) const;

		/* **************** Deleters ***********************/
	
		/*!
		*  \brief Supprime le noeud dont l'id est pass� en argument , renvoie un bool sur la r�ussite de l'op�ration
		*
		*  \param idNode : id du noeud que l'on veut supprimer
		*/
		bool deleteNode (unsigned int idNode) const;

		/*!
		*  \brief Supprime toutes les Nodes de la base
		*/
		bool deleteNode () const; // On supprime tous les Noeuds

		/* **************** Inserters **********************/
		/*!
		*  \brief Ins�re un noeud dans la BDD et renvoie l'id du Noeud.
		*
		* \param idDAG : id du DAG dans lequel on l'ins�re.
		*/
		unsigned int insertNode(const int idDAG) const;


		/* **************** Retrievers **********************/


		/* **************  Singleton *********************/
		/*!
		*  \brief Constructeur
		*
		*  Constructeur de la classe DatabaseManager, sa d�claration en priv� emp�che toute instanciation depuis l'ext�rieur.
		*
		*  \param path : chemin du fichier de la base SQLite
		*  \param user : utilisateur de la base de donn�es
		*  \param pass : mot de passe de la base de donn�es
		*/
		DatabaseManager(const QString &path, const QString &user, const QString &pass) throw(DBException);	// Interdit l'instanciation directe

		/*!
		*  \brief Constructeur de recopie
		*  La recopie est interdite
		*/
		DatabaseManager(const DatabaseManager& nm);	// Interdit la recopie

		/*!
		*  \brief Op�rateur =
		*  La recopie est interdite
		*/
		DatabaseManager& operator=(const DatabaseManager& n);	// Interdit la recopie

		static DatabaseManager *s_inst;	// Contient le singleton s'il est instanci�

		/*!
		*  \brief La destruction est interdite
		*/
		~DatabaseManager();
};

#endif // DatabaseManager_H
