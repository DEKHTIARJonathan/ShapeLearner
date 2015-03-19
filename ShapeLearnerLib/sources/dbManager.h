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
*	\brief Interface entre le logiciel et la base de donn�es. Elle est la seule � r�aliser des acc�s � la BDD
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

		/* **************************************** Access Restricted to Graph Manager *************************** */
		/**
		*	\brief Sous classe permettant l'acc�s � DatabaseManager uniquement � la classe GraphManager.
		*/
		class Interface{
			friend class GraphManager;
			private :

				/* *************** DB Handlers ***********************/

				/*!
				*	\brief M�thode static �tablissant la connection � la base de donn�es.
				*	\param dbName : Nom de la Base de Donn�es.
				*	\param dbPath : Chemin du dossier contenant la base SQLite
				*	\param dbUser : Utilisateur de la base de donn�es
				*	\param dbPass : Mot de passe de la base de donn�es
				*	\param dbHost : Adresse du serveur de BDD.
				*	\param dbPort : Port du serveur de BDD.
				*	\param dbType : Type de base de donn�es (voir constants.h)
				*/
				static void openDatabase(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort , const unsigned int& dbType, const string& dbInit = "") throw(ShapeLearnerExcept);

				/*!
				*  \brief M�thode static d�truisant la base de donn�es
				*/
				static void	closeDatabase() throw(ShapeLearnerExcept);

				static bool isDbOpen();

				/* *************** Updaters ***********************/

				template <class T> static bool updateObject(T& obj) throw (ShapeLearnerExcept){
					transaction t1 (database->begin ());
					try
					{
						#ifdef _TRACER_
							t1.tracer (stderr_tracer);
						#endif
						database->update (obj);
						t1.commit ();
						return true;
					}
					catch (const std::exception& e)
					{
						t1.rollback();
						transaction t2 (database->begin ());
						#ifdef _TRACER_
							t2.tracer (stderr_tracer);
						#endif
						database->load (obj.getKey(), obj);
						t2.commit ();
						throw ShapeLearnerExcept ("DatabaseManager::Interface::updateObject", "Unable to update object of class : "+ obj.getClassName() +". Restoring last saved state. // Error = " + e.what());
						return false;
					}
					
				}

				template <class T> static boost::shared_ptr<T> loadObject(string keyDB) throw (ShapeLearnerExcept){
					transaction t (database->begin ());
					try{
						#ifdef _TRACER_
							t.tracer (stderr_tracer);
						#endif
						boost::shared_ptr<T> rslt (database->load<T>(keyDB));
						t.commit ();

						return rslt;
					}
					catch(const std::exception& e){
						t.rollback();
						throw ShapeLearnerExcept("DatabaseManager::Interface::loadObject", "Unable to perform operation // Key Requested : "+ keyDB +" Error = "+ boost::lexical_cast<std::string>(e.what()));
						return boost::shared_ptr<T>();
					}
				}

				template <class T> static boost::shared_ptr<T> loadObject(int keyDB) throw (ShapeLearnerExcept){
					transaction t (database->begin ());
					try
					{
						#ifdef _TRACER_
							t.tracer (stderr_tracer);
						#endif
						boost::shared_ptr<T> rslt (database->load<T> (keyDB));
						t.commit ();

						return rslt;
					}
					catch (const std::exception& e)
					{
						t.rollback();
						throw ShapeLearnerExcept ("DatabaseManager::Interface::loadObject", "Unable to load object // Error = "+ string(e.what()));
						return boost::shared_ptr<T>();
					}
				}
					
				template <class T, class Y> static vector<unsigned long> getForeignRelations(Y foreignKey) throw (ShapeLearnerExcept){
					transaction t (database->begin());
					try {
						typedef odb::query<T> query;
						typedef odb::result<T> result;
					
						#ifdef _TRACER_
							t.tracer (stderr_tracer);
						#endif

						result r (database->query<T> ("'"+boost::lexical_cast<std::string>(foreignKey)+"'"));

						vector<unsigned long> rslt;

						for (result::iterator i (r.begin ()); i != r.end (); ++i)
							rslt.push_back(i->id);

						t.commit ();

						return rslt;
					}
					catch(const std::exception& e){
						t.rollback();
						throw ShapeLearnerExcept("DatabaseManager::Interface::getForeignRelations", "Unable to perform operation // Error = "+ boost::lexical_cast<std::string>(e.what()));
						return vector<unsigned long> ();
					}
					
				}
				/* *************** Savers ***********************/

				template<class T> static unsigned long saveObject(T& obj) throw(ShapeLearnerExcept){
					transaction t (database->begin());
					try{	
						#ifdef _TRACER_
							t.tracer (stderr_tracer);
						#endif
						unsigned long rslt = database->persist (obj);
											
						t.commit ();

						return rslt;
					}
					catch (const std::exception& e){
						t.rollback();
						throw ShapeLearnerExcept("DatabaseManager::Interface::saveObject // ID", "Unable to save object of class : "+obj.getClassName() + " // Error : "+ e.what());
						return 0;
					}

					
				}

				static string saveObject(ObjectClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}
				static string saveObject(GraphClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}

				/* *************** Deleters ***********************/

				template <class T> static bool deleteObject(T& obj) throw (ShapeLearnerExcept){
					transaction t (database->begin ());
					try
					{
						#ifdef _TRACER_
							t.tracer (stderr_tracer);
						#endif
						database->erase (obj);
						t.commit ();
						return true;
					}
					catch (const std::exception& e)
					{
						t.rollback();
						throw ShapeLearnerExcept ("DatabaseManager::Interface::deleteObject", "Unable to delete object of class : "+ obj.getClassName() +". // Error = "+ e.what());
						return false;
					}	
				}

				/* *************** Query DB **********************/
				static unsigned long getPointCountInNode (const unsigned long idNode) throw (ShapeLearnerExcept);

				static void test () throw (ShapeLearnerExcept);
		};

	private:

		/* ****************** Attributs ********************/
		static odb::database* database;
		static const unsigned int dbType; // Contient le type de BDD => Voir constants.h

		/* **************** DB Requests ********************/

		/*!
		*  \brief �x�cute une query pass�e en argument, renvoie un bool sur la r�ussite de l'op�ration
		*  \param query : la query que l'on veut �x�cuter
		*/
		static bool query(const string& query) throw(ShapeLearnerExcept);

		/*!
		*  \brief Renvoie le dernier ID ins�r� dans la BDD sur toutes les Tables.
		*/
		static unsigned int getLastID() throw(ShapeLearnerExcept);

		/* ******************** Escaper ********************/
		/*!
		*  \brief Escape une query de mani�re � limiter les injections SQL
		*  \param s : la query que l'on cherche � escape
		*/
		static void escape(string& str);

		/*!
		*  \brief Renvoie une string avec la premi�re lettre en majuscule et les autres en minuscule : "Exemple"
		*  \param str : la string que l'on veut capitalize
		*/
		static void capitalize(string& str);

		/* **************** DB Handlers ***********************/

		/*!
		*  \brief Initialise les tables de la DB et la structure compl�te du mod�le relationnel de la BDD.
		*/
		static bool initDB(const string& filename);

		/* **************** Savers *************************/

		template<class T>
		static string saveObjectString(T& obj){
			transaction t (database->begin());
			try{
				#ifdef _TRACER_
					t.tracer (stderr_tracer);
				#endif
				string rslt = database->persist(obj);
				t.commit ();

				return rslt;
			}
			catch (const std::exception& e){
				t.rollback();
				throw ShapeLearnerExcept("DatabaseManager::saveObjectString // String", "Unable to save object : "+obj.getKey()+" // Class : "+obj.getClassName() + "// Error : "+ e.what());
				return "";
			}
			
		}

		/* ****************** Readers **********************/

		static string get_file_contents(const string& filename) throw(ShapeLearnerExcept);

		/* **************  No instanciation *********************/

		DatabaseManager();

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		DatabaseManager(const DatabaseManager&);

		/*!
		*  \brief Op�rateur =  =>> La recopie est interdite
		*/
		DatabaseManager& operator=(const DatabaseManager&);

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
	*	\brief Renvoie le type du Graph dont l'id est pass� en argument
	*	\return Renvoie une string contenant le type du graph.
	*	\param idGraph : id du graph dont on veut connaitre le type.
	*/
//	string getGraphType(const unsigned int idGraph) const;

	/* **************** Inserters ********************** */

	/*!
	*	\brief Ins�re un noeud dans la BDD.
	*	\return Renvoie un Unsigned Int repr�sentant l'ID du Node.
	*	\param idGraph : id du Graph dans lequel on l'ins�re.
	*/
//	unsigned int insertNode(const unsigned int idGraph) const;

	/* **************** Deleters *********************** */

	/*!
	*	\brief On r�initialise la DB � l'�tat initial, on la vide compl�tement : Nodes + Graphs + Edges + Points + ObjectClass
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*/
//	bool flushDB () const;

	/*!
	*	\brief On supprime de la BDD toutes les entr�es de la table ObjectClass.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*/
//	bool deleteAllObjectClass() const;

	/*!
	*	\brief On supprime de la BDD tous les Graphs. Tous les Nodes, les Points et les Edges de la BDD sont supprim�s en cascade. Les tables Nodes, Graphs, Edges, et Points sont donc enti�rement vid�es.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*/
//	bool deleteAllGraphs() const;

	/*!
	*	\brief On supprime de la BDD tous les Nodes existants. Tous les Points et Edges sont supprim�s en cascade.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*/
//	bool deleteAllNodes() const;

	/*!
	*	\brief On supprime de la BDD tous les Nodes du Graph identifi�. Tous les Points et Edges associ�s � ce Node sont supprim�s en cascade.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*	\param idGraph : id du Graph dans lequel on veut supprimer tous les noeuds.
	*/
//	bool deleteAllNodeInGraph(unsigned int const idGraph) const;

	/*!
	*	\brief On supprime de la BDD tous les points.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*/
//	bool deleteAllPoints () const;

	/*!
	*	\brief On supprime de la BDD tous les points associ�s au Node identifi�.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*	\param idNode : id du Node dans lequel on veut supprimer tous les Points.
	*/
//	bool deleteAllPointsInNode (unsigned int const idNode) const;

	/*!
	*	\brief On supprime de la BDD tous les points associ�s au Graph identifi�.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*	\param idGraph : id du Node � supprimer
	*/
//	bool deleteAllPointsInGraph (unsigned int const idGraph) const;

	/*!
	*	\brief On supprime de la BDD toutes les Edges.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*/
//	bool deleteAllEdges () const;

	/*!
	*	\brief On supprime de la BDD toutes les Edges associ�es au Graph identifi�.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*	\param idGraph : id du Graph dans lequel on veut supprimer toutes les Edges.
	*/
//	bool deleteAllEdgesInGraph (unsigned int const idGraph) const;

	/*!
	*	\brief On supprime de la BDD toutes les Edges connect�es en entr�e ou en sortie vers un Node sp�cifique.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*	\param idNode : id du Node que l'on veut isoler.
	*/
//	bool deleteAllEdgesConnected2Node (unsigned int const idNode) const;

	/*!
	*	\brief On supprime de la BDD toutes les Edges connect�es o� source = Node identifi�.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*	\param idNode : id du Node dont on veut couper tous les Edges sortantes.
	*/
//	bool deleteAllEdgeFromNode (unsigned int const idNode) const;

	/*!
	*	\brief  On supprime de la BDD toutes les Edges connect�es o� target = Node identifi�.
	*	\return Renvoie un bool sur la r�ussite de l'op�ration.
	*	\param idNode : id du Node dont on veut couper tous les Edges entrantes.
	*/
//	bool deleteAllEdgeToNode (unsigned int const idNode) const;
