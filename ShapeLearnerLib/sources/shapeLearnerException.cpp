/* ************* Begin file shapeLearnerException.h ***************************************/
/*
** 2015 March 05
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file shapeLearnerException.cpp
*	\brief Exception class used by the whole library.
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "allHeaders.h"
using namespace std;

/********************************************************************
 *                            Constructor                           *
 ********************************************************************/

// Generic DBException constructor
ShapeLearnerExcept::ShapeLearnerExcept( const string &procedure, const string &msgError ) throw() :  std::exception(), m_procedure(procedure), m_msgError(msgError) {
	
	stringstream ss;

	ss << "An error occured on '" << m_procedure << "' procedure :" << endl;
	ss << "-----------------Error-----------------" << endl;
	ss << m_msgError << endl;
	ss << "----------------------------------------" << endl;

	m_what = ss.str().data();
}

/********************************************************************
 *                          Implementation                          *
 ********************************************************************/

const char* ShapeLearnerExcept::what() const throw(){
	return m_what.c_str();
}
/********************************************************************
 *                            Destructor                            *
 ********************************************************************/

ShapeLearnerExcept::~ShapeLearnerExcept() throw(){}