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

boost::shared_ptr<Edge> Edge::Access::createEdge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph){
	return boost::shared_ptr<Edge>(new Edge(_source, _target, _refGraph));
}

Edge::Edge(boost::weak_ptr<Node> _source, boost::weak_ptr<Node> _target, boost::weak_ptr<Graph> _refGraph) :
	source(_source),
	target(_target),
	refGraph(_refGraph),
	idEdge(0)
	{
		weight = -1;
		sourceDFSIndex = -1;
		targetDFSIndex = -1;
		idEdge = saveInDB();
		Logger::Log("New Object Instanciated : Edge("+ to_string((_ULonglong)getKey())+")");
	}

unsigned long Edge::getKey() const {return idEdge;}

int Edge::getWeight() const {return weight;}
void Edge::setWeight(const int _weight, bool asynchronous) {
	weight = _weight;
	if (!asynchronous)
		updateInDB();
}

int Edge::getSourceDFSIndex() const {return sourceDFSIndex;}
void Edge::setSourceDFSIndex(const int _sourceDFSIndex, bool asynchronous){
	sourceDFSIndex = _sourceDFSIndex;
	if (!asynchronous)
		updateInDB();
}

int Edge::getTargetDFSIndex() const {return targetDFSIndex;}
void Edge::setTargetDFSIndex(const int _targetDFSIndex, bool asynchronous){
	targetDFSIndex = _targetDFSIndex;
	if (!asynchronous)
		updateInDB();
}

string Edge::getClassName() const {return "Edge";}

void Edge::updateInDB(){
		GraphDB::ObjectInterface::updateObject(*this);
}

unsigned long Edge::saveInDB(){
	return GraphDB::ObjectInterface::saveObject(*this);
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

void Edge::resynchronize(){
	updateInDB();
}