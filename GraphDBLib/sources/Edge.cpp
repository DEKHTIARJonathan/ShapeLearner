/* ************ Begin file Edge.cpp **************************************
**
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
*	\file Edge.cpp
*	\brief Edge Cpp File
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
using namespace graphDBLib;

Edge::Edge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph) :
	source(_source),
	target(_target),
	refGraph(_refGraph),
	idEdge(0)
	{
		idEdge = saveInDB();
		Logger::Log("New Object Instanciated : Edge("+ to_string((_ULonglong)getKey())+")");
	}

void Edge::setWeight(const unsigned long _weight) {
	weight = _weight;
	updateInDB();
}

void Edge::updateInDB(){
	#ifdef _MSC_VER
		GraphDB::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

unsigned long Edge::saveInDB(){
	#ifdef _MSC_VER
		return GraphDB::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER
}

void Edge::checkCorrectness(odb::callback_event e, odb::database&) const throw(StandardExcept) {
	switch (e)
	{
		case odb::callback_event::pre_persist:
		{
			unsigned long idGraphSource = source.lock()->getParentGraph().lock()->getKey();
			unsigned long idGraphTarget = target.lock()->getParentGraph().lock()->getKey();
			unsigned long idGraphEdge = refGraph.lock()->getKey();

			if (idGraphSource != idGraphTarget || idGraphTarget != idGraphEdge)
				throw StandardExcept((string)__FUNCTION__ ,"Impossible to save this edge. The Nodes connected are not in the same graph or/and is different from Edge's Graph.");
			break;
		}
		case odb::callback_event::pre_update:
			unsigned long idGraphSource = source.lock()->getParentGraph().lock()->getKey();
			unsigned long idGraphTarget = target.lock()->getParentGraph().lock()->getKey();
			unsigned long idGraphEdge = refGraph.lock()->getKey();

			if (idGraphSource != idGraphTarget || idGraphTarget != idGraphEdge)
				throw StandardExcept((string)__FUNCTION__ ,"Impossible to update this edge. The Nodes connected are not in the same graph or/and is different from Edge's Graph.");
			break;
	}
}