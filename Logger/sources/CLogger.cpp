/* ************* Begin file CLogger.cpp ***************************************/
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
*	\file CLogger.cpp
*	\brief Logger Source File
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "CLogger.h"

using namespace std;
using namespace boost::posix_time;

volatile Logger Logger::_inst;
boost::mutex Logger::mutexLogger;

volatile void Logger::Log(string text, unsigned int logFile, const unsigned long threadID){
	if (logFile > 4 || logFile < 1){
		Log("Call to "+ (string) __FUNCTION__ +" // logFile ("+to_string((_ULonglong)logFile)+") doesn't belong to [1,4]", constants::LogError, threadID);
		throw StandardExcept((string)__FUNCTION__, "logFile ("+to_string((_ULonglong)logFile)+") doesn't belong to [1,4]");
	}

	string output;

	Logger* Log = const_cast<Logger*>(&_inst);

	mutexLogger.lock();
	{
		if (threadID != Log->mainThreadID)
			output = "Thread " + to_string((_ULonglong) threadID) + " : " + text;
		else
			output = "Main Thread : " + text;

		switch(logFile){
		case constants::LogDB:
			Log->writeDB(output);
			break;
		case constants::LogCore:
			Log->writeCore(output);
			break;
		case constants::LogExec:
			Log->writeExec(output);
			break;
		case constants::LogError:
			Log->writeError(output);
			break;
		}
	}
    mutexLogger.unlock();
}

void Logger::writeDB (string text){
	bool debugOut = true;
	bool lineSkip = false;
	if (text.compare("postgres: BEGIN") == 0 || text.compare("postgres: ROLLBACK") == 0 || text.find("postgres: EXECUTE") != std::string::npos)
		debugOut = false;
	else if (text.compare("postgres: COMMIT") == 0){
		debugOut = false;
		lineSkip = true;
	}

	string localTime = getTime();
	outstreamDB << localTime<< " # " << text << endl;
	if (lineSkip)
		outstreamDB << "#################################################"<< endl;
	#ifdef _DEBUG
		if(debugOut)
			cout << text << endl;
	#endif
}
void Logger::writeCore (string text){
	string localTime = getTime();
	outstreamCore << localTime<< " # " << text << endl;
	#ifdef _DEBUG
		cout << text << endl;
	#endif
}
void Logger::writeExec (string text){
	string localTime = getTime();
	outstreamExec << localTime<< " # " << text << endl;
	#ifdef _DEBUG
		cout << text << endl;
	#endif
}
void Logger::writeError (string text){
	string localTime = getTime();
	outstreamError << localTime<< " # " << text << endl;
	#ifdef _DEBUG
		cout << text << endl;
	#endif
}

string Logger::getTime() const{
	std::stringstream stream;
	boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S:%f");
	const boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();

	stream.imbue(std::locale(std::locale::classic(), facet));
	stream << time;
	return stream.str();
}

Logger::Logger(){
	mainThreadID = getThreadId();
	outstreamDB.open("ShapeLearner.DB.log", std::ofstream::out | std::ofstream::app); // Insert at the end of the file.
	outstreamError.open("ShapeLearner.Error.log", std::ofstream::out | std::ofstream::app); // Insert at the end of the file.
	outstreamExec.open("ShapeLearner.Exec.log", std::ofstream::out | std::ofstream::app); // Insert at the end of the file.
	outstreamCore.open("ShapeLearner.Core.log", std::ofstream::out | std::ofstream::app); // Insert at the end of the file.
	writeExec("Main Thread : Application has been started successfully");
}

Logger::~Logger(){
	writeExec("Main Thread : Application has been shut down successfully");
	outstreamDB.close();
	outstreamError.close();
	outstreamExec.close();
	outstreamCore.close();
}

const unsigned long Logger::getThreadId(){
    std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
    unsigned long threadNumber = 0;
    sscanf(threadId.c_str(), "%lx", &threadNumber);
    return threadNumber;
}