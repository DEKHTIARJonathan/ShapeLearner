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
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
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
	Logger::Log("New Object Instanciated : Point("+ to_string((_ULonglong)getKey())+")");
}

void Point::setxCoord(const double _xCoord){
	xCoord = _xCoord;
	updateInDB();
}

void Point::setyCoord(const double _yCoord){
	yCoord = _yCoord;
	updateInDB();
}

void Point::setRadius(const double _radius){
	radius = _radius;
	updateInDB();
}

void Point::updateInDB(){
	#ifdef _MSC_VER
		ShapeLearner::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

unsigned long Point::saveInDB(){
	#ifdef _MSC_VER
		return ShapeLearner::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

boost::weak_ptr<Node> Point::getParentNode(){
	if(refNode.expired())
		refNode.swap(odb::boost::lazy_weak_ptr<Node>(ShapeLearner::CommonInterface::getNode(refNode.object_id<Node>())));
	return refNode.get_eager();	
}

boost::weak_ptr<Graph> Point::getParentGraph(){
	if(refGraph.expired())
		refGraph.swap(odb::boost::lazy_weak_ptr<Graph>(ShapeLearner::CommonInterface::getGraph(refGraph.object_id<Graph>())));
	return refGraph.get_eager();	
}

void Point::checkCorrectness(odb::callback_event e, odb::database&) const throw(ShapeLearnerExcept) {
	switch (e)
	{
		case odb::callback_event::pre_persist:
		{
			unsigned long idGraphPoint = refNode.lock()->getParentGraph().lock()->getKey();
			unsigned long idGraphNode = refGraph.lock()->getKey();

			if (idGraphPoint != idGraphNode)
				throw ShapeLearnerExcept((string)__FUNCTION__ ,"Impossible to save this Point. The Point is not in the same Graph as the linked Node.");
			break;
		}
		case odb::callback_event::pre_update:
			unsigned long idGraphPoint = refNode.lock()->getParentGraph().lock()->getKey();
			unsigned long idGraphNode = refGraph.lock()->getKey();

			if (idGraphPoint != idGraphNode)
				throw ShapeLearnerExcept((string)__FUNCTION__ ,"Impossible to update this Point. The Point is not in the same Graph as the linked Node.");
			break;
	}
}