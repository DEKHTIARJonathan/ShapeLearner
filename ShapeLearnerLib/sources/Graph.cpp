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

Graph::Graph(GraphClass* _graphClass, ObjectClass* _objectClass, string const _objectName, unsigned int const _viewNumber) : 
	graphClass(_graphClass), 
	objectClass(_objectClass), 
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
