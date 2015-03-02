/* ************ Begin file Graph.cpp **************************************
**
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
*	\file Graph.cpp
*	\brief Graph Cpp File
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

Graph::Graph(GraphClass* grClass, ObjectClass* objClass, unsigned int const view, string const name) : 
	graphClass(grClass), 
	objectClass(objClass), 
	viewNumber(view), 
	objectName(name), 
	idGraph(0) 
	{}
