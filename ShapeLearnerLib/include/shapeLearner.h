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

using namespace std;

class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h

class ShapeLearner
{
	public:
		// Setters pour les paramètres de la BDD.
		static void getDbCredentials(const bool dbInit = false) throw(ShapeLearnerExcept);
		static void openDatabase() throw(ShapeLearnerExcept);
		static void closeDatabase() throw(ShapeLearnerExcept);
};


#endif //_SHAPE_LEARNER_H_