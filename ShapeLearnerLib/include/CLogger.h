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
*	\brief Logger Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _CLOGGER_
#define _CLOGGER_

#include <string>
#include "constants.h"

using namespace std;
/*!
*	\class Logger
*	\brief Class allowing us to log every actions performed in the program.
*	In DEBUG : Everything is redirected to the 4 log files + standard output.
*	In RELEASE : Everything is redirected to the 4 log files + Errors only to the standard output.
*/
class Logger{
public:
	
	/*!
	*	\fn static volatile void Log (string text, unsigned int logFile = constants::LogExec)
	*	\brief Static method called to perform logging, it is "thread safe" and automaticly redirect to the correct log file.
	*	\param text : What do we want to log.
	*	\param logFile : What log file do you aim ? See constants.h for values
	*/
	static volatile void Log (string text, unsigned int logFile = constants::LogExec);
};


#endif // _CLOGGER_
