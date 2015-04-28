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
	};


}

#endif // _GRAPH_