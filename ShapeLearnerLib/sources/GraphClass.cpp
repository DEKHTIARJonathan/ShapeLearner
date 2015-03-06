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
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

GraphClass::GraphClass(const string& name, bool isDirect, bool isAcyclic) : graphClassName(name), directGraph(isDirect), acyclicGraph(isAcyclic) {
	GraphManager::ObjectInterface::saveObject(*this);
	#ifdef _VERBOSE_
		cout << "GraphClass key : "+ getKey() <<endl;
	#endif
}

inline void GraphClass::setKey(const string& key) {	
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::deleteObject(*this);
	#endif //_MSC_VER
		
		graphClassName = key;

	#ifdef _MSC_VER
		GraphManager::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

inline void GraphClass::setIsDirect(const bool _directGraph){
	directGraph = _directGraph;
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

inline void GraphClass::setIsAcyclicGraph(const bool _acyclicGraph){
	acyclicGraph = _acyclicGraph;
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}