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
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
using namespace graphDBLib;

Graph::Graph(boost::weak_ptr<GraphClass> _refGraphClass,boost::weak_ptr<ObjectClass> _refObjectClass, string const _objectName) :
	refGraphClass(_refGraphClass),
	refObjectClass(_refObjectClass),
	objectName(_objectName),
	idGraph(0),
	viewNumber(0),
	nodeCount(0),
	edgeCount(0),
	cumulativeMass(0),
	DAGCost(0),
	fileOffset(0),
	MaxTSVDimension(0),
	totalTSVSum(0),
	shape_xMax(0),
	shape_xMin(0),
	shape_yMax(0),
	shape_yMin(0),
	shape_Height(0),
	shape_Width(0),
	XMLSignature("")
	{
		idGraph = saveInDB();
		Logger::Log("New Object Instanciated : Graph key("+ to_string((_ULonglong)getKey())+")");
	}

void Graph::updateInDB(){
	#ifdef _MSC_VER
		GraphDB::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

unsigned long Graph::saveInDB(){
	#ifdef _MSC_VER
		return GraphDB::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

boost::weak_ptr<GraphClass> Graph::getParentGraphClass(){
	if(refGraphClass.expired())
		refGraphClass.swap(odb::boost::lazy_weak_ptr<GraphClass>(GraphDB::CommonInterface::getGraphClass(refGraphClass.object_id<GraphClass>())));
	return refGraphClass.get_eager();
}

boost::weak_ptr<ObjectClass> Graph::getParentObjectClass(){
	if(refObjectClass.expired())
		refObjectClass.swap(odb::boost::lazy_weak_ptr<ObjectClass>(GraphDB::CommonInterface::getObjectClass((string)refObjectClass.object_id<ObjectClass>())));
	return refObjectClass.get_eager();
}

vector<unsigned long> Graph::getNodes(){
	return GraphDB::ObjectInterface::getForeignRelations<NodeIdViewByGraph>(idGraph);
}

vector<unsigned long> Graph::getEdges(){
	return GraphDB::ObjectInterface::getForeignRelations<EdgeIdViewByGraph>(idGraph);
}

vector<unsigned long> Graph::getPoints(){
	return GraphDB::ObjectInterface::getForeignRelations<PointIdViewByGraph>(idGraph);
}

unsigned long Graph::getKey() const {
	return idGraph;
}

string Graph::getClassName() const {
	return "Graph";
}

unsigned long Graph::getView() const {
	return viewNumber;
}

void Graph::setView(const unsigned long _viewNumber) {
	viewNumber = _viewNumber;
	updateInDB();
}

unsigned int Graph::getNodeCount() const{
	return nodeCount;
}

void Graph::setNodeCount(const unsigned int _nodeCount){
	nodeCount = _nodeCount;
	updateInDB();
}

unsigned int Graph::getEdgeCount() const{
	return edgeCount;
}

void Graph::setEdgeCount(const unsigned int _edgeCount){
	edgeCount = _edgeCount;
	updateInDB();
}

string Graph::getObjectName() const {
	return objectName;
}

ShapeDims Graph::getShapeDimensions() const{
	ShapeDims rslt;
	rslt.xmin = shape_xMin;
	rslt.xmax = shape_xMax;
	rslt.ymin = shape_yMin;
	rslt.ymax = shape_yMax;

	return rslt;
}

void Graph::setShapeDimensions(const double _xmin, const double _xmax, const double _ymin, const double _ymax){
	shape_xMin = _xmin;
	shape_xMax = _xmax;
	shape_yMax = _ymax;
	shape_yMin = _ymin;
	shape_Height = _ymax - _ymin;
	shape_Width = _xmax - _xmin;
	updateInDB();
}

int Graph::getCumulativeMass() const{
	return cumulativeMass;
}

void Graph::setCumulativeMass(const int _cumulativeMass){
	cumulativeMass = _cumulativeMass;
	updateInDB();
}

double Graph::getFileOffset() const{
	return fileOffset;
}

void Graph::setFileOffset(const int _fileOffset){
	fileOffset = _fileOffset;
	updateInDB();
}

int Graph::getDAGCost() const{
	return DAGCost;
}

void Graph::setDAGCost(const int _DAGCost){
	DAGCost = _DAGCost;
	updateInDB();
}

int Graph::getMaxTSVDimension() const{
	return MaxTSVDimension;
}

void Graph::setMaxTSVDimension(const int _MaxTSVDimension){
	MaxTSVDimension = _MaxTSVDimension;
	updateInDB();
}

double Graph::getTotalTSVSum() const{
	return totalTSVSum;
}

void Graph::setTotalTSVSum(const double _totalTSVSum){
	totalTSVSum = _totalTSVSum;
	updateInDB();
}

string Graph::getXMLSignature() const{
	return XMLSignature;
}

void Graph::setXMLSignature(const string& str){
	XMLSignature = str;
	updateInDB();
}