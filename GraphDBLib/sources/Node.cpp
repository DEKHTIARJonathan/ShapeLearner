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

unsigned long Node::getKey() const {return idNode;}

int Node::getIndex() const {
	return index;
}
void Node::setIndex(const int _index) {
	index = _index;
	updateInDB();
}

int Node::getLevel() const {
	return level;
}
void Node::setLevel(const int _level) {
	level = _level;
	updateInDB();
}

int Node::getMass() const {
	return mass;
}
void Node::setMass(const int _mass) {
	mass = _mass;
	updateInDB();
}

int Node::getType() const {
	return type;
}
void Node::setType(const int _type) {
	type = _type;
	updateInDB();
}

string Node::getLabel() const {
	return label;
}
void Node::setLabel(const string& _label) {
	label = _label;
	updateInDB();
}

NODE_ROLE Node::getRole() const{
	return role;
}
void Node::setRole(const NODE_ROLE _role){
	role = _role;
	updateInDB();
}

double Node::getContourLength1() const{
	return contourLength1;
}
void Node::setContourLength1(const double _contourLength1){
	contourLength1 = _contourLength1;
	updateInDB();
}

double Node::getContourLength2() const{
	return contourLength2;
}
void Node::setContourLength2(const double _contourLength2){
	contourLength2 = _contourLength2;
	updateInDB();
}

double Node::getSubtreeCost() const{
	return subtreeCost;
}
void Node::setSubtreeCost(const double _subtreeCost){
	subtreeCost = _subtreeCost;
	updateInDB();
}

double Node::getTSVNorm() const{
	return tsvNorm;
}
void Node::setTSVNorm(const double _tsvNorm){
	tsvNorm = _tsvNorm;
	updateInDB();
}

int Node::getPointCount() const{
	return pointCount;
}
void Node::setPointCount(const int _pointCount){
	pointCount = _pointCount;
	updateInDB();
}
int Node::getPointCountFromDB() const {
	return GraphDB::ObjectInterface::getPointCountInNode(idNode);
}

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