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
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _POINT_
#define _POINT_

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::Point.h  - this should happen just once per project.\n")
	#include "stdafx.h"
	#include "allHeaders.h"
#else
	#include "odbHeaders.h"
#endif

using namespace std;

namespace graphDBLib{
	class Graph; //Forward Declaration of the class contained in Graph.h
	class Node; //Forward Declaration of the class contained in Node.h
	class GraphDB; // Forward Declaration of the class contained in graphDB.h

	/*!
	*	\class Point
	*	\brief Part of the Graph Data Model. A Node is composed of many Points. Only useful in case of using shockGraph, it draws the shape of the Node.
	*/
	class Point
	{
	public:
		/*!
		*	\class Point::Access
		*	\brief Limit instantiation only to GraphDB. Static subclass which role is only to execute its unique static method.
		*/
		class Access {
			friend class GraphDB;
			/*!
			*	\fn static boost::shared_ptr<Point> createPoint(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph, double _xCoord = 0, double _yCoord = 0, double _radius = 1);
			*	\brief Return a boost::shared_ptr<Point> on a newly created Point.
			*	\param _refNode : The Node which contains the new Point.
			*	\param _refGraph : The Graph which contains the new Point.
			*	\param _xCoord : The "X" Coordinate of the Point.
			*	\param _yCoord : The "Y" Coordinate of the Point.
			*	\param _radius : The "Radius" of the Point.
			*/
			static boost::shared_ptr<Point> createPoint(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph, double _xCoord = 0, double _yCoord = 0, double _radius = 1){
				return boost::shared_ptr<Point>(new Point(_refNode, _refGraph, _xCoord, _yCoord , _radius));
			}
		};

		unsigned long getKey() const {return idPoint;}

		double getxCoord() const {return xCoord;}
		void setxCoord(const double _xCoord);

		double getyCoord() const {return yCoord;}
		void setyCoord(const double _yCoord);

		double getRadius() const {return radius;}
		void setRadius(const double _radius);

		boost::weak_ptr<Node> getParentNode();
		boost::weak_ptr<Graph> getParentGraph();

		/* =========== Template function =========== */
		string getClassName() const { return "Point"; }
		/* =========== Template function =========== */

	protected:
		unsigned long idPoint;
		double xCoord;
		double yCoord;
		double radius;

		odb::boost::lazy_weak_ptr<Graph> refGraph;
		odb::boost::lazy_weak_ptr<Node> refNode;

		/*!
		*	\brief  Classical constructor needed to let ODB load objects from DB.
		*/
		Point() {}
		Point(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph, double _xCoord = 0, double _yCoord = 0, double _radius = 1);

		/*!
		*	\fn void updateInDB();
		*	\brief Update the object in the database.
		*/
		void updateInDB();

		/*!
		*	\fn unsigned long saveInDB();
		*	\brief Persist the object in the database.
		*/
		unsigned long saveInDB();

		/*!
		*	\fn void checkCorrectness(odb::callback_event e, odb::database&) const throw(StandardExcept);
		*	\brief C++ Trigger Launched on each DB Operation on this object.
		*/
		void checkCorrectness(odb::callback_event e, odb::database&) const throw(StandardExcept);

		/*!
		*	\brief Friendship required in order to let ODB manage the object.
		*/
		friend class odb::access;
	};

	#pragma db value(std::string) type("VARCHAR(255)")
	#pragma db object(Point) callback(checkCorrectness)
	#pragma db member(Point::idPoint) id auto
	#pragma db member(Point::refGraph) not_null on_delete(cascade)
	#pragma db member(Point::refNode) not_null on_delete(cascade)
	#pragma db member(Point::xCoord) default("0") not_null
	#pragma db member(Point::yCoord) default("0") not_null
	#pragma db member(Point::radius) default("1") not_null
	#pragma db index(Point::"index_Point_refNode") method("BTREE") member(refNode)
	#pragma db index(Point::"index_Point_refGraph") method("BTREE") member(refGraph)
	#pragma db index(Point::"index_Point_region2D") method("BTREE") members(xCoord, yCoord)

	#pragma db view object(Point)
	struct pointsInNode
	{
	  #pragma db column("count(1)")
	  int value;
	};

	#pragma db view object(Point) query("\"refGraph\" = ")
	struct PointIdViewByGraph
	{
	  #pragma db column("idPoint")
	  unsigned long id;
	};

	#pragma db view object(Point) query("\"refNode\" = ")
	struct PointIdViewByNode
	{
	  #pragma db column("idPoint")
	  unsigned long id;
	};
}

#endif // _POINT_