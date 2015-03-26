/* ************* Begin file CLogger.h ***************************************/
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
*	\file CLogger.h
*	\brief CLogger Header
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _CLOGGER_
#define _CLOGGER_

#include <string>
#include "constants.h"

using namespace std;

class Logger{
public:
	static volatile void Log (std::string text, unsigned int logFile = constants::LogApp);
};

class appTracer;


#endif // _CLOGGER_
