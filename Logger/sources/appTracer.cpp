/* ************* Begin file AppTracer.cpp ***************************************/
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
*	\file AppTracer.cpp
*	\brief AppTracer ODB Overload SourceFile
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "CLogger.h"

void AppTracer::prepare (odb::pgsql::connection& c, const odb::pgsql::statement& s)
{
	Logger::Log(c.database ().db () + ": PREPARE " + s.name () + " AS " + s.text (), constants::LogDB);
}

void AppTracer::execute (odb::pgsql::connection& c, const odb::pgsql::statement& s)
{
	Logger::Log(c.database ().db () + ": EXECUTE " + s.name (), constants::LogDB);
}

void AppTracer::execute (odb::pgsql::connection& c, const char* statement)
{
	Logger::Log(c.database ().db () + ": " + statement, constants::LogDB);
}

void AppTracer::deallocate (odb::pgsql::connection& c, const odb::pgsql::statement& s)
{
	Logger::Log(c.database ().db () + ": DEALLOCATE " + s.name (), constants::LogDB);
}