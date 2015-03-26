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
*	\file shapeLearner.cpp
*	\brief Interface publique d'accès au logiciel.
*	\version 1.1
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

void ShapeLearner::getDbCredentials(const bool dbInit) throw(ShapeLearnerExcept) {
	GraphManager::UserInterface::getDbCredentials(dbInit);
}

void ShapeLearner::openDatabase() throw(ShapeLearnerExcept) {
	GraphManager::UserInterface::openDatabase();
}

void ShapeLearner::closeDatabase() throw(ShapeLearnerExcept) {
	GraphManager::UserInterface::closeDatabase();
}