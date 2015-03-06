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

Graph::Graph(GraphClass* _graphClass, ObjectClass* _objectClass, unsigned int const _viewNumber, string const _objectName) : 
	graphClass(_graphClass), 
	objectClass(_objectClass), 
	viewNumber(_viewNumber), 
	objectName(_objectName), 
	idGraph(0) 
	{
		idGraph = GraphManager::openManager().saveObject(*this);
		#ifdef _VERBOSE_
			cout << "Graph key : "+ to_string((_ULonglong)getKey()) <<endl;
		#endif
	}

inline void Graph::setKey(const unsigned int key) {
	#ifdef _MSC_VER
		GraphManager::openManager().deleteObject(*this);
	#endif //_MSC_VER
	idGraph = key;
	#ifdef _MSC_VER
		GraphManager::openManager().saveObject(*this);
	#endif //_MSC_VER
}

inline void Graph::setObjectName(const string& _objectName) {
	objectName = _objectName;
	#ifdef _MSC_VER
		GraphManager::openManager().updateObject(*this);
	#endif //_MSC_VER
}

inline void Graph::setView(const unsigned int _viewNumber) {
	viewNumber = _viewNumber;
	#ifdef _MSC_VER
		GraphManager::openManager().updateObject(*this);
	#endif //_MSC_VER
}
