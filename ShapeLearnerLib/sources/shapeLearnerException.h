/* ************* Begin file shapeLearnerException.h ***************************************/
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
*	\file shapeLearnerException.h
*	\brief Exception class used by the whole library.
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _SHAPE_LEARNER_EXCEPTION_
#define _SHAPE_LEARNER_EXCEPTION_

#include "stdafx.h"

#ifdef _MSC_VER
	#pragma message("Compiling ShapeLearnerLib::shapeLearnerException.h  - this should happen just once per project.\n")
#endif

using namespace std;

/*!	
*	\class ShapeLearnerExcept
*	\brief The only exception class in the project. It has been developped in order to display as much info as possible.
*	It derives publicly from the standard exception class : std::exception.
*/
class ShapeLearnerExcept : public std::exception
{
	public:
		// ================== Constructor ========================
		/*!
		*	\fn ShapeLearnerExcept(const string &procedure, const string &msgError ) throw();
		*	\brief Public constructor, formats and save the exception's message in the attribut m_what.
		*	\param procedure : Name of the calling function.
		*	\param msgError : Message explaining the error.
		*/
		ShapeLearnerExcept(const string &procedure, const string &msgError ) throw();

		// ================== Implementation ========================

		/*!
		*	\fn virtual const char*	what() const throw();
		*	\brief Virtual function, format and return the error as a unique string.
		*/
		virtual const char*	what() const throw();

		// ================== Implementation ========================

		/*!
		*	\fn virtual ~ShapeLearnerExcept() throw();
		*	\brief virtual destructor.
		*/
		virtual ~ShapeLearnerExcept() throw();

	private:
		/*!
		*	The variable containing the name of the procedure.
		*/
		string			m_procedure;
		
		/*!
		*	The variable containing the error's message.
		*/
		string			m_msgError;

		/*!
		*	Exception message well formated.
		*/
		string			m_what;
};

#endif //_SHAPE_LEARNER_EXCEPTION_
