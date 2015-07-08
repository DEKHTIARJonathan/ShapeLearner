#include <iostream>
#include "boost/filesystem.hpp"
#include "graphDB.h"
#include "StandardException.h"
#include "ShapeLearner.h"
#include "jobManager.h"
#include "DAGMatcherLib.h"
#include "CLogger.h"

using namespace std;
using namespace boost::filesystem;
using namespace graphDBLib ;

void signBinaryImage(char* _imgPath, char* _imgClass, unsigned int _jobID);
unsigned int getActiveThread();

int main()
{
	string _dbInit = "structure.sql";
	GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 11111, _dbInit);
	if (_dbInit != "") {
		dml::DAGMatcherLib::InitDAGMatcherLib();
	}
	JobManager::initJobManager();
	cout << "Image signing Go !" << endl;
	signBinaryImage("img.ppm", "production", 2);
	ShapeLearner::waitForComputation ();
	return 0;
}

void signBinaryImage(char* _imgPath, char* _imgClass, unsigned int _jobID)
{
   try {
      ShapeLearner::createShockGraph(img2Parse(_imgPath, _imgClass, _jobID));
   }
   catch (const std::exception& e)
   {
      Logger::Log(e.what (), constants::LogError);
   }
}

unsigned int getActiveThread()
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