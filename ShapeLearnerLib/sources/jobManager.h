/* ************* Begin file jobManager.h ***************************************/
/*
** 2015 April 03
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file jobManager.h
*	\brief jobManager Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _JOB_MANAGER_
#define _JOB_MANAGER_

#ifdef _MSC_VER
	#pragma message("Compiling ShapeLearnerLib::shockGraphGenerator.h  - this should happen just once per project.\n")
#endif

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;

class ShapeLearner; // Forward Declaration of the class contained in ShapeLearner.h
class StandardExcept; //Forward Declaration of the class contained in StandardException.h
class ObjectClass; //Forward Declaration of the class contained in ObjectClass.h
class GraphClass; //Forward Declaration of the class contained in GraphClass.h
class Graph; //Forward Declaration of the class contained in Graph.h
class Node; //Forward Declaration of the class contained in Node.h
class Point; //Forward Declaration of the class contained in Point.h
class Edge; //Forward Declaration of the class contained in Edge.h

enum JobStatus_e {Waiting,Ongoing,Error,Finished};
enum StepName_e {WaitingGen, StartGen, StartSaving, EndSaving, ErrorGen, ErrorSaving};

struct JobStatus{
	JobStatus_e status;
	std::string getStatus();
	JobStatus(JobStatus_e _status);
};

struct StepName{
	StepName_e step;
	std::string getStep();
	StepName(StepName_e _step);
};

class JobManager : boost::noncopyable {
public:

	/*!
	*	\fn static volatile void Log (string text, unsigned int logFile = constants::LogExec)
	*	\brief Static method called to perform logging, it is "thread safe" and automaticly redirect to the correct log file.
	*	\param text : What do we want to log.
	*	\param logFile : What log file do you aim ? See constants.h for values
	*/
	static bool Log (unsigned long jobID, JobStatus jobStatus, std::string partID, StepName stepName, std::string filepath,
								std::string jobServerIP = "http://127.0.0.1:", unsigned int jobServerPort = 8888);
	static void initJobManager();
private:

	/*!
    *	Mutex in order to prevent multiple access to critical sections.
    */
	static boost::mutex mutexJob;
	static map<std::string, std::string> messagesMap;
	static std::string dbServerIP;
	static std::string dbServerPort;

	static string serializeMap(const map<string,string>& val);
};

#endif // _JOB_MANAGER_