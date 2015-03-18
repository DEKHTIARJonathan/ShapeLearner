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
class GraphManager; // Forward Declaration of the class contained in graphManager.h

class Graph
{
public:
	class Access {
		friend class GraphManager;
		static boost::shared_ptr<Graph> createGraph(boost::weak_ptr<GraphClass> _graphClass, boost::weak_ptr<ObjectClass> _objectClass, string const _objectName, unsigned int const _viewNumber = 1){
			return boost::shared_ptr<Graph>(new Graph(_graphClass, _objectClass, _objectName, _viewNumber));
		}
	};	

	unsigned long getKey() const {return idGraph;}
	void setKey(const unsigned int key);

	string getObjectName() const {return objectName;}
	void setObjectName(const string& _objectName);

	unsigned long getView() const {return viewNumber;}
	void setView(const unsigned int _viewNumber);

	boost::weak_ptr<GraphClass> getParentGraphClass();
	boost::weak_ptr<ObjectClass> getParentObjectClass();

	/* =========== Template function =========== */
	string getClassName() const { return "Graph"; }
	/* =========== Template function =========== */

	void removeFromDB();
	void updateInDB();
	unsigned long saveInDB();

private:
	Graph() {}
	Graph(boost::weak_ptr<GraphClass> _graphClass, boost::weak_ptr<ObjectClass> _objectClass, string const _objectName, unsigned int const _viewNumber = 1);

	unsigned long idGraph;

	odb::boost::lazy_weak_ptr<GraphClass> refGraphClass;
	odb::boost::lazy_weak_ptr<ObjectClass> refObjectClass;

	string objectName; // The name of the image file.
	unsigned int viewNumber;

	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(Graph)
#pragma db member(Graph::idGraph) id auto
#pragma db member(Graph::refGraphClass) not_null on_delete(cascade)
#pragma db member(Graph::refObjectClass) not_null on_delete(cascade)
#pragma db member(Graph::objectName) not_null
#pragma db member(Graph::viewNumber)  default("1") not_null
#pragma db index(Graph::"index_Graph_graphClass") method("BTREE") member(refGraphClass)
#pragma db index(Graph::"index_Graph_objectClass") method("BTREE") member(refObjectClass)
#pragma db index(Graph::"index_Graph_objectName") unique method("BTREE") member(objectName)

#endif // _GRAPH_