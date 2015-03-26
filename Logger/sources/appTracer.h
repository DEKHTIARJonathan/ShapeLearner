/* ************* Begin file appTracer.h ***************************************/
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
*	\file appTracer.h
*	\brief appTracer ODB Overload Header
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _APPTRACER_
#define _APPTRACER_

#include "CLogger.h"
using namespace std;

/*!	
*	\class appTracer
*	\brief Class derivated from public odb::pgsql::tracer, it allows us to log every actions performed by ODB.
*/

class appTracer: public odb::pgsql::tracer
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