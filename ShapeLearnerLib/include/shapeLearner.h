/* ************* Begin file shapeLearner.h ***************************************/
/*
** 2015 March 06
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file shapeLearner.h
*	\brief Interface publique d'accès au logiciel.
*	\version 1.1
*	\author DEKHTIAR Jonathan
*/

#ifndef _SHAPE_LEARNER_H_
#define _SHAPE_LEARNER_H_

#include "CLogger.h"
#include "constants.h"
#include "shapeLearnerException.h"

using namespace std;

class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h

class ShapeLearner
{
	public:


		/*!
		*	\fn static void openDatabase(const string& _dbUser, const string& _dbPass, const string& _dbName, const string& _dbHost, const unsigned int& _dbPort, const string& _dbInit = "") throw(ShapeLearnerExcept);
		*	\brief Open the connection to the PostgreSQL database.
		*	\brief Static Method setting all the DB Connection's credentials.
		*	\param _dbUser : The username used to connect to the DB.
		*	\param _dbPass : The password associated with the username.
		*	\param _dbName : The database's name.
		*	\param _dbHost : The DB's server's hostname or IP address.
		*	\param _dbPort : The DB's server's listening port.
		*	\param _dbInit : A relative path to the DB Init File, this field is not necessary. If empty => DB not initialized and assume it has been already created.
		*/
		static void openDatabase(const string& _dbUser = "postgres", const string& _dbPass = "postgres", const string& _dbName = "postgres", const string& _dbHost = "localhost", const unsigned int& _dbPort = 5432, const string& _dbInit = "") throw(ShapeLearnerExcept);
		
		/*!
		*	\fn static void closeDatabase() throw(ShapeLearnerExcept);
		*	\brief Close the connection to the PostgreSQL database.
		*/
		static void closeDatabase() throw(ShapeLearnerExcept);
};


#endif //_SHAPE_LEARNER_H_