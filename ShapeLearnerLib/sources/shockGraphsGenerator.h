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

#ifdef _MSC_VER
	#pragma message("Compiling ShapeLearnerLib::shockGraphGenerator.h  - this should happen just once per project.\n")
#endif

#include "stdafx.h"
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
	shockGraphsGenerator(const string& _imgPath, const unsigned long _taskNum);
	
	bool taskExecute();

private:
	const string imgPath;
	const unsigned long taskNum;
	ShapeMatchingParams m_matchInfo;
	ShapeRepresentationParams m_shapeInfo;

	/*!
	*	\brief Computes a shock graph from the given ppm file.
	*	\param szFileName : PPM file name
	*	\param info : Parameters for the SG computation code
	*	\param bAsyncProcessing : Whether a TIME OUT must be used to control the SG computation
	*
	*	Since the SG computation used to fail, the async processing was necessary to
	*	avoid a batch processing of files getting stuck in a particular file.
	*/
	void processFile(bool bAsyncProcessing);
	bool AddBumpsAndNotches(dml::ImageInfo* pImgInfo);
};

#endif //_Shock_Graphs_Generator_