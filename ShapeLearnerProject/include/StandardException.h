/* ************* Begin file StandardException.h ***************************************/
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
*	\file StandardException.h
*	\brief Exception class used by the whole library.
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _STANDARD_EXCEPTION_
#define _STANDARD_EXCEPTION_

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

#ifdef _MSC_VER
	#pragma message("Compiling StandardException.h  - this should happen just once per project.\n")
#endif

using namespace std;

/*!	
*	\class StandardExcept
*	\brief The only exception class in the project. It has been developped in order to display as much info as possible.
*	It derives publicly from the standard exception class : std::exception.
*/
class StandardExcept : public std::exception
{
	public:
		// ================== Constructor ========================
		/*!
		*	\fn StandardExcept(const string &procedure, const string &msgError ) throw();
		*	\brief Public constructor, formats and save the exception's message in the attribut m_what.
		*	\param procedure : Name of the calling function.
		*	\param msgError : Message explaining the error.
		*/
		StandardExcept(const string &procedure, const string &msgError ) throw();

		// ================== Implementation ========================

		/*!
		*	\fn virtual const char*	what() const throw();
		*	\brief Virtual function, format and return the error as a unique string.
		*/
		virtual const char*	what() const throw();

		// ================== Implementation ========================

		/*!
		*	\fn virtual ~StandardExcept() throw();
		*	\brief virtual destructor.
		*/
		virtual ~StandardExcept() throw();
};

#endif //_STANDARD_EXCEPTION_
