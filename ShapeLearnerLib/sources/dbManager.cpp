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

#include "allHeaders.h"

using namespace std;
using namespace odb::core;

/* *******************************************************************
*                            DB Requests                             *
 ********************************************************************/

bool DatabaseManager::query(const string &query) throw(ShapeLearnerExcept)
{
	bool rslt = false;
	for (unsigned short retry_count (0); ; retry_count++)
	{
		transaction t (database->begin ());
		try
		{
			if (database->execute(query) != 0 ) // == 0 Successful.
			{
				t.rollback();
				throw ShapeLearnerExcept((string)__FUNCTION__, query);
				break;
			}
			else
			{
				t.commit();
				rslt = true;
				break;
			}
		}
		catch (const odb::recoverable& e)
		{
			if (retry_count > constants::MAX_DB_RETRY){
				t.rollback();
				throw ShapeLearnerExcept((string)__FUNCTION__, "Retry Limit exceeded" + (string)e.what());
			}
			else
				continue;
		}
	}
	return rslt; // Execution successful.
}

bool DatabaseManager::initDB(const string& filename)
{
	string script = get_file_contents(filename);
	return query(script);
}

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
*                             Readers                                *
 ********************************************************************/

string DatabaseManager::get_file_contents(const string& filename) throw(ShapeLearnerExcept){
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	else{
		unsigned int maxSizeError = 1500;
		char error[1500];
		strerror_s(error, maxSizeError, errno);
		throw ShapeLearnerExcept((string)__FUNCTION__, error);
	}
}

/* *******************************************************************
*                         Database Handling                          *
 ********************************************************************/

odb::pgsql::database* DatabaseManager::database = NULL;
AppTracer DatabaseManager::appliTracer;

void DatabaseManager::Interface::openDatabase(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort, const string& dbInit) throw(ShapeLearnerExcept){
	if( database == NULL ){
		Logger::Log("Opening Connection to the Database.", constants::LogExec);
		database = new odb::pgsql::database (dbUser, dbPass, dbName, dbHost, dbPort);
		database->tracer(appliTracer);
		
		if (dbInit.compare("") && !initDB(dbInit))
			throw ShapeLearnerExcept((string)__FUNCTION__,"Erreur lors de l'initialisation de la BDD");
	}
	else
		throw ShapeLearnerExcept((string)__FUNCTION__, "Database already opened");
}

void DatabaseManager::Interface::closeDatabase() throw(ShapeLearnerExcept){
	Logger::Log("Closing Connection to the Database.", constants::LogExec);
	if( database != NULL ){
		delete database;
		database = NULL;
	}
	else
		throw ShapeLearnerExcept((string)__FUNCTION__, "Database already closed");
}

bool DatabaseManager::Interface::isDbOpen() {
	if(database == NULL)
		return false;
	else
		return true;
}

unsigned long DatabaseManager::Interface::getPointCountInNode (const unsigned long idNode) throw (ShapeLearnerExcept){
	transaction t (database->begin());
	try{
		typedef odb::query<pointsInNode> query;

		pointsInNode rslt (database->query_value<pointsInNode> (query::refNode == idNode));

		t.commit ();

		return rslt.value;
	}
	catch (const std::exception& e)
	{
		t.rollback();
		throw ShapeLearnerExcept ((string)__FUNCTION__, "Unable to count Points in Node // Error = "+ boost::lexical_cast<std::string>(e.what()));
		return 0;
	}
}