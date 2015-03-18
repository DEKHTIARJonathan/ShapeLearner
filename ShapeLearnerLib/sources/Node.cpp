/* ************ Begin file Node.cpp **************************************
**
** 2015 March 04
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file Node.cpp
*	\brief Node Cpp File
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

Node::Node(boost::weak_ptr<Graph> _refGraph, unsigned int _index, unsigned int _level, unsigned int _mass, unsigned int _type, string _label) :
	refGraph(_refGraph),
	index(_index),
	level(_level),
	mass(_mass),
	type(_type),
	label(_label),
	idNode(0)
	{
		idNode = saveInDB();
		#ifdef _VERBOSE_
			cout << "Node key : "+ to_string((_ULonglong)getKey()) <<endl;
		#endif
	}

void Node::setKey(const unsigned int key) {
	removeFromDB();
	idNode = key;
	saveInDB();
}

void Node::setIndex(const unsigned int _index) {
	index = _index;
	updateInDB();
}

void Node::setLevel(const unsigned int _level) {
	level = _level;
	updateInDB();
}

void Node::setMass(const unsigned int _mass) {
	mass = _mass;
	updateInDB();
}

void Node::setType(const unsigned int _type) {
	type = _type;
	updateInDB();
}

void Node::setLabel(const string& _label) {
	label = _label;
	updateInDB();
}

unsigned long Node::getPointCount() const {
	return GraphManager::ObjectInterface::getPointCountInNode(idNode);
}

void Node::removeFromDB(){
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::deleteObject(*this);
	#endif //_MSC_VER
}

void Node::updateInDB(){
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

unsigned long Node::saveInDB(){
	#ifdef _MSC_VER
		return GraphManager::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

boost::weak_ptr<Graph> Node::getParentGraph(){
	if(refGraph.expired())
		refGraph.swap(odb::boost::lazy_weak_ptr<Graph>(GraphManager::CommonInterface::getGraph(refGraph.object_id<Graph>())));
	return refGraph.get_eager();	
}