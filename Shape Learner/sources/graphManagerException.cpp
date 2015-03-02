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
*	\file graphManagerException.cpp
*	\brief Classe d'Exception pour la classe Façade GraphManager
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;


/********************************************************************
 *                            Constructor                           *
 ********************************************************************/

// Generic DBException constructor
GraphManagerExcept::GraphManagerExcept( const string &procedure, const string &msgError ) throw() : std::exception()
{
	m_procedure	= procedure;
	m_msgError	= msgError;

	stringstream ss;

	ss << "An error occured on '" << m_procedure << "' procedure :" << endl;
	ss << "-----------------Error-----------------" << endl;
	ss << m_msgError << endl;
	ss << "----------------------------------------" << endl;

	m_what = ss.str().data();

}

/********************************************************************
 *                              Getter                              *
 ********************************************************************/

const string &	GraphManagerExcept::getProcedure() const throw() {	return m_procedure;	}
const string &	GraphManagerExcept::getMsgError() const throw() {	return m_msgError;	}

/********************************************************************
 *                          Implementation                          *
 ********************************************************************/

const char* GraphManagerExcept::what() const throw(){
	return m_what.c_str();
}
/********************************************************************
 *                            Destructor                            *
 ********************************************************************/

GraphManagerExcept::~GraphManagerExcept() throw(){}

