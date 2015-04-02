/* ************* Begin file CmdLineException.h ***************************************/
/*
** 2015 February 24
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file CmdLineException.h
*	\brief Exception class for the class CmdLine - Header file
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _CCMDLINEEXCEPTION_
#define _CCMDLINEEXCEPTION_

#include <stdexcept>
#include <sstream>
#include <string>
using namespace std;

/*!	
*	\class CmdLineException
*	\brief The exception class working with the class CmdLine. It has been developped in order to display as much info as possible.
*	It derives publicly from the standard exception class : std::exception.
*/
class CmdLineException : public std::exception
{
	public:
		/*!
		*	\fn CmdLineException(const string &procedure, const string &msgError ) throw();
		*	\brief Public constructor, format automaticly the exception message as wanted.
		*	\param procedure : The function raising the exception
		*	\param msgError : The message explaining why an exception has been raised.
		*/
		CmdLineException(const string &procedure, const string &msgError ) throw();

		/*!
		*	\fn virtual const char*	what() const throw();
		*	\brief Returns the exception's message.
		*/
		virtual const char*	what() const throw();

		/*!
		*	\fn virtual ~CmdLineException() throw();
		*	\brief Public virtual destructor
		*/
		virtual ~CmdLineException() throw();

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

#endif //_CCMDLINEEXCEPTION_