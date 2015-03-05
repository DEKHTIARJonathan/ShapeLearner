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
*	\file dbManager.h
*	\brief Interface entre le logiciel et la base de données. Elle est la seule à réaliser des accès à la BDD
*	\version 1.1
*	\author DEKHTIAR Jonathan
*/

#ifndef DatabaseManager_H
#define DatabaseManager_H

#include "allHeaders.h"
using namespace std;
using namespace odb::core;

class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h
class GraphManager; //Forward Declaration of the class contained in graphManager.h
class ObjectClass;
class GraphClass;
class Graph;
class Node;
class Point;
class Edge;


class DatabaseManager
{
	public:

		/* **************** Getters *********************** */

		/*!
		*  \brief Renvoie le path du fichier SQLite de la base de donnée
		*/
		const string getPath() const;
		const string getName() const;

		/* ****************** Setters ********************* */

		/* **************** DB REQUESTS ******************* */

		/*!
		*  \brief Initialise les tables de la DB et la structure complète du modèle relationnel de la BDD.
		*/
		bool initDB(const string& filename);

		template <class T>
		bool deleteObject(T& obj) throw (ShapeLearnerExcept){
			try
			{
				transaction t (database->begin ());
				database->erase (obj);
				t.commit ();
			}
			catch (const std::exception& e)
			{
				throw ShapeLearnerExcept ("DatabaseManager::deleteObject", "Unable to delete object of class : "+ obj.getClassName() +". // Error = "+ e.what());
			}
			return true;
		}


		/* ********* AssocBuilders // AssocRemovers **********/


		/* *************** Fillers ********************/

		/* *************** Savers ********************/

		template<class T>
		unsigned long saveObject(T& obj) throw(ShapeLearnerExcept){
			unsigned long rslt = 0;

			transaction t (database->begin());
			#ifdef _TRACER_
				t.tracer (stderr_tracer);
			#endif

			// Make objects persistent and save their ids for later use.
			//
			rslt = database->persist (obj);
			if(!rslt){
				t.rollback();
				throw ShapeLearnerExcept("DatabaseManager::saveObject // ID", "Unable to save object id = "+  std::to_string((_ULonglong)rslt) +" // Class : "+obj.getClassName());
			}
			else
				t.commit ();

			return rslt;
		}

		string saveObject(ObjectClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}
		string saveObject(GraphClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}

		/* *************** Updaters ***********************/

		template <class T>
		bool updateObject(T& obj) throw (ShapeLearnerExcept){
			try
			{
				transaction t (database->begin ());
				database->update (obj);
				t.commit ();
			}
			catch (const std::exception& e)
			{
				transaction t (database->begin ());
				database->load (obj.getKey(), obj);
				t.commit ();
				throw ShapeLearnerExcept ("DatabaseManager::updateObject", "Unable to update object of class : "+ obj.getClassName() +". Restoring last saved state. // Error = "+ e.what());
				
			}
			
			return true;
		}

		/* **************************************** Access Restricted to Graph Manager *************************** */
		/*!
		*	\brief Sous classe permettant l'accès à DatabaseManager uniquement à la classe GraphManager.
		*/
		class Key{
			friend class GraphManager;
			private :
				
				/*!
				*	\brief Constructeur de la sous classe DatabaseManager::Key rendu inutilisable. On cherche à empecher l'instanciation de cet objet.
				*/
				Key(); //On empèche l'instanciation de cet objet qui n'est pas voué à être instancié.
				
				/*!
				*	\brief Destructeur de la sous classe DatabaseManager::Key rendu inutilisable. On cherche à empecher l'instanciation de cet objet.
				*/
				~Key(); //On empèche la destruction de cet objet qui n'est pas voué à être instancié.

				// Dans C++11
				// Key() = delete; // Pour empecher la génération du constructeur.

				
				/*!
				*	\brief Méthode static renvoyant une référence sur le singleton DatabaseManager (la classe mère).
				*	\param dbName : Nom de la Base de Données.
				*	\param dbPath : Chemin du dossier contenant la base SQLite
				*	\param dbUser : Utilisateur de la base de données
				*	\param dbPass : Mot de passe de la base de données
				*	\param dbHost : Adresse du serveur de BDD.
				*	\param dbPort : Port du serveur de BDD.
				*	\param dbType : Type de base de données (voir constants.h)
				*/
				static DatabaseManager& getInstance(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort , const unsigned int& dbType, const bool &dbInit);

				/*!
				*  \brief Méthode static détruisant le singleton
				*/
				static void	destroy();
		};



	private:

		/* ****************** Attributs ********************/
		unique_ptr<odb::database> database;
		static DatabaseManager *s_inst;	// Contient le singleton s'il est instancié
		const unsigned int dbType; // Contient le type de BDD => Voir constants.h

		/* **************** DB Requests ********************/

		/*!
		*  \brief Éxécute une query passée en argument, renvoie un bool sur la réussite de l'opération
		*  \param query : la query que l'on veut éxécuter
		*/
		bool query(const string& query) const throw(ShapeLearnerExcept);

		/*!
		*  \brief Renvoie le dernier ID inséré dans la BDD sur toutes les Tables.
		*/
		unsigned int getLastID() const throw(ShapeLearnerExcept);

		/* ******************** Escaper ********************/
		/*!
		*  \brief Escape une query de manière à limiter les injections SQL
		*  \param s : la query que l'on cherche à escape
		*/
		void escape(string& str) const;

		/*!
		*  \brief Renvoie une string avec la première lettre en majuscule et les autres en minuscule : "Exemple"
		*  \param str : la string que l'on veut capitalize
		*/
		void capitalize(string& str) const;

		/* **************** Deleters ***********************/


		/* **************** Inserters **********************/


		/* **************** Retrievers *********************/

		/* **************** Savers *************************/

		template<class T> string saveObjectString(T& obj){
			string rslt = "";
			transaction t (database->begin());
			#ifdef _TRACER_
				t.tracer (stderr_tracer);
			#endif

			// Make objects persistent and save their ids for later use.
			//
			rslt = database->persist(obj);
			if(!rslt.compare("")){ // Is Equal
				t.rollback();
				throw ShapeLearnerExcept("DatabaseManager::saveObjectString // String", "Unable to save object : "+rslt+" // Class : "+obj.getClassName());
			}
			else
				t.commit ();

			return rslt;
		}

		/* ****************** Readers **********************/

		string get_file_contents(const string& filename) throw(ShapeLearnerExcept);

		/* **************  Singleton *********************/


		/*!
		*	\brief Constructeur de la classe DatabaseManager, sa déclaration en privé empêche toute instanciation depuis l'extérieur.
		*	\param dbName : Nom de la Base de Données.
		*	\param dbPath : Chemin du dossier contenant la base SQLite
		*	\param dbUser : Utilisateur de la base de données
		*	\param dbPass : Mot de passe de la base de données
		*	\param dbHost : Adresse du serveur de BDD.
		*	\param dbPort : Port du serveur de BDD.
		*	\param dbType : Type de base de données (voir constants.h)
		*/
		DatabaseManager(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort , const unsigned int& dbType, const bool &dbInit);

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		DatabaseManager(const DatabaseManager& inst);

		/*!
		*  \brief Opérateur =  =>> La recopie est interdite
		*/
		DatabaseManager& operator=(const DatabaseManager& inst);

		/*!
		*  \brief La destruction est interdite
		*/
		~DatabaseManager();

};


#endif // DatabaseManager_H




/* **************** Retrievers ******************** */

	/*!
	*	\brief Renvoie la liste de tous les idGraphs de la BDD.
	*	\return Renvoie une liste d'entier => std::list<unsigned int>.
	*/
//	vector<unsigned int> getAllidGraphs() const;  

	/*!
	*	\brief Renvoie le type du Graph dont l'id est passé en argument
	*	\return Renvoie une string contenant le type du graph.
	*	\param idGraph : id du graph dont on veut connaitre le type.
	*/
//	string getGraphType(const unsigned int idGraph) const; 

	/* **************** Inserters ********************** */
		
	/*!
	*	\brief Insère un noeud dans la BDD.
	*	\return Renvoie un Unsigned Int représentant l'ID du Node.
	*	\param idGraph : id du Graph dans lequel on l'insère.
	*/
//	unsigned int insertNode(const unsigned int idGraph) const; 

	/* **************** Deleters *********************** */

	/*!
	*	\brief On réinitialise la DB à l'état initial, on la vide complètement : Nodes + Graphs + Edges + Points + ObjectClass
	*	\return Renvoie un bool sur la réussite de l'opération.
	*/
//	bool flushDB () const; 

	/*!
	*	\brief On supprime de la BDD toutes les entrées de la table ObjectClass.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*/
//	bool deleteAllObjectClass() const; 

	/*!
	*	\brief On supprime de la BDD tous les Graphs. Tous les Nodes, les Points et les Edges de la BDD sont supprimés en cascade. Les tables Nodes, Graphs, Edges, et Points sont donc entièrement vidées.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*/
//	bool deleteAllGraphs() const; 

	/*!
	*	\brief On supprime de la BDD tous les Nodes existants. Tous les Points et Edges sont supprimés en cascade.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*/
//	bool deleteAllNodes() const; 

	/*!
	*	\brief On supprime de la BDD tous les Nodes du Graph identifié. Tous les Points et Edges associés à ce Node sont supprimés en cascade.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*	\param idGraph : id du Graph dans lequel on veut supprimer tous les noeuds.
	*/
//	bool deleteAllNodeInGraph(unsigned int const idGraph) const; 

	/*!
	*	\brief On supprime de la BDD tous les points.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*/
//	bool deleteAllPoints () const; 

	/*!
	*	\brief On supprime de la BDD tous les points associés au Node identifié.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*	\param idNode : id du Node dans lequel on veut supprimer tous les Points.
	*/
//	bool deleteAllPointsInNode (unsigned int const idNode) const; 

	/*!
	*	\brief On supprime de la BDD tous les points associés au Graph identifié.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*	\param idGraph : id du Node à supprimer
	*/
//	bool deleteAllPointsInGraph (unsigned int const idGraph) const; 

	/*!
	*	\brief On supprime de la BDD toutes les Edges.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*/
//	bool deleteAllEdges () const; 

	/*!
	*	\brief On supprime de la BDD toutes les Edges associées au Graph identifié.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*	\param idGraph : id du Graph dans lequel on veut supprimer toutes les Edges.
	*/
//	bool deleteAllEdgesInGraph (unsigned int const idGraph) const; 

	/*!
	*	\brief On supprime de la BDD toutes les Edges connectées en entrée ou en sortie vers un Node spécifique.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*	\param idNode : id du Node que l'on veut isoler.
	*/
//	bool deleteAllEdgesConnected2Node (unsigned int const idNode) const; 

	/*!
	*	\brief On supprime de la BDD toutes les Edges connectées où source = Node identifié.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*	\param idNode : id du Node dont on veut couper tous les Edges sortantes.
	*/
//	bool deleteAllEdgeFromNode (unsigned int const idNode) const; 

	/*!
	*	\brief  On supprime de la BDD toutes les Edges connectées où target = Node identifié.
	*	\return Renvoie un bool sur la réussite de l'opération.
	*	\param idNode : id du Node dont on veut couper tous les Edges entrantes.
	*/
//	bool deleteAllEdgeToNode (unsigned int const idNode) const; 
