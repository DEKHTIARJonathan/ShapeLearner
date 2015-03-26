/*************** Begin file CLogger.h ***************************************
**
** 2015 March 24
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/*!
*	\file CLogger.h
*	\brief CLogger Header
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _CLOGGER_
#define _CLOGGER_

#include <iostream>
#include <time.h>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <fstream>
#include <odb/pgsql/tracer.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/pgsql/connection.hxx>
#include <odb/pgsql/statement.hxx>
#include "../../ShapeLearnerLib/sources/constants.h"
#include "../../ShapeLearnerLib/sources/shapeLearnerException.h"
#include "appTracer.h"

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
	*	\fn static volatile void Log (string text, unsigned int logFile = constants::LogApp)
	*	\brief Static method called to perform logging, it is "thread safe" and automaticly redirect to the correct log file.
	*	\param text : What do we want to log.
	*	\param logFile : What log file do you aim ? See constants.h for values
	*/
	static volatile void Log (string text, unsigned int logFile = constants::LogApp);

private:
	ofstream outstreamDB;
	ofstream outstreamError;
	ofstream outstreamApp;
	ofstream outstreamCore;
	static volatile Logger _inst;
	static boost::mutex mutexLogger;

	/*!
	*	\fn void writeDB (string text)
	*	\brief Private method called to perform logging into the DB log file.
	*	\param text : What do we want to log.
	*/
	void writeDB (string text);

	/*!
	*	\fn void writeApp (string text)
	*	\brief  Private method called to perform logging into the App log file.
	*	\param text : What do we want to log.
	*/
	void writeApp (string text);
	
	/*!
	*	\fn void writeCore (string text)
	*	\brief Private method called to perform logging into the Core log file.
	*	\param text : What do we want to log.
	*/
	void writeCore (string text);
	
	/*!
	*	\fn void writeError (string text)
	*	\brief Private method called to perform logging into the Error log file.
	*	\param text : What do we want to log.
	*/
	void writeError (string text);
	
	/*!
	*	\fn string getTime() const
	*	\brief Private method called to get the actual time well formated.
	*/
	string getTime() const;
	
	// Singleton Thread-Safe 
	
	/*!
	*	\fn Logger()
	*	\brief Private constructor
	*/
    Logger();
	
	/*!
	*	\fn ~Logger()
	*	\brief Private Destructor, automaticly called at the end of the program (object instanciated on stack). Close all ofstreams.
	*/
	~Logger();
	
	/*!
	*	\fn Logger(const Logger&)
	*	\brief Singleton Pattern, forbid copy
	*/
	Logger(const Logger&);
	
	/*!
	*	\fn void operator=(const Logger&)
	*	\brief Singleton Pattern, forbid copy
	*/
	void operator=(const Logger&);
};


#endif // _CLOGGER_
