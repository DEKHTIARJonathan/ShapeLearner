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
 * $Log: GeneralizedSkeletalGraph.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __GENERALIZED_SKELETAL_GRAPH_H__
#define __GENERALIZED_SKELETAL_GRAPH_H__

#include <list>
#include <LEDA/graph/graph.h>
#include "LEDA_utils.h"
#include "SkeletalGraph.h"
#include "LigatureDetection.h"
#include "SharedPtr.h"

namespace dml {
struct GSGParams
{
	bool bUseKnownPtsToRestoreBndry;
	int nIgnoreLigature;
	int nSubtractLigatureFromCrossings;
	int nShowDependencyGraph;
	int nShowGSG;

	LTDouble maxRadiusRatio, maxCumulativeBAR, maxCornerAngle;

	GSGParams()
	{
		// When testing InterpolateBoundaryGap(), we may want to set
		// this parameter to true, for comparing results (see function's notes)
		bUseKnownPtsToRestoreBndry = false;
		nIgnoreLigature = 0;
		nSubtractLigatureFromCrossings = 1;
		nShowDependencyGraph = 0;
		nShowGSG = 0;

		maxRadiusRatio.Set(0.9);
		maxCumulativeBAR.Set(0.75);
		maxCornerAngle.Set(120);
	}
};

enum GSGNodeType {GSG_BRANCH, GSG_JUNCTION, GSG_ATTACHMENT, GSG_CROSSING, GSG_ENDPOINT};
enum GSGEdgeType {GSG_N_EDGE, GSG_J_EDGE, GSG_A_EDGE};

//! Renames BranchGapInfo to a more meaningful type for the GSG context
typedef BranchGapInfo AttachEdges;

/*!
	@brief Node of a generalized skeletal graph.

	If a node is set as "broken", it means that it has multiple
	segments and there is no "fixed branch" info.
*/
class GSGNode
{
	bool m_bIsBroken;                //!< branch got new segments since last fix
	SkelBranch* m_pFixedBranch;      //!< interval and pointer to last fixed branch
	leda::node m_nodeHolder;         //!< the leda node that contains the GSGNode

public:
	GSGNodeType m_type;              //!< node type: JUNCTION or ATTACHMENT
	SkelJoint* m_pJoint;             //!< related joint point (used if type is JUNCTION)
	BranchSegmentList m_segments;    //!< branch segments forming the branch

	BezierSegmentArray m_gapAxes;  //!< Set of axis functions of all gap segments

	GSGNode()
	{
		m_bIsBroken = false;
		m_pFixedBranch = NULL;
		m_nodeHolder = nil;
	}

	GSGNode(GSGNodeType type, SkelJoint* pJoint)
	{
		m_type = type;
		m_pJoint = pJoint;
		m_bIsBroken = false;
		m_pFixedBranch = NULL;
		m_nodeHolder = nil;
	}

	GSGNode(GSGNodeType type, SkelBranch* pBranch)
	{
		m_type = type;
		m_pJoint = NULL;
		m_bIsBroken = false;
		m_pFixedBranch = NULL;
		m_nodeHolder = nil;

		m_segments.push_back(BranchSegment(pBranch));
	}

	void operator=(const GSGNode& rhs)
	{
		m_type         = rhs.m_type;
		m_pJoint       = rhs.m_pJoint;
		m_segments     = rhs.m_segments;

		// Do not copy node holder.

		// Do not copy the fixed branch. It can be fixed again if needed.
		CleanBranchFix();
		m_bIsBroken    = (rhs.m_bIsBroken || rhs.m_pFixedBranch);
	}

	~GSGNode() { CleanBranchFix(); }

	void CleanBranchFix()
	{
		if (m_pFixedBranch)
		{
			delete m_pFixedBranch;
			m_pFixedBranch = NULL;
		}
	}

	void SetNodeHolder(leda::node v) { m_nodeHolder = v; }

	void SetBroken() { m_bIsBroken = true; }

	bool HasBrokenBranch() const { return m_bIsBroken; }
	bool HasFixedBranch() const  { return (!m_bIsBroken && m_pFixedBranch); }

	//! Sets a new fixed branch. If a fixed branch already exists, it deletes it
	void SetFixedBranch(SkelBranch* pNewFixedBranch)
	{
		delete m_pFixedBranch;

		m_pFixedBranch = pNewFixedBranch;
		m_bIsBroken = false;
	}

	//! Returns a pointer to fixed branch (without doing any checks)
	SkelBranch* GetFixedBranch() const
	{
		return m_pFixedBranch;
	}

	//! Returns the fixed branch if there is one, or the original branch if not
	SkelBranch* GetCurrentBranch() const
	{
		return (HasFixedBranch()) ? m_pFixedBranch : m_segments.front().m_pBranch;
	}

	//! Removes the trailing ligature in m_segments around a gap joint
	void SubtractGapLigature(const LigatureSegment& ls)
	{
		// We cannot subtract ls from the fixed branch because
		// ls is given wrt the original branch. Otherwise, we would
		// need an ls1 given wrt the fixed branch (plus the given ls)
		if(HasFixedBranch())
			SetBroken();

		m_segments.SubtractLigatureInterval(ls);
	}

	BranchSegment GetBonePart() const;

	SkelPtCoord GetJointCoord() const
	{
		ASSERT(m_pJoint);
		return m_pJoint->fp.p;
	}

	friend std::ostream& operator<<(std::ostream& os, const GSGNode& v) { return os; }
	friend std::istream& operator>>(std::istream& is, GSGNode& v) { return is; }
};

//! Edge of a generalized skeletal graph
class GSGEdge
{
	LigatureSegment m_ligature;   //!< the ligature info represented by the edge
	leda::edge m_edgeHolder;      //!< the leda edge that contains the GSGEdge
	GSGEdgeType m_edgeType;

public:
	SkelPtIndex m_position;       //!< position of the attachment wrt source branch
	int m_nSide;                  //!< side of the attachment wrt source branch

	SkelJoint* m_pAttachJoint;    //!< joint at the break point

	GSGEdge(SkelJoint* pJoint = NULL)
	{
		m_edgeHolder = nil;
		m_pAttachJoint = pJoint;
		m_edgeType = GSG_N_EDGE;
		m_nSide = 0;
		m_position = 0;
	}

	//! Sets the information associated with the junction represented by the edge
	void SetJunctionInfo(leda::edge e, SkelJoint* pJoint)
	{
		ASSERT(e != nil);
		ASSERT(pJoint != NULL);
		ASSERT(m_ligature.m_pBranch == NULL);

		m_edgeHolder = e; // so that we know who contains this edge
		m_pAttachJoint = pJoint;
		m_edgeType = GSG_J_EDGE;
	}

	//! Sets the ligature information associated with the GSGEdge contained in node 'e'
	void SetLigatureInfo(leda::edge e, SkelJoint* pJoint, const LigatureSegment& ls)
	{
		ASSERT(e != nil);
		ASSERT(pJoint != NULL);
		//ASSERT(ls.m_pBranch != NULL);
		ASSERT(!ls.IsEmpty());

		m_edgeHolder = e; // so that we know who contains this edge
		m_pAttachJoint = pJoint;
		m_ligature = ls;
		m_edgeType = GSG_A_EDGE;
	}

	void UpdateLigatureInfo();

	//! Gets the up-to-date ligature information
	const LigatureSegment& GetLigatureInfo()
	{
		UpdateLigatureInfo(); // makes sure info is up to date

		return m_ligature;
	}

	//! Returns true iff the associated ligature segment is not empty
	bool HasLigaturePoints()
	{
		return !GetLigatureInfo().IsEmpty();
	}

	//! Gets the target branch of the attachment represented by the edge
	const SkelBranch* GetTargetBranch()
	{
		return GetLigatureInfo().m_pBranch;
	}

	void ExtendLigature(const LigatureSegment& ls);

	void RemoveLigature()
	{
		UpdateLigatureInfo(); // makes sure info is up to date

		m_ligature.SetEmpty();
	}

	friend std::ostream& operator<<(std::ostream& os, const GSGEdge& v) { return os; }
	friend std::istream& operator>>(std::istream& is, GSGEdge& v) { return is; }
};

/*!
	@brief The GeneralizedSkeletalGraph class ...
*/
class GeneralizedSkeletalGraph : public leda::GRAPH<GSGNode, GSGEdge>
{
protected:
	//!< Dependency graph node information
	struct DGNode
	{
		SkelJoint* pJoint;
		GSGNodeType type;
		SkelEdges ligBranches;
		SkelEdges nonLigBranches;
		friend std::ostream& operator<<(std::ostream& os, const DGNode& v) { return os; }
		friend std::istream& operator>>(std::istream& is, DGNode& v) { return is; }
	};

	typedef SharedPtr<DGNode> DGNodePtr;

	friend class GSGEdge;  //!< Needs to call FixBrokenBranch() for some cases

public:
	//!< Dependency graph of SkeletalGraph nodes
	class DependencyGraph : public leda::GRAPH<DGNodePtr, int>
	{
	public:
		leda::node NewNode(DGNodePtr ptrNode)
		{
			return new_node(ptrNode);
		}

		leda::edge NewEdge(const SkelNode* p0, const SkelNode* p1)
		{
			return new_edge((leda::node)p0->pNodeData, (leda::node)p1->pNodeData);
		}

		const DGNode& GetNode(const SkelNode* pNode) const
		{
			return *(inf((leda::node)pNode->pNodeData));
		}

		GSGNodeType GetType(const SkelNode* pNode) const
		{
			return (pNode->degree() > 1) ? GetNode(pNode).type:GSG_ENDPOINT;
		}

		SkelNode* GetJoint(leda::node v)
		{
			return operator[](v)->pJoint;
		}

		void AddNodeDependecies(const SkelJoint* pJoint0,
			const SkelBranch* pSrcBranch, const SkelJoint* pSrcJoint = NULL);
	};

protected:
	DependencyGraph m_dependencyGraph;     //!< Dependcy graph for skeletal nodes

	struct MergeInfo
	{
		leda::node attachNode;     //!< ATTACHMENT node
		AttachEdges* pAttachEdges; //!< edges incident to the attchment

		MergeInfo(leda::node v, AttachEdges* p)
		{
			attachNode = v;
			pAttachEdges = p;
		}
	};

protected:
	SkeletalGraph* m_pSkeletalGraph;      //!< Source skeleton to translate
	GSGParams m_params;                   //!< GSG creation parameters
	SkelEdges m_extraSkelEdges;           //!< extra Skel Graph edges to delete

protected:
	GSGNodeType LabelJoint(SkelJoint* pJoint, SkelEdges& nonLigBranches,
		LigatureSegmentList& ligSegs, bool bUseFixedBranch = false);

	void CreateDependencyGraph();
	bool HasPotentialNestedLigatureEndpoint(SkelJoint* pJoint, SkelBranch* pBranch) const;
	void ProcessJoint(SkelJoint* pJoint);
	MergeInfo MergeBranchNodes(leda::node u, leda::node v, SkelJoint* pJoint);
	void FixBrokenBranch(leda::node v);
	void RestoreAxisFunction(GSGNode& gnode, SkelBranch* pNewBranch);
	void RestoreDerivedValues(GSGNode& gnode, SkelBranch* pNewBranch);
	void RemoveCrossingSpokes(GSGNode& gnode, SkelBranch* pNewBranch);
	void UpdateFixedBranchSegment(GSGNode& gnode, SkelBranch* pNewBranch);
	SkelBranch* FindSourceBranch(SkelJoint* pJoint, const SkelEdges& branches);
	SkelBranch* FindCrossingMiddleBranch(SkelJoint* pJoint);

	bool CheckAxialSmoothness(SkelJoint* pJoint,
		const LigatureSegmentList& nonLig,
		const LigatureSegmentList& ligSegs);

	double ComputeMaxGapCurvatureAngle(SkelJoint* pJoint,
		const LigatureSegmentList& nonLig,
		const LigatureSegmentList& ligSegs);

	bool ValidateBranchMerge(SkelJoint* pJoint, const LigatureSegmentList& nonLig,
		const LigatureSegmentList& ligSegs);

	bool ValidateAttachment(SkelJoint* pJoint, const LigatureSegmentList& nonLig,
		const LigatureSegmentList& ligSegs);

	bool AreBranchesCollinear(const LigatureSegment& ls0, const LigatureSegment& ls1);

	void GetLigatureAtJoint(SkelJoint* pJoint, LigatureSegmentList& nonLig,
		LigatureSegmentList& ligSegs, LigatureSegmentList& semiLigSegs,
		bool bUseFixedBranch, SkelBranch* pExcludeBranch = NULL);

	bool LabelCrossing(SkelJoint* pJoint, LigatureSegmentList& nonLig,
		LigatureSegmentList& ligSegs, LigatureSegmentList& semiLigSegs,
		bool bUseFixedBranch);

	bool IsEndToEndAttachment(SkelJoint* pJoint, LigatureSegmentList& nonLig,
		LigatureSegmentList& ligSegs, LigatureSegmentList& semiLigSegs);

	bool IsElbowAttachment(SkelJoint* pJoint, LigatureSegmentList& nonLig,
		LigatureSegmentList& ligSegs, LigatureSegmentList& semiLigSegs);

	void InterpolateSkeletalGap(const BranchSegmentList::const_iterator& it0,
		bool bIsFirstSegment, const BranchSegmentList::const_iterator& it1,
		bool bIsLastSegment, const BranchGapList::iterator& attEdgeIt,
		sg::FluxPointList& gapFluxPts, sg::BoundaryInfoList& gapBndryPts, CubicBezier& cb);

	void InterpolateBoundaryGap(int nSide, const CubicBezierParams& cbp,
		const SmartArray<double>& gapParams, const sg::FluxPointList& fpl,
		sg::BoundaryInfoList& bil);

	void ComputeInterpolationCurve(const BranchSegment& bs0, bool bIsFirstSeg0,
		const BranchSegment& bs1, bool bIsLastSeg1, const LigatureSegment& ls,
		CubicBezier& cb, SkelPtIndex& ptIdx0, SkelPtIndex& ptIdx1);

	int SetAttachmentSides(const BranchSegment& bs0, const BranchSegment& bs1,
		AttachEdges& attchs);
	void SetAttachmentPositions(SkelJoint* pJoint, const sg::FluxPointList& fpl,
		AttachEdges& attchs);
	inline void IncrementAttachmentPositions(SkelPtIndex inc, AttachEdges& attchs);

	SkelBranch* GetOriginalBranch(SkelBranch* pFixedBranch, SkelJoint* pJoint);

	leda::node NewNode(GSGNodeType type, SkelJoint* pJoint)
	{
		leda::node v = new_node(GSGNode(type, pJoint));
		operator[](v).SetNodeHolder(v);
		return v;
	}

	leda::node NewBranchNode(SkelBranch* pBranch)
	{
		leda::node v = new_node(GSGNode(GSG_BRANCH, pBranch));
		operator[](v).SetNodeHolder(v);
		return v;
	}

	leda::node NewJunctionNode(SkelJoint* pJoint)
	{
		return NewNode(GSG_JUNCTION, pJoint);
	}

	leda::node NewAttachmentNode(SkelJoint* pJoint)
	{
		return NewNode(GSG_ATTACHMENT, pJoint);
	}

	leda::node NewCrossingNode(SkelJoint* pJoint)
	{
		return NewNode(GSG_CROSSING, pJoint);
	}

	leda::edge NewEmptyEdge(leda::node u, leda::node v)
	{
		return new_edge(u, v); // inits position, side, etc to zero
	}

	leda::edge NewJunctionEdge(leda::node u, leda::node v, SkelJoint* pJoint)
	{
		leda::edge e = new_edge(u, v); // inits position, side, etc to zero
		operator[](e).SetJunctionInfo(e, pJoint);
		return e;
	}

	leda::edge NewLigatureEdge(leda::node u, leda::node v, SkelJoint* pJoint,
		const LigatureSegment& ls)
	{
		leda::edge e = new_edge(u, v); // inits position, side, etc to zero
		operator[](e).SetLigatureInfo(e, pJoint, ls);
		return e;
	}

public:
	leda::node GetNode(SkelBranch* pBranch)
	{
		return (leda::node)pBranch->pEdgeData;
	}

	GSGNode& GetNode(leda::node v)
	{
		return operator[](v);
	}

	GSGNodeType GetNodeType(leda::node v)
	{
		return GetNode(v).m_type;
	}

	GSGNode& GetNodeData(SkelBranch* pBranch)
	{
		return GetNode(GetNode(pBranch));
	}

	GSGEdge& GetEdge(leda::edge e)
	{
		return operator[](e);
	}

	leda::node AddRootBranchNodeToJunctionNode(leda::node v);

	SkelBranch* NewSkelBranchFromJoint(SkelJoint* pJoint);

	//! Returns the first out edge of v with target node of type 'type'
	leda::edge GetOutEdge(leda::node v, GSGNodeType type)
	{
		leda::edge e;

		forall_out_edges(e, v)
			if (GetNode(target(e)).m_type == type)
				return e;

		return nil;
	}

	//! Returns the node of type 'type' from the list of out edges of v
	leda::node GetTargetNode(leda::node v, GSGNodeType type)
	{
		leda::edge e;

		forall_out_edges(e, v)
			if (GetNode(target(e)).m_type == type)
				return target(e);

		return nil;
	}

	//! Returns the in-edges whose source is an attached node (at most 2)
	void GetSourceAttachments(leda::node v, std::vector<leda::edge>* vec)
	{
		leda::edge e;

		vec->clear();

		forall_in_edges(e, v)
			if (GetNode(source(e)).m_type == GSG_ATTACHMENT)
				vec->push_back(e);

		ASSERT(vec->size() <= 2);
	}

public:
	~GeneralizedSkeletalGraph();

	bool Create(SkeletalGraph* pSkeletalGraph, const GSGParams& params);

	bool ExtendRootedLigature(leda::node v, const LigatureSegment& ls);

	SkeletalGraph* GetOriginalSkeletalGraph() { return m_pSkeletalGraph; }
};
} // namespace dml

#endif //__GENERALIZED_SKELETAL_GRAPH_H__
