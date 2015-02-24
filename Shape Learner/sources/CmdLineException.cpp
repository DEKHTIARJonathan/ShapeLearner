/* ************* Begin file CmdLineException.h ****************************
**
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
*	\file CmdLineException.cpp
*	\brief Classe d'Exception pour la classe CCmdLine
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/
#include "allHeaders.h"
//#include "CmdLineException.h"
using namespace std;


/* ********************************************************************
*                              Constructor                            *
**********************************************************************/

// Generic DBException constructor
CCmdLineException::CCmdLineException( const QString &procedure, const QString &msgError ) throw() : std::exception()
{
	m_procedure	= procedure;
	m_msgError	= msgError;

	stringstream ss;

	ss << "An error occured on '" << m_procedure.toStdString() << "' procedure :" << endl;
	ss << "-----------------Error-----------------" << endl;
	ss << m_msgError.toStdString() << endl;
	ss << "----------------------------------------" << endl;

	m_what = ss.str().data();

}

/* *******************************************************************
*                                Getter                              *
*********************************************************************/

const QString &	CCmdLineException::getProcedure() const throw() {return m_procedure;}
const QString &	CCmdLineException::getMsgError() const throw() {return m_msgError;}

/* *******************************************************************
*                          Implementation                           *
********************************************************************/

const char* CCmdLineException::what() const throw(){
	return m_what.c_str();
}
/* *******************************************************************
*                            Destructor                              *
*********************************************************************/

CCmdLineException::~CCmdLineException() throw(){}