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
		case WaitingGen:	return "WaitingGen";
		case StartGen:		return "StartGen";
		case StartSaving:	return "StartSaving";
		case EndSaving:		return "EndSaving";
		case ErrorGen:		return "ErrorGen";
		case ErrorSaving:	return "ErrorSaving";
		default:			return "";
    }
}

JobStatus::JobStatus(JobStatus_e _status): status(_status){}
std::string JobStatus::getStatus(){
   switch (status)
    {
		case Waiting:		return "Waiting";
		case Ongoing:		return "Ongoing";
		case Error:			return "Error";
		case Finished:		return "Finished";
		default:			return "";
    }
}

//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//std::string narrow = converter.to_bytes(wide_utf16_source_string);
//std::wstring wide = converter.from_bytes(narrow_utf8_source_string);

bool JobManager::Log(unsigned long jobID, JobStatus jobStatus, unsigned long partID, StepName stepName, std::string filepath, std::string jobServerIP, unsigned int jobServerPort){
   std::string url(jobServerIP+ to_string((_ULonglong) jobServerPort) + "/updateJob");
   std::map<std::string,std::string> resultat;

   resultat["jobID"] = to_string((_ULonglong)jobID);
   resultat["jobStatus"] = jobStatus.getStatus();
   resultat["partID"] = to_string((_ULonglong)partID);
   resultat["partName"] = filepath;
   resultat["serverIP"] = dbServerIP;
   resultat["serverPort"] = dbServerPort;
   resultat["message"] = messagesMap[stepName.getStep()];

   std::string json_body(serializeMap(resultat)) ;
   std::wstring json_content_type(L"application/json");

   mutexJob.lock();
   {
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      std::wstring wURL = converter.from_bytes(url);
      WinHttpClient client(wURL) ;

      // Set post data.
      client.SetAdditionalDataToSend((BYTE *)json_body.c_str(), json_body.size());

      // Set request headers.
      wstring headers = L"Content-Length: ";
      headers += std::to_wstring((_ULonglong) json_body.size()) ;//szSize;
      headers += L"\r\nContent-Type: "+json_content_type+L"\r\n";
      client.SetAdditionalRequestHeaders(headers);

      // Send HTTP post request.
      client.SendHttpRequest(L"POST");

      wstring httpResponseHeader = client.GetResponseHeader();
      wstring httpResponseContent = client.GetResponseContent();
   }
    mutexJob.unlock();

   return true;
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