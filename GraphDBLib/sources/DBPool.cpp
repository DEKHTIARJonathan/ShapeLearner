/* ************* Begin file DBPool.cpp ***************************************/
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
*	\file DBPool.cpp
*	\brief DBPool source file. Database Connection Object Pool - Thread Safe
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
using namespace odb::core;
using namespace graphDBLib;

DBPool* DBPool::_inst = NULL;
boost::mutex DBPool::mtxPool;

DBPool* DBPool::accessor::getPool(const string &dbUser, const string &dbPass, const string &dbName, const string &dbHost, const unsigned int &dbPort, const string& dbInit ){
	_Lock_Pool_
	if (_inst == NULL){
		_inst = new DBPool(dbUser, dbPass, dbName, dbHost, dbPort);
		_Unlock_Pool_
		if (dbInit.compare("") && !_inst->initDB(dbInit))
			throw StandardExcept((string)__FUNCTION__,"Erreur lors de l'initialisation de la BDD");
		return _inst;
	}
	else{
		_Unlock_Pool_
		return _inst;
	}
}

DBPool* DBPool::accessor::getPool(){
	_Lock_Pool_
	if (_inst != NULL){
		_Unlock_Pool_
		return _inst;
	}
	else{
		throw StandardExcept((string)__FUNCTION__,"Connection Parameters not given.");
		return NULL;
	}
}

bool DBPool::accessor::delPool(){
	if (_inst != NULL){
		delete _inst;
		_inst = NULL;
		return true;
	}else{
		throw StandardExcept((string)__FUNCTION__, "Database already closed");
		return false;
	}
}

DBPool::DBPool(const string &_dbUser, const string &_dbPass, const string &_dbName, const string &_dbHost, const unsigned int &_dbPort) :
dbUser(_dbUser),
dbPass(_dbPass),
dbName(_dbName),
dbHost(_dbHost),
dbPort(_dbPort)
{}

/* *******************************************************************
*                             DB I/O                                *
 ********************************************************************/

bool DBPool::initDB(const string& filename)
{
	try{
		string script = get_file_contents(filename);
		return query(script);
	}
	catch(const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);

		return EXIT_FAILURE;
	}
}

bool DBPool::query(const string &query) throw(StandardExcept)
{
	bool rslt = false;
	for (unsigned short retry_count (0); ; retry_count++)
	{
		transaction t (this->connect()->begin());
		try
		{
			if (this->connect()->execute(query) != 0 ) // == 0 Successful.
			{
				t.rollback();
				throw StandardExcept((string)__FUNCTION__, query);
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
				throw StandardExcept((string)__FUNCTION__, "Retry Limit exceeded" + (string)e.what());
			}
			else
				continue;
		}
	}
	return rslt; // Execution successful.
}

odb::pgsql::database* DBPool::connect() throw(StandardExcept){
	_Lock_Pool_
		if (_inst == NULL){
			_Unlock_Pool_
			throw StandardExcept((string)__FUNCTION__, "Connection not initiated");
		}
		else{
			odb::pgsql::database* rslt;
			unsigned long idThread = getThreadId();
			map<unsigned long, odb::pgsql::database*>::iterator it = connectionMAP.find(idThread);
			if (it != connectionMAP.end())
				rslt = it->second;
			else{
				odb::pgsql::database* db (new odb::pgsql::database (dbUser, dbPass, dbName, dbHost, dbPort));
				connectionMAP.insert(pair<unsigned long, odb::pgsql::database*> (idThread, db));
				db->tracer(appliTracer);
				rslt = db;
			}
			_Unlock_Pool_
			return rslt;
		}
}

bool DBPool::threadDisconnect() throw(StandardExcept){
	_Lock_Pool_
		if (_inst == NULL){
			_Unlock_Pool_
			throw StandardExcept((string)__FUNCTION__, "Connection not initiated");
		}
		else{
			unsigned long idThread = getThreadId();
			Logger::Log("Closing Connection to the Database for the thread : " + to_string((_ULonglong)idThread), constants::LogDB);
			bool rslt;

			map<unsigned long, odb::pgsql::database*>::iterator it = connectionMAP.find(idThread);
			if (it != connectionMAP.end()){
				odb::pgsql::database* db = it->second;
				delete db;
				connectionMAP.erase(it);
				rslt = true;
			}
			else
				rslt = false;
			_Unlock_Pool_
			return rslt;
		}
}

bool DBPool::reconnect() throw(StandardExcept){
	_Lock_Pool_
		if (_inst == NULL){
			_Unlock_Pool_
			throw StandardExcept((string)__FUNCTION__, "Connection not initiated");
		}
		else{
			unsigned long idThread = getThreadId();
			map<unsigned long, odb::pgsql::database*>::iterator it = connectionMAP.find(idThread);
			if (it != connectionMAP.end()){
				delete it->second;
				odb::pgsql::database* db (new odb::pgsql::database (dbUser, dbPass, dbName, dbHost, dbPort));
				db->tracer(appliTracer);
				it->second = db;
				_Unlock_Pool_
				return EXIT_SUCCESS;
			}
			else{
				_Unlock_Pool_
				throw StandardExcept((string)__FUNCTION__, "No existing connection to reconnect");
				return EXIT_FAILURE;
			}
		}
}
/* *******************************************************************
*                             Readers                                *
 ********************************************************************/

string DBPool::get_file_contents(const string& filename) throw(StandardExcept){
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
		throw StandardExcept((string)__FUNCTION__, error);
	}
}

/* *******************************************************************
*                          Thread Management                         *
 ********************************************************************/

const unsigned long DBPool::getThreadId(){
    std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
    unsigned long threadNumber = 0;
    sscanf(threadId.c_str(), "%lx", &threadNumber);
    return threadNumber;
}

DBPool::~DBPool(){
	for (map<unsigned long, odb::pgsql::database*>::iterator it = connectionMAP.begin(); it != connectionMAP.end();){
		_Lock_Pool_
			odb::pgsql::database* db = it->second;
			db->connection().release();
			delete db;
			connectionMAP.erase(it);
		_Unlock_Pool_
	}
}