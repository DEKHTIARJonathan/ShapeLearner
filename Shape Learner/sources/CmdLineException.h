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
*	\brief Classe d'Exception pour la classe CCmdLine
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _CCMDLINEEXCEPTION_
#define _CCMDLINEEXCEPTION_

#include "allHeaders.h"
//#include <stdexcept>
//#include <sstream>
//#include <QtCore\string.h>
using namespace std;

class CCmdLineException : public std::exception
{
	public:
		// Constructor
		CCmdLineException(const string &procedure, const string &msgError ) throw();

		// Getter
		const string &	getProcedure() const throw();
		const string &	getMsgError() const throw();

		// Implementation
		virtual const char*	what() const throw();

		// Destructor
		virtual ~CCmdLineException() throw();

	private:
		// Member
		string			m_procedure;
		string			m_msgError;
		string			m_what;

};

#endif //_CCMDLINEEXCEPTION_