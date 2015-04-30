/* ************* Begin file Node.h ***************************************/
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
*	\file Node.h
*	\brief Node Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _NODE_
#define _NODE_

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
	class GraphDB;
	class Point;

	enum NODE_ROLE {UNK_ROLE = 0, BIRTH, PROTRUSION, UNION, DEATH, DUMMY};

	class Node
	{
	public:
		class Access {
			static boost::shared_ptr<Node> createNode(boost::weak_ptr<Graph> _refGraph);
		};

		unsigned long getKey() const;

		int getIndex() const;
		void setIndex(const int _index, bool asynchronous = false);

		string getLabel() const;
		void setLabel(const string& _label, bool asynchronous = false);

		int getLevel() const;
		void setLevel(const int _level, bool asynchronous = false);

		int getMass() const;
		void setMass(const int _mass, bool asynchronous = false);

		int getType() const;
		void setType(const int _type, bool asynchronous = false);

		NODE_ROLE getRole() const;
		void setRole(const NODE_ROLE _role, bool asynchronous = false);

		int getPointCount() const;
		void setPointCount(const int _pointCount, bool asynchronous = false);
		int getPointCountFromDB() const;

		double getContourLength1() const;
		void setContourLength1(const double _contourLength1, bool asynchronous = false);

		double getContourLength2() const;
		void setContourLength2(const double _contourLength2, bool asynchronous = false);

		double getSubtreeCost() const;
		void setSubtreeCost(const double _subtreeCost, bool asynchronous = false);

		double getTSVNorm() const;
		void setTSVNorm(const double _tsvNorm, bool asynchronous = false);

		boost::weak_ptr<Graph> getParentGraph();

		vector<unsigned long> getEdgesBySource();
		vector<unsigned long> getEdgesByTarget();
		vector<unsigned long> getEdges();
		vector<unsigned long> getPoints();

		/* =========== Template function =========== */
		string getClassName() const;
		/* =========== Template function =========== */

		void resynchronize();

	private:
		unsigned long	idNode;
		int				index;
		string			label;
		int				level;
		int				mass;
		int				type;
		NODE_ROLE		role;
		int				pointCount;
		double			contourLength1;
		double			contourLength2;
		double			subtreeCost;
		double			tsvNorm;

		odb::boost::lazy_weak_ptr<Graph>	refGraph;

		Node() {}
		Node(boost::weak_ptr<Graph> _refGraph);
	};
}

#endif // _NODE_