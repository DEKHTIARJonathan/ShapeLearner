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
*	\author DEKHTIAR Jonathan
*/

#ifndef _SHAPE_LEARNER_EXCEPTION_
#define _SHAPE_LEARNER_EXCEPTION_

#pragma message("Compiling ShapeLearnerUser::shapeLearnerException.h - this should happen just once per project.\n")

#include <string>
#include <iostream>
#include <exception>

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
		*	\brief Public constructor
		*	\param procedure : Name of the calling function.
		*	\param msgError : Message explaining the error.
		*/
		ShapeLearnerExcept(const string &procedure, const string &msgError ) throw();

		// ================== Getters ========================

		/*!
		*	\fn const string &	getProcedure() const throw();
		*	\brief Getter for the attribut : m_procedure;
		*/
		const string &	getProcedure() const throw();

		/*!
		*	\fn const string &	getMsgError() const throw();
		*	\brief Getter for the attribut : m_msgError;
		*/
		const string &	getMsgError() const throw();

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
};

#endif //_SHAPE_LEARNER_EXCEPTION_
