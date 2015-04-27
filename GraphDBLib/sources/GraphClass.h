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

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::GraphClass.h  - this should happen just once per project.\n")
	#include "stdafx.h"
	#include "allHeaders.h"
#else
	#include "odbHeaders.h"
#endif

using namespace std;
class StandardExcept;

namespace graphDBLib{
	class GraphDB; // Forward Declaration of the class contained in graphDB.h

	/*!
	*	\class GraphClass
	*	\brief Part of the Graph Data Model. A GraphClass represents the type of graph being used : ShockGraph, Precedance Graph, Topological Graph.
	*/
	class GraphClass
	{
	public:
		/*!
		*	\class GraphClass::Access
		*	\brief Limit instantiation only to GraphDB. Static subclass which role is only to execute its unique static method.
		*/
		class Access {
			friend class GraphDB;
			static  boost::shared_ptr<GraphClass> createGraphClass(string name, bool isDirect = false, bool isAcyclic = false){
				return  boost::shared_ptr<GraphClass>(new GraphClass(name, isDirect, isAcyclic));
			}
		};

		string getKey() const {return graphClassName;}

		bool getIsDirect() const {return directGraph;}
		void setIsDirect(const bool _directGraph);

		bool getIsAcyclicGraph() const {return acyclicGraph;}
		void setIsAcyclicGraph(const bool _acyclicGraph);

		vector<unsigned long> getGraphs();

		/* =========== Template function =========== */
		string getClassName() const { return "GraphClass"; }
		/* =========== Template function =========== */

	private:
		string graphClassName;
		bool directGraph;
		bool acyclicGraph;

		/*!
		*	\brief  Classical constructor needed to let ODB load objects from DB.
		*/
		GraphClass() {}
		GraphClass(string name, bool isDirect = false, bool isAcyclic = false);

		/*!
		*	\fn void updateInDB();
		*	\brief Update the object in the database.
		*/
		void updateInDB();

		/*!
		*	\fn string saveInDB();
		*	\brief Persist the object in the database.
		*/
		string saveInDB();

		/*!
		*	\brief Friendship required in order to let ODB manage the object.
		*/
		friend class odb::access;
	};

	#pragma db value(std::string) type("VARCHAR(255)")
	#pragma db object(GraphClass)
	#pragma db member(GraphClass::graphClassName) id
	#pragma db member(GraphClass::directGraph) default("0")
	#pragma db member(GraphClass::acyclicGraph) default("0")
}

#endif //_GRAPH_CLASS_
