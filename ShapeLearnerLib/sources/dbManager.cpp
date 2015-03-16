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
*	\brief Interface entre le logiciel et la base de données. Elle est la seule à réaliser des accès à la BDD
*	\version 1.1
*	\author DEKHTIAR Jonathan
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
			#ifdef _TRACER_
				t.tracer (stderr_tracer);
			#endif

			if (database->execute(query) != 0 ) // == 0 Successful.
			{
				t.rollback();
				throw ShapeLearnerExcept("DatabaseManager::query", query);
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
				throw ShapeLearnerExcept("DatabaseManager::query", "Retry Limit exceeded" + (string)e.what());
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
	/*
	odb::transaction t (database->begin ());
	#ifdef _TRACER_
		t.tracer (stderr_tracer);
	#endif
	odb::schema_catalog::create_schema (*database);
	t.commit ();
	return true;
	*/
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
		throw ShapeLearnerExcept("DatabaseManager::get_file_contents", error);
	}
	
}

/* *******************************************************************
*                         Database Handling                          *
 ********************************************************************/

odb::database* DatabaseManager::database = NULL;

void DatabaseManager::Interface::openDatabase(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort , const unsigned int& dbType, const string& dbInit) throw(ShapeLearnerExcept){
	if( database == NULL ){

		switch(dbType)
		{
			case constants::DB_MYSQL :
				database = new odb::pgsql::database (dbUser, dbPass, dbName, dbHost, dbPort);
				break;
			case constants::DB_PGSQL :
				database = new odb::pgsql::database (dbUser, dbPass, dbName, dbHost, dbPort);
				break;
			case constants::DB_ORACLE :
				database = new odb::pgsql::database (dbUser, dbPass, dbName, dbHost, dbPort);
				break;
		}
	
		if (dbInit.compare("") && !initDB(dbInit))
			throw ShapeLearnerExcept("DatabaseManager::DatabaseManager","Erreur lors de l'initialisation de la BDD");
	}
	else
		throw ShapeLearnerExcept("DatabaseManager::Interface::openDatabase", "Database already opened");
}

void DatabaseManager::Interface::closeDatabase() throw(ShapeLearnerExcept){
	if( database != NULL ){
		delete database;
		database = NULL;
	}
	else
		throw ShapeLearnerExcept("DatabaseManager::Interface::disconnectDB", "Database already closed");
}

bool DatabaseManager::Interface::isDbOpen() { 
	if(database == NULL)
		return false;
	else
		return true;
}

void DatabaseManager::Interface::test () throw (ShapeLearnerExcept){
	
	
}

int DatabaseManager::Interface::getPointCountInNode (const int idNode) throw (ShapeLearnerExcept){
	typedef odb::query<pointsInNode> query;

	transaction t (database->begin());

	#ifdef _TRACER_
		t.tracer (stderr_tracer);
	#endif

	pointsInNode rslt (database->query_value<pointsInNode> (query::refNode == idNode));

	t.commit ();

	return rslt.value;
}