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

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include "StandardException.h"
#include "SDK/GraphClass.h"
#include "SDK/ObjectClass.h"
#include "SDK/Graph.h"
#include "SDK/Node.h"
#include "SDK/Point.h"
#include "SDK/Edge.h"

// ODB Classes
#include <odb/core.hxx>
#include <odb/callback.hxx>
#include <odb/boost/lazy-ptr.hxx>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/schema-catalog.hxx>


using namespace std;

namespace graphDBLib{
	class Graph; //Forward Declaration of the class contained in Graph.h
	class Node; //Forward Declaration of the class contained in Node.h
	class GraphDB; // Forward Declaration of the class contained in graphDB.h

	enum BRANCH_DIR {UNK_DIR = -1 ,FORWARD, BACKWARD};

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
			*	\fn static boost::shared_ptr<Point> createPoint(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph);
			*	\brief Return a boost::shared_ptr<Point> on a newly created Point.
			*	\param _refNode : The Node which contains the new Point.
			*	\param _refGraph : The Graph which contains the new Point.
			*/
			static boost::shared_ptr<Point> createPoint(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph){
				return boost::shared_ptr<Point>(new Point(_refNode, _refGraph));
			}
		};

		unsigned long getKey() const;

		double getxCoord() const;
		void setxCoord(const double _xCoord);

		double getyCoord() const;
		void setyCoord(const double _yCoord);

		double getRadius() const;
		void setRadius(const double _radius);
		
		double getSpeed() const;
		void setSpeed(const double _speed);

		double getDr_Ds() const;
		void setDr_Ds(const double _dr_ds);

		char getColor() const;
		void setColor(const char _color);

		double getDr() const;
		void setDr(const double _dr);

		int getType() const;
		void setType(const int _type);

		BRANCH_DIR getDirection() const;
		void setDirection(BRANCH_DIR _direction);

		boost::weak_ptr<Node> getParentNode();
		boost::weak_ptr<Graph> getParentGraph();

		/* =========== Template function =========== */
		string getClassName() const { return "Point"; }
		/* =========== Template function =========== */

	protected:
		unsigned long idPoint;
		double		xCoord; //!< X coordinate of branch points.
		double		yCoord; //!< Y coordinate of branch points.
		double		radius; //!< Radii at the branch points.
		double		speed;	//!< Velocities at the branch points.
		double		dr_ds;
		char		color;
		double		dr;		//!< Delta radius. Diff in radius with that of the previous point
		int			type;
		BRANCH_DIR	direction;		//!< Direction: increasing=1, decreasing=-1, constant=0

		odb::boost::lazy_weak_ptr<Graph> refGraph;
		odb::boost::lazy_weak_ptr<Node> refNode;

		/*!
		*	\brief  Classical constructor needed to let ODB load objects from DB.
		*/
		Point() {}
		Point(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph);

	};

}

#endif // _POINT_