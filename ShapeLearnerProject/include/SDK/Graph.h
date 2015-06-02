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

#include <odb/core.hxx>
#include <odb/callback.hxx>
#include <odb/boost/lazy-ptr.hxx>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/schema-catalog.hxx>

using namespace std;

namespace graphDBLib{

	class GraphClass;
	class ObjectClass;
	class GraphDB;

	struct ShapeDims
	{
		double xmin, xmax, ymin, ymax;
	};
	
	class Graph
	{
	public:
		class Access {
			static boost::shared_ptr<Graph> createGraph(boost::weak_ptr<GraphClass> _graphClass, boost::weak_ptr<ObjectClass> _objectClass, string const _objectName);
		};

		unsigned long getKey() const;

		string getObjectName() const;

		int getCumulativeMass() const;
		void setCumulativeMass(const int _cumulativeMass, bool asynchronous = false);

		int getDAGCost() const;
		void setDAGCost(const int _DAGCost, bool asynchronous = false);

		int getMaxTSVDimension() const;
		void setMaxTSVDimension(const int _MaxTSVDimension, bool asynchronous = false);

		double getTotalTSVSum() const;
		void setTotalTSVSum(const double _totalTSVSum, bool asynchronous = false);

		ShapeDims  getShapeDimensions() const;
		void setShapeDimensions(const double _xmin, const double _xmax, const double _ymin, const double _ymax, bool asynchronous = false);

		string getXMLSignature() const;
		void setXMLSignature(const string& str, bool asynchronous = false);

		boost::weak_ptr<GraphClass> getParentGraphClass();
		boost::weak_ptr<ObjectClass> getParentObjectClass();

		vector<unsigned long> getNodes();
		vector<unsigned long> getEdges();
		vector<unsigned long> getPoints();

		/* =========== Template function =========== */
		string getClassName() const;
		/* =========== Template function =========== */

		void resynchronize();

	private:
		Graph() {}
		Graph(boost::weak_ptr<GraphClass> _graphClass, boost::weak_ptr<ObjectClass> _objectClass, string const _objectName);
	};


}

#endif // _GRAPH_