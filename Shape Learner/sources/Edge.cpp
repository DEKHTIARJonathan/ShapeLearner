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
	idEdge(0) 
	{
		idEdge = GraphManager::openManager().saveObject(*this);
		#ifdef _VERBOSE_
			cout << "Edge key : "+ to_string((_ULonglong)getKey()) <<endl;
		#endif;
	}

inline void Edge::setKey(const unsigned int key) {
		#ifdef _MSC_VER
			GraphManager::openManager().deleteObject(*this);
		#endif //_MSC_VER
		idEdge = key;
		#ifdef _MSC_VER
			GraphManager::openManager().saveObject(*this);
		#endif //_MSC_VER
	}

inline void Edge::setWeight(const unsigned int _weight) {
	weight = _weight;
	#ifdef _MSC_VER
		GraphManager::openManager().updateObject(*this);
	#endif //_MSC_VER
}