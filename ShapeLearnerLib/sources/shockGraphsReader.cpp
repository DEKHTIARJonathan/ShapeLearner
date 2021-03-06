/* ************* Begin file ShockGraphsReader.h ***************************************/
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
*	\file ShockGraphsReader.h
*	\brief ShockGraphGenerator Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
using namespace dml;

/*
_Longlong fact(_Longlong nbr){
	_Longlong tmp = nbr;
	_Longlong rslt = 1;
	while(tmp > 0)
	{
		rslt *= tmp--;
	}

	return rslt;
}
*/

string genFileName(const unsigned int len = 10)
{
    const string alphanum = "0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	string rslt;

    for (unsigned int i = 0; i < len; ++i)
		rslt += alphanum[rand() % (int)(alphanum.size() -1)];

    return rslt+".ppm";
}

ShockGraphsReader::ShockGraphsReader(const string& _filepath, const string& _objClass, const unsigned int& _jobID) : filepath(_filepath), objClass(_objClass), jobID(_jobID) {
	parametersInit();
	Logger::Log("Start file : " + filepath, constants::LogCore);
}

void ShockGraphsReader::parametersInit(){
	// =========== Parameters for Experiment =================== //

	m_experimentId = -1; // Performs experiment with given ID
	m_experimentStep = 0; // Experiment step
	m_experimentFirstParam = 0; // First experiment parameter
	m_experimentLastParam = 0; // Last experiment parameter
	m_szExperimentName = ""; // Optional name of the experiment

	// ================= Global Parameters =================== //

	m_szMatrixFileName = "matrix.m"; // Outputs similarity matrix to file [val] for all the models in the database // Create database using some training techinque (experimental)
	m_target = ""; // "Match DAG [DAG ID] // Match a target database against all other databases // Compares DAGs without using indexing // Saves the graphs from '-from ID' to '-to ID' in XML format
	m_viewDag = ""; // Visualize a DAG from ID or name (eg, objNN) // Visualize a DAG (from ID) and its 9 neighbors
	m_nSplitProportion = 50; // Split database in random pieces of sizes N% and (100-N)% of the total size (see splitByObject) [N]
	m_nNumSplitParts = 3; // Incremental split of database in N subdatabase with decreasing number of views per object [N] {-iterations}
	m_dNeigRange = 180; // List the neighbours withing a given range in deg.
	m_sourceDB = 0; // Sets the soure DB for viewing operations
	m_nVerbose = 0; // Execute in verbose mode

	// ================  ADD DAG MATCHING PARAMETERS ============== //
	m_matchParams.nExtreme = 0;  //!< Load all DAGs to memory before matching

	m_matchParams.dTSVSimWeight = 0.3; //!< TSV similarity weight
	m_matchParams.dSimilMassWeight = DEFAULT_SIM_MASS_WEIGHT; //!< Similarity vs node's relative mass weight
	m_matchParams.dRelMassWeight = 0.5; //!< Relative mass weight of the query vs. tot model

	m_matchParams.dDiffAttachPosWeight = 0.5; //!< Relative importance of attachment position and angle

	m_matchParams.nMatchingAlgorithm = DEFAULT_MATCH_ALGORITHM; //!< Matching algorithm ID
	m_matchParams.nNodeSimilarityFunction = DEFAULT_NODE_SIM_FUNCTION; //!< Node distance/similarity function ID

	m_matchParams.dBreakAncestorRelPen = 0; //!< Penalty term for breaking ancestor relationships (0=max)
	m_matchParams.dBreakDescendantRelPen = 0; //!< Penalty term for breaking desendant relationships (0=max)
	m_matchParams.dBreakSiblingRelPen = 0.8; //!< Penalty term for breaking sibling relationships (0=max)

	m_matchParams.dAncestorPathSigma = 0; //!< Rate of decay of ancestor relationship penalty (0=no decay)
	m_matchParams.dDescendantPathSigma = 0; //!< Rate of decay of descendant relationship penalty (0=no decay)
	m_matchParams.dSiblingPathSigma = 0; //!< Rate of decay of sibling relationship penalty (0=no decay)

	m_matchParams.nCompareEdges = 1; //!< Measure edge similarity
	m_matchParams.nCompareNodes = 1; //!< Measure node similarity

	m_matchParams.nDisableNodeSkipping = 0; //!< Forbids the skipping of nodes when matching

	m_matchParams.dCertaintyParam = 2; //!< Parameter in the equation of node match certainty

	m_matchParams.nNodeAssignSortType = 1; //!< Type of sorting for node assignments

	m_matchParams.nPreserveAncestorRel = 1; //!< Preserve the ancestor relations when matching

	m_matchParams.nUseMWBMHeuristic = 1; //!< Use or not MWBM before selecting correspondences

	m_matchParams.nMaxNumSolSets = 1; //!< Maximum number of solutions sets
	m_matchParams.nMaxSolSetsPerIter = 1; //!< Maximum number of new solutions sets per iterations

	m_matchParams.dSlopeSigma = 0.15; //!< Slope sigma for bone graphs
	m_matchParams.dBGWrongSidePen = 0.6;  //!< Wrong side penalty for bone graphs
	m_matchParams.dBGPositionSigma = 0;  //!< Wrong side/position penalty rate for bone graphs (0=linear decay)

	m_matchParams.dSaliencyParam = 1; //!< Normalization value used to compute node saliency (>= 1)

	m_matchParams.nUseNewVoteWeightFunc = 1; //!< Indexing: Use new vote weighting function
	m_matchParams.nUseMOOVC = 1; //!< Indexing: Use multiple one-to-one vote counting

	// =========== ADD MATCHING PROCESS INFORMATION ================= //

	m_matchInfo.modelSimWeight = 0.5; // Model similarity weight

	m_matchInfo.idxrange = 0.4; // Index range
	m_matchInfo.idxKBest = DEFAULT_INDEXING_K_BEST; // Match only k best from index
	m_matchInfo.idxtau = DEFAULT_INDEXING_SIM_TAU; // Index similarity threshold
	m_matchInfo.matchtau = DEFAULT_MATCHING_SIM_TAU; // Matcher similarity threshold

	m_matchInfo.computeStats = DEFAULT_COMPUTE_STATS; // Compute view statistics
	m_matchInfo.statsOutDir = "."; // Output directory for the stats files

	m_matchInfo.asyncCompu = 0; // Compute SG's in async mode (see timeout)

	m_matchInfo.showResults = DEFAULT_SHOW_RESULTS; // Show matching results
	m_matchInfo.saveResults = DEFAULT_SAVE_RESULTS; // Save matching results
	m_matchInfo.saveNodeSim = DEFAULT_SAVE_NODE_SIM; // Save node similarity values

	m_matchInfo.firstModelId = -1; // ID of the first model shape to evaluate
	m_matchInfo.lastModelId = -1; // ID of the last model shape to evaluate

	m_matchInfo.selectionMethod = 0; // Match all query and model DB Parsingss

	// =========== ADD EXPERIMENT PARAMETERS =================== //

	m_objName = ""; // Set object name
	m_iterations = 1; // Parameter for the SG computation
	m_maxdags = -1; // Maximum number of DAGs to process
	m_imgstep = 1; // Image step

	m_from = 0; // "Match DAGs from ID N
	m_to = -1; // Match DAGs up to ID M

	m_firstDBId = -1; // First database ID to create/match
	m_lastDBId = -1; // Last database ID to create/match

	m_maxOcclusionRate = 0; // Maximum occlusion rate
	m_saveSubsamplingInfo = 0; // Save info to split DB when matching
	m_nSplitByObject = 1; // Split DB maintaining equal object proportions
	m_shapeInfo.nBumpSize = 0; // Size of noise bumps
	m_shapeInfo.nNotchSize = 0; // Size of noise notches
	m_shapeInfo.outDir = "./NoisyImgs/"; // Sets the output directory for noisy images

	// ===================  Display parameters =================== //

	m_xpos = 0; // Window x-coord position
	m_ypos = 0; // Window y-coord position

	// =========== Generic DAG construction parameters =================== //

	DAG::s_constructParams.nUseDirNameAsObjName = 1; // Use the (sub)directory name (if given) as the object name

	// =========== Generic shape representation parameters ================== //

	m_shapeInfo.shapeRepType = DEFAULT_SHAPE_REP; // Shape representation: ShockGraph = 1, GestureGraph = 2, BoneGraph = 3

	// ================= Shock Graph parameters  ==================== //

	m_shapeInfo.timeout = 800; // SG computation Timeout in seconds
	m_shapeInfo.sgparams.nSlipt3s = 0; // Should split 3s into left and right 3s
	m_shapeInfo.sgparams.dMinSlope = 0.05; // Slope that is to be considered as zero
	m_shapeInfo.sgparams.dMinError = 2.0; // #Pts/'value' defines least-square error tolerated
	m_shapeInfo.sgparams.dMaxYDiff = 0.1; // Maximum difference in y-coord discontinuity
	m_shapeInfo.sgparams.dMaxAccelChg = 0.5; // Max acceleration change tolerated

	// ================= Bone Graph parameters  ==================== //

	m_shapeInfo.bgparams.nMaxNumSegments = 8; // Max number of segments in piecewise function
	m_shapeInfo.bgparams.nSmoothIter = 0; // Boundary smoothing interations = 0,...,5 (for AFMM only)
	m_shapeInfo.bgparams.maxCumulativeBAR.loose = 0.75; // Maximum cumulative bar for a ligature segment
	m_shapeInfo.bgparams.maxCumulativeBAR.tight = 0.75; // Maximum cumulative bar for a ligature segment
	m_shapeInfo.bgparams.maxRadiusRatio.loose = 0.7; // Maximum radius ratio for a ligature segment
	m_shapeInfo.bgparams.maxRadiusRatio.tight = 0.7; // Maximum radius ratio for a ligature segment
	m_shapeInfo.bgparams.maxCornerAngle.loose = 151.2608; // Maximum corner angle for a ligature segment
	m_shapeInfo.bgparams.maxCornerAngle.tight = 151.2608; // Maximum corner angle for a ligature segment
	m_shapeInfo.bgparams.nSpecializeBones = 0; // Partitions bones according to some criteria
	m_shapeInfo.bgparams.nIgnoreLigature = 0; // Testing: ignores ligature information
	m_shapeInfo.bgparams.nSubtractLigatureFromCrossings = 1; // Testing: subtracts ligature rooted at crossings
	m_shapeInfo.bgparams.nSubtractLigatureFromJunctions = 0; // "Testing: subtracts ligature at junctions
	m_shapeInfo.bgparams.nShowDependencyGraph = 0; // Show junction point dependencies
	m_shapeInfo.bgparams.nShowGSG = 0; // Show generalized skeletal graph

	// ================= Skeleton Graph parameters  ==================== //

	m_shapeInfo.skelparams.nSkelAlgorithm = DEFAULT_SKEL_CODE; // Use skeletonization code number = {Voronoi = 1, Flux = 2, AFMM = 3}

	// ================= Skeleton Approximation parameters  ==================== //

	m_shapeInfo.skelparams.dMinLinearError = 2.0; // #Pts/'value' defines linear skeleton approximation error
	m_shapeInfo.skelparams.dMinBezierError = 8.0; // #Pts/'value' defines cubic Bezier skeleton approximation error
	m_shapeInfo.skelparams.dMaxLineSegments = 8; // Max number of line segments per skeleton branch
	m_shapeInfo.skelparams.dMaxBezierSegments = 4; // Max number of cubic Beziers per skeleton branch

	// ================= Flux Skeleton params  ==================== //

	m_shapeInfo.skelparams.fluxStep = 1.0; // 1/'value' defines the sampling step for Flux skeletons
	m_shapeInfo.skelparams.fluxTau = 2.5; // Flux branch threshold for minimum angle angle
	m_shapeInfo.skelparams.fluxResolution = 16; // Flux value resolution

	// ================= AFMM Skeleton params ==================== //

	m_shapeInfo.skelparams.afmmTau1 = 0; // AFMM skeleton threshold 1 (deprecated)
	m_shapeInfo.skelparams.afmmTau2 = 20; // AFMM branch threshold for minimum radius

	// ================= Skeleton Simplification params  ==================== //

	m_shapeInfo.skelparams.nBndryRecErrorWeight =	500; // Boundary reconstruction error weight
	m_shapeInfo.skelparams.nStrucRecErrorWeight = 500; // Structural reconstruction error weight
	m_shapeInfo.skelparams.nSimplifyExternal = 1; // Perform the external branch simplification on the skeleton
	m_shapeInfo.skelparams.nSimplifyInternal = 0; // Perform the internal branch simplification on the skeleton
	m_shapeInfo.skelparams.nSimplifyComplete = 0; // Perform the external and internal branch simplification
}

bool ShockGraphsReader::taskExecute()
{
	Logger::Log("Adding object ("+filepath+")to database...", constants::LogCore);

	processFile(m_matchInfo.asyncCompu != 0);

	Logger::Log("Object ("+filepath+") has been added to database...", constants::LogCore);

	return true;
}

void ShockGraphsReader::processFile(bool bAsyncProcessing)
{
	DAGPtr pDag;
	bool bIsRead;
	const char* szFileExt;

	szFileExt = DirWalker::FindFileExtension(filepath.c_str());

	if (!strcmp(szFileExt, "ppm") || !strcmp(szFileExt, "pgm") ||
		!strcmp(szFileExt, "bmp") || !strcmp(szFileExt, "tif") ||
		!strcmp(szFileExt, "jpg") || !strcmp(szFileExt, "png"))
	{
		ImageInfo imgInfo(filepath.c_str());

		// Add artificial noise if requested
		if (m_shapeInfo.nBumpSize > 0 || m_shapeInfo.nNotchSize > 0)
		{
			ShowMsg("Adding noise to the image...");
			//AddBumpsAndNotches(&imgInfo); => NOT WORKING
		}

		// Choose the appropriate shape representation
		if (m_shapeInfo.shapeRepType == SGShapeRep) // == 1
		{
			ShockGraph* pSG = new ShockGraph;
			pDag = pSG;
			//pSG->Read(
			JobManager::Log(jobID,Ongoing,0,StartGen, filepath);
			bIsRead = pSG->Create(imgInfo, m_shapeInfo.sgparams, m_shapeInfo.skelparams);
			saveInDB(*pSG);
		}
		else if (m_shapeInfo.shapeRepType == BGShapeRep) // == 2
		{
			BoneGraph* pBG = new BoneGraph;
			pDag = pBG;

			bIsRead = pBG->Create(imgInfo, m_shapeInfo.bgparams, m_shapeInfo.skelparams);
		}
		else
		{
			bIsRead = false;
			ShowError("Only ShockGraph or BoneGraph are valid options for image files.");
		}
	}
	else if (!strcmp(szFileExt, "gg"))
	{
		GestureGraph* pGG = new GestureGraph;
		pDag = pGG;

		bIsRead = pGG->Read(filepath.c_str());
	}
	else
	{
		cerr << "\nUnknown file extension: " << szFileExt << endl;
		bIsRead = false;
	}

	if(bIsRead)
	{
		Logger::Log("DONE! (" + to_string((_Longlong)pDag->GetNodeCount()) + " nodes)... ", constants::LogCore);

		Logger::Log("WRITTEN TO DB!", constants::LogCore);
	}
	else
		Logger::Log("ERROR: Can't read dag.", constants::LogCore);
}

void ShockGraphsReader::saveInDB(const ShockGraph& graph){
	try{
		JobManager::Log(jobID,Ongoing,0,StartSaving, filepath);
		/* ===================== GRAPH SAVING ====================== */

		boost::weak_ptr<graphDBLib::Graph> graphPtr = graphDBLib::GraphDB::CommonInterface::getGraph( \
			graphDBLib::GraphDB::CommonInterface::getGraphClass((string)graph.ClassName()), \
			graphDBLib::GraphDB::CommonInterface::getObjectClass(objClass), \
			(string) graph.GetDAGLbl());
		JobManager::Log(jobID,Ongoing,graphPtr.lock()->getKey(),StartSaving, filepath); // Update the job with the PartID.

		graphPtr.lock()->setCumulativeMass(graph.GetCumulativeMass(), true);
		graphPtr.lock()->setDAGCost(graph.GetDAGCost(), true);
		graphPtr.lock()->setMaxTSVDimension(graph.GetMaxTSVDimension(), true);
		graphPtr.lock()->setTotalTSVSum(graph.GetTotalTSVSum(), true);

		ShapeDims sh = graph.GetDims();
		graphPtr.lock()->setShapeDimensions(sh.xmin, sh.xmax, sh.ymin, sh.ymax, true);

		std::stringstream testStream;
		graph.Print(testStream, true);
		graphPtr.lock()->setXMLSignature(testStream.str(), true);

		graphPtr.lock()->resynchronize();

		leda::list<leda::graph::node> nodeList (graph.all_nodes());
		leda::list<leda::graph::edge> edgeList (graph.all_edges());

		map<int, boost::weak_ptr<graphDBLib::Node>> NodeMap;

		for(leda::list<leda::graph::node>::iterator it = nodeList.begin(); it != nodeList.end(); it++){
			/* ===================== Node SAVING ====================== */
			boost::weak_ptr<graphDBLib::Node> NodePtr = graphDBLib::GraphDB::CommonInterface::getNode(graphPtr);

			leda::graph::node ledaNode = *it;
			const SGNode* curNode = graph.GetSGNode(ledaNode);

			NodeMap.insert(pair<int, boost::weak_ptr<graphDBLib::Node>>((*it)->id(), NodePtr));

			NodePtr.lock()->setIndex(curNode->GetDFSIndex(), true);
			NodePtr.lock()->setLabel(curNode->GetNodeLbl().c_str(), true);
			NodePtr.lock()->setLevel(curNode->GetLevel(), true);
			NodePtr.lock()->setMass(curNode->GetMass(), true);
			NodePtr.lock()->setType(curNode->GetType(), true);
			NodePtr.lock()->setRole(NodeRoleConverter2GraphDBLib(curNode->GetNodeRole()), true);
			NodePtr.lock()->setPointCount(curNode->GetShockCount(), true);

			NodePtr.lock()->setSubtreeCost(curNode->GetSubtreeCost(), true);
			NodePtr.lock()->setTSVNorm(curNode->GetTSVNorm(), true);

			NodePtr.lock()->resynchronize();

			/* ===================== Point SAVING ====================== */

			ShockBranch branch = curNode->m_shocks;

			for (int i = 0; i < branch.GetSize(); i++){
				boost::weak_ptr<graphDBLib::Point> PointPtr = graphDBLib::GraphDB::CommonInterface::getPoint(NodePtr, graphPtr);

				PointPtr.lock()->setDirection(BranchDirConverter2GraphDBLib(branch[i].dir), true);
				PointPtr.lock()->setDr(branch[i].dr, true);
				PointPtr.lock()->setDr_Ds(branch[i].dr_ds, true);
				PointPtr.lock()->setRadius(branch[i].radius, true);
				PointPtr.lock()->setSpeed(branch[i].speed, true);
				PointPtr.lock()->setxCoord(branch[i].xcoord, true);
				PointPtr.lock()->setyCoord(branch[i].ycoord, true);

				PointPtr.lock()->resynchronize();
			}
		}
		/* ===================== Edge SAVING ====================== */
		for(leda::list<leda::graph::edge>::iterator itEdge = edgeList.begin(); itEdge != edgeList.end(); itEdge++){
			leda::graph::edge ledaEdge = *itEdge;
			leda::graph::node source = ledaEdge->terminal(0); // term[0] = source and term[1] = target
			leda::graph::node target = ledaEdge->terminal(1); // term[0] = source and term[1] = target

			int idSource = source->id();
			int idTarget = target->id();

			map<int, boost::weak_ptr<graphDBLib::Node>>::iterator itNodeSource = NodeMap.find(idSource);
			map<int, boost::weak_ptr<graphDBLib::Node>>::iterator itNodeTarget = NodeMap.find(idTarget);

			if(itNodeSource == NodeMap.end() || itNodeTarget == NodeMap.end())
				throw StandardExcept((string)__FUNCTION__,"Error while fetching the Nodes (Source: "+ to_string((_Longlong)idSource) +", Target: "+ to_string((_Longlong)idTarget) +") Connected to the Edge: "+ to_string((_Longlong)ledaEdge->id()) +".");

			boost::weak_ptr<graphDBLib::Node> NodeSource = itNodeSource->second;
			boost::weak_ptr<graphDBLib::Node> NodeTarget = itNodeTarget->second;

			boost::weak_ptr<graphDBLib::Edge> EdgePtr = graphDBLib::GraphDB::CommonInterface::getEdge(NodeSource, NodeTarget, graphPtr);

			EdgePtr.lock()->setWeight(graph.GetEdgeWeight(ledaEdge), true);
			EdgePtr.lock()->setSourceDFSIndex(graph.GetNodeDFSIndex(source), true);
			EdgePtr.lock()->setTargetDFSIndex(graph.GetNodeDFSIndex(target), true);

			EdgePtr.lock()->resynchronize();
		}
		JobManager::Log(jobID,Finished,graphPtr.lock()->getKey(),EndSaving, filepath);

		//graphDBLib::GraphDB::CommonInterface::delObj(graphPtr, false);
	}
	catch(std::exception e){
		Logger::Log((string)__FUNCTION__ + " // Error while saving: " + (string)e.what(), constants::LogError);
	}
}

void ShockGraphsReader::saveInDB(const BoneGraph& graph){
	cout <<"Classname : " << graph.ClassName() <<endl;
}

void ShockGraphsReader::saveInDB(const GestureGraph& graph){
	cout <<"Classname : " << graph.ClassName() <<endl;
}

dml::NODE_ROLE ShockGraphsReader::NodeRoleConverter2DML(graphDBLib::NODE_ROLE tmp) const{
	return (dml::NODE_ROLE) tmp;
}

graphDBLib::NODE_ROLE ShockGraphsReader::NodeRoleConverter2GraphDBLib(dml::NODE_ROLE tmp) const{
	return (graphDBLib::NODE_ROLE) tmp;
}

dml::ShockBranch::BRANCH_DIR ShockGraphsReader::BranchDirConverter2DML(graphDBLib::BRANCH_DIR tmp) const{
	return (dml::ShockBranch::BRANCH_DIR) tmp;
}

graphDBLib::BRANCH_DIR ShockGraphsReader::BranchDirConverter2GraphDBLib(dml::ShockBranch::BRANCH_DIR tmp) const{
	return (graphDBLib::BRANCH_DIR) tmp;
}

graphDBLib::BRANCH_DIR ShockGraphsReader::BranchDirConverter2GraphDBLib(int tmp) const{
	return (graphDBLib::BRANCH_DIR) tmp;
}