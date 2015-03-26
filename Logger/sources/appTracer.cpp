/* ************* Begin file appTracer.cpp ***************************************/
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
*	\file appTracer.cpp
*	\brief appTracer ODB Overload SourceFile
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "CLogger.h"

void appTracer::prepare (odb::pgsql::connection& c, const odb::pgsql::statement& s)
{
	Logger::Log(c.database ().db () + ": PREPARE " + s.name () + " AS " + s.text (), constants::LogDB);
}

void appTracer::execute (odb::pgsql::connection& c, const odb::pgsql::statement& s)
{
	Logger::Log(c.database ().db () + ": EXECUTE " + s.name (), constants::LogDB);
}

void appTracer::execute (odb::pgsql::connection& c, const char* statement)
{
	Logger::Log(c.database ().db () + ": " + statement, constants::LogDB);
}

void appTracer::deallocate (odb::pgsql::connection& c, const odb::pgsql::statement& s)
{
	Logger::Log(c.database ().db () + ": DEALLOCATE " + s.name (), constants::LogDB);
}