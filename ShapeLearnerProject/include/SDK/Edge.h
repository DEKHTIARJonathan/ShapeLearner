/* ************* Begin file Edge.h ***************************************/
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
*	\file Edge.h
*	\brief Edge Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _EDGE_
#define _EDGE_

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

	class Edge
	{
	public:
		class Access {
			static boost::shared_ptr<Edge> createEdge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph);
		};

		unsigned long getKey() const;

		int getWeight() const;
		void setWeight(const int _weight, bool asynchronous = false);

		int getSourceDFSIndex() const;
		void setSourceDFSIndex(const int _sourceDFSIndex, bool asynchronous = false);

		int getTargetDFSIndex() const;
		void setTargetDFSIndex(const int _targetDFSIndex, bool asynchronous = false);

		/* =========== Template function =========== */
		string getClassName() const;
		/* =========== Template function =========== */

		void resynchronize();

	private:		
		Edge() {}
		Edge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph);

	};

}

#endif // _EDGE_