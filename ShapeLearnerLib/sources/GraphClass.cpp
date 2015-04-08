/* ************ Begin file GraphClass.cpp **************************************
**
** 2015 February 27
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file GraphClass.cpp
*	\brief GraphClass Cpp File
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;

GraphClass::GraphClass(string name, bool isDirect, bool isAcyclic) : graphClassName(name), directGraph(isDirect), acyclicGraph(isAcyclic) {
	saveInDB();
	Logger::Log("New Object Instanciated : GraphClass("+ getKey()+")");
}

void GraphClass::setIsDirect(const bool _directGraph){
	directGraph = _directGraph;
	updateInDB();
}

void GraphClass::setIsAcyclicGraph(const bool _acyclicGraph){
	acyclicGraph = _acyclicGraph;
	updateInDB();
}

void GraphClass::updateInDB(){
	#ifdef _MSC_VER
		ShapeLearner::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

string GraphClass::saveInDB(){
	#ifdef _MSC_VER
		return ShapeLearner::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

vector<unsigned long> GraphClass::getGraphs(){
	return ShapeLearner::ObjectInterface::getForeignRelations<GraphIdViewByGraphClass>(graphClassName);
}