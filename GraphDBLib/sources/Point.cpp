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
using namespace graphDBLib;

Point::Point(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph) :refGraph(_refGraph), refNode(_refNode), idPoint(0){
	xCoord = -1;
	yCoord = -1;
	radius = -1;
	direction = UNK_DIR;
	speed = -1;
	dr_ds = -1;
	//color = char('');
	dr = -1;

	idPoint = saveInDB();
	Logger::Log("New Object Instanciated : Point("+ to_string((_ULonglong)getKey())+")");
}

unsigned long Point::getKey() const {return idPoint;}

double Point::getxCoord() const {return xCoord;}
void Point::setxCoord(const double _xCoord, bool asynchronous){
	xCoord = _xCoord;
	if (!asynchronous)
		updateInDB();
}

double Point::getyCoord() const {return yCoord;}
void Point::setyCoord(const double _yCoord, bool asynchronous){
	yCoord = _yCoord;
	if (!asynchronous)
		updateInDB();
}

double Point::getRadius() const {return radius;}
void Point::setRadius(const double _radius, bool asynchronous){
	radius = _radius;
	if (!asynchronous)
		updateInDB();
}

double Point::getSpeed() const{return speed;}
void Point::setSpeed(const double _speed, bool asynchronous){
	speed = _speed;
	if (!asynchronous)
		updateInDB();
}

double Point::getDr_Ds() const{return dr_ds;}
void Point::setDr_Ds(const double _dr_ds, bool asynchronous){
	dr_ds = _dr_ds;
	if (!asynchronous)
		updateInDB();
}

double Point::getDr() const{return dr;}
void Point::setDr(const double _dr, bool asynchronous){
	dr = _dr;
	if (!asynchronous)
		updateInDB();
}

BRANCH_DIR Point::getDirection() const{return direction;}
void Point::setDirection(BRANCH_DIR _direction, bool asynchronous){
	direction = _direction;
	if (!asynchronous)
		updateInDB();
}

string Point::getClassName() const { return "Point"; }

void Point::updateInDB(){
		GraphDB::ObjectInterface::updateObject(*this);
}

unsigned long Point::saveInDB(){
		return GraphDB::ObjectInterface::saveObject(*this);
}

boost::weak_ptr<Node> Point::getParentNode(){
	if(refNode.expired())
		refNode.swap(odb::boost::lazy_weak_ptr<Node>(GraphDB::CommonInterface::getNode(refNode.object_id<Node>())));
	return refNode.get_eager();
}

boost::weak_ptr<Graph> Point::getParentGraph(){
	if(refGraph.expired())
		refGraph.swap(odb::boost::lazy_weak_ptr<Graph>(GraphDB::CommonInterface::getGraph(refGraph.object_id<Graph>())));
	return refGraph.get_eager();
}

void Point::checkCorrectness(odb::callback_event e, odb::database&) const throw(StandardExcept) {
	switch (e)
	{
		case odb::callback_event::pre_persist:
		{
			unsigned long idGraphPoint = refNode.lock()->getParentGraph().lock()->getKey();
			unsigned long idGraphNode = refGraph.lock()->getKey();

			if (idGraphPoint != idGraphNode)
				throw StandardExcept((string)__FUNCTION__ ,"Impossible to save this Point. The Point is not in the same Graph as the linked Node.");
			break;
		}
		case odb::callback_event::pre_update:
			unsigned long idGraphPoint = refNode.lock()->getParentGraph().lock()->getKey();
			unsigned long idGraphNode = refGraph.lock()->getKey();

			if (idGraphPoint != idGraphNode)
				throw StandardExcept((string)__FUNCTION__ ,"Impossible to update this Point. The Point is not in the same Graph as the linked Node.");
			break;
	}
}

void Point::resynchronize(){
	updateInDB();
}