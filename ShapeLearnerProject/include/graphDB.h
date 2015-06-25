/* ************* Begin file GraphDB.h ***************************************/
/*
** 2015 February 23
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file GraphDB.h
*	\brief GraphDB header file. Central point in the software. It realizes the task distribution to the different instances of the software and make them communicate without knowing of each other.
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _GRAPH_DATABASE_H_
#define _GRAPH_DATABASE_H_

#pragma message("Compiling GraphDBUser::GraphDB.h - this should happen just once per project.\n")

#include "CLogger.h"
#include "constants.h"
#include "StandardException.h"

using namespace std;
class StandardExcept; //Forward Declaration of the class contained in graphDBException.h

namespace graphDBLib{

	/*!	
	*	\class GraphDB
	*	\brief Static class, the central point of the whole architecture. It redistributes actions to the different actors.
	*	The different actors doesn't need to know about each others. That way, a new handler can be added without any change whatsoever.
	*	It implements different interfaces to input some actions. It allows to control the granularity access of "Who can access what".
	*/
	class GraphDB
	{
		public:

			/*!
			*	\fn static void openDatabase(const string& _dbUser, const string& _dbPass, const string& _dbName, const string& _dbHost, const unsigned int& _dbPort, const string& _dbInit = "") throw(StandardExcept);
			*	\brief Open the connection to the PostgreSQL database.
			*	\param _dbUser : The username used to connect to the DB.
			*	\param _dbPass : The password associated with the username.
			*	\param _dbName : The database's name.
			*	\param _dbHost : The DB's server's hostname or IP address.
			*	\param _dbPort : The DB's server's listening port.
			*	\param _dbInit : A relative path to the DB Init File, this field is not necessary. If empty => DB not initialized and assume it has been already created.
			*/
			static void openDatabase(const string& _dbUser, const string& _dbPass, const string& _dbName, const string& _dbHost, const unsigned int& _dbPort, const string& _dbInit = "") throw(StandardExcept);
			
			/*!
			*	\fn static void closeDatabase() throw(StandardExcept);
			*	\brief Close the connection to the PostgreSQL database.
			*/
			static bool closeDatabase() throw(StandardExcept);
	};

}



#endif //_GRAPH_DATABASE_H_