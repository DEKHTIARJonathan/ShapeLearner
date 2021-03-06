/* ************* Begin file stdafx.h ***************************************/
/*
** 2015 March 08
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file stdafx.h
*	\brief Header File containg every headers that needs to be precompile.
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _STDAFX_GRAPH_DB_
#define _STDAFX_GRAPH_DB_

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::stdafx.h  - this should happen just once per project.\n")
#endif

//For /Wall
#pragma warning(push)
#pragma warning(disable : 4820)
#pragma warning(disable : 4619)
#pragma warning(disable : 4548)
#pragma warning(disable : 4668)
#pragma warning(disable : 4365)
#pragma warning(disable : 4710)
#pragma warning(disable : 4371)
#pragma warning(disable : 4826)
#pragma warning(disable : 4061)
#pragma warning(disable : 4640)

// ============================= START ADDING LIBS =============================

#include <iostream>
#include <iomanip>
#include <io.h>
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
#include <random>
#include <stack>
#include <algorithm>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <math.h>
#include <fcntl.h> // for open()
#include <cstdio> // for perror()

// ODB Classes
#include <odb/core.hxx>
#include <odb/callback.hxx>
#include <odb/boost/lazy-ptr.hxx>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/schema-catalog.hxx>

//GraphDB Specific Files

#include "StandardException.h"
#include "constants.h"
#include "CLogger.h"
#include "appTracer.h"

// ============================= STOP ADDING LIBS ==============================

#pragma warning(pop) //For /Wall

#endif // _STDAFX_GRAPH_DB_