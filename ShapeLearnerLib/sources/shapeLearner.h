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

using namespace std;

class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h
class GraphManager;

class ShapeLearner
{
	public:
		// Setters pour les paramètres de la BDD.
		static void getDbCredentials(const bool dbInit = false) throw(ShapeLearnerExcept);
		static void openDatabase() throw(ShapeLearnerExcept);
		static void closeDatabase() throw(ShapeLearnerExcept);
		static void test () throw(ShapeLearnerExcept);
	
	private:

		/* **************  No instanciation *********************/

		/**
		* \brief Constructeur de le class GraphManager.  Il instancie également la classe DatabaseManager.
		*/
		ShapeLearner() throw(ShapeLearnerExcept);

		/**
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		ShapeLearner(const ShapeLearner&);

		/**
		*  \brief Opérateur =  =>> La recopie est interdite
		*/
		ShapeLearner& operator=(const ShapeLearner&);

		/**
		*  \brief La destruction est interdite
		*/
		~ShapeLearner();
};


#endif //_SHAPE_LEARNER_H_