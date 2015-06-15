#include <iostream>
#include "boost/filesystem.hpp"
#include "graphDB.h"
#include "StandardException.h"
#include "ShapeLearner.h"
#include "DAGMatcherLib.h"
#include "CLogger.h"

using namespace std;
using namespace boost::filesystem;
using namespace graphDBLib;

#ifdef __cplusplus
extern "C" {
#endif

//__declspec(dllexport) bool openDataBase(string initFile = "")
__declspec(dllexport) void openDataBase(char* _dbUser, char* _dbPass, char* _dbName, char* _dbHost, unsigned int _dbPort, char* _dbInit = "")
{
	GraphDB::openDatabase(_dbUser, _dbPass, _dbName, _dbHost, _dbPort, _dbInit);
	if (_dbInit != "") {
		dml::DAGMatcherLib::InitDAGMatcherLib();
	}
}

__declspec(dllexport) void signBinaryImage(char* _imgPath, char* _imgClass)
{
	try {
		ShapeLearner::createShockGraph(img2Parse(_imgPath, _imgClass));
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
	}
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