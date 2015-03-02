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

#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4068 )
#endif //_MSC_VER

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <direct.h>
#include <memory>
#include <cerrno>
//#include <ppl.h>


#include <odb/core.hxx>
#ifdef _MSC_VER
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>
#endif //_MSC_VER


#ifdef _MSC_VER // Prevent access from ODB compiler.
#include "CmdLine.h"
#include "CmdLineException.h"
#include "graphManager.h"
#include "graphManagerException.h"
#include "dbManager.h"
#include "dbexception.h"
#endif //_MSC_VER

#include "constants.h"

/* Class for ODB */
#include "GraphClass.h"
#include "ObjectClass.h"
#include "Graph.h"
/* Class for ODB */

#ifdef _MSC_VER // ODB Generated
#include "GraphClass-odb.hxx"
#include "ObjectClass-odb.hxx"
#endif // _MSC_VER


#define GetCurrentDir _getcwd


#endif //_ALL_HEADERS_