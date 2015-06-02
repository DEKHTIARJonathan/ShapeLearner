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

#include <odb/core.hxx>
#include <odb/callback.hxx>
#include <odb/boost/lazy-ptr.hxx>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/schema-catalog.hxx>

using namespace std;

namespace graphDBLib{
	class Graph;
	class Node;
	class GraphDB;

	enum BRANCH_DIR {UNK_DIR = -1 ,FORWARD, BACKWARD};

	class Point
	{
	public:
		class Access {
			static boost::shared_ptr<Point> createPoint(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph);
		};

		unsigned long getKey() const;

		double getxCoord() const;
		void setxCoord(const double _xCoord, bool asynchronous = false);

		double getyCoord() const;
		void setyCoord(const double _yCoord, bool asynchronous = false);

		double getRadius() const;
		void setRadius(const double _radius, bool asynchronous = false);
		
		double getSpeed() const;
		void setSpeed(const double _speed, bool asynchronous = false);

		double getDr_Ds() const;
		void setDr_Ds(const double _dr_ds, bool asynchronous = false);

		double getDr() const;
		void setDr(const double _dr, bool asynchronous = false);

		BRANCH_DIR getDirection() const;
		void setDirection(BRANCH_DIR _direction, bool asynchronous = false);

		boost::weak_ptr<Node> getParentNode();
		boost::weak_ptr<Graph> getParentGraph();

		/* =========== Template function =========== */
		string getClassName() const;
		/* =========== Template function =========== */

		void resynchronize();

	protected:
		Point() {}
		Point(boost::weak_ptr<Node> _refNode, boost::weak_ptr<Graph> _refGraph);

	};

}

#endif // _POINT_