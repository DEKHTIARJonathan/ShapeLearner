/* ************* Begin file jobManager.cpp ***************************************/
/*
** 2015 February 23
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file jobManager.cpp
*	\brief jobManager source file.
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;

StepName::StepName(StepName_e _step): step(_step){}
std::string StepName::getStep(){
	switch (step)
    {
		case WaitingGen:   return "WaitingGen";
        case StartGen:   return "StartGen";
        case StartSaving: return "StartSaving";
		case EndSaving: return "EndSaving";
		case ErrorGen: return "ErrorGen";
		case ErrorSaving: return "ErrorSaving";
        default:      return "";
    }
}

JobStatus::JobStatus(JobStatus_e _status): status(_status){}
std::string JobStatus::getStatus(){
	switch (status)
    {
        case Waiting:   return "Waiting";
        case Ongoing:   return "Ongoing";
        case Error: return "Error";
		case Finished: return "Finished";
        default:      return "";
    }
}

bool JobManager::Log(unsigned long jobID, JobStatus jobStatus, std::string partID, StepName stepName, std::string filepath, std::string jobServerIP, unsigned int jobServerPort){
	using namespace boost::network;

    std::string url(jobServerIP+ to_string((_ULonglong) jobServerPort) + "/updateJob");
	std::map<std::string,std::string> resultat;

	resultat["jobID"] = to_string((_ULonglong)jobID);
	resultat["jobStatus"] = jobStatus.getStatus();
	resultat["partID"] = partID;
	resultat["partName"] = filepath;
	resultat["serverIP"] = dbServerIP;
	resultat["serverPort"] = dbServerPort;
	resultat["message"] = messagesMap[stepName.getStep()];

	std::string str_rslt = serializeMap(resultat);
	std::string json_body(str_rslt);
	cout << endl << endl << str_rslt << endl << endl;
    std::string json_content_type("application/json");

	std::string rslt;
	mutexJob.lock();
	{
		http::client client;
		http::client::request request(url);
		request << header("Connection", "close");
		rslt = body(client.post(request, json_body, json_content_type));
	}
    mutexJob.unlock();

	return (rslt == "ok");
}

string JobManager::serializeMap(const map<string,string>& val){
	string rslt = "";
	for(map<string,string>::const_iterator it = val.begin(); it != val.end(); it++){
		if (rslt == "")
			rslt.append("{");
		else
			rslt.append(",");
		rslt.append("\"" + it->first + "\":\"" + it->second + "\"");
	}
	rslt.append("}");
	return rslt;
}

void JobManager::initJobManager(){
	pair<std::string, std::string> dbServerInfos = graphDBLib::GraphDB::getServerInfos();

	JobManager::mutexJob.lock();
	{
		dbServerIP = dbServerInfos.first;
		dbServerPort = dbServerInfos.second;

		messagesMap["WaitingGen"] = "Job has been pooled and is now waiting to start.";
		messagesMap["StartGen"] = "Generating the part's ShockGraph.";
		messagesMap["StartSaving"] = "Saving the generated ShockGraph to the Database.";
		messagesMap["EndSaving"] = "ShockGraph successfully saved to DB.";
		messagesMap["ErrorGen"] = "An error occured during the ShockGraph's Generation.";
		messagesMap["ErrorSaving"] = "An error occured while saving the ShockGraph to the DB. ";
	}
	mutexJob.unlock();
}

map<std::string, std::string> JobManager::messagesMap;

boost::mutex JobManager::mutexJob;
std::string JobManager::dbServerIP;
std::string JobManager::dbServerPort;