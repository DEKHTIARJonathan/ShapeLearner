/* ************* Begin file Graph.h ***************************************/
/*
** 2015 March 02
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file Graph.h
*	\brief Graph Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _GRAPH_
#define _GRAPH_

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::Graph.h  - this should happen just once per project.\n")
	#include "stdafx.h"
	#include "allHeaders.h"
#else
	#include "odbHeaders.h"
#endif

using namespace std;

namespace graphDBLib{
	class GraphClass; //Forward Declaration of the class contained in GraphClass.h
	class ObjectClass; //Forward Declaration of the class contained in ObjectClass.h
	class GraphDB; // Forward Declaration of the class contained in graphDB.h

	struct ShapeDims
	{
		double xmin, xmax, ymin, ymax;
	};

	/*!
	*	\class GraphClass
	*	\brief Part of the Graph Data Model. It is the center piece of the modeL. It has one ObjectClass and one GraphClass. It is composed of many Nodes, Edges, Points...
	*/
	class Graph
	{
	public:
		/*!
		*	\class GraphClass::Access
		*	\brief Limit instantiation only to GraphDB. Static subclass which role is only to execute its unique static method.
		*/
		class Access {
			friend class GraphDB;
			static boost::shared_ptr<Graph> createGraph(boost::weak_ptr<GraphClass> _graphClass, boost::weak_ptr<ObjectClass> _objectClass, string const _objectName){
				return boost::shared_ptr<Graph>(new Graph(_graphClass, _objectClass, _objectName));
			}
		};

		unsigned long getKey() const;

		string getObjectName() const;

		unsigned long getView() const;
		void setView(const unsigned long _viewNumber);

		unsigned int getNodeCount() const;
		void setNodeCount(const unsigned int _nodeCount);

		unsigned int getEdgeCount() const;
		void setEdgeCount(const unsigned int _edgeCount);

		int getCumulativeMass() const;
		void setCumulativeMass(const int _cumulativeMass);

		double getFileOffset() const;
		void setFileOffset(const int _fileOffset);

		int getDAGCost() const;
		void setDAGCost(const int _DAGCost);

		int getMaxTSVDimension() const;
		void setMaxTSVDimension(const int _MaxTSVDimension);

		double getTotalTSVSum() const;
		void setTotalTSVSum(const double _totalTSVSum);

		ShapeDims  getShapeDimensions() const;
		void setShapeDimensions(const double _xmin, const double _xmax, const double _ymin, const double _ymax);

		string getXMLSignature() const;
		void setXMLSignature(const string& str);

		boost::weak_ptr<GraphClass> getParentGraphClass();
		boost::weak_ptr<ObjectClass> getParentObjectClass();

		vector<unsigned long> getNodes();
		vector<unsigned long> getEdges();
		vector<unsigned long> getPoints();

		/* =========== Template function =========== */
		string getClassName() const;
		/* =========== Template function =========== */

	private:
		unsigned long idGraph;
		string objectName;			//!< The name of the Image File.
		unsigned long viewNumber;	//!< Object View

		unsigned int nodeCount;
		unsigned int edgeCount;

		int cumulativeMass;			//!< Sum of all the nodes' masses
		double DAGCost;				//!< Som of all node and edge costs
		int fileOffset;				//!< file offset where this DAG is located.
		int MaxTSVDimension;		//!< Maximum branching factor of the DAG : m_nMaxBFactor + 1;
		double totalTSVSum;			//!< Sum of all the node's TSV magnitudes.

		/* Shape Infos */
		double shape_xMax;
		double shape_xMin;
		double shape_yMax;
		double shape_yMin;
		double shape_Height;
		double shape_Width;

		string XMLSignature;

		odb::boost::lazy_weak_ptr<GraphClass> refGraphClass;
		odb::boost::lazy_weak_ptr<ObjectClass> refObjectClass;

		/*!
		*	\brief  Classical constructor needed to let ODB load objects from DB.
		*/
		Graph() {}
		Graph(boost::weak_ptr<GraphClass> _graphClass, boost::weak_ptr<ObjectClass> _objectClass, string const _objectName);

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
		*	\brief Friendship required in order to let ODB manage the object.
		*/
		friend class odb::access;
	};

	#pragma db value(std::string) type("VARCHAR(255)")
	#pragma db object(Graph)
	#pragma db member(Graph::idGraph) id auto
	#pragma db member(Graph::refGraphClass) not_null on_delete(cascade)
	#pragma db member(Graph::refObjectClass) not_null on_delete(cascade)
	#pragma db member(Graph::objectName) not_null
	#pragma db member(Graph::nodeCount)
	#pragma db member(Graph::edgeCount)
	#pragma db member(Graph::cumulativeMass)
	#pragma db member(Graph::fileOffset)
	#pragma db member(Graph::DAGCost)
	#pragma db member(Graph::MaxTSVDimension)
	#pragma db member(Graph::totalTSVSum)
	#pragma db member(Graph::viewNumber)
	#pragma db member(Graph::shape_xMax)
	#pragma db member(Graph::shape_xMin)
	#pragma db member(Graph::shape_yMax)
	#pragma db member(Graph::shape_yMin)
	#pragma db member(Graph::shape_Height)
	#pragma db member(Graph::shape_Width)
	#pragma db member(Graph::XMLSignature) type("TEXT")
	#pragma db index(Graph::"index_Graph_graphClass") method("BTREE") member(refGraphClass)
	#pragma db index(Graph::"index_Graph_objectClass") method("BTREE") member(refObjectClass)
	#pragma db index(Graph::"index_Graph_objectName") unique method("BTREE") member(objectName)

	#pragma db view object(Graph) query("\"refGraphClass\" = ")
	struct GraphIdViewByGraphClass
	{
	  #pragma db column("idGraph")
	  unsigned long id;
	};

	#pragma db view object(Graph) query("\"refObjectClass\" = ")
	struct GraphIdViewByObjectClass
	{
	  #pragma db column("idGraph")
	  unsigned long id;
	};
}

#endif // _GRAPH_