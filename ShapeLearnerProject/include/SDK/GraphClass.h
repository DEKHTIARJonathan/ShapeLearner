/* ************* Begin file GraphClass.h ***************************************/
/*
** 2015 February 27
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file GraphClass.h
*	\brief GraphClass Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _GRAPH_CLASS_
#define _GRAPH_CLASS_

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

	class GraphDB;

	class GraphClass
	{
	public:
		class Access {
			static  boost::shared_ptr<GraphClass> createGraphClass(string name, bool isDirect = false, bool isAcyclic = false);
		};

		string getKey() const;

		bool getIsDirect() const;
		void setIsDirect(const bool _directGraph, bool asynchronous = false);

		bool getIsAcyclicGraph() const;
		void setIsAcyclicGraph(const bool _acyclicGraph, bool asynchronous = false);

		vector<unsigned long> getGraphs();

		/* =========== Template function =========== */
		string getClassName() const;
		/* =========== Template function =========== */

		void resynchronize();

	private:
		string graphClassName;
		bool directGraph;
		bool acyclicGraph;
		
		/*!
		*	\brief  Classical constructor needed to let ODB load objects from DB.
		*/
		GraphClass() {}
		GraphClass(string name, bool isDirect = false, bool isAcyclic = false);
	};
}

#endif //_GRAPH_CLASS_
