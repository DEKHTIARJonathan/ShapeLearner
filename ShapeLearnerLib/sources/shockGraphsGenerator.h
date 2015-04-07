/* ************* Begin file shockGraphsGenerator.h ***************************************/
/*
** 2015 April 03
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file shockGraphsGenerator.h
*	\brief ShockGraphGenerator Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _Shock_Graphs_Generator_
#define _Shock_Graphs_Generator_

#include "allHeaders.h"
using namespace std;

class ShapeLearner; // Forward Declaration of the class contained in shapeLearner.h
class ShapeLearnerExcept; //Forward Declaration of the class contained in shapeLearnerException.h
class ObjectClass; //Forward Declaration of the class contained in ObjectClass.h
class GraphClass; //Forward Declaration of the class contained in GraphClass.h
class Graph; //Forward Declaration of the class contained in Graph.h
class Node; //Forward Declaration of the class contained in Node.h
class Point; //Forward Declaration of the class contained in Point.h
class Edge; //Forward Declaration of the class contained in Edge.h

/*!
*	\class shockGraphsGenerator
*	\brief Class allowing us to log every actions performed in the program.
*	In DEBUG : Everything is redirected to the 4 log files + standard output.
*	In RELEASE : Everything is redirected to the 4 log files + Errors only to the standard output.
*/
class shockGraphsGenerator{
public:

private:
	static volatile unsigned int _nbActiveThread;;
	static volatile shockGraphsGenerator _inst;
	/*!
	*	\fn shockGraphsGenerator()
	*	\brief Private constructor
	*/
    shockGraphsGenerator();
	
	/*!
	*	\fn ~shockGraphsGenerator()
	*	\brief Private Destructor, automaticly called at the end of the program (object instanciated on stack). Close all ofstreams.
	*/
	~shockGraphsGenerator();
	
	/*!
	*	\fn shockGraphsGenerator(const shockGraphsGenerator&)
	*	\brief Singleton Pattern, forbid copy
	*/
	shockGraphsGenerator(const shockGraphsGenerator);
	
	/*!
	*	\fn void operator=(const shockGraphsGenerator&)
	*	\brief Singleton Pattern, forbid copy
	*/
	void operator=(const shockGraphsGenerator&);
};

#endif //_Shock_Graphs_Generator_