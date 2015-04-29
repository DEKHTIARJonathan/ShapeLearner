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
	class GraphDB; // Forward Declaration of the class contained in graphDB.h
	class Point; //Forward Declaration of the class contained in Point.h

	enum NODE_ROLE {UNK_ROLE = 0, BIRTH, PROTRUSION, UNION, DEATH, DUMMY};

	/*!
	*	\class Node
	*	\brief Part of the Graph Data Model. A Node is composed of many Points and can be link to many Edges. It is one of the key components of a Graph.
	*/
	class Node
	{
	public:
		/*!
		*	\class Node::Access
		*	\brief Limit instantiation only to GraphDB. Static subclass which role is only to execute its unique static method.
		*/
		class Access {
			friend class GraphDB;
			static boost::shared_ptr<Node> createNode(boost::weak_ptr<Graph> _refGraph){
				return boost::shared_ptr<Node>(new Node(_refGraph));
			}
		};

		unsigned long getKey() const;

		int getIndex() const;
		void setIndex(const int _index);

		string getLabel() const;
		void setLabel(const string& _label);

		int getLevel() const;
		void setLevel(const int _level);

		int getMass() const;
		void setMass(const int _mass);

		int getType() const;
		void setType(const int _type);

		NODE_ROLE getRole() const;
		void setRole(const NODE_ROLE _role);

		int getPointCount() const;
		void setPointCount(const int _pointCount);
		int getPointCountFromDB() const;

		double getContourLength1() const;
		void setContourLength1(const double _contourLength1);

		double getContourLength2() const;
		void setContourLength2(const double _contourLength2);

		double getSubtreeCost() const;
		void setSubtreeCost(const double _subtreeCost);

		double getTSVNorm() const;
		void setTSVNorm(const double _tsvNorm);

		boost::weak_ptr<Graph> getParentGraph();

		vector<unsigned long> getEdgesBySource();
		vector<unsigned long> getEdgesByTarget();
		vector<unsigned long> getEdges();
		vector<unsigned long> getPoints();

		/* =========== Template function =========== */
		string getClassName() const { return "Node"; }
		/* =========== Template function =========== */

	private:
		unsigned long	idNode;
		int				index; //!< Index relative to a depth-first search : nDFSIndex
		string			label; //!< Node's label : nodeLbl
		int				level; //!< Node level in the hierarchical structure : nLevel
		int				mass; //!< Node mass : nMass
		int				type; //!< Shock branch type (1, 2, 3, 4) : m_nType
		NODE_ROLE		role;
		int				pointCount;
		double			contourLength1; //!< Contour segment's lengths
		double			contourLength2; //!< Contour segment's lengths
		double			subtreeCost; //!< Cost of the subtree rooted at the node
		double			tsvNorm; //!< Node TSV's norm

		odb::boost::lazy_weak_ptr<Graph>	refGraph;

		/*!
		*	\brief  Classical constructor needed to let ODB load objects from DB.
		*/
		Node() {}
		Node(boost::weak_ptr<Graph> _refGraph);
	};
}

#endif // _NODE_