/* ************* Begin file Point.h ***************************************/
/*
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
*	\file Point.h
*	\brief Point Header
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _POINT_
#define _POINT_

#include "allHeaders.h"
using namespace std;

class Graph; //Forward Declaration of the class contained in Graph.h
class Node; //Forward Declaration of the class contained in Node.h
class GraphManager; // Forward Declaration of the class contained in graphManager.h

class Point
{
public:
	Point(Node* _refNode, Graph* _refGraph, double _xCoord = 0, double _yCoord = 0, double _radius = 1);
	
	unsigned long getKey() const {return idPoint;}
	void setKey(const unsigned int key);

	double getxCoord() const {return xCoord;}
	void setxCoord(const unsigned int _xCoord);

	double getyCoord() const {return yCoord;}
	void setyCoord(const unsigned int _yCoord);

	double getRadius() const {return radius;}
	void setRadius(const unsigned int _radius);

	/* =========== Template function =========== */
	string getClassName() const { return "Point"; }
	/* =========== Template function =========== */

	void removeFromDB();
	void updateInDB();
	unsigned long saveInDB();

private:
	Point() {}

	unsigned long idPoint;
	double xCoord;
	double yCoord;
	double radius;

	Graph* refGraph;
	Node* refNode;

	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(Point)
#pragma db member(Point::idPoint) id auto
#pragma db member(Point::refGraph) not_null on_delete(cascade)
#pragma db member(Point::refNode) not_null on_delete(cascade)
#pragma db member(Point::xCoord) default("0") not_null
#pragma db member(Point::yCoord) default("0") not_null
#pragma db member(Point::radius) default("1") not_null
#pragma db index(Point::"index_Point_refNode") method("BTREE") member(refNode)
#pragma db index(Point::"index_Point_refGraph") method("BTREE") member(refGraph)
#pragma db index(Point::"index_Point_region2D") method("BTREE") members(xCoord, yCoord)

#endif // _POINT_