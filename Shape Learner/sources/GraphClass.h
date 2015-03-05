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
*	\author DEKHTIAR Jonathan
*/

#ifndef _GRAPH_CLASS_
#define _GRAPH_CLASS_

#include "allHeaders.h"
using namespace std;

class GraphManager; // Forward Declaration of the class contained in graphManager.h

class GraphClass
{
public:

	GraphClass(const string& name, bool isDirect, bool isAcyclic);

	string getKey() const {return graphClassName;}
	void setKey(const string& key) {
		#ifdef _MSC_VER
			GraphManager::openManager().deleteObject(*this);
		#endif //_MSC_VER
		graphClassName = key;
		#ifdef _MSC_VER
			GraphManager::openManager().saveObject(*this);
		#endif //_MSC_VER
	}

	/* =========== Template function =========== */
	string getClassName() const { return "GraphClass"; }
	/* =========== Template function =========== */

private:
	GraphClass() {}
	string graphClassName;
	bool directGraph;
	bool acyclicGraph;

	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(GraphClass)
#pragma db member(GraphClass::graphClassName) id
#pragma db member(GraphClass::directGraph) default("0")
#pragma db member(GraphClass::acyclicGraph) default("0")

#endif //_GRAPH_CLASS_

