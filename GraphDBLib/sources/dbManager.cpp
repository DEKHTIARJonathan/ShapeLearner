/////* ************* Begin file dbManager.cpp ***************************************/
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
*	\file dbManager.cpp
*	\brief DatabaseManager source file. Interface between the software and database system.
*	It is the only access to the Database. The class is more or less specified for postgreSQL, some changes might be necessary in order to change the DBMS.
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"
#include "DBPool.h"

using namespace std;
using namespace odb::core;
using namespace graphDBLib;

/* *******************************************************************
*                     Escapers / Capitalizers                        *
 ********************************************************************/

void DatabaseManager::escape(string& str)
{
	/*
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (str[i] == "'") // If Equal
			str[i] = "''";
	}
	*/
}

void DatabaseManager::capitalize(string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::transform(str.begin(), str.begin()+1, str.begin(), ::toupper);
}

/* *******************************************************************
*                         Database Handling                          *
 ********************************************************************/

DBPool* DatabaseManager::dbPool (NULL);
string DatabaseManager::dbServerIP = "";
string DatabaseManager::dbServerPort = "";

void DatabaseManager::Interface::openDatabase(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort, const string& dbInit) throw(StandardExcept){
	if( dbPool == NULL ){
		Logger::Log("Opening Connection to the Database.", constants::LogDB);

		dbServerIP = dbHost;
		dbServerPort = to_string((_ULonglong)dbPort);

		dbPool = DBPool::accessor::getPool(dbUser, dbPass, dbName, dbHost, dbPort, dbInit);
	}
	else
		throw StandardExcept((string)__FUNCTION__, "Database already opened");
}

bool DatabaseManager::Interface::closeDatabase() throw(StandardExcept){
	Logger::Log("Closing Connection to the Database.", constants::LogDB);
	if( dbPool != NULL ){
		try{
			DBPool::accessor::delPool();
			return true;
		}
		catch (const std::exception& e)
		{
			Logger::Log("Database is not opened: " + string(e.what()), constants::LogError);
			return false;
		}
	}
	else
		throw StandardExcept((string)__FUNCTION__, "Database already closed");
}

bool DatabaseManager::Interface::closeThreadConnection() throw(StandardExcept){
	if( dbPool != NULL ){
		return dbPool->threadDisconnect();
	}
	else
		throw StandardExcept((string)__FUNCTION__, "Database not opened");
}

bool DatabaseManager::Interface::isDbOpen() {
	if(dbPool == NULL)
		return false;
	else
		return true;
}

pair<string,string> DatabaseManager::Interface::getServerInfos() throw(StandardExcept){
	return std::make_pair<string,string>(dbServerIP, dbServerPort);
}

unsigned long DatabaseManager::Interface::getPointCountInNode (const unsigned long idNode) throw (StandardExcept){
	for (unsigned short retry_count (0); ; retry_count++)
	{
		transaction t (dbPool->connect()->begin());
		try{
			typedef odb::query<pointsInNode> query;
			pointsInNode rslt (dbPool->connect()->query_value<pointsInNode> (query::refNode == idNode));
			t.commit ();

			return rslt.value;
		}
		catch (const odb::connection_lost& e)
		{
			t.rollback();
			if (retry_count > constants::MAX_DB_RETRY){
				throw StandardExcept ((string)__FUNCTION__, "DB Connection Failure : ConnectionLost, " + to_string((_Longlong)retry_count) + " reconnection attempts realized.\n" + (string)e.what());
				return EXIT_FAILURE;
			}
			else{
				Sleep(500);
				if(!dbPool->reconnect())
					throw StandardExcept ((string)__FUNCTION__, "Unable to reconnect to the DB" + (string)e.what());
				else{
					t.reset(dbPool->connect()->begin());
					continue;
				}
			}
		}
		catch (const odb::timeout& e){
			t.rollback();
			if (retry_count > constants::MAX_DB_RETRY){
				throw StandardExcept ((string)__FUNCTION__, "DB Connection Failure : TimeOut, " + to_string((_Longlong)retry_count) + " attempts realized.\n" + (string)e.what());
				return EXIT_FAILURE;
			}
			else{
				Sleep(500);
				if(!dbPool->reconnect())
					throw StandardExcept ((string)__FUNCTION__, "Unable to reconnect to the DB" + (string)e.what());
				else{
					t.reset(dbPool->connect()->begin());
					continue;
				}
			}
		}
		catch (const std::exception& e)
		{
			t.rollback();
			throw StandardExcept ((string)__FUNCTION__, "Unable to count Points in Node // Error = "+ (string)e.what());
			return 0;
		}
	}
}