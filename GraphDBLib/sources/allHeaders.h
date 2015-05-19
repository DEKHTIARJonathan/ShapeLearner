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

#ifndef _ALL_HEADERS_GRAPH_DB_
#define _ALL_HEADERS_GRAPH_DB_

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::allHeaders.h  - this should happen just once per project.\n")
	#pragma warning(disable : 4290)
	#pragma warning(disable : 4244)
	#pragma warning(disable : 4068)

	#include "stdafx.h"

	// Software's Common Files

	// Database Objects
	#include "GraphClass.h"
	#include "ObjectClass.h"
	#include "Graph.h"
	#include "Node.h"
	#include "Point.h"
	#include "Edge.h"

	/* Class for ODB */
	#include "GraphClass-odb.hxx"
	#include "ObjectClass-odb.hxx"
	#include "Graph-odb.hxx"
	#include "Node-odb.hxx"
	#include "Edge-odb.hxx"
	#include "Point-odb.hxx"
	/* Class for ODB */

	// Software's Core Files
	#include "DBPool.h"
	#include "dbManager.h"
	#include "graphDB.h"
#endif //_MSC_VER

#endif //_ALL_HEADERS_GRAPH_DB_