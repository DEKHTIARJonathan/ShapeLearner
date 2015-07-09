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
   #pragma message("Compiling ShapeLearnerLib::shockGraphsGenerator.h  - this should happen just once per project.\n")
#endif

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;

class ShapeLearner; // Forward Declaration of the class contained in ShapeLearner.h
class StandardExcept; //Forward Declaration of the class contained in StandardException.h
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
   //typedef bool (ShapeMatcher::*DAG_ACTION)();
   typedef std::map<std::string, int> StrIntMap;
   typedef const char* LPCSTR;

   dml::DAGMatcher::MatchParams m_matchParams;

   shockGraphsGenerator(const string& _filepath, const string& _objClass, const unsigned int& _jobID);
   bool taskExecute();

private:
   const string filepath;
   const string objClass;
   const unsigned int jobID;

   ShapeMatchingParams m_matchInfo;
   ShapeRepresentationParams m_shapeInfo;

   double m_maxOcclusionRate, m_dNeigRange;
   int m_iterations, m_maxdags, m_imgstep, m_from, m_to, m_nNumSplitParts;
   int m_nVerbose, m_saveSubsamplingInfo, m_nSplitProportion, m_nSplitByObject;
   int m_xpos, m_ypos, m_sourceDB;

   int m_firstDBId, m_lastDBId;

   int m_experimentId, m_experimentStep, m_experimentFirstParam, m_experimentLastParam;
   LPCSTR m_szExperimentName;

   LPCSTR m_target, m_objName, m_delobj, m_createObj, m_viewDag, m_szMatrixFileName;

   void parametersInit ();

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

   void saveInDB(const dml::ShockGraph& graph);
   void saveInDB(const dml::BoneGraph& graph);
   void saveInDB(const dml::GestureGraph& graph);

   dml::NODE_ROLE NodeRoleConverter2DML(graphDBLib::NODE_ROLE tmp) const;
   graphDBLib::NODE_ROLE NodeRoleConverter2GraphDBLib(dml::NODE_ROLE tmp) const;

   dml::ShockBranch::BRANCH_DIR BranchDirConverter2DML(graphDBLib::BRANCH_DIR tmp) const;
   graphDBLib::BRANCH_DIR BranchDirConverter2GraphDBLib(dml::ShockBranch::BRANCH_DIR tmp) const;
   graphDBLib::BRANCH_DIR BranchDirConverter2GraphDBLib(int tmp) const;
};

#endif //_Shock_Graphs_Generator_