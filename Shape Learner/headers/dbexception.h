/* ************* Begin file dbexception.cpp ***************************************/
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

#ifndef DBEXCEPTION_H
#define DBEXCEPTION_H

#include "allHeaders.h"
using namespace std;

/**
 * \file dbexception.h
 * \brief Classe d'Exception pour l'interface DatabaseManager
 * \version	1.0
 * \author DEKHTIAR Jonathan
 */

class DBException : public std::exception
{

	public:
		// Constructor
		DBException( const QString & procedure, const QString & msgError );

		// Getter
		const QString &	getProcedure() const;
		const QString &	getMsgError() const;

		// Implementation
		const char*		what() const throw();

		// Destructor
		~DBException() throw();

	private:
		// Member
		QString			m_procedure;
		QString			m_msgError;

};

#endif
