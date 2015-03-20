/* ************ Begin file Point.cpp **************************************
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
*	\file Point.cpp
*	\brief Point Cpp File
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

Point::Point(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph, double _xCoord, double _yCoord, double _radius) :
xCoord(_xCoord),
yCoord(_yCoord),
radius(_radius),
refGraph(_refGraph),
refNode(_refNode),
idPoint(0)
{
	idPoint = saveInDB();
	#ifdef _VERBOSE_
		cout << "Point key : "+ to_string((_ULonglong)getKey()) <<endl;
	#endif
}

void Point::setxCoord(const unsigned int _xCoord){
	xCoord = _xCoord;
	updateInDB();
}

void Point::setyCoord(const unsigned int _yCoord){
	yCoord = _yCoord;
	updateInDB();
}

void Point::setRadius(const unsigned int _radius){
	radius = _radius;
	updateInDB();
}

void Point::updateInDB(){
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

unsigned long Point::saveInDB(){
	#ifdef _MSC_VER
		return GraphManager::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

boost::weak_ptr<Node> Point::getParentNode(){
	if(refNode.expired())
		refNode.swap(odb::boost::lazy_weak_ptr<Node>(GraphManager::CommonInterface::getNode(refNode.object_id<Node>())));
	return refNode.get_eager();	
}

boost::weak_ptr<Graph> Point::getParentGraph(){
	if(refGraph.expired())
		refGraph.swap(odb::boost::lazy_weak_ptr<Graph>(GraphManager::CommonInterface::getGraph(refGraph.object_id<Graph>())));
	return refGraph.get_eager();	
}