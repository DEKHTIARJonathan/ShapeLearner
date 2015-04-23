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

#ifndef _STDAFX_DAG_MATCHER_
#define _STDAFX_DAG_MATCHER_

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::stdafx.h  - this should happen just once per project.\n")
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

// ============================= START ADDING LIBS =============================

#include <iostream>
#include <iomanip>
#include <io.h>
#include <string>
#include <map>
#include <list>
#include <set>
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <math.h>
#include <fcntl.h> // for open()
#include <cstdio> // for perror()
#include <memory.h>
#include <search.h>
#include <assert.h>
#include <float.h>
#include <ctype.h>
#include <stdarg.h>
//

#define _USE_MATH_DEFINES
#include <math.h>

/* ================= LEDA HEADERS ================ */

#include <LEDA/internal/PREAMBLE.h>
#include <LEDA/core/list.h>
#include <LEDA/core/string.h> 
#include <LEDA/numbers/integer.h>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_alg.h>
#include <LEDA/core/queue.h>
#include <LEDA/core/sortseq.h>
#include <LEDA/core/p_queue.h>
#include <LEDA/core/array.h>
#include <LEDA/core/d_array.h> // Include d_array.h AFTER graph_alg.h!
#include <LEDA/core/h_array.h>
#include <LEDA/graphics/color.h>
#include <LEDA/graphics/graphwin.h>
#include <LEDA/graphics/window.h>

#include "LEDA_issues.h"
#include "LEDA_utils.h"
#include "LEDA_window_utils.h"

/* ================ EXTERNAL LIBS ================ */

#include "FluxSkeleton\include\Point.h"
#include "FluxSkeleton\include\DDSGraphProject.h"
#include "FluxSkeleton\include\sg.h"

#include "ann_1.1/include/ANN/ANN.h"

#include "AFMMSkeleton\include\field.h"

#include "tools\connected.h"

#include "Newmat\newmat.h"
#include "Newmat\newmatap.h"
#include "Newmat\newmatio.h"

//#include "CImg/CImg.h"

#include "HnSRTree/HnSRTreeFile.hh"

/* ================ DAG Matcher ================ */

#include "BasicUtils.h"
#include "BasicTypes.h"
#include "DirWalker.h"

#include "HelperFunctions.h"
#include "Exceptions.h"

#include "SmartArray.h"
#include "SmartMatrix.h"
#include "SmartPtr.h"
#include "SharedPtr.h"

#include <DDSGraphProject.h>
#include "DDSGraphUtils.h"
#include "EuclideanLineSegment.h"
#include "TSV.h"
#include "DMLString.h" 

#include "LineSegment.h"
#include "PiecewiseApprox.h"
#include "PolyLineApprox.h"

#include "PolyLineWarper.h"
#include "CubicBezier.h"
#include "PolyLineTLSApprox.h"
#include "PolyBezierApprox.h"

#include "SkeletalGraph.h"

#include "KDTree.h" 

#include "BipartiteGraph.h"
#include "BipartiteNodeGraph.h"

#include "BoundaryInterval.h"
#include "BoundaryPointFinder.h"
#include "BoundaryIntervalWithGaps.h"
#include "BoundaryCorner.h"
#include "MedialAxisCorner.h"

#include "BranchSegment.h"
#include "BranchWalkInfo.h"
#include "LigatureSegment.h"
#include "BranchSegmentList.h"
#include "LigatureSegmentList.h"
#include "PartnerBranch.h"

#include "BranchWalkInfo.h"
#include "XORIntegral.h"

#include "BasicTypedefs.h"

#include "MatchedNodePair.h"

#include "GeneralizedSkeletalGraph.h"

#include "DAGNode.h" 
#include "DAGEdge.h"
#include "DAGView.h"
#include "VisualDAG.h"

//#include "Emd.h"
#include "PlotView.h"

#include "ShockPoint.h"
#include "SGNode.h"
#include "ShockGraph.h" 
#include "ShockGraphView.h"

#include "BGElement.h"
#include "BGNode.h"
#include "BGEdge.h"
#include "BoneGraphConstructor.h"
#include "BoneGraph.h"
#include "BoneGraphView.h"

#include "GGNode.h"
#include "GestureGraph.h"

#include "ModelFit.h"

#include "DAGSearchDatabase.h"
#include "DAGDBFile.h"
#include "DAGDatabase.h"
#include "DAGMatcher.h"

#include "DAG.h" 
#include "NodePairInfo.h" // needed by FillNodeMap
#include "NodeMatchInfo.h"

#include "NodeAssignment.h"
#include "SolutionSet.h"
#include "VoteCounter.h"

#include "SimilarityMeasurerT.h"
#include "DAGMatcherOptimal.h"
#include "DAGMatcherGreedy.h"
#include "DAGMatcherTopological.h"
#include "DAGMatcherAdaptive.h"
#include "SGSimilarityMeasurer.h"
#include "BGSimilarityMeasurer.h"

// ============================= STOP ADDING LIBS ==============================

#pragma warning(pop) //For /Wall

#endif // _STDAFX_DAG_MATCHER_