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

Point::Point(double _xCoord, double _yCoord, double _radius, Graph* _refGraph, Node* _refNode) : 
	xCoord(_xCoord),
	yCoord(_yCoord),
	radius(_radius),
	refGraph(_refGraph),
	refNode(_refNode),
	idPoint(0)
	{
		idPoint = GraphManager::openManager().saveObject(*this);
		#ifdef _VERBOSE_
			cout << "Point key : "+ to_string((_ULonglong)getKey()) <<endl;
		#endif
	}