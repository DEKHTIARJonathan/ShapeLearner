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
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;

Node::Node(boost::weak_ptr<Graph> _refGraph, unsigned long _index, unsigned long _level, unsigned long _mass, unsigned long _type, string _label) :
	refGraph(_refGraph),
	index(_index),
	level(_level),
	mass(_mass),
	type(_type),
	label(_label),
	idNode(0)
	{
		idNode = saveInDB();
		Logger::Log("New Object Instanciated : Node("+ to_string((_ULonglong)getKey())+")");
	}


void Node::setIndex(const unsigned long _index) {
	index = _index;
	updateInDB();
}

void Node::setLevel(const unsigned long _level) {
	level = _level;
	updateInDB();
}

void Node::setMass(const unsigned long _mass) {
	mass = _mass;
	updateInDB();
}

void Node::setType(const unsigned long _type) {
	type = _type;
	updateInDB();
}

void Node::setLabel(const string& _label) {
	label = _label;
	updateInDB();
}

unsigned long Node::getPointCount() const {
	return GraphDB::ObjectInterface::getPointCountInNode(idNode);
}

void Node::updateInDB(){
	#ifdef _MSC_VER
		GraphDB::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

unsigned long Node::saveInDB(){
	#ifdef _MSC_VER
		return GraphDB::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

boost::weak_ptr<Graph> Node::getParentGraph(){
	if(refGraph.expired())
		refGraph.swap(odb::boost::lazy_weak_ptr<Graph>(GraphDB::CommonInterface::getGraph(refGraph.object_id<Graph>())));
	return refGraph.get_eager();	
}

vector<unsigned long> Node::getEdgesBySource(){
	return GraphDB::ObjectInterface::getForeignRelations<EdgeIdViewBySource>(idNode);
}

vector<unsigned long> Node::getEdgesByTarget(){
	return GraphDB::ObjectInterface::getForeignRelations<EdgeIdViewByTarget>(idNode);
}

vector<unsigned long> Node::getEdges(){
	vector <unsigned long> rslt, rslt1, rslt2;
	rslt1 = getEdgesBySource();
	rslt2 = getEdgesByTarget();
	rslt.reserve( rslt1.size() + rslt2.size() );
	rslt.insert( rslt.end(), rslt1.begin(), rslt1.end() );
	rslt.insert( rslt.end(), rslt2.begin(), rslt2.end() );
	
	return rslt;
}

vector<unsigned long> Node::getPoints(){
	return GraphDB::ObjectInterface::getForeignRelations<PointIdViewByNode>(idNode);
}