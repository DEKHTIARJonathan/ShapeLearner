/* ************* Begin file ShapeLearner.h ***************************************/
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
*	\file ShapeLearner.h
*	\brief ShapeLearner header file. Public Interface and Central point in the software. It realizes the task distribution to the different instances of the software and make them communicate without knowing of each other.
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _SHAPE_LEARNER_H_
#define _SHAPE_LEARNER_H_

#ifdef _MSC_VER
	#pragma message("Compiling ShapeLearnerLib::ShapeLearner.h  - this should happen just once per project.\n")
#endif
 
#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
using namespace boost::threadpool;

class ShapeLearnerExcept; //Forward Declaration of the class contained in ShapeLearnerException.h

/*!	
*	\class ShapeLearner
*	\brief Static class, the central point of the whole architecture. It redistributes actions to the different actors.
*	The different actors doesn't need to know about each others. That way, a new handler can be added without any change whatsoever.
*	It implements different interfaces to input some actions. It allows to control the granularity access of "Who can access what".
*/
class ShapeLearner
{
	public:

		static void createShockGraph (const vector<const string> &imgVect) throw(ShapeLearnerExcept);

	private:

		static boost::threadpool::pool	Pool;


		/* ************** Multi Threading Workers ***************/

		static bool createShockGraphWorker (const string& imgPath) throw(ShapeLearnerExcept);
		
		/* **************  No instanciation *********************/

		/*!
		*	\brief Constructeur de le class ShapeLearner.  Il instancie également la classe DatabaseManager.
		*/
		ShapeLearner() throw(ShapeLearnerExcept);

		/*!
		*	\brief Constructeur de recopie => La recopie est interdite
		*/
		ShapeLearner(const ShapeLearner&);

		/*!
		*	\brief Opérateur =  =>> La recopie est interdite
		*/
		ShapeLearner& operator=(const ShapeLearner&);

		/*!
		*	\brief La destruction est interdite
		*/
		~ShapeLearner();
};

#endif //_SHAPE_LEARNER_H_