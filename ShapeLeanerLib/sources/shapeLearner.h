/* ************* Begin file graphManager.h ***************************************/
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
*	\file graphManager.h
*	\brief Interface publique d'accès au logiciel. Elle concentre les accès à la BDD, le Génarateur de Graph et le Graph Matcher.
*	\version 1.1
*	\author DEKHTIAR Jonathan
*/

#ifndef _SHAPE_LEARNER_
#define _SHAPE_LEARNER_

#include "allHeaders.h"
using namespace std;

class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h
class GraphManager; //Forward Declaration of the class contained in dbManager.h

class ShapeLearner
{
	// Setters pour les paramètres de la BDD.
	static void getDbCredentials() throw(ShapeLearnerExcept);

private:
	ShapeLearner(void);
	/*!
	*	\brief Constructeur de recopie => La recopie est interdite
	*/
	ShapeLearner(const ShapeLearner& inst);

	/*!
	*  \brief Opérateur =  =>> La recopie est interdite
	*/
	ShapeLearner& operator=(const ShapeLearner& inst);

	/*!
	*  \brief La destruction est interdite
	*/
	~ShapeLearner();
};

#endif //_SHAPE_LEARNER_