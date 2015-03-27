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
*	\brief Classe d'Exception pour la classe Façade ShapeLearner
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _SHAPE_LEARNER_EXCEPTION_
#define _SHAPE_LEARNER_EXCEPTION_

#include <string>
#include <iostream>
#include <exception>

using namespace std;

class ShapeLearnerExcept : public std::exception
{
	public:
		// Constructor
		ShapeLearnerExcept(const string &procedure, const string &msgError ) throw();

		// Getter
		const string &	getProcedure() const throw();
		const string &	getMsgError() const throw();

		// Implementation
		virtual const char*	what() const throw();

		// Destructor
		virtual ~ShapeLearnerExcept() throw();

	private:
		// Members
		string			m_procedure;
		string			m_msgError;
		string			m_what;
};

#endif //_SHAPE_LEARNER_EXCEPTION_
