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

#ifndef _ALL_HEADERS_
#define _ALL_HEADERS_

#pragma warning( disable : 4290 )

/**
 * \file allHeaders.h
 * \brief Fichier contenant tous les includes à réaliser de manière à simplifier les inclusions et éviter les omissions.
 * \version	1.0
 * \author DEKHTIAR Jonathan
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <QtCore\qstring.h>
#include <QtCore\qdir.h>
#include <QtSql\qsqldatabase.h>
#include <QtSql\qsqlquery.h>
#include <QtSql\qsqlerror.h>
#include <QtCore/qvariant.h>

#include "dbManager.h"
#include "dbexception.h"
#include "constants.h"

#endif //_ALL_HEADERS_