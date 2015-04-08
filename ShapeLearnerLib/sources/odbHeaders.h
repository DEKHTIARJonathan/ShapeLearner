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
*	\brief File containg every needed headers for the needed librairies. This code doesn't change over time.
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _ODB_HEADERS_
#define _ODB_HEADERS_

#ifndef _MSC_VER
	#include <iostream>
	#include <string>
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
	#include <boost/thread/mutex.hpp>
	#include <boost/smart_ptr.hpp>

	#include "constants.h"
	#include "shapeLearnerException.h"
	
	#include <odb/core.hxx>
	#include <odb/callback.hxx>
	#include <odb/boost/lazy-ptr.hxx>

	/* Object Classes for ODB */
	#include "GraphClass.h"
	#include "ObjectClass.h"
	#include "Graph.h"
	#include "Node.h"
	#include "Point.h"
	#include "Edge.h"
	/* Class for ODB */

#endif //_MSC_VER

#endif //_ODB_HEADERS_