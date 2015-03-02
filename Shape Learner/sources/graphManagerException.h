/* ************* Begin file graphManagerException.h ***************************************/
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
*	\file graphManagerException.h
*	\brief Classe d'Exception pour la classe Façade GraphManager
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _GraphManagerExcept_
#define _GraphManagerExcept_

#include "allHeaders.h"
using namespace std;

class GraphManagerExcept : public std::exception
{
	public:
		// Constructor
		GraphManagerExcept(const string &procedure, const string &msgError ) throw();

		// Getter
		const string &	getProcedure() const throw();
		const string &	getMsgError() const throw();

		// Implementation
		virtual const char*	what() const throw();

		// Destructor
		virtual ~GraphManagerExcept() throw();

	private:
		// Member
		string			m_procedure;
		string			m_msgError;
		string			m_what;

};

#endif //_GraphManagerExcept_
