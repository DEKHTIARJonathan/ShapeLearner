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

Edge::Edge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph, unsigned long _weight) :
	source(_source),
	target(_target),
	refGraph(_refGraph),
	weight(_weight),
	idEdge(0)
	{
		idEdge = saveInDB();
		Logger::Log("New Object Instanciated : Edge("+ to_string((_ULonglong)getKey())+")");
	}

void Edge::setWeight(const unsigned long _weight) {
	weight = _weight;
	updateInDB();
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