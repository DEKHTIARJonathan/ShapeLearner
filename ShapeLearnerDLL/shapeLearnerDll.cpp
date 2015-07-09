#include <iostream>
#include "boost/filesystem.hpp"
#include "graphDB.h"
#include "StandardException.h"
#include "ShapeLearner.h"
#include "jobManager.h"
#include "DAGMatcherLib.h"
#include "CLogger.h"

#include "shapeLearnerDll.h"

using namespace std;
using namespace boost::filesystem;


#ifdef __cplusplus
extern "C" {
#endif


__declspec(dllexport) void openDataBase(char* _dbUser, char* _dbPass, char* _dbName, char* _dbHost, unsigned int _dbPort, char* _dbInit /*= ""*/)
{
   using namespace graphDBLib ;
   GraphDB::openDatabase(_dbUser, _dbPass, _dbName, _dbHost, _dbPort, _dbInit);
   if (_dbInit != "") {
      dml::DAGMatcherLib::InitDAGMatcherLib();
   }
   JobManager::initJobManager();
}



__declspec(dllexport) void initMatcher()
{
   JobManager::initJobManager();
}






__declspec(dllexport) void signBinaryImage(char* _imgPath, char* _imgClass, unsigned int _jobID)
{
   try {
      ShapeLearner::createShockGraph(img2Parse(_imgPath, _imgClass, _jobID));
   }
   catch (const std::exception& e)
   {
      Logger::Log(e.what (), constants::LogError);
   }
}



__declspec(dllexport) void loadShockGraph (/*char* _imgPath, char* _imgClass, unsigned int _jobID*/)
{
   try {
      ShapeLearner::loadShockGraph(/*img2Parse(_imgPath, _imgClass, _jobID)*/) ;
   }
   catch (const std::exception& e) {
      Logger::Log(e.what (), constants::LogError);
   }
}


__declspec(dllexport) unsigned int getActiveThread()
{
   try {
      return ShapeLearner::getActiveThread();
   }
   catch (const std::exception& e)
   {
      Logger::Log(e.what (), constants::LogError);
   }
   return -1 ;
}





__declspec(dllexport) void waitBeforeClosing()
{
   try {
      ShapeLearner::waitForComputation();
   }
   catch (const std::exception& e)
   {
      Logger::Log(e.what (), constants::LogError);
   }
}

#ifdef __cplusplus
}
#endif