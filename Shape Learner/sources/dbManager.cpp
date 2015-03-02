/* ************* Begin file dbManager.cpp ***************************************/
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
**                           Constructers                            *
*********************************************************************/

DatabaseManager::DatabaseManager(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort , const unsigned int& dbType, const bool &dbInit) throw(DBException) : dbType(dbType)
{
	switch(dbType)
	{
		case constants::DB_MYSQL :
			database = std::unique_ptr<odb::database>(new odb::pgsql::database (dbUser, dbPass, dbName, dbHost, dbPort));
			break;
		case constants::DB_PGSQL :
			database = std::unique_ptr<odb::database>(new odb::pgsql::database (dbUser, dbPass, dbName, dbHost, dbPort));
			break;
		case constants::DB_ORACLE :
			database = std::unique_ptr<odb::database>(new odb::pgsql::database (dbUser, dbPass, dbName, dbHost, dbPort));
			break;
	}
	
	if (dbInit && !initDB("sources/structure.sql"))
		throw DBException("DatabaseManager::DatabaseManager","Erreur lors de l'initialisation de la BDD");
}

/* *******************************************************************
*                              getters                               *
 ********************************************************************/

/* *******************************************************************
*                            DB Requests                             *
 ********************************************************************/

bool DatabaseManager::query(const string &query) const throw(DBException)
{
	for (unsigned short retry_count (0); ; retry_count++)
	{
		try
		{
			transaction t (database->begin ());
			#ifdef _DEBUG
				t.tracer (stderr_tracer);
			#endif

			if (database->execute(query) != 0 ) // == 0 Successful.
			{
				t.rollback();
				throw DBException("DatabaseManager::query", query);
				return false; // Execution failed.
			}
			else
			{
				t.commit();
				return true; // Execution successful.
			}
			break;
		}
		catch (const odb::recoverable& e)
		{
			if (retry_count > constants::MAX_DB_RETRY)
				throw DBException("DatabaseManager::query", "Retry Limit exceeded" + (string)e.what());
			else
				continue;
		}
	}
}


bool DatabaseManager::initDB(const string& filename)
{
	string script = get_file_contents(filename);
	return query(script);
}

/*
QSqlError DatabaseManager::lastError()
{
	// If opening database has failed user can ask 
	// error description by QSqlError::text()
	return database->lastError();
	
}
*/

/*
unsigned int DatabaseManager::getLastID() const throw(DBException)
{

	
	string sql = "SELECT last_insert_rowid()";
	if(!query.exec(sql))
		//throw DBException(sql, query.lastError().databaseText());

	query.next();// Only one result no need of the while loop

	return query.value(0).toUInt();
	
	return 0;
}
*/

/* *******************************************************************
*                              Deleters                              *
 ********************************************************************/

// bool DatabaseManager::flushDB () const {return true;}

/* *******************************************************************
*                     Escapers / Capitalizers                        *
 ********************************************************************/

void DatabaseManager::escape(string& str) const
{
	/*
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (str[i] == "'") // If Equal
			str[i] = "''";
	}
	*/

}

void DatabaseManager::capitalize(string& str) const
{

	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::transform(str.begin(), str.begin()+1, str.begin(), ::toupper);

}

/* *******************************************************************
*                            Inserters                               *
 ********************************************************************/


/* *******************************************************************
*                             Updaters                               *
 ********************************************************************/

/* *******************************************************************
*                            Retrievers                              *
 ********************************************************************/

/* *******************************************************************
*                             Fillers                                *
 ********************************************************************/

/* *******************************************************************
*                             Readers                                *
 ********************************************************************/


string DatabaseManager::get_file_contents(const string& filename) throw(DBException){
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
		throw DBException("DatabaseManager::get_file_contents", error);
	}
	
}


/* *******************************************************************
*                            Destructor                              *
 ********************************************************************/

DatabaseManager::~DatabaseManager()
{
	database.release();
	database = NULL;
}

/* *******************************************************************
*                            Singleton                               *
 ********************************************************************/

DatabaseManager* DatabaseManager::s_inst = NULL;

DatabaseManager& DatabaseManager::Key::getInstance(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort , const unsigned int& dbType, const bool &dbInit){
	if( DatabaseManager::s_inst == NULL )
		DatabaseManager::s_inst =  new DatabaseManager(dbUser, dbPass, dbName, dbHost, dbPort, dbType, dbInit);
	return (*DatabaseManager::s_inst);
}

void DatabaseManager::Key::destroy()
{
	if( DatabaseManager::s_inst != NULL ){
		delete DatabaseManager::s_inst;
		DatabaseManager::s_inst = NULL;
	}
}