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
using namespace graphDBLib;

GraphClass::GraphClass(string name, bool isDirect, bool isAcyclic) : graphClassName(name), directGraph(isDirect), acyclicGraph(isAcyclic) {
	saveInDB();
	Logger::Log("New Object Instanciated : GraphClass("+ getKey()+")");
}

boost::shared_ptr<GraphClass> GraphClass::Access::createGraphClass(string name, bool isDirect, bool isAcyclic){
	return  boost::shared_ptr<GraphClass>(new GraphClass(name, isDirect, isAcyclic));
}

string GraphClass::getKey() const {return graphClassName;}

bool GraphClass::getIsDirect() const {return directGraph;}
void GraphClass::setIsDirect(const bool _directGraph, bool asynchronous){
	directGraph = _directGraph;
	if (!asynchronous)
		updateInDB();
}

bool GraphClass::getIsAcyclicGraph() const {return acyclicGraph;}
void GraphClass::setIsAcyclicGraph(const bool _acyclicGraph, bool asynchronous){
	acyclicGraph = _acyclicGraph;
	if (!asynchronous)
		updateInDB();
}

string GraphClass::getClassName() const { return "GraphClass"; }

void GraphClass::updateInDB(){
	GraphDB::ObjectInterface::updateObject(*this);
}

string GraphClass::saveInDB(){
	return GraphDB::ObjectInterface::saveObject(*this);
}

vector<unsigned long> GraphClass::getGraphs(){
	return GraphDB::ObjectInterface::getForeignRelations<GraphIdViewByGraphClass>(graphClassName);
}

void GraphClass::resynchronize(){
	updateInDB();
}