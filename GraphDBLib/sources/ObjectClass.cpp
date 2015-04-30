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

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
using namespace graphDBLib;

ObjectClass::ObjectClass(string name) : objectClassName(name) {
	GraphDB::ObjectInterface::saveObject(*this);
	Logger::Log("New Object Instanciated : ObjectClass("+ getKey()+")");
}

boost::shared_ptr<ObjectClass> ObjectClass::Access::createObjectClass(string name){
	return boost::shared_ptr<ObjectClass> (new ObjectClass(name));
}

string ObjectClass::getKey() const {return objectClassName;}

string ObjectClass::getClassName() const {return "ObjectClass";}

void ObjectClass::updateInDB(){
	#ifdef _MSC_VER
		GraphDB::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

string ObjectClass::saveInDB(){
	#ifdef _MSC_VER
		return GraphDB::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

vector<unsigned long> ObjectClass::getGraphs(){
	return GraphDB::ObjectInterface::getForeignRelations<GraphIdViewByObjectClass>(objectClassName);
}

void ObjectClass::resynchronize(){
	updateInDB();
}