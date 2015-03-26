/* ************ Begin file ObjectClass.cpp **************************************
**
** 2015 March 02
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file ObjectClass.cpp
*	\brief ObjectClass Cpp File
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

ObjectClass::ObjectClass(string name) : objectClassName(name) {
	GraphManager::ObjectInterface::saveObject(*this);
	Logger::Log("New Object Instanciated : ObjectClass("+ getKey()+")");
}

void ObjectClass::updateInDB(){
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

string ObjectClass::saveInDB(){
	#ifdef _MSC_VER
		return GraphManager::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

vector<unsigned long> ObjectClass::getGraphs(){
	return GraphManager::ObjectInterface::getForeignRelations<GraphIdViewByObjectClass>(objectClassName);
}