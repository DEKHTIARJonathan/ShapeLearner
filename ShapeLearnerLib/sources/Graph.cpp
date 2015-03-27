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

Graph::Graph(boost::weak_ptr<GraphClass> _refGraphClass,boost::weak_ptr<ObjectClass> _refObjectClass, string const _objectName, unsigned long const _viewNumber) :
	refGraphClass(_refGraphClass),
	refObjectClass(_refObjectClass),
	viewNumber(_viewNumber),
	objectName(_objectName),
	idGraph(0)
	{
		idGraph = saveInDB();
		Logger::Log("New Object Instanciated : Graph key("+ to_string((_ULonglong)getKey())+")");
	}

void Graph::setObjectName(const string& _objectName) {
	objectName = _objectName;
	updateInDB();
}

void Graph::setView(const unsigned long _viewNumber) {
	viewNumber = _viewNumber;
	updateInDB();
}

void Graph::updateInDB(){
	#ifdef _MSC_VER
		ShapeLearner::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

unsigned long Graph::saveInDB(){
	#ifdef _MSC_VER
		return ShapeLearner::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

boost::weak_ptr<GraphClass> Graph::getParentGraphClass(){
	if(refGraphClass.expired())
		refGraphClass.swap(odb::boost::lazy_weak_ptr<GraphClass>(ShapeLearner::CommonInterface::getGraphClass(refGraphClass.object_id<GraphClass>())));
	return refGraphClass.get_eager();	
}

boost::weak_ptr<ObjectClass> Graph::getParentObjectClass(){
	if(refObjectClass.expired())
		refObjectClass.swap(odb::boost::lazy_weak_ptr<ObjectClass>(ShapeLearner::CommonInterface::getObjectClass((string)refObjectClass.object_id<ObjectClass>())));
	return refObjectClass.get_eager();	
}

vector<unsigned long> Graph::getNodes(){
	return ShapeLearner::ObjectInterface::getForeignRelations<NodeIdViewByGraph>(idGraph);
}

vector<unsigned long> Graph::getEdges(){
	return ShapeLearner::ObjectInterface::getForeignRelations<EdgeIdViewByGraph>(idGraph);
}

vector<unsigned long> Graph::getPoints(){
	return ShapeLearner::ObjectInterface::getForeignRelations<PointIdViewByGraph>(idGraph);
}