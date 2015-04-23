/**------------------------------------------------------------------------
 * @file
 *
 * Copyright 2001 by the University of Toronto (UT).
 *
 *                       All Rights Reserved
 *
 * Permission to copy and modify this software and its documentation
 * only for internal use in your organization is hereby granted,
 * provided that this notice is retained thereon and on all copies.
 * UT makes no representations as to the suitability and operability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * UT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
 * SHALL UT BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * No other rights, including for example, the right to redistribute
 * this software and its documentation or the right to prepare
 * derivative works, are granted unless specifically provided in a
 * separate license agreement.
 *
 * Author: Diego Macrini
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

using namespace dml;

/////////////////////////////////////////////////////////////////////////////
// Init static variables

DAGMatcher* BoneGraph::s_pDAGMatcher = NULL;

/////////////////////////////////////////////////////////////////////////////
// BoneGraph class implementation

BoneGraph::BoneGraph()
{
	m_pSkeleton = NULL;

	// Init the matching algorithm for bone graphs
	if (s_pDAGMatcher == NULL)
		SetMatchingAlgorithm();
}

//! Sets the matching algorithm from the values in the match params
void BoneGraph::SetMatchingAlgorithm()
{
	int matchAlgo = DAGMatcher::GetMatchParams().nMatchingAlgorithm;
	int nodeSimFunc = DAGMatcher::GetMatchParams().nNodeSimilarityFunction;
	SimilarityMeasurer* pNDM;

	if (nodeSimFunc == 0) // default is zero
	{
		pNDM = new BGSimilarityMeasurer;
	}
	else if (nodeSimFunc == 1)
	{
		pNDM = new BGContextualSimilarityMeasurer;
	}
	else
	{
		ShowError("Unknown node distance function ID");
		throw;
	}

	// Delete any existing algorithm before creating a new one
	delete s_pDAGMatcher;

	if (matchAlgo == 0)
		s_pDAGMatcher = new DAGMatcherGreedy(pNDM);
	else if (matchAlgo == 1)
		s_pDAGMatcher = new DAGMatcherOptimal(pNDM);
	else if (matchAlgo == 2)
		s_pDAGMatcher = new DAGMatcherTopological(pNDM);
	else if (matchAlgo == 3)
		s_pDAGMatcher = new DAGMatcherAdaptive(pNDM);
	else
	{
		ShowError("Unknown matching algorithm ID");
		throw;
	}
}

BoneGraph::~BoneGraph()
{
	delete m_pSkeleton;
}

//! Clears all data associated with the bone graph
void BoneGraph::Clear()
{
	DAG::Clear();

	delete m_pSkeleton;
	m_pSkeleton = NULL;
}

/*!
	Compute the derived values specific to bone graphs
*/
void BoneGraph::ComputeDerivedValues()
{
	DAG::ComputeDerivedValues();

	const BGNode* pParent;
	BGNode* pNode;
	leda::node v;

	// Find corner bones
	forall_nodes(v, *this)
	{
		if (outdeg(v) == 0 && indeg(v) == 1 && GetNodeLevel(v) >= 2)
		{
			pNode = UnsafeGetBGNode(v);
			pParent = GetBGNode(source(first_in_edge(v)));

			// It should NOT have ligature points
			if (pNode->FirstShockPoint().pt == pParent->FirstShockPoint().pt ||
				pNode->FirstShockPoint().pt == pParent->LastShockPoint().pt)
			{
				// Must have a single decreasing radius segment
				if (pNode->GetRadiusFunction().Size() == 1 &&
					pNode->GetRadiusFunction().GetHead().m < 0)
				{
					// Cannot be too long
					if (pNode->Length() < 2 * pNode->FirstShockPoint().radius)
					{
						pNode->SetType(BGElement::CORNER_BONE);
					}
				}
			}
		}
	}
}

//! Extends the behaviour of the same function in the base class.
std::istream& BoneGraph::Read(std::istream& is, bool bOnlyDataForMatching /*= false*/)
{
	DAG::Read(is, bOnlyDataForMatching);

	if (!bOnlyDataForMatching)
	{
		// pSkeleton works as a boolean variable here
		is.read((char*) &m_pSkeleton, sizeof(m_pSkeleton));
		if (m_pSkeleton)
	  	{
			m_pSkeleton = new SkeletalGraph();
			m_pSkeleton->Read(is);
	  	}

		m_skeletalGaps.Read(is);
		m_boundaryGaps.ReadRecursive(is);
	}

	return is;
}

//! Extends the behaviour of the same function in the base class.
std::ostream& BoneGraph::Write(std::ostream& os) const
{
	DAG::Write(os);

	// pSkeleton works as a boolean variable here
	os.write((char*) &m_pSkeleton, sizeof(m_pSkeleton));
	if (m_pSkeleton)
		m_pSkeleton->Write(os);

	m_skeletalGaps.Write(os);
	m_boundaryGaps.WriteRecursive(os);

	return os;
}

//! Extends the behaviour of the same function in the base class.
void BoneGraph::Print(std::ostream& os, bool bXMLFormat) const
{
	if (bXMLFormat)
	{
		XML_OPEN_ATTS(os, "DAG");
		XML_PRINT_ATT2(os, "class", ClassName());
		XML_PRINT_ATT2(os, "nodeCount", GetNodeCount());
		XML_END_ATTS(os);

		DAG::Print(os, bXMLFormat);

		XML_CLOSE(os, "DAG");
	}
	else
	{
		DAG::Print(os, bXMLFormat);

		os << "\nBoneGraph specific:\n";
	}
}

/*!
	We MUST have this operator, otherwise C++ would create one for us
	with this exact parameters (and highest precedence over every other
	operator=), and that may cause problems as we add certain member variables.
*/
BoneGraph& BoneGraph::operator=(const BoneGraph& rhs)
{
	DAG::operator=(rhs);

	// copy some other stuff here

	return *this;
}

/*!
	Extends the behaviour of the same function in the base class. This function may
	never be called, but we leave it to make sure that if it is ever called, it does the
	right thing.
*/
DAG& BoneGraph::operator=(const DAG& rhs)
{
	// Make sure we have a ShockGraph on the rhs
	const BoneGraph* pRhs = dynamic_cast<const BoneGraph*>(&rhs);

	if (!pRhs)
		THROW_EXCEPTION("Invalid pointer type.");

	// Now, can can proceed as usual with the right operator=
	return operator=(*pRhs);
}

/*!
	Relation function for two Shock Graph nodes.
	For now, all nodes of the same type are related.
*/
bool BoneGraph::AreNodesRelated(leda_node u, const DAG& from, leda_node v) const
{
	const BoneGraph& bgFrom = dynamic_cast<const BoneGraph&>(from);

	// add stuff here

	return true;
}

//! Creates a new bone graph object.
DAG* BoneGraph::CreateObject() const
{
	return new BoneGraph;
}

//! Creates a new bone graph node. Is used in VisualDAG() to add nodes dinamically
DAGNodePtr BoneGraph::CreateNodeObject(NODE_LABEL lbl) const
{
	return new BGNode;
}

/*!
	Creates a new BGNode and fill its contents with the current
	information in the stream.
*/
DAGNodePtr BoneGraph::ReadNode(std::istream& is) const
{
	DAGNodePtr node(new BGNode);

	node->Read(is);

	return node;
}

/*!
	Creates a new BGEdge and fill its contents with the current
	information in the stream.
*/
DAGEdgePtr BoneGraph::ReadEdge(std::istream& is)
{
	DAGEdgePtr edge(new BGEdge);

	edge->Read(is);

	return edge;
}

//! Returns the classs name
String BoneGraph::ClassName() const
{
	return "BoneGraph";
}

/*!
	@brief Gives the slope direction of the shock points of bone node u
	with respect to the parent node v.
*/
int BoneGraph::GetSlopeDir(leda::node u, leda::node wrtV) const
{
	leda::edge e;

	forall_in_edges(e, u)
	{
		// When found, revert the slope dir if it does NOT flow from parent
		if (source(e) == wrtV)
			return GetBGNode(u)->SlopeDir(!GetBGEdge(e)->FlowsFromParent());
	}

	ASSERT(false);
	return 0;
}

/*!
	@brief Creates a bone graph from an image file or an image in memory.
	If imgInfo.pField == NULL, then an image file is read from imgInfo.strFileName.
*/
bool BoneGraph::Create(const ImageInfo& imgInfo, const BoneGraphParams& bgparams,
                        const SkeletalGraphParams& skelparams)
{
	Clear(); //make sure the current bone graph is empty

	SetDAGLbl(imgInfo.strFileName);

	m_pSkeleton = new SkeletalGraph();

	m_pSkeleton->Create(imgInfo, skelparams, &m_dims);

	// Make sure that we have a valid skeletal graph
	if (!m_pSkeleton->Regularize())
		return false;

	// Assign boundary information to each skeleton point. Make sure that
	// FLUX-BASED skeletons have zero smoothing
	if (skelparams.nSkelAlgorithm == SkeletalGraphParams::FluxAlgorithm)
		m_pSkeleton->AssignBoundaryInfo(0);
	else
		m_pSkeleton->AssignBoundaryInfo(bgparams.nSmoothIter);

	BoneGraphConstructor bgc(this, bgparams);

	//m_pSkeleton->SetBezierApproximation(bgc.GetBezierCurves());

	bool bCreated = bgc.CreateBoneGraph(m_pSkeleton);

	if (bCreated)
		ComputeDerivedValues();

	return bCreated;
}

/*!
	@brief Creates a bone graph from a given skeletal graph.

	The dimensions and graph label must be given, since they
	depend on the source image of the skeleton.
*/
bool BoneGraph::Create(SkeletalGraph* pSkeleton, const ShapeDims& dims,
					   String strLbl, const BoneGraphParams& bgparams)
{
  	DAG::Clear();

	m_dims = dims;
	SetDAGLbl(strLbl);

	m_pSkeleton = pSkeleton;

	// Make sure that we have a valid skeletal graph
	if (!m_pSkeleton->Regularize())
		return false;

	// Recompute boundary information to each skeleton point.
	m_pSkeleton->AssignBoundaryInfo(bgparams.nSmoothIter);

	BoneGraphConstructor bgc(this, bgparams);

	bool bCreated = bgc.CreateBoneGraph(m_pSkeleton);

	if (bCreated)
		ComputeDerivedValues();

	return bCreated;
}

/*!
	@brief Computes the topological signature vectors of all nodes
	that can be reached from the 'root' while respecting edge directions.
*/
double BoneGraph::ComputeTSVs(leda::node root)
{
	return DAG::ComputeTSVs(root);
}