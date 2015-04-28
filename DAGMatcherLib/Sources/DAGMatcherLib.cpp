/* ************* Begin file DAGMatcherLib.cpp ***************************************/
/*
** 2015 April 28
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file DAGMatcherLib.cpp
*	\brief DAGMatcherLib Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"

void dml::DAGMatcherLib::InitDAGMatcherLib() {
	graphDBLib::GraphDB::CommonInterface::getGraphClass("BoneGraph", true, true);
	graphDBLib::GraphDB::CommonInterface::getGraphClass("GestureGraph", true, true);
	graphDBLib::GraphDB::CommonInterface::getGraphClass("ShockGraph", true, true);
}