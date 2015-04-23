/* ************* Begin file stdafx.h ***************************************/
/*
** 2015 March 08
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file stdafx.h
*	\brief Header File containg every headers that needs to be precompile.
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _STDAFX_SHAPE_LEARNER_
#define _STDAFX_SHAPE_LEARNER_

#ifdef _MSC_VER
	#pragma message("Compiling ShapeLearnerLib::stdafx.h  - this should happen just once per project.\n")
#endif

//For /Wall
#pragma warning(push)
#pragma warning(disable : 4820)
#pragma warning(disable : 4619)
#pragma warning(disable : 4548)
#pragma warning(disable : 4668)
#pragma warning(disable : 4365)
#pragma warning(disable : 4710)
#pragma warning(disable : 4371)
#pragma warning(disable : 4826)
#pragma warning(disable : 4061)
#pragma warning(disable : 4640)

// ============================= Global Variables =============================

// Version of the ShapeMatcher project
#define SL_VERSION_NUMBER 1.0

// Subset of default values that are most likely to be changed
#define DEFAULT_SHAPE_REP 1        // ShockGraph = 1, GestureGraph = 2, BoneGraph = 3
#define DEFAULT_SKEL_CODE 2        // Voronoi = 1, Flux = 2, AFMM = 3
#define DEFAULT_COMPUTE_STATS 0
#define DEFAULT_SHOW_RESULTS 1
#define DEFAULT_SAVE_RESULTS 0
#define DEFAULT_SAVE_NODE_SIM 0
#define DEFAULT_MATCH_ALGORITHM 0 // Greedy = 0, Optimal = 1, Topological = 2, Adaptive = 3
#define DEFAULT_NODE_SIM_FUNCTION 0

#define DEFAULT_INDEXING_K_BEST 101
#define DEFAULT_INDEXING_SIM_TAU 0.3
#define DEFAULT_MATCHING_SIM_TAU 0//0.1
#define DEFAULT_SIM_MASS_WEIGHT 0.7

// ============================= START ADDING LIBS =============================

#include <iostream>
#include <iomanip>
#include <io.h>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <direct.h>
#include <cerrno>
#include <memory>
#include <random>
#include <stack>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/threadpool.hpp>	
#include <boost/thread.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <math.h>
#include <fcntl.h> // for open()
#include <cstdio> // for perror()

// External Libs
#include <newmatap.h>
#include <newmatio.h>
#include <tools/connected.h>
#include <CImg/CImg.h>
#include <FHImgSegment/segment-graph.h>

// DAG Matcher Lib
#include <BasicTypedefs.h>
#include <BasicUtils.h>
#include <SmartMatrix.h>
#include <ShockGraph.h>
#include <ShockGraphView.h>
#include <BoneGraph.h>
#include <BoneGraphView.h>
#include <GestureGraph.h>
#include <DAGDatabase.h>
#include <DAG.h>
#include <DirWalker.h>
#include <ImageProcessing.h>
#include <Exceptions.h>
#include <HelperFunctions.h>

//LEDA
#include <LEDA/core/p_queue.h>




//ShapeLearner Specific Files

#include "constants.h"
#include "StandardException.h"	
#include "CLogger.h"
#include "graphDB.h"
#include "infoStructures.h"

// ============================= STOP ADDING LIBS ==============================

#pragma warning(pop) //For /Wall

#endif // _STDAFX_SHAPE_LEARNER_