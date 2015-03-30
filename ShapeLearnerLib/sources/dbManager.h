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

/*!
*	\file dbManager.h
*	\brief DatabaseManager header file. Interface between the software and database system.
*	It is the only access to the Database. The class is more or less specified for postgreSQL, some changes might be necessary in order to change the DBMS.
*	\version 1.1
*	\author DEKHTIAR Jonathan
*/
#ifndef DatabaseManager_H
#define DatabaseManager_H

#include "allHeaders.h"
using namespace std;
using namespace odb::core;

class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h
class ShapeLearner; //Forward Declaration of the class contained in shapeLearner.h
class ObjectClass; //Forward Declaration of the class contained in ObjectClass.h
class GraphClass; //Forward Declaration of the class contained in GraphClass.h
class Graph; //Forward Declaration of the class contained in Graph.h
class Node; //Forward Declaration of the class contained in Node.h
class Point; //Forward Declaration of the class contained in Point.h
class Edge; //Forward Declaration of the class contained in Edge.h


/*!	
*	\class DatabaseManager
*	\brief Static class, the only interface to the PostgreSQL database.
*	It uses ODB Library in order to work the most independently as possible from the database system.
*	ODB : http://www.codesynthesis.com/products/odb/
*/
class DatabaseManager
{
	public:

		/* **************************************** Access Restricted to Graph Manager *************************** */
		/*!	
		*	\class DatabaseManager::Interface
		*	\brief Embedded Static class of DatabaseManager.
		*	It allows to manage granularity access to certain methods by the use of friendship.
		*	Only ShapeLearner and its subclasses can access this Interface.
		*/
		class Interface{
			friend class ShapeLearner;
			private :

				/* *************** DB Handlers ***********************/

				/*!
				*	\fn static void openDatabase(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int& dbPort, const string& dbInit = "") throw(ShapeLearnerExcept);
				*	\brief Static Method establishing the connection to the DB.
				*	\param dbUser : Username used to connect to the DB
				*	\param dbPass : Password associated with the username
				*	\param dbName : DB's name
				*	\param dbHost : DB's server address.
				*	\param dbPort : DB's server listening port.
				*	\param dbInit : Path to the DB init file. SQL File of the DB Structure. If this parameter is empty, then DB is not initialized, assuming it has already been done.
				*/
				static void openDatabase(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int& dbPort, const string& dbInit = "") throw(ShapeLearnerExcept);

				/*!
				*	\fn static void	closeDatabase() throw(ShapeLearnerExcept);
				*	\brief Static Method closing the connection to the DB and deleting the pointer to the DB.
				*/
				static void	closeDatabase() throw(ShapeLearnerExcept);

				/*!
				*	\fn static bool isDbOpen();
				*	\brief Static Method returning true if the DatabaseConnection is opened and false if it's not.
				*/
				static bool isDbOpen();

				/* *************** Updaters ***********************/
				/*!
				*	\fn template <class T> static bool updateObject(T& obj) throw (ShapeLearnerExcept);
				*	\brief Template of a static method returning true if the DBobject given in argument could have been updated in DB. Else it returns false.
				*/
				template <class T> 
				static bool updateObject(T& obj) throw (ShapeLearnerExcept){
					transaction t1 (database->begin ());
					try
					{
						database->update (obj);
						t1.commit ();
						return true;
					}
					catch (const std::exception& e)
					{
						t1.rollback();
						transaction t2 (database->begin ());
						database->load (obj.getKey(), obj);
						t2.commit ();
						throw ShapeLearnerExcept ((string)__FUNCTION__, "Unable to update object of class : "+ obj.getClassName() +". Restoring last saved state. // Error = " + e.what());
						return false;
					}
					
				}

				/*!
				*	\fn template <class T, class Y> static boost::shared_ptr<T> loadObject(Y keyDB) throw (ShapeLearnerExcept);
				*	\brief Template of a static method returning a shared_ptr to the DBObject just loaded. It only treats objects with primary key as string.
				*	\param keyDB The object's key of the record we want to retrieve.
				*/
				template <class T, class Y>
				static boost::shared_ptr<T> loadObject(Y keyDB) throw (ShapeLearnerExcept){
					transaction t (database->begin ());
					try{
						boost::shared_ptr<T> rslt (database->load<T>(keyDB));
						t.commit ();

						return rslt;
					}
					catch(...){
						t.rollback();
						return boost::shared_ptr<T>();
					}
				}
				
				/*!
				*	\fn template <class T, class Y> static vector<unsigned long> getForeignRelations(Y foreignKey) throw (ShapeLearnerExcept);
				*	\brief Template of a static method returning a vector of keys in the DB. This gives all the objects of a certain type which have a certain foreign key.
				*	Exemple : All the Nodes contained in a given graph.
				*	\param foreignKey The foreign key we are looking for.
				*/
				template <class T, class Y> 
				static vector<unsigned long> getForeignRelations(Y foreignKey) throw (ShapeLearnerExcept){
					transaction t (database->begin());
					try {
						typedef odb::query<T> query;
						typedef odb::result<T> result;

						result r (database->query<T> ("'"+boost::lexical_cast<std::string>(foreignKey)+"'"));

						vector<unsigned long> rslt;

						for (result::iterator i (r.begin ()); i != r.end (); ++i)
							rslt.push_back(i->id);

						t.commit ();

						return rslt;
					}
					catch(const std::exception& e){
						t.rollback();
						throw ShapeLearnerExcept((string)__FUNCTION__, "Unable to perform operation // Error = "+ boost::lexical_cast<std::string>(e.what()));
						return vector<unsigned long> ();
					}
					
				}
				
				/* *************** Savers ***********************/

				/*!
				*	\fn template<class T> static unsigned long saveObject(T& obj) throw(ShapeLearnerExcept);
				*	\brief Template of a static method returning the key of a new object we'd like to insert in the DB.
				*	\param obj The object we'd like to insert in the DB.
				*/
				template<class T> 
				static unsigned long saveObject(T& obj) throw(ShapeLearnerExcept){
					transaction t (database->begin());
					try{	
						unsigned long rslt = database->persist (obj);
											
						t.commit ();

						return rslt;
					}
					catch (const std::exception& e){
						t.rollback();
						throw ShapeLearnerExcept((string)__FUNCTION__ + " // Key : unsigned Long", "Unable to save object of class : "+obj.getClassName() + " // Error : "+ e.what());
						return 0;
					}

					
				}
				
				/*!
				*	\fn static string saveObject(ObjectClass& obj) throw(ShapeLearnerExcept);
				*	\brief Static method returning the key of a new ObjectClass we'd like to insert in the DB.
				*	\param obj The object we'd like to insert in the DB.
				*/
				static string saveObject(ObjectClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}
				
				/*!
				*	\fn static string saveObject(GraphClass& obj) throw(ShapeLearnerExcept);
				*	\brief Static method returning the key of a new GraphClass we'd like to insert in the DB.
				*	\param obj The object we'd like to insert in the DB.
				*/
				static string saveObject(GraphClass& obj) throw(ShapeLearnerExcept) {return saveObjectString(obj);}

				/* *************** Deleters ***********************/

				/*!
				*	\fn template <class T> static bool deleteObject(boost::shared_ptr<T> obj) throw (ShapeLearnerExcept);
				*	\brief Template of a static method returning a boolean on the deletion of a given object in the DB.
				*	\param obj The object we'd like to delete in the DB.
				*/
				template <class T> 
				static bool deleteObject(boost::shared_ptr<T> obj) throw (ShapeLearnerExcept){
					transaction t (database->begin ());
					try
					{
						T* ptr = obj.get();
						database->erase (ptr);
						t.commit ();
						return true;
					}
					catch (const std::exception& e)
					{
						t.rollback();
						throw ShapeLearnerExcept ((string)__FUNCTION__, "Unable to delete object of class : "+ obj->getClassName() +". // Error = "+ e.what());
						return false;
					}	
				}

				/* *************** Query DB **********************/
				/*!
				*	\fn static unsigned long getPointCountInNode (const unsigned long idNode) throw (ShapeLearnerExcept);
				*	\brief Function returning the number of points contained inside a Node.
				*	\param obj The Node we'd like to lookup to.
				*/
				static unsigned long getPointCountInNode (const unsigned long idNode) throw (ShapeLearnerExcept);
		};

	private:

		/* ****************** Attributs ********************/
		/*!
		*	Pointer to the PostgreSQL database.
		*/
		static odb::pgsql::database* database;

		/*!
		*	Tracer for the Database. Only working with postgreSQL databases. If you want to use it for MySQL / Oracle / ..., you need to create a similar class for this specific DB System.
		*/
		static AppTracer appliTracer;

		/* **************** DB Requests ********************/

		/*!
		*	\fn static bool query(const string& query) throw(ShapeLearnerExcept);
		*	\brief Static Method executing a query given in argument.
		*	\return A boolean on the success of the query's execution.
		*	\param query : The query we want to execute.
		*/
		static bool query(const string& query) throw(ShapeLearnerExcept);

		/* ******************** Escaper ********************/
		/*!
		*	\fn static void escape(string& str);
		*	\brief Static method to escape a query in order to prevent SQL injections.
		*	\param s : The query we want to escape.
		*/
		static void escape(string& str);

		/*!
		*	\fn static void capitalize(string& str);
		*	\brief Static method to return a string with the first letter in capital letter and all the others in lowercase : "Example"
		*	\param str : The string we want to capitalize
		*/
		static void capitalize(string& str);

		/* **************** DB Handlers ***********************/

		/*!
		*	\fn static bool initDB(const string& filename);
		*	\brief  Static method to initialize the database structures and create the functions we need. If the tables already exists, the scripts deleting them first.
		*	\param filename : Relative path to the SQL file to initialize the DB.
		*/
		static bool initDB(const string& filename);

		/* **************** Savers *************************/

		/*!
		*	\fn template<class T> static string saveObjectString(T& obj) throw(ShapeLearnerExcept);
		*	\brief  Static method to insert an object in the DB which have a primary key as string.
		*	\param obj : The object we want to insert in the DB.
		*/
		template<class T>
		static string saveObjectString(T& obj) throw(ShapeLearnerExcept){
			transaction t (database->begin());
			try{
				string rslt = database->persist(obj);
				t.commit ();

				return rslt;
			}
			catch (const std::exception& e){
				t.rollback();
				throw ShapeLearnerExcept((string)__FUNCTION__, "Unable to save object : "+obj.getKey()+" // Class : "+obj.getClassName() + "// Error : "+ e.what());
				return "";
			}
			
		}

		/* ****************** Readers **********************/

		/*!
		*	\fn static string get_file_contents(const string& filename) throw(ShapeLearnerExcept);
		*	\brief Static method to read a file and returns its content as a string.
		*	\param filename : The file we want to read.
		*/
		static string get_file_contents(const string& filename) throw(ShapeLearnerExcept);

		/* **************  No instanciation *********************/

		/*!
		*	\fn DatabaseManager();
		*	\brief Private constructor, forbid instanciation.
		*/
		DatabaseManager();

		/*!
		*	\fn DatabaseManager(const DatabaseManager&);
		*	\brief Private constructor, forbid instanciation by copy.
		*/
		DatabaseManager(const DatabaseManager&);

		/*!
		*	\fn DatabaseManager(const DatabaseManager&);
		*	\brief Copy forbidden
		*/
		DatabaseManager& operator=(const DatabaseManager&);

		/*!
		*	\fn ~DatabaseManager();
		*	\brief Private Destructor, forbid destruction.
		*/
		~DatabaseManager();
};

#endif // DatabaseManager_H