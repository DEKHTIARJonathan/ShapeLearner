/* ************ Begin file GraphClass.cpp **************************************
**
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
*	\file GraphClass.cpp
*	\brief GraphClass Cpp File
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

GraphClass::GraphClass(const string& name, bool isDirect, bool isAcyclic) : graphClassName(name), directGraph(isDirect), acyclicGraph(isAcyclic) {
	GraphManager::openManager().saveObject(*this);
}
