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
	refGraph(_refGraph),
	idNode(0)
	{
		idNode = GraphManager::openManager().saveObject(*this);
		#ifdef _VERBOSE_
			cout << "Node key : "+ to_string((_ULonglong)getKey()) <<endl;
		#endif
	}


inline void Node::setKey(const unsigned int key) {
	#ifdef _MSC_VER
		GraphManager::openManager().deleteObject(*this);
	#endif //_MSC_VER
	idNode = key;
	#ifdef _MSC_VER
		GraphManager::openManager().saveObject(*this);
	#endif //_MSC_VER
}

inline void Node::setIndex(const unsigned int _index) {
	index = _index;
	#ifdef _MSC_VER
		GraphManager::openManager().updateObject(*this);
	#endif //_MSC_VER
}

inline void Node::setLevel(const unsigned int _level) {
	level = _level;
	#ifdef _MSC_VER
		GraphManager::openManager().updateObject(*this);
	#endif //_MSC_VER
}

inline void Node::setMass(const unsigned int _mass) {
	mass = _mass;
	#ifdef _MSC_VER
		GraphManager::openManager().updateObject(*this);
	#endif //_MSC_VER
}

inline void Node::setType(const unsigned int _type) {
	type = _type;
	#ifdef _MSC_VER
		GraphManager::openManager().updateObject(*this);
	#endif //_MSC_VER
}

inline unsigned long Node::getPointCount() const {
	string query = "SELECT count(1) FROM \"Point\" WHERE \"refNode\" = '" + to_string(_ULonglong(idNode)) + "'";
	return 1;
} // à créer en fonction SQL
