/* ************* Begin file AppTracer.h ***************************************/
/*
** 2015 March 24
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file AppTracer.h
*	\brief AppTracer ODB Overload Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _APPTRACER_
#define _APPTRACER_

#ifdef _MSC_VER
	#pragma message("Compiling CLogger::appTracer.h - this should happen just once per project.\n")
#endif

#include "CLogger.h"
using namespace std;

/*!	
*	\class AppTracer
*	\brief Class derivated from public odb::pgsql::tracer, it allows us to log every actions performed by ODB by calling Logger::Log
*/
class AppTracer: public odb::pgsql::tracer
{
	/*!
	*	\fn virtual void prepare (odb::pgsql::connection& c, const odb::pgsql::statement& s)
	*	\brief Virtual method called for tracing while preparing a DB Request
	*	\param c : Opened Connection to the DB.
	*	\param s : Request being prepared.
	*/
	virtual void prepare (odb::pgsql::connection& c, const odb::pgsql::statement& s);

	/*!
	*	\fn void execute (odb::pgsql::connection& c, const odb::pgsql::statement& s)
	*	\brief Virtual method called for tracing while executing a DB Request which was prepared
	*	\param c : Opened Connection to the DB.
	*	\param s : Request being executed.
	*/
	virtual void execute (odb::pgsql::connection& c, const odb::pgsql::statement& s);

	/*!
	*	\fn virtual void execute (odb::pgsql::connection& c, const char* statement)
	*	\brief Virtual method called for tracing while executing a DB Request not prepared.
	*	\param c : Opened Connection to the DB.
	*	\param s : Request being executed.
	*/
	virtual void execute (odb::pgsql::connection& c, const char* statement);

	/*!
	*	\fn virtual void deallocate (odb::pgsql::connection& c, const odb::pgsql::statement& s)
	*	\brief Virtual method called for tracing while deallocating an object.
	*	\param c : Opened Connection to the DB.
	*	\param s : Request being prepared.
	*/
	virtual void deallocate (odb::pgsql::connection& c, const odb::pgsql::statement& s);
};

#endif