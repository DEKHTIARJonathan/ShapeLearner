/* ************ Begin file Edge.cpp **************************************
**
** 2015 March 03
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file Edge.cpp
*	\brief Edge Cpp File
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;


Edge::Edge(Node* _source, Node* _target, Graph* _refGraph, unsigned int _weight) : 
	source(_source), 
	target(_target), 
	refGraph(_refGraph), 
	weight(_weight),
	idEdge(0) 
	{
		idEdge = saveInDB();
		#ifdef _VERBOSE_
			cout << "Edge key : "+ to_string((_ULonglong)getKey()) <<endl;
		#endif;
	}

void Edge::setKey(const unsigned int key) {
		removeFromDB();
		idEdge = key;
		saveInDB();
	}

void Edge::setWeight(const unsigned int _weight) {
	weight = _weight;
	updateInDB();
}

void Edge::removeFromDB(){
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::deleteObject(*this);
	#endif //_MSC_VER
}

void Edge::updateInDB(){
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

unsigned long Edge::saveInDB(){
	#ifdef _MSC_VER
		return GraphManager::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}