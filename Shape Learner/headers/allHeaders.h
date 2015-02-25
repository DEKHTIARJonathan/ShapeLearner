/* ************* Begin file allHeaders.h ***************************************/
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
*	\file allHeaders.h
*	\brief Fichier contenant tous les includes à réaliser de manière à simplifier les inclusions et éviter les omissions.
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _ALL_HEADERS_
#define _ALL_HEADERS_

// tell the compiler to shut up
//#pragma warning(disable:4786)
#pragma warning( disable : 4290 )

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <QtCore\qstring.h>
#include <QtCore\qdir.h>
#include <QtSql\qsqldatabase.h>

#include <QtSql\qsqldriver.h>
#include <QtSql\qsql.h>

#include <QtSql\qsqlquery.h>
#include <QtSql\qsqlerror.h>
#include <QtCore/qvariant.h>
#include <QtCore\qtextstream.h>
//#include <ppl.h>

#include "dbManager.h"
#include "dbexception.h"
#include "constants.h"
#include "graphManager.h"
#include "graphManagerException.h"
#include "CmdLine.h"
#include "CmdLineException.h"

#endif //_ALL_HEADERS_