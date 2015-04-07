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
*	\brief File containg every needed headers.
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
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
#include <cerrno>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/threadpool.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/smart_ptr.hpp>

//#include <ppl.h>

#include <odb/core.hxx>
#include <odb/callback.hxx>
#include <odb/boost/lazy-ptr.hxx>


#ifdef _MSC_VER
	// ODB Classes
	#include <odb/database.hxx>
	#include <odb/transaction.hxx>
	#include <odb/pgsql/database.hxx>
	#include <odb/schema-catalog.hxx>
	

	// Software's architecture
	#include "CLogger.h"
	#include "shapeLearner.h"
	#include "dbManager.h"
	#include "shockGraphsGenerator.h"
#endif //_MSC_VER

#include "constants.h"
#include "shapeLearnerException.h"

/* Object Classes for ODB */
#include "GraphClass.h"
#include "ObjectClass.h"
#include "Graph.h"
#include "Node.h"
#include "Point.h"
#include "Edge.h"
/* Class for ODB */

#ifdef _MSC_VER // ODB Generated
	#include "GraphClass-odb.hxx"
	#include "ObjectClass-odb.hxx"
	#include "Graph-odb.hxx"
	#include "Node-odb.hxx"
	#include "Edge-odb.hxx"
	#include "Point-odb.hxx"
#endif // _MSC_VER

#endif //_ALL_HEADERS_