/* ************* Begin file dbexception.h ***************************************/
/*
** 2015 February 18
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
 * \file dbexception.h
 * \brief Classe d'Exception pour l'interface DatabaseManager
 * \version	1.0
 * \author DEKHTIAR Jonathan
 */

#ifndef DBEXCEPTION_H
#define DBEXCEPTION_H

#include "allHeaders.h"
using namespace std;

class DBException : public std::exception
{
	public:
		// Constructor
		DBException(const QString &procedure, const QString &msgError ) throw();

		// Getter
		const QString &	getProcedure() const throw();
		const QString &	getMsgError() const throw();

		// Implementation
		virtual const char*	what() const throw();

		// Destructor
		virtual ~DBException() throw();

	private:
		// Member
		QString			m_procedure;
		QString			m_msgError;
		string			m_what;

};

#endif
