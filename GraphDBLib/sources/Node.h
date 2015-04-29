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

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::Node.h  - this should happen just once per project.\n")
	#include "stdafx.h"
	#include "allHeaders.h"
#else
	#include "odbHeaders.h"
#endif

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

		/*!
		*	\fn void updateInDB();
		*	\brief Update the object in the database.
		*/
		void updateInDB();

		/*!
		*	\fn int saveInDB();
		*	\brief Persist the object in the database.
		*/
		int saveInDB();

		/*!
		*	\brief Friendship required in order to let ODB manage the object.
		*/
		friend class odb::access;
	};

	#pragma db value(std::string) type("VARCHAR(255)")
	#pragma db object(Node)
	#pragma db member(Node::idNode) id auto
	#pragma db member(Node::index) default("-1")
	#pragma db member(Node::level) default("-1")
	#pragma db member(Node::mass) default("-1")
	#pragma db member(Node::type) default("-1")
	#pragma db member(Node::label) default("-1")
	#pragma db member(Node::pointCount) default("-1")
	#pragma db member(Node::contourLength1) default("-1")
	#pragma db member(Node::contourLength2) default("-1")
	#pragma db member(Node::subtreeCost) default("-1")
	#pragma db member(Node::tsvNorm) default("-1")
	#pragma db member(Node::role) default(UNK_ROLE)

	#pragma db member(Node::refGraph) not_null on_delete(cascade)
	#pragma db index(Node::"Node_RefGraph") method("BTREE") member(refGraph)

	#pragma db view object(Node) query("\"refGraph\" = ")
	struct NodeIdViewByGraph
	{
	  #pragma db column("idNode")
	  unsigned long id;
	};
}

#endif // _NODE_