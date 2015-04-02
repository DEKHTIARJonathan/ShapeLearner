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
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "allHeaders.h"
using namespace std;

ObjectClass::ObjectClass(string name) : objectClassName(name) {
	ShapeLearner::ObjectInterface::saveObject(*this);
	Logger::Log("New Object Instanciated : ObjectClass("+ getKey()+")");
}

void ObjectClass::updateInDB(){
	#ifdef _MSC_VER
		ShapeLearner::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

string ObjectClass::saveInDB(){
	#ifdef _MSC_VER
		return ShapeLearner::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

vector<unsigned long> ObjectClass::getGraphs(){
	return ShapeLearner::ObjectInterface::getForeignRelations<GraphIdViewByObjectClass>(objectClassName);
}