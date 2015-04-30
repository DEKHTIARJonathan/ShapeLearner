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
using namespace graphDBLib;

Node::Node(boost::weak_ptr<Graph> _refGraph) : refGraph(_refGraph),	idNode(0) {
	index = -1;
	label = "";
	level = -1;
	mass = -1;
	type = -1;
	role = UNK_ROLE;
	pointCount = -1;
	contourLength1 = -1;
	contourLength2 = -1;
	subtreeCost = -1;
	tsvNorm = -1;
	idNode = saveInDB();
	Logger::Log("New Object Instanciated : Node("+ to_string((_ULonglong)getKey())+")");
}

boost::shared_ptr<Node> Node::Access::createNode(boost::weak_ptr<Graph> _refGraph){
	return boost::shared_ptr<Node>(new Node(_refGraph));
}

unsigned long Node::getKey() const {return idNode;}

int Node::getIndex() const {return index;}
void Node::setIndex(const int _index, bool asynchronous) {
	index = _index;
	if (!asynchronous)
		updateInDB();
}

int Node::getLevel() const {return level;}
void Node::setLevel(const int _level, bool asynchronous) {
	level = _level;
	if (!asynchronous)
		updateInDB();
}

int Node::getMass() const {return mass;}
void Node::setMass(const int _mass, bool asynchronous) {
	mass = _mass;
	if (!asynchronous)
		updateInDB();
}

int Node::getType() const {return type;}
void Node::setType(const int _type, bool asynchronous) {
	type = _type;
	if (!asynchronous)
		updateInDB();
}

string Node::getLabel() const {return label;}
void Node::setLabel(const string& _label, bool asynchronous) {
	label = _label;
	if (!asynchronous)
		updateInDB();
}

NODE_ROLE Node::getRole() const{return role;}
void Node::setRole(const NODE_ROLE _role, bool asynchronous){
	role = _role;
	if (!asynchronous)
		updateInDB();
}

double Node::getContourLength1() const{return contourLength1;}
void Node::setContourLength1(const double _contourLength1, bool asynchronous){
	contourLength1 = _contourLength1;
	if (!asynchronous)
		updateInDB();
}

double Node::getContourLength2() const{return contourLength2;}
void Node::setContourLength2(const double _contourLength2, bool asynchronous){
	contourLength2 = _contourLength2;
	if (!asynchronous)
		updateInDB();
}

double Node::getSubtreeCost() const{return subtreeCost;}
void Node::setSubtreeCost(const double _subtreeCost, bool asynchronous){
	subtreeCost = _subtreeCost;
	if (!asynchronous)
		updateInDB();
}

double Node::getTSVNorm() const{return tsvNorm;}
void Node::setTSVNorm(const double _tsvNorm, bool asynchronous){
	tsvNorm = _tsvNorm;
	if (!asynchronous)
		updateInDB();
}

int Node::getPointCount() const{return pointCount;}
void Node::setPointCount(const int _pointCount, bool asynchronous){
	pointCount = _pointCount;
	if (!asynchronous)
		updateInDB();
}
int Node::getPointCountFromDB() const {
	return GraphDB::ObjectInterface::getPointCountInNode(idNode);
}

string Node::getClassName() const {return "Node";}

void Node::updateInDB(){
	GraphDB::ObjectInterface::updateObject(*this);
}

int Node::saveInDB(){
	return GraphDB::ObjectInterface::saveObject(*this);
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

void Node::resynchronize(){
	updateInDB();
}