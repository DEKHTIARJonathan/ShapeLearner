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
*	\author DEKHTIAR Jonathan
*/

#ifndef _GRAPH_
#define _GRAPH_

#include "allHeaders.h"
using namespace std;

class GraphClass; //Forward Declaration of the class contained in GraphClass.h
class ObjectClass; //Forward Declaration of the class contained in ObjectClass.h

class Graph
{
public:
	Graph(GraphClass* grClass, ObjectClass* objClass, unsigned int const view, string const name);
	
	void setID(const unsigned long id){
		idGraph = id;
	}

private:
	Graph() {}

	unsigned long idGraph;

	GraphClass* graphClass;
	ObjectClass* objectClass;

	string objectName; // The name of the image file.
	unsigned int viewNumber;

	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(Graph)
#pragma db member(Graph::idGraph) id auto
#pragma db member(Graph::graphClass) not_null on_delete(cascade)
#pragma db member(Graph::objectClass) not_null on_delete(cascade)
#pragma db member(Graph::objectName) not_null
#pragma db member(Graph::viewNumber)  default("1") not_null
#pragma db index(Graph::"index_Graph_graphClass") method("BTREE") member(graphClass)
#pragma db index(Graph::"index_Graph_objectClass") method("BTREE") member(objectClass)
#pragma db index(Graph::"index_Graph_objectName") unique method("BTREE") member(objectName)

#endif // _GRAPH_