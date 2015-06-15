/* ************* Begin file DBPool.h ***************************************/
/*
** 2015 May 15
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/*!
*	\file DBPool.h
*	\brief DBPool header file. Database Connection Object Pool - Thread Safe
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/
#ifndef _DBPool_H_
#define _DBPool_H_

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::DBPool.h  - this should happen just once per project.\n")
#endif

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
using namespace odb::core;

#define _Lock_Pool_ graphDBLib::DBPool::mtxPool.lock();
#define _Unlock_Pool_ graphDBLib::DBPool::mtxPool.unlock();

class StandardExcept; //Forward Declaration of the class contained in StandardException.h

namespace graphDBLib {
	class DatabaseManager; //Forward Declaration of the class contained in dbManager.h

	class DBPool{
	public:
		class accessor{
			friend DatabaseManager;
			static DBPool* getPool(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort, const string& dbInit = "");
		};

	/*!
	*	\fn odb::pgsql::database* connect() throw(StandardExcept);
	*	\brief Get the thread specific DB Connection
	*/
	odb::pgsql::database* connect() throw(StandardExcept);

	bool reconnect() throw(StandardExcept);

	/*!
	*	\fn static bool query(const string& query) throw(StandardExcept);
	*	\brief Static Method executing a query given in argument.
	*	\return A boolean on the success of the query's execution.
	*	\param query : The query we want to execute.
	*/
	bool query(const string& query) throw(StandardExcept);

	bool closeConnections() {return true;}

	private:
		AppTracer appliTracer;
		const string dbUser;
		const string dbPass;
		const string dbName;
		const string dbHost;
		const unsigned int dbPort;
		static boost::mutex mtxPool;
		static DBPool* _inst;

		map<unsigned long, odb::pgsql::database*> connectionMAP;

		/* ****************** DB I/O **********************/

		/*!
		*	\fn static bool initDB(const string& filename);
		*	\brief  Static method to initialize the database structures and create the functions we need. If the tables already exists, the scripts deleting them first.
		*	\param filename : Relative path to the SQL file to initialize the DB.
		*/
		bool initDB(const string& filename);

		/* ****************** File Reader **********************/

		/*!
		*	\fn static string get_file_contents(const string& filename) throw(StandardExcept);
		*	\brief Static method to read a file and returns its content as a string.
		*	\param filename : The file we want to read.
		*/
		static string get_file_contents(const string& filename) throw(StandardExcept);

		/* ***************** Thread Management *****************/

		const unsigned long getThreadId();

		/* ****************** ObjectPool Management **********************/

		/*!
		*	\fn DBPool(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort);
		*	\brief Private constructor, forbid instanciation.
		*/
		DBPool(const string &_dbUser, const string &_dbPass, const string &_dbName, const string &_dbHost, const unsigned int &_dbPort);

		/*!
		*	\fn DBPool(const DBPool&);
		*	\brief Private constructor, forbid instanciation by copy.
		*/
		DBPool(const DBPool&);

		/*!
		*	\fn DBPool(const DBPool&);
		*	\brief Copy forbidden
		*/
		DBPool& operator=(const DBPool&);

		/*!
		*	\fn ~DBConneDBPoolction();
		*	\brief Private Destructor, forbid destruction.
		*/
		~DBPool();
	};
}

#endif //_DBPool_H_
