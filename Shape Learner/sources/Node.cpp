/* ************ Begin file Node.cpp **************************************
**
** 2015 March 04
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file Node.cpp
*	\brief Node Cpp File
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

Node::Node(unsigned int _index, unsigned int _level, unsigned int _mass, unsigned int _type, unsigned int _pointCount, unsigned int _label, Graph* _refGraph) : 
	index(_index),
	level(_level),
	mass(_mass),
	type(_type),
	pointCount(_pointCount),
	label(_label),
	idNode(0)
	{
		GraphManager::openManager().saveObject(*this);
		cout << "Node key : "+ to_string((_ULonglong)this->getKey()) <<endl;
	}