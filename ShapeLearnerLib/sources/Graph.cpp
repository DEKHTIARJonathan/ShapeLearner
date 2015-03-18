/* ************ Begin file Graph.cpp **************************************
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
*	\file Graph.cpp
*	\brief Graph Cpp File
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

Graph::Graph(boost::weak_ptr<GraphClass> _refGraphClass,boost::weak_ptr<ObjectClass> _refObjectClass, string const _objectName, unsigned int const _viewNumber) :
	refGraphClass(_refGraphClass),
	refObjectClass(_refObjectClass),
	viewNumber(_viewNumber),
	objectName(_objectName),
	idGraph(0)
	{
		idGraph = saveInDB();
		#ifdef _VERBOSE_
			cout << "Graph key : "+ to_string((_ULonglong)getKey()) <<endl;
		#endif
	}

void Graph::setKey(const unsigned int key) {
	removeFromDB();
	idGraph = key;
	saveInDB();
}

void Graph::setObjectName(const string& _objectName) {
	objectName = _objectName;
	updateInDB();
}

void Graph::setView(const unsigned int _viewNumber) {
	viewNumber = _viewNumber;
	updateInDB();
}

void Graph::removeFromDB(){
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::deleteObject(*this);
	#endif //_MSC_VER
}

void Graph::updateInDB(){
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

unsigned long Graph::saveInDB(){
	#ifdef _MSC_VER
		return GraphManager::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

boost::weak_ptr<GraphClass> Graph::getParentGraphClass(){
	if(refGraphClass.expired())
		refGraphClass.swap(odb::boost::lazy_weak_ptr<GraphClass>(GraphManager::CommonInterface::getGraphClass(refGraphClass.object_id<GraphClass>())));
	return refGraphClass.get_eager();	
}

boost::weak_ptr<ObjectClass> Graph::getParentObjectClass(){
	if(refObjectClass.expired())
		refObjectClass.swap(odb::boost::lazy_weak_ptr<ObjectClass>(GraphManager::CommonInterface::getObjectClass((string)refObjectClass.object_id<ObjectClass>())));
	return refObjectClass.get_eager();	
}