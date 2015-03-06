/* ************ Begin file Point.cpp **************************************
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
*	\file Point.cpp
*	\brief Point Cpp File
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

Point::Point(Node* _refNode, Graph* _refGraph, double _xCoord, double _yCoord, double _radius) : 
xCoord(_xCoord),
yCoord(_yCoord),
radius(_radius),
refGraph(_refGraph),
refNode(_refNode),
idPoint(0)
{
	idPoint = GraphManager::ObjectInterface::saveObject(*this);
	#ifdef _VERBOSE_
		cout << "Point key : "+ to_string((_ULonglong)getKey()) <<endl;
	#endif
}

inline void Point::setKey(const unsigned int key) {

	#ifdef _MSC_VER
		GraphManager::ObjectInterface::deleteObject(*this);
	#endif //_MSC_VER

	idPoint = key;

	#ifdef _MSC_VER
		GraphManager::ObjectInterface::saveObject(*this);
	#endif //_MSC_VER

}

inline void Point::setxCoord(const unsigned int _xCoord){
	xCoord = _xCoord;
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

inline void Point::setyCoord(const unsigned int _yCoord){
	yCoord = _yCoord;
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}

inline void Point::setRadius(const unsigned int _radius){
	radius = _radius;
	#ifdef _MSC_VER
		GraphManager::ObjectInterface::updateObject(*this);
	#endif //_MSC_VER
}