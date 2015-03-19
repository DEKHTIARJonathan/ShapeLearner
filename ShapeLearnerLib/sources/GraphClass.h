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
class ShapeLearnerExcept;

class GraphClass
{
public:
	class Access {
		friend class GraphManager;
		static  boost::shared_ptr<GraphClass> createGraphClass(string name, bool isDirect = false, bool isAcyclic = false){
			return  boost::shared_ptr<GraphClass>(new GraphClass(name, isDirect, isAcyclic));
		}
	};

	string getKey() const {return graphClassName;}
	void setKey(const string& key);

	bool getIsDirect() const {return directGraph;}
	void setIsDirect(const bool _directGraph);

	bool getIsAcyclicGraph() const {return acyclicGraph;}
	void setIsAcyclicGraph(const bool _acyclicGraph);

	vector<unsigned long> getGraphs();

	/* =========== Template function =========== */
	string getClassName() const { return "GraphClass"; }
	/* =========== Template function =========== */

	void removeFromDB();
	void updateInDB();
	string saveInDB();

private:
	GraphClass() {}
	GraphClass(string name, bool isDirect = false, bool isAcyclic = false);

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
