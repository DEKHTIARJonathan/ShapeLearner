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
 * $Log: GeneralizedSkeletalGraph.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

#ifdef _DEBUG
#define GSG_ENABLE_GRAPH_WIN
#endif

#ifdef GSG_ENABLE_GRAPH_WIN
#include "LEDA_graphwin_utils.h"
#endif

#define UNKNOWN_RADIUS_UPPER_BOUND_FACTOR 2
//#define PI_OVER_TWO 1.57079632679

using namespace dml;

DBG_ONLY(extern LogFile g_dagCreationLog)

typedef GeneralizedSkeletalGraph GSG;

// Helper functions prototypes
unsigned int GetFirstConnectedPointIndex(const sg::FluxPointList& fpl,
										 const BranchSegment& bs);

unsigned int GetFirstConnectedPointIndex(const sg::FluxPointList& fpl0,
										 const sg::FluxPointList& fpl1);

void RemoveFirstMutualDependency(GSG::DependencyGraph& dg)
{
	leda::edge e0, e1;
	leda::node v0, v1;

	forall_edges(e0, dg)
	{
		v0 = dg.source(e0);
		v1 = dg.target(e0);

		forall_out_edges(e1, v1)
		{
			if (target(e1) == v0)
			{
				dg.del_edge(e1);
				return;
			}
		}
	}

	ASSERT(false);
}

#ifdef GSG_ENABLE_GRAPH_WIN
void ShowDependencyGraph(GSG::DependencyGraph& dg);
void ShowGSG(GSG& g, leda::node redNode = nil, leda::node blueNode = nil);
#endif

/*!
	@brief Removes all extra skeletal branches created during the construction
	of the GSG
*/
GSG::~GeneralizedSkeletalGraph()
{
	SkelBranch* pBranch;

	forall_branches(pBranch, m_extraSkelEdges)
		delete pBranch;
}

/*!
	@brief Creates a generalized skeletal graph from the given skeletal graph.

	TODO: outline algorithm.
	- initially, all branches are assigned to a different "branch" node
	- construct a dependency graph and process the nodes according to it
	  - each junction points is assigned to a new "junction" node
	  - each attachment points causes the merging of two branch node and
	    is assigned to a new or existing "attachment" node
*/
bool GSG::Create(SkeletalGraph* pSkeletalGraph, const GSGParams& params)
{
	m_pSkeletalGraph = pSkeletalGraph;
	m_params = params;

	LigatureSegment::s_pBndryPtFinder    = m_pSkeletalGraph->GetBndryPtFinder();

	LigatureSegment::s_maxRadiusRatio    = m_params.maxRadiusRatio;
	LigatureSegment::s_maxCumulativeBAR  = m_params.maxCumulativeBAR;
	LigatureSegment::s_minCornerCosAngle = m_params.maxCornerAngle.Cos(true);

	LigatureSegment::s_minCornerCosAngle.tight = LigatureSegment::s_minCornerCosAngle.loose;

	CreateDependencyGraph();

	DBG_C_LOG_BEGIN_GROUP
	DBG_C_LOG("Dependency graph created. Begining branch fusion operations...")

#ifdef GSG_ENABLE_GRAPH_WIN
	if (params.nShowDependencyGraph)
		ShowDependencyGraph(m_dependencyGraph);
#endif

	SkelBranch* pBranch;

	forall_branches(pBranch, m_pSkeletalGraph->getEdges())
	{
		pBranch->pEdgeData = NewBranchNode(pBranch);
	}

	leda::list<leda::node> roots;
	leda::node v;
	int nNodesProcessed;

	while (!m_dependencyGraph.empty())
	{
		nNodesProcessed = 0;

		forall_nodes(v, m_dependencyGraph)
		{
			if (outdeg(v) == 0)
			{
				roots.push_back(v);

				const DGNode& dgnode = *m_dependencyGraph.inf(v);

				DBG_C_LOG_BEGIN_GROUP
				DBG_C_LOG("Searching for branches that need restoration "
					"before processing joint at " << PRN_JOINT_PTR(dgnode.pJoint))

				// Only for attachments and crossings...
				if (dgnode.type == GSG_ATTACHMENT || dgnode.type == GSG_CROSSING)
				{
					// Fix ligature branches that are broken
					forall_const_branches(pBranch, dgnode.ligBranches)
						if (GetNodeData(pBranch).HasBrokenBranch())
							FixBrokenBranch(GetNode(pBranch));

					// If a non-ligatire branch ends at a junction points that is
					// a potential nested attachment, and it's broken, we fix it
					forall_const_branches(pBranch, dgnode.nonLigBranches)
						if (HasPotentialNestedLigatureEndpoint(dgnode.pJoint, pBranch) &&
							GetNodeData(pBranch).HasBrokenBranch())
						{
							FixBrokenBranch(GetNode(pBranch));
						}
				}
				else if (dgnode.type == GSG_JUNCTION)
				{
					// Fix non-ligature branches that are broken
					forall_const_branches(pBranch, dgnode.nonLigBranches)
						if (GetNodeData(pBranch).HasBrokenBranch())
							FixBrokenBranch(GetNode(pBranch));
				}

				ProcessJoint(m_dependencyGraph.GetJoint(v));

				//DBG_MSG2(m_dependencyGraph.GetJoint(v)->fp, "processed");

				nNodesProcessed++;
			}
		}

		WARNING(nNodesProcessed == 0, "There are dependency cycles (crossings issue?)");

		if (nNodesProcessed == 0)
			RemoveFirstMutualDependency(m_dependencyGraph);

		//DBG_STREAM("\n----------------------------------");

		m_dependencyGraph.del_nodes(roots);
		roots.clear();
	}

	// Make sure that all branch nodes are repaired
	forall_nodes(v, *this)
	{
		if (GetNode(v).HasBrokenBranch())
			FixBrokenBranch(v);
	}

#ifdef GSG_ENABLE_GRAPH_WIN
	if (params.nShowGSG)
		ShowGSG(*this);
#endif

	return true;
}

/*!
	@brief If the given joint is a junction points, it is assigned to a new node.
	Else, it is an attachment points, which causes the merging of two branch node
	and the association with a new or existing attachment node.
*/
void GSG::ProcessJoint(SkelJoint* pJoint)
{
	LigatureSegmentList ligSegs;
	SkelEdges nonLigBranches;

	GSGNodeType type = LabelJoint(pJoint, nonLigBranches, ligSegs, true);

	ASSERT(!m_params.nIgnoreLigature || (m_params.nIgnoreLigature && type == GSG_JUNCTION));

	if (type == GSG_JUNCTION)
	{
		leda::node j = NewJunctionNode(pJoint);

		// Add edges between all insident branches and the junction node
		SkelBranch* pBranch;

		forall_branches(pBranch, nonLigBranches)
		{
			NewJunctionEdge(j, GetNode(pBranch), pJoint); // flows away from joint
		}

		////////////
		LigatureSegmentList::iterator it;

		for (it = ligSegs.begin(); it != ligSegs.end(); it++)
		{
			NewLigatureEdge(j, GetNode(it->m_pBranch), pJoint, *it);
		}
		//////////////
	}
	else if (type == GSG_ATTACHMENT)
	{
		leda::node u, v;
		LigatureSegment ls;

		u = GetNode(nonLigBranches[0]);
		v = GetNode(nonLigBranches[1]);

		DBG_C_LOG("Attachment detected. Finding gap ligature")

		// Find the gap ligature. Make sure to use the original branches instead of the
		// fixed branches. Otherwise, the gap lig can't be subtracted from 'm_segments'
		ls.FindGapLigature(GetOriginalBranch(nonLigBranches[0], pJoint), pJoint, ligSegs);
		GetNode(u).SubtractGapLigature(ls);

		ls.FindGapLigature(GetOriginalBranch(nonLigBranches[1], pJoint), pJoint, ligSegs);
		GetNode(v).SubtractGapLigature(ls);

		DBG_C_LOG("Gap ligature found. Recording broken branches to be fused (later)")

		// Merge nodes. u is the node that stays and v the one that is deleted
		MergeInfo mi = MergeBranchNodes(u, v, pJoint);

		// Add edges between the attaching branches and the ATTACHMENT node 'a'
		LigatureSegmentList::iterator it;
		leda::edge e;

		for (it = ligSegs.begin(); it != ligSegs.end(); it++)
		{
			v = GetNode(it->m_pBranch);

			e = NewLigatureEdge(mi.attachNode, v, pJoint, *it);

			mi.pAttachEdges->push_back(e);  // assoc the joint to the new edge
		}
	}
	else
	{
		ASSERT(type == GSG_CROSSING);

		leda::node c, v;

		if (nonLigBranches.size() == 0) //non-generic crossing
		{
			SkelBranch* pSrcBranch = NewSkelBranchFromJoint(pJoint);

			v = NewBranchNode(pSrcBranch);
			c = nil;

			pSrcBranch->pEdgeData = v;
		}
		else
		{
			ASSERT(nonLigBranches.size() == 1);

			v = GetNode(nonLigBranches[0]);

			// See if the branch has an assoc attachment
			c = GetTargetNode(v, GSG_CROSSING);
		}

		// If there is no associated crossing node, create one
		// else, we already processed this crossing (from other endpoint)
		if (c == nil)
		{
			c = NewCrossingNode(pJoint);
			NewEmptyEdge(v, c); // flows towars the crossing

			LigatureSegmentList::iterator it;

			for (it = ligSegs.begin(); it != ligSegs.end(); it++)
			{
				v = GetNode(it->m_pBranch);

				if (m_params.nSubtractLigatureFromCrossings && it->IsLigature())
				{
					// note that a crossing there may be multiple joint points
					NewLigatureEdge(c, v, it->m_pJoint /*joint of this ligature*/, *it);
				}
				else
				{
					NewLigatureEdge(c, v, pJoint, *it);
				}
			}
		}
	}
}

/*!
	@brief Finds the branch in the input skeleton associated with the
	given fixed branch and joint. Such a branch is the one that
	refers to the same GSG node than the fixed branch and that is incident
	on the given junction point.

	Note: if 'pFixedBranch' is already the original branch, ie, the branch was
	never fixed, the function returns 'pFixedBranch'.
*/
SkelBranch* GSG::GetOriginalBranch(SkelBranch* pFixedBranch, SkelJoint* pJoint)
{
	SkelBranch* pBranch;

	forall_branches(pBranch, pJoint->edges)
	{
		if (pBranch->pEdgeData == pFixedBranch->pEdgeData)
			return pBranch;
	}

	ASSERT(false);

	return NULL;
}

/*!
	@brief Merges two "branch" nodes and labels the resulting branch
	as broken. Every merged branch has an associated attachment node that
	can be found with the GetAttachmentNode() function.

	The node 'v' is always the one that is removed, and all its atrributes
	are moved to node 'u'. The function performs faster if node 'v' is the
	node with less branch segments.

	@param u destination branch node
	@param v source branch node
	@param pJoint is the joint shared by the two branches
	@return the attachment node of the merged node 'u', and the attachment
	        edges incident to the given joint.
*/
GSG::MergeInfo GSG::MergeBranchNodes(leda::node u, leda::node v, SkelJoint* pJoint)
{
	GSGNode& uNode = GetNode(u);
	GSGNode& vNode = GetNode(v);

	ASSERT(uNode.m_type == GSG_BRANCH && vNode.m_type == GSG_BRANCH);

	// Should have at most two in-(junction/attachment) and one out-(junction/attachment)
	//ASSERT(outdeg(u) <= 1 && indeg(u) <= 2 && indeg(u) + outdeg(u) <= 2);
	//ASSERT(outdeg(v) <= 1 && indeg(v) <= 2 && indeg(v) + outdeg(v) <= 2);

	leda::edge e;

	// Move all in edges of v as in edges of u
	forall_in_edges(e, v)
	{
		move_edge(e, source(e), u);
	}

	// Move all out edges of v that go to a junction node as out edges of u
	forall_out_edges(e, v)
	{
		if (GetNode(target(e)).m_type == GSG_JUNCTION)
			move_edge(e, u, target(e));
	}

	/*if (outdeg(v) > 1 || outdeg(u) > 3)
	{
		ShowGSG(*this, u, v);
	}*/

	// u and v can only have one target attachment. u can also have a target junction
	ASSERT(outdeg(v) <= 1 && outdeg(u) <= 3);

	// There can be at most one attachment node per branch
	leda::edge edgeUA1 = GetOutEdge(u, GSG_ATTACHMENT); // goes from branch to attachment
	leda::edge edgeVA2 = GetOutEdge(v, GSG_ATTACHMENT); // goes from branch to attachment
	leda::node a1;

	if (edgeUA1 != nil && edgeVA2 != nil) // both have attachments
	{
		leda::node a2;

		a1 = target(edgeUA1); //ie, there is an edge already from a1 to u
		a2 = target(edgeVA2);

		// Append v's edges to u's list of edges
		forall_out_edges(e, a2)
			move_edge(e, a1, target(e));

		del_node(a2);
	}
	else if (edgeVA2 != nil) // U has no attachment, but V does
	{
		a1 = target(edgeVA2);       // use a2 as a1 but...
		move_edge(edgeVA2, u, a1);  // now make it leave from U instead of V
	}
	else if (edgeUA1 != nil) // U has an attachment and V doesn't
	{
		a1 = target(edgeUA1);
	}
	else // none have attachments, so we must create an attachment node
	{
		a1 = NewAttachmentNode(pJoint);
		edgeUA1 = NewEmptyEdge(u, a1); // flows towards the attachment
	}

	// Make all branches that pointed to v now point to u
	BranchSegmentList::iterator it;

	for (it = vNode.m_segments.begin(); it != vNode.m_segments.end(); it++)
		it->m_pBranch->pEdgeData = u;

	// Merge the branch segment lists of U and V (use OWN Splice())
	BranchGapInfo* pGapInfo = uNode.m_segments.Splice(vNode.m_segments);

	// We are done with node v
	del_node(v);

	// Merging means the branch is now broken
	uNode.SetBroken();

	return MergeInfo(a1, pGapInfo);
}

/*!
	@brief Creates a dependecy graph between nodes such that the attaching branches
	at a junction (ligature branches) are processed before the broken branch. In
	addition, dependencies for the branches incident at a junction are also
	added such that they are processed before the joint. This gives the classifier of
	the junction complete branches to analyze, in case of ties when finding the
	source of the junction.
*/
void GSG::CreateDependencyGraph()
{
	LigatureSegmentList ligSegs;
	LigatureSegmentList::iterator it;
	SkelBranch* pBranch;
	SkelJoint* pJoint;
	DGNode* pDGNode;
	GSGNodeType type;

	// Label all joints
	forall_joints(pJoint, m_pSkeletalGraph->getNodes())
	{
		pDGNode = new DGNode;

		pDGNode->pJoint = pJoint;
		pDGNode->type = LabelJoint(pJoint, pDGNode->nonLigBranches, ligSegs);

		// Copy all ligature branches (most likely just 1 branch per joint)
		for (it = ligSegs.begin(); it != ligSegs.end(); it++)
			pDGNode->ligBranches.push_back(it->m_pBranch);

		pJoint->pNodeData = m_dependencyGraph.NewNode(pDGNode);
	}

	// Add dependencies
	forall_joints(pJoint, m_pSkeletalGraph->getNodes())
	{
		const DGNodePtr& ptrNode = m_dependencyGraph.GetNode(pJoint);
		type = m_dependencyGraph.GetType(pJoint);

		if (type == GSG_ATTACHMENT)
		{
			forall_const_branches(pBranch, ptrNode->ligBranches)    // LIGATURE BRANCHES
				m_dependencyGraph.AddNodeDependecies(pJoint, pBranch);

			// Also consider neighbouring joints with strictly smaller radius than that of pJoint
			forall_const_branches(pBranch, ptrNode->nonLigBranches) // NON LIGATURE BRANCHES
				if (HasPotentialNestedLigatureEndpoint(pJoint, pBranch))
					m_dependencyGraph.AddNodeDependecies(pJoint, pBranch);
		}
		else if (type == GSG_JUNCTION)
		{
			forall_const_branches(pBranch, ptrNode->nonLigBranches) // NON LIGATURE BRANCHES
				m_dependencyGraph.AddNodeDependecies(pJoint, pBranch);
		}
		else
		{
			ASSERT(type == GSG_CROSSING);

			SkelJoint* pJoint2;

			forall_const_branches(pBranch, ptrNode->ligBranches)    // LIGATURE BRANCHES
			{
				if (IsEndpointOf(pJoint, pBranch))
					m_dependencyGraph.AddNodeDependecies(pJoint, pBranch);
				else
				{
					//DBG_PRINT1(ptrNode->ligBranches.size())
					//DBG_PRINT1(ptrNode->nonLigBranches.size())
					ASSERT(ptrNode->nonLigBranches.size() == 1); // one middle branch

					pJoint2 = GetOtherEndpoint(ptrNode->nonLigBranches.front(), pJoint);

					m_dependencyGraph.AddNodeDependecies(pJoint, pBranch, pJoint2);
				}
			}
		}
	}
}

/*!
	@brief Checks if the other endpoint of the given branch is a junction
	with *strictly* smaller radius that that of the given joint, R, and whether
	it is located at a distance smaller or equal to R.
*/
bool GSG::HasPotentialNestedLigatureEndpoint(SkelJoint* pJoint, SkelBranch* pBranch) const
{
	SkelEndpt* pEndpt = GetOtherEndpoint(pBranch, pJoint);

	if (pEndpt->degree() > 1 &&	pEndpt->fp.radius() < pJoint->fp.radius() &&
		pJoint->fp.p.dist(pEndpt->fp.p) <= pJoint->fp.radius() * 1.2)
	{
		return true;
	}

	return false;
}

/*!
	@brief Adds recursively all dependecies along the given branch until the first
	juntion or the first attachment with an attaching branch equal to the given branch.
*/
void GSG::DependencyGraph::AddNodeDependecies(const SkelJoint* pJoint0,
											  const SkelBranch* pSrcBranch,
											  const SkelJoint* pSrcJoint /*=NULL*/)
{
	if (pSrcJoint == NULL)
		pSrcJoint = pJoint0; // the last joint we've seen

	ASSERT(pSrcBranch->n1 == pSrcJoint || pSrcBranch->n2 == pSrcJoint);

	SkelJoint* pJoint1 = (pSrcBranch->n1 != pSrcJoint) ? pSrcBranch->n1:pSrcBranch->n2;

	if (pJoint1->degree() > 1) // if it doesn't end at an endpoint
	{
		const DGNode& node = GetNode(pJoint1);

		if (node.type == GSG_ATTACHMENT)
		{
			SkelBranch* pTgtBranch;

			ASSERT(node.ligBranches.size() >= 1);
			ASSERT(node.nonLigBranches.size() == 2);

			// Check that we don't have an "H" case
			forall_const_branches(pTgtBranch, node.ligBranches)
				if (pTgtBranch == pSrcBranch)
					return;

			NewEdge(pJoint0, pJoint1);

			// There are two non-ligature branches at an attachment, we continue by
			// following the one that extends the path we came from.
			forall_const_branches(pTgtBranch, node.nonLigBranches)
				if (pTgtBranch != pSrcBranch)
					AddNodeDependecies(pJoint0, pTgtBranch, pJoint1);
		}
	}
}

/*!
	@brief Returns the non-ligature, full ligature, and semiligature segments that
	flow away from the joint to the corresponding lists. The sum of non-ligature and
	full ligature segments is guaranteed to be equal to the degree of the joint.

	The semi-ligature segments are a subset of all the ligature segments.

	@param bUseFixedBranch if true, all broken branches incident to the joint are
	fixed before performing the ligature analysis.

	@param pExcludeBranch if not null, it signals a branch that should be excluded
	from the sets of ligature and non-ligature segments. This option is useful
	for branch crossing cases in which the role of the middle branch is already known.

	@see LabelCrossing
*/
void GSG::GetLigatureAtJoint(SkelJoint* pJoint, LigatureSegmentList& nonLig,
                    LigatureSegmentList& ligSegs, LigatureSegmentList& semiLigSegs,
					bool bUseFixedBranch, SkelBranch* pExcludeBranch /*=NULL*/)
{
	// Empty all the given lists
	nonLig.clear();
	ligSegs.clear();
	semiLigSegs.clear();

	DBG_C_LOG_NEW_LINE
	DBG_C_LOG(DBG_VAL(pJoint->fp.p))

	SkelEdges branches(pJoint->degree());
	SkelBranch* pBranch;

	unsigned int i = 0;
	std::vector<int> wasFixed(pJoint->degree());

	forall_branches(pBranch, pJoint->edges)
	{
		wasFixed[i] = false;

		// If we are supposed to used fixed branches, and the current
		// branch is associated with a GSG node that has a fixed branch,
		// then we use the fixed branch instead
		if (bUseFixedBranch && GetNodeData(pBranch).HasFixedBranch()
			&& pBranch != pExcludeBranch)
		{
			pBranch = GetNodeData(pBranch).GetFixedBranch();
			wasFixed[i] = true;
		}

		branches[i++] = pBranch;
	}

	ligSegs.FindRootedLigature(branches, pJoint);

	LigatureSegmentList::iterator it;

	for (it = ligSegs.begin(); it != ligSegs.end(); )
	{
		if (it->m_pBranch == pExcludeBranch)
		{
			it = ligSegs.erase(it); // ignore the lig seg of this branch
		}
		else if (it->IsLigature())
		{
			if (it->IsSemiLigature())
				semiLigSegs.push_back(*it); // add to the special semiLig list too

			it++;
		}
		else // it's a non-ligatures segment
		{
			nonLig.push_back(*it);
			it = ligSegs.erase(it);
		}
	}

	ASSERT((!pExcludeBranch && nonLig.size() + ligSegs.size() == pJoint->degree()) ||
		   (pExcludeBranch && nonLig.size() + ligSegs.size() == pJoint->degree() - 1));
}

/*
void GSG::GetLigatureAtJoint(SkelJoint* pJoint, LigatureSegmentList& nonLig,
                    LigatureSegmentList& ligSegs, LigatureSegmentList& semiLigSegs,
					bool bUseFixedBranch, SkelBranch* pExcludeBranch)
{
	LigatureSegment ls;
	SkelBranch* pBranch;

	nonLig.clear();
	ligSegs.clear();
	semiLigSegs.clear();

	DBG_C_LOG_NEW_LINE
	DBG_C_LOG(DBG_VAL(pJoint->fp.p))

	forall_branches(pBranch, pJoint->edges)
	{
		if (pBranch != pExcludeBranch)
		{
			// If we are supposed to used fixed branches, and the current
			// branch is associated with a GSG node that has a fixed branch,
			// then we use the fixed branch instead
			if (bUseFixedBranch && GetNodeData(pBranch).HasFixedBranch())
				pBranch = GetNodeData(pBranch).GetFixedBranch();

			ls.FindRootedLigature(pBranch, pJoint);

			if (ls.IsFullLigature())
			{
				ligSegs.push_back(ls);
				DBG_C_LOG("it's full ligature. min radius is = " <<DBG_VAL(ls.m_dTargetRadius))
			}
			else if (ls.IsSemiLigature())
			{
				ligSegs.push_back(ls);
				semiLigSegs.push_back(ls); // add to the special semiLig list too
				DBG_C_LOG("it's semi-ligature. min radius is = " <<DBG_VAL(ls.m_dTargetRadius))
			}
			else
			{
				nonLig.push_back(ls);
				DBG_C_LOG("it's non ligature. 'ls' has bndry info =\n" << DBG_VAL(ls))
			}
		}
	}

	// Followinf a clockwise order of branches, there should consistancy in the
	// corners found since neigbouring branches 'see' the same corners!!!

	//DBG_PRINT4(pJoint->fp.p, pJoint->degree(), nonLig.size(), nSemiLig)

	ASSERT((!pExcludeBranch && nonLig.size() + ligSegs.size() == pJoint->degree()) ||
		   (pExcludeBranch && nonLig.size() + ligSegs.size() == pJoint->degree() - 1));
}
*/

/*!
	@brief Determine whether a joint is an "attachment point" or a
	"junction point".

	An attachment point with N+2 branches must have at least N branches that
	begin with a ligature segment. These branches are called "attaching branches"
	while the remaining two are called "parts of a braken branch."

	If more than N branches meet the condition above, the N "attaching" branches
	can be detected by the following property:

	   Let each ligature segment i = 1,...,M, for N < M <= N+2, be defined by
	   skeleton points [a_i, b_i], such that point a_i is common to all N+2 branches.
	   The N attaching branches are those with the smallest radius at their b_i.
*/
GSGNodeType GSG::LabelJoint(SkelJoint* pJoint, SkelEdges& nonLigBranches,
                            LigatureSegmentList& ligSegs, bool bUseFixedBranch)
{
	LigatureSegmentList nonLig, semiLigSegs;

	ASSERT(pJoint->degree() >= 3);

	DBG_C_LOG_BEGIN_GROUP
	DBG_C_LOG(DBG_VAL(pJoint->fp.p) << DBG_VAL(pJoint->degree()) << DBG_VAL(bUseFixedBranch))

	GetLigatureAtJoint(pJoint, nonLig, ligSegs, semiLigSegs, bUseFixedBranch);

	// See if its a crossing (lists are updated as needed)
	if (LabelCrossing(pJoint, nonLig, ligSegs, semiLigSegs, bUseFixedBranch))
	{
		nonLig.CopyTo(nonLigBranches);

		DBG_C_LOG("it's a crossing" << DBG_VAL(nonLigBranches.size()))

		return GSG_CROSSING;
	}

	// Check elbow pattern before EE pattern
	/*if (IsElbowAttachment(pJoint, nonLig, ligSegs, semiLigSegs))
	{
		nonLig.CopyTo(nonLigBranches);

		return GSG_ATTACHMENT;
	}*/

	if (IsEndToEndAttachment(pJoint, nonLig, ligSegs, semiLigSegs))
	{
		nonLig.CopyTo(nonLigBranches);

		return GSG_JUNCTION;
	}

	/*!
		If we have two non-ligature segments, we choose them as the pieces of
		a broken branch. However, if we have less than two non-ligature segments,
		we need to determine which are the branches that must be merged.
		We can either have zero or one one ligatire segments in this case, and
		we select the broken branches by testing all possible merges.
	*/
	if (nonLig.size() < 2)
	{
		LigatureSegmentList::iterator it, nonLigIt0, nonLigIt1;
		LigatureSegmentList testNonLig, testLig, finalNonLig, finalLig;

		DBG_C_LOG("There aren't enough non-ligature segments. Testing all possibilities..")
		DBG_C_LOG_IF(nonLig.front(), !nonLig.empty())

		finalNonLig = nonLig;

		// Iterate over every ligature segment and assume it is an attached branch. Then
		// among the remaining incident branches, there must be two than lead to a valid merge.
		for (it = ligSegs.begin(); it != ligSegs.end(); it++)
		{
			testLig.clear();      // init to zero the list of attached branches
			testNonLig = nonLig;  // init to zero OR one the list of broken branches

			// If there are no broken branch, we select one that is NOT the attached
			// branch being tested. For simplicity, choose the first incident branch.
			if (testNonLig.empty())
				testNonLig.push_back(*ligSegs.FirstDifferentThan(it->m_pBranch));

			// Now, we now that we do have exactly one broken branch, and so need to
			// find a second one. In this case, simply choose the first one that is neither
			// the attached branch or the broken branch.
			testNonLig.push_back(*ligSegs.FirstDifferentThan(it->m_pBranch,
				testNonLig.front().m_pBranch));

			// Add the attached branch being tested to the list of potential attached branches
			testLig.push_back(*it);

			// Check if the pair of broken branches are a valid merge wrt the attached branch
			if (ValidateBranchMerge(pJoint, testNonLig, testLig))
				finalLig.push_back(*it); // if it is, then it is conformed as an attached branch
			else
				finalNonLig.push_back(*it); // if not, it must be a broken branch
		}

		ligSegs = finalLig;   // update the ligature seg list with the info we collected
		nonLig = finalNonLig; // update the non ligature seg list with the info we collected
	}

	// The test above only adds the contextual information provided by the validity of
	// branch merges. It does not guarantee that the number of confirmed broken and attached
	// branches have changed or anything else. It simply added an extra attribute to the decision.

	// If there is less than 2 non-lig branches, we have an incomplete attachment
	if (nonLig.size() < 2)
	{
		/*DBG_C_LOG("***looking for more non-ligature branches "
			<< DBG_VAL(nonLig.size())
			<< DBG_VAL(ligSegs.front().m_dTargetRadius)
			<< DBG_VAL(ligSegs.back().m_dTargetRadius))
		DBG_MSG1("THERE ARE MULTIPLE SOLUTIONS TO MERGING")*/

		// ...we need exactly two non-lig branches to form a gap
		ligSegs.sort(); // sorts ligature segments by ending radius size
		//ASSERT(ligSegs.front().m_dTargetRadius <= ligSegs.back().m_dTargetRadius);

		do {
			nonLig.push_back(ligSegs.back());
			ligSegs.pop_back();
		} while (nonLig.size() < 2); // find 2 non-lig branches
	}
	else if (pJoint->degree() > 3 && nonLig.size() > 2) // it's a non-generic junction
	{
		DBG_C_LOG("***non generic junction " << DBG_VAL(nonLig.size()) << DBG_VAL(ligSegs.size()))

		// All branches must be non-ligature
		nonLig.splice(nonLig.end(), ligSegs);
	}

	if (nonLig.size() != pJoint->degree() &&
		!ValidateBranchMerge(pJoint, nonLig, ligSegs))
	{
		//nonLig.splice(nonLig.end(), ligSegs);
		DBG_C_LOG("invalid branch merge")

		nonLig.CopyTo(nonLigBranches);

		return GSG_JUNCTION;
	}

	/*if (bUseFixedBranch && nonLig.size() != pJoint->degree() &&
		!ValidateAttachment(pJoint, nonLig, ligSegs))
	{
		GetLigatureAtJoint(pJoint, nonLig, ligSegs, semiLigSegs, bUseFixedBranch);
		//nonLig.splice(nonLig.end(), ligSegs);
		nonLig.CopyTo(nonLigBranches);
		DBG_C_LOG("invalid branch attachment")
		return GSG_JUNCTION;
	}*/

	// This are the only valid cases
	ASSERT((nonLig.size() == pJoint->degree() && ligSegs.size() == 0) ||
		   (nonLig.size() == 2 && ligSegs.size() == pJoint->degree() - 2));

	nonLig.CopyTo(nonLigBranches);

	// Return the type of the joint
	if (nonLig.size() == pJoint->degree())
	{
		DBG_C_LOG("it's a junction")
		return GSG_JUNCTION;
	}
	else
	{
		DBG_C_LOG("it's an attachment.\n\tBroken branches: "
			<< PRN_BRANCH_PTR(nonLigBranches[0]) << " -- "
			<< PRN_BRANCH_PTR(nonLigBranches[1]))

		return GSG_ATTACHMENT;
	}
}

/*!
	@brief
*/
bool GSG::AreBranchesCollinear(const LigatureSegment& ls0, const LigatureSegment& ls1)
{
	return false;
}

/*!
	@brief Sets the side of the attachment branches incident at the common joint

	Returns side value if there is only one attachment branch and zero otherwise.
*/
int GSG::SetAttachmentSides(const BranchSegment& bs0, const BranchSegment& bs1,
							AttachEdges& attchs)
{
	// Define the bdry interval formed by the two branches
	TBBInterval tbbi(m_pSkeletalGraph, bs0.m_pBranch, bs1.m_pBranch);
	bool bSwapSides = bs0.IsInverted(); // sides are given wrt left branch above

	// Assign the a side to each attachment
	for (unsigned int i = 0; i < attchs.size(); i++)
	{
		GSGEdge& ge = GetEdge(attchs[i]);

		ge.m_nSide = tbbi.Side(ge.GetTargetBranch(), ge.m_pAttachJoint);

		ASSERT(ge.m_nSide == 1 || ge.m_nSide == 2);

		if (bSwapSides)
			ge.m_nSide = (ge.m_nSide == 1) ? 2 : 1;
	}

	// If there are multiple attachments, there isn't an unique side to return
	return (attchs.size() == 1) ? GetEdge(attchs[0]).m_nSide : 0;
}

/*!
	@brief Sets the position of the attachment within the gap points array.

	It considers the index of the point closest to the joint point
	as the attachment position.
*/
void GSG::SetAttachmentPositions(SkelJoint* pJoint, const sg::FluxPointList& fpl,
								AttachEdges& attchs)
{
	double dist, minDist;
	SkelPtIndex i, minIdx = 0;

	for (i = 0; i < fpl.size(); i++)
	{
		dist = pJoint->fp.p.sqDist(fpl[i].p);

		if (i == 0 || dist < minDist)
		{
			minIdx = i;
			minDist = dist;
		}
	}

	for (i = 0; i < attchs.size(); i++)
		GetEdge(attchs[i]).m_position = minIdx;
}

/*!
	@brief Increments the position of all the given attachment edges by 'inc'
*/
void GSG::IncrementAttachmentPositions(SkelPtIndex inc, AttachEdges& attchs)
{
	for (SkelPtIndex i = 0; i < attchs.size(); i++)
		GetEdge(attchs[i]).m_position += inc;
}

/*!
	@brief Restores the information in all the gaps between
	branch segments.
*/
void GSG::FixBrokenBranch(leda::node v)
{
	GSGNode& gnode = GetNode(v);
	ASSERT(gnode.HasBrokenBranch());

	SkelBranch* pBranch = new SkelBranch(v); // memory will be released by CleanBranchFix()

	gnode.m_segments.GetEndpoints(&pBranch->n1, &pBranch->n2);

	RestoreAxisFunction(gnode, pBranch);

	RemoveCrossingSpokes(gnode, pBranch);

	RestoreDerivedValues(gnode, pBranch);

	gnode.SetFixedBranch(pBranch);
}

/*!
	@brief Reorients crossing spokes at restored points (with no boundary index)
	such that they do not intersect neighbouring pts with boundary index.
*/
void GSG::RemoveCrossingSpokes(GSGNode& gnode, SkelBranch* pNewBranch)
{
	const sg::FluxPointList& fpl = pNewBranch->getFluxPoints();
	sg::BoundaryInfoList& bil = pNewBranch->getBoundaryInfoList();

	const unsigned int sz = bil.size();
	char s;

	// Make sure that the list is not empty
	if (sz == 0)
		return;

	// Eliminate crossing spokes while moving forward
	for (unsigned int i = 1; i < sz; i++)
	{
		for (s = '1'; s <= '2'; s++)
		{
			if (bil[i][s].index < 0 &&
				DoSegmentsIntersect(fpl[i].p, bil[i][s].pt, fpl[i - 1].p, bil[i - 1][s].pt))
			{
				bil[i][s].pt = bil[i - 1][s].pt;
			}
		}
	}

	// Eliminate crossing spokes while moving backward
	for (unsigned int j = sz - 1; j >= 1; j--)
	{
		for (s = '1'; s <= '2'; s++)
		{
			if (bil[j - 1][s].index < 0 &&
				DoSegmentsIntersect(fpl[j - 1].p, bil[j - 1][s].pt, fpl[j].p, bil[j][s].pt))
			{
				bil[j - 1][s].pt = bil[j][s].pt;
			}
		}
	}

	// Eliminate crossing spokes while moving backward
	/*for (i = sz - 2; i >= 0; i--)
		for (s = '1'; s <= '2'; s++)
			if (bil[i][s].index < 0 &&
				DoSegmentsIntersect(fpl[i].p, bil[i][s].pt, fpl[i + 1].p, bil[i + 1][s].pt))
			{
				bil[i][s].pt = bil[i + 1][s].pt;
			}*/
}

/*!
	@brief Restores the cumulative axis distance and the cumulative boundary
	distance of all points. In addition, it computes the set of connected bundary
	points that is associated with spokes of each interpolated skeletal segemnt.
	Since the endpoints of these spokes need not be connected, a conected chain
	must be derived from them.

	It is assumed that RemoveCrossingSpokes() has been previously called with
	the same given parameters.
*/
void GSG::RestoreDerivedValues(GSGNode& gnode, SkelBranch* pNewBranch)
{
	const sg::FluxPointList& fpl = pNewBranch->getFluxPoints();
	sg::BoundaryInfoList& bil = pNewBranch->getBoundaryInfoList();
	sg::BoundarySegmentLists& bsl = pNewBranch->getBoundarySegments();
	sg::BoundarySegment bs[2];

	ASSERT(bsl.empty()); // every new branch must have an empty list

	// Current and previous cumulative axis and bndry distances
	double cbd[2] = {0, 0}, prevCBD[2] = {0, 0};
	int j;
	char s;

	// Update the info of the first point
	ASSERT(!bil.empty());

	// Init cumulative axis distance to zero
	bil[0].cumAxisDist         = 0;

	// Init cum bndry distance and bndry interval limits
	for (s = '1', j = 0; j < 2; s++, j++)
	{
		sg::BoundaryPoint& bp = bil[0][s];

		// Init cumulative boundary distance to zero
		bil[0][s].cumBndryDist = 0;

		// Init limits of the bndry interval. If index is equal to -1,
		// it means that it is the first "gap" point (see below -index - 1)
		ASSERT(bp.index >= 0 || bp.index == -1);

		bs[j].start = bs[j].end = bp.index;

		// Init the segment index (subindex) to the first element
		bp.subindex = 0;

		// If first pt is gap point, add a bndry segment, [-1,-1], and add the
		// gap pt to it (the [-1,-1] inteval is not referenced by any pt)
		if (bp.index == -1)
		{
			bsl[s].push_back(bs[j]);
			bsl[s].back().addGapPoint(bp.pt);
		}
	}

	// Continue updating the info of every other point
	const unsigned int N = bil.size() - 1;

	for (unsigned int i = 1; i <= N; i++)
	{
		const sg::BoundaryInfo& bi0 = bil[i - 1];
		sg::BoundaryInfo& bi = bil[i];

		bi.cumAxisDist = bi0.cumAxisDist + fpl[i].p.dist(fpl[i - 1].p);

		for (s = '1', j = 0; j < 2; s++, j++)
		{
			const sg::BoundaryPoint& bp0 = bi0[s];
			sg::BoundaryPoint& bp = bi[s];

			// Initially, the a gap point index is -1, and it is only later
			// changed to some negative number (bp0.index < 0 for a gap point).
			// For consistency, we always use the < 0 condition for both bp0 and bp,
			// but here we check that the bp index looks normal.
			ASSERT(bp.index >= 0 || bp.index == -1);

			// If at a gap's boundary, we must compute distances since
			// the location of the skeletal points changed and so did
			// their correspoding boundary points (regardless of side)
			if (bi0.first.index < 0 || bi.first.index < 0 ||
				bi0.second.index < 0 || bi.second.index < 0)
			{
				cbd[j] += bp.pt.dist(bp0.pt);
			}
			else
			{
				ASSERT(bp.cumBndryDist >= prevCBD[j]);

				cbd[j] += bp.cumBndryDist - prevCBD[j];
			}

			prevCBD[j] = bp.cumBndryDist;
			bp.cumBndryDist = cbd[j];

			// Fill the boundary segment list with the gap and standard pts info

			// If going from a standard point to a gap point...
			if (bp0.index >= 0 && bp.index < 0)
			{
				// finish adding standard pts. add the ones already seen.
				bsl[s].push_back(bs[j]);
			}
			// or else, if going from a gap point to a standard poing
			else if (bp0.index < 0 && bp.index >= 0)
			{
				// add gap pts in between (without including the current point)
				bsl[s].back().addGapPoints(bp0.pt, bp.pt, false);

				// start adding standard pts. save the first point
				bs[j].start = bp.index;
			}

			// If current point is a gap pt, it may not be
			// connected (at the pixel level) with the previous point,
			// so we add a connected array of gap points instead
			if (bp.index < 0)
			{
				// Add a connected array of gap points to the last
				// BoundarySegment segment in the list
				bsl[s].back().addGapPoints(bp0.pt, bp.pt, true);

				// Set the index of bp to the last gap point added
				// but express as -index - 1 (so that [0, N] are
				// standard points and [-M, -1] are gap points
				bp.index = -1 * bsl[s].back().gapPts.size();

				// Set subindex of bp to the "current" segment index
				bp.subindex = bsl[s].size() - 1;
			}
			else // it's a standard point. Assume it's the last one by now...
			{
				bs[j].end = bp.index;
				bp.subindex = bsl[s].size(); // "next" seg index; seg is added later
			}
		}
	}

	// If any of the last point's indices is not a gap, then
	// we need to still add the current bs.
	if (bil[N]['1'].index >= 0)
		bsl['1'].push_back(bs[0]);

	if (bil[N]['2'].index >= 0)
		bsl['2'].push_back(bs[1]);
}

/*!
	@brief Restores the information of each gap between segments
	by fitting a cubic Bezier to each gap and doing a lot more stuff...
*/
void GSG::RestoreAxisFunction(GSGNode& gnode, SkelBranch* pNewBranch)
{
	BranchSegmentList::const_iterator it0, it1;
	BranchGapList::iterator attEdgeIt;
	CubicBezier interpCurve;
	unsigned int i, totalGapSize;

	// Find the number of gaps to process
	const unsigned int numGaps = gnode.m_segments.size() - 1;

	// Declare an array of segments to store the branch and gap skeletal points
	std::vector<sg::FluxPointList> gapFluxPts(numGaps);

	// Declare an array of segments to store the branch and gap boundary info
	std::vector<sg::BoundaryInfoList> gapBndryPts(numGaps);

	ASSERT(numGaps >= 1);
	ASSERT(gnode.m_segments.GapSize() == numGaps);

	attEdgeIt = gnode.m_segments.GapBegin(); // first gap segment
	it1 = gnode.m_segments.begin();          // first branch segment
	i = 0;                                   // init number of gaps interpolated
	totalGapSize = 0;                        // init total number of gap points

	// Remove previous Bezier curves (used for display only)
	gnode.m_gapAxes.Clear();

	DBG_C_LOG("Restoring axis with " << numGaps << " gaps")

	// Fill all gaps between branches
	for (it0 = it1++; it1 != gnode.m_segments.end(); it0++, it1++, i++, attEdgeIt++)
	{
		DBG_C_LOG("(" << i << ") Fusing " << *it0 << " with " << *it1)

		// Iterpolate the skeletal gap, and indicate whether it is the first or
		// the last branch segment
		InterpolateSkeletalGap(it0, i == 0, it1, i == numGaps - 1, attEdgeIt,
			gapFluxPts[i] /*out*/, gapBndryPts[i] /*out*/, interpCurve /*out*/);

		totalGapSize += gapFluxPts[i].size();

		// @debug Save the gap axis curve so that we can inspect it later
		gnode.m_gapAxes.AddTail(interpCurve);
	}

	// Finally, combine the original branch points and the interpolated gap points
	// into a whole "fixed" branch
	sg::FluxPointList& fpl = pNewBranch->getFluxPoints();
	sg::BoundaryInfoList& bil = pNewBranch->getBoundaryInfoList();
	unsigned int j, maxSize;
	int k, gapSize;

	// Reserve some memory (this is an upper bound)
	maxSize = gnode.m_segments.Size() + totalGapSize;
	fpl.reserve(maxSize);
	bil.reserve(maxSize);

	attEdgeIt = gnode.m_segments.GapBegin();
	it1 = gnode.m_segments.begin();
	i = 0;

	// Iterate over every pair of connected branch segments and gap segments,
	// and copy them (in order) to the output array of fixed branch points.
	// Note: gap points include the endpoints of the segment that define the gap
	// This points must be ignored. This code deals with empty segments!!!
	for (it0 = it1++; it0 != gnode.m_segments.end(); it0++, i++)
	{
		// Copy branch segment points
		if (!it0->IsEmpty())
		{
			// Make sure that the last "fixed" branch point and the
			// first branch point of the segment are different
			j = GetFirstConnectedPointIndex(fpl, *it0);

			ASSERT(fpl.empty() || j > it0->Last() || fpl.back().p != (*it0)[j].p);

			for (; j <= it0->Last(); j++)
			{
				fpl.push_back((*it0)[j]); // copy skeletal point info
				bil.push_back((*it0)(j)); // copy boundary point info
			}
		}

		// Copy gap points
		if (it1 != gnode.m_segments.end())
		{
			ASSERT(!gapFluxPts[i].empty());

			// Make sure that the last "fixed" branch point and the
			// first gap point are different
			k = GetFirstConnectedPointIndex(fpl, gapFluxPts[i]);

			ASSERT((fpl.empty() && k == 0) || (!fpl.empty() && (k == 0 || k == 1)));

			// Keep track of attachment poinitions, which were set wrt their associated gap
			// point array, by shifting them wrt the num of points added to the "fixed" branch
			if (!fpl.empty())
				IncrementAttachmentPositions(fpl.size() - k, *attEdgeIt);

			// Copy the info of each gap point
			gapSize = gapFluxPts[i].size();

			ASSERT(fpl.empty() || k >= gapSize || fpl.back().p != gapFluxPts[i][k].p);

			for (; k < gapSize; k++)
			{
				fpl.push_back(gapFluxPts[i][k]);  // copy skeletal point info
				bil.push_back(gapBndryPts[i][k]); // copy boundary point info
			}

			it1++;
			attEdgeIt++;
		}
	}

	WARNING(fpl.size() > maxSize, "Consider increasing init vector size");

	ASSERT(pNewBranch->getFirstXYPoint().dist(pNewBranch->n1->fp.p) < 1);
	ASSERT(pNewBranch->getLastXYPoint().dist(pNewBranch->n2->fp.p) < 1);

	// Make sure that endpoint invariants are preserved. Basically, this
	// requires that the skeletal and boundary info of the first and last
	// point of the broken branch remains the same

	// Front and back point must agree with branch joint coordinates.
	fpl.front().p = pNewBranch->n1->fp.p;
	fpl.back().p  = pNewBranch->n2->fp.p;

	// Bounadry info of first point in first segment and last point in last segmet
	// must be equal to the first and last points pf the fixed branch, respectively
	//bil.front() = gnode.m_segments.front().m_pBranch->getFirstBndryInfo();
	//bil.back()  = gnode.m_segments.back().m_pBranch->getLastBndryInfo();
}

/*!
	@brief Iterpolates the skeletal and boundary gap point define
	by the branch segments referenced by it0 and it1, and the attachment
	information referenced by attEdgeIt.

	@param gapFluxPts returns the interpolated skeletal points
	@param gapBndryPts returns the interponaled boundary points
	@param cb returns a cubic Bezier curve that interpolates the skeletal points
*/
void GSG::InterpolateSkeletalGap(const BranchSegmentList::const_iterator& it0,
								 bool bIsFirstSegment,
								 const BranchSegmentList::const_iterator& it1,
								 bool bIsLastSegment,
								 const BranchGapList::iterator& attEdgeIt,
								 sg::FluxPointList& gapFluxPts,
								 sg::BoundaryInfoList& gapBndryPts,
								 CubicBezier& cb)
{
	SkelPtIndex gapPtIdx0, gapPtIdx1;
	SkelJoint* pJoint;
	POINTS gapPts;
	SmartArray<double> gapParams;
	sg::FluxPoint fp;
	unsigned int maxGapSize;
	int k, nSide, gapSize;

	// Get a cubic bezier 'cb' and the indices of the gap's first and
	// last point on the given pair of medial axis segments
	ComputeInterpolationCurve(*it0, bIsFirstSegment, *it1, bIsLastSegment,
		GetEdge(attEdgeIt->front()).GetLigatureInfo(),
		cb /*out*/, gapPtIdx0 /*out*/, gapPtIdx1 /*out*/);

	// Find an approximation to the maximum size of the gap
	maxGapSize = 2 * (it0->RightMarginSize() + it1->LeftMarginSize() + 2);

	// Rasterize the curve that "fills" the gap
	CubicBezierParams cbp(cb);
	gapPts = cbp.Rasterize(maxGapSize, &gapParams);

	// Find the common joint between the branches
	pJoint = GetCommonSkelJoint(it0->m_pBranch, it1->m_pBranch);

	// The radius of the joint is a good upper bound to init ALL dist values,
	// which are needed for finding the boundary points
	fp.dist = fabs(pJoint->fp.dist) * UNKNOWN_RADIUS_UPPER_BOUND_FACTOR;

	gapSize = gapPts.GetSize();

	gapFluxPts.resize(gapSize);
	gapBndryPts.resize(gapSize);

	// Form an array of with the gaps flux points' x-y coordinates only (ie, no radius)
	for (k = 0; k < gapSize; k++)
	{
		AssignPtCoord(fp.p, gapPts[k]);
		gapFluxPts[k] = fp;
	}

	// Assign the boundary point info to each skeleton point in the gap
	// by finding the original boundary point to each skeletal point. This
	// is only useful for the brnch side (if any) that is NOT associated
	// with a boundary gap. For the other side, this is a waste of computation time.
	// Provide first and last branch points to constraint the assignment of spokes.
	// @todo improve this by adding a side parameter, and setting the boundary info
	// for that side alone
	m_pSkeletalGraph->AssignBoundaryPoints(gapFluxPts, gapBndryPts,
		(*it0)(gapPtIdx0), (*it1)(gapPtIdx1));

	// Using the boundary info, assign the radius information to each axis gap
	// point. The radius is the smallest distance to any of the closest boundary
	// points on either side of the branch.
	// @todo this could also be made for the side with no boundary gap only
	m_pSkeletalGraph->AssignRadiusValues(gapBndryPts, gapFluxPts);

	// Set the position of the attachment branch(es) within the gap
	// by finding the new skeletal point closest to the originl joint point
	SetAttachmentPositions(pJoint, gapFluxPts, *attEdgeIt);

	// Set the side of the attachment branch(es)
	nSide = SetAttachmentSides(*it0, *it1, *attEdgeIt);

	// Use the interpolated axis to interpolate the boundary gap
	// associated with the skeletal gap
	InterpolateBoundaryGap(nSide, cbp, gapParams, gapFluxPts, gapBndryPts);
}

/*!
	@brief Restore the boundary info lost at an attachment point.

	There are two cases: the standard case (99.9% of the cases) that has one
	single attaching branch, and the rare case, which has more than one.

	For the standard case, there are two ways of recovering the missing boundary
	information.

	(1) use the points on the valid boundary side and the tangents on the
	    medial axis points to recover alpha. Then use alpha, the radius of each
	    point and the tangent to recover the boundary points on the other side.

		PROS: it preserves the distances to the known boundary side.
		CONS: if the known distances are affected by noise, then the new
		      boundary points will be noisy too.

    (2) fit a 2-segment polyline to the (possibly noisy) radius function of
	    the gap points. Use the slope of the lines as the first derivative of
		the radius function. Alpha can be recovered from this information.

		PROS: works for both the standard case and the rare case.
		CONS: requires to fit lines. Plus, if we want to have a valid medial axis,
		      the known points on the valid boudnary side must be changed to so
			  that they have equal alphas.

	For the rare case, we can only use option (2). In addition, we do not have
	enough information to fit a 2-segment polyline and instead we can only
	fit a single line to the radius values at the gap's endpoints.

	Conclusion: implement both approaches and evaluate them in practice. This is
				also good for pedagogical purposes.
*/
void GSG::InterpolateBoundaryGap(int nSide, const CubicBezierParams& cbp,
								 const SmartArray<double>& gapParams,
								 const sg::FluxPointList& fpl,
								 sg::BoundaryInfoList& bil)
{
	ASSERT(nSide >= 0 || nSide <= 2);

	sg::Point ptTan0;
	sg::Vector t, v;
	double m, b, r0, rN, x0, xN, alpha, radius;
	bool bForwardDir;
	int i, sz = gapParams.Size();
	char as = (nSide == 1) ? '1':'2'; // attachment side (zero => '2')
	char os = (nSide == 1) ? '2':'1'; // opposite side    (zero => '1')

	ASSERT(sz >= 2);

	if (nSide > 0 && m_params.bUseKnownPtsToRestoreBndry) // method number 1
	{
		ASSERT(false);
		DBG_MSG1("Using known bndry points to restore gap information")

		for (i = 0; i < sz; i++)
		{
			t = AssignPtCoord(ptTan0, cbp.GetTangent(gapParams[i]));
			v = bil[i][os].pt - fpl[i].p;

			t.normalize();
			v.normalize();

			alpha = ComputeObjectAngle(t, v);
			t.rotate(alpha);
			t.scale(fpl[i].dist);
			bil[i][as].pt.set(fpl[i].p.x + t.x, fpl[i].p.y + t.y);
			bil[i][as].index = -1; // there's no bndry pt associated
		}
	}
	else // method number 2
	{
		/*POINTS rdata(sz);

		for (i = 0; i < sz; i++)
			rdata[i].Set(bil[i].cumAxisDist, fabs(fpl[i].dist));

		PolyLineApprox polyline(sz / 3.0, 0.05, 2, 0.1);

		//std::cout << "disp('Fitting gap from " << fpl[0].p << " to "
		//	<< fpl[sz - 1].p << " on side " << nSide << "');\n";
		//polyline.SetDbgMode(true); // outputs matlab data in dbg mode

		polyline.Fit(rdata);*/

		r0 = fabs(fpl.front().dist);
		rN = fabs(fpl.back().dist);

		x0 = bil.front().cumAxisDist;
		xN = bil.back().cumAxisDist;

		ASSERT(x0 >= 0 && x0 < xN);

		m = (rN - r0) / (xN - x0); // tangent
		b = r0;                    // y-intercept

		WARNING1(m < -1 || m > 1, "Slope of radius function is too steep", m);

		for (i = 0; i < sz; i++)
		{
			//m = polyline.GetSlope(i);
			t = AssignPtCoord(ptTan0, cbp.GetTangent(gapParams[i])); // convert POINT to Vector

			t.normalize();
			ASSERT_VALID_POINT(t);

			ptTan0.x = fpl[i].p.x + t.x;
			ptTan0.y = fpl[i].p.y + t.y;

			// Save the direction of the tangent: forward / backward
			if (i == 0)
				bForwardDir = (ptTan0.dist(fpl[i + 1].p) < 1);
			else if (i == sz - 1)
				bForwardDir = (ptTan0.dist(fpl[i - 1].p) > 1);
			else
				bForwardDir = (ptTan0.sqDist(fpl[i - 1].p) > ptTan0.sqDist(fpl[i + 1].p));

			// Tangent must point in the direction of decreasing radius
			if ((bForwardDir && m > 0) || (!bForwardDir && m < 0))
				t.scale(-1);

			// Get the object angle
			alpha = SafeArcCosine(m);

			// If alpha is greater than pi/2, it means that it is expressed wrt
			// a tangent going in the opposite direction
			if (fabs(alpha) > M_PI_2)
				t.scale(-1);

			// Since we don't know the sign of alpha, use the info of the known pt
			// This works even when nSide == 0, since we just care about signs
			v = bil[i][os].pt - fpl[i].p;

			WARNING(v.x == 0 && v.y == 0, "Null vector cannot be normalized. Using (1,0) instead.");

			if (v.x == 0 && v.y == 0)
				v.x = 1;

			v.normalize();

			if (ComputeObjectAngle(t, v) < 0)
				alpha = -alpha;

			// Now, we have all we need
			t.rotate(alpha);
			ASSERT_VALID_POINT(t);

			//radius = polyline.GetPointSegment(i).Y(rdata[i].x);
			radius = m * bil[i].cumAxisDist + b;

			// Try considering an average between the interpolated
			// radius and the true radius.
			//ASSERT(fpl[i].dist >= 0);
			//radius = (fpl[i].dist + radius) / 2;

			t.scale(radius);

			bil[i][as].pt.set(fpl[i].p.x + t.x, fpl[i].p.y + t.y);
			bil[i][as].index = -1; // there's no bndry pt associated

			// If we have attachments on both sides, set the other bndry pt too
			if (nSide == 0)
			{
				t.rotate(-2 * alpha);

				bil[i][os].pt.set(fpl[i].p.x + t.x, fpl[i].p.y + t.y);
				bil[i][os].index = -1; // there's no bndry pt associated
			}
		}
	}
}

/*!
	@brief Fills gaps on the medial axis formed around attachment points.

	@pre { bs0[bs0.BranchLast()].p == bs1[bs1.BranchFirst()].p }

	In the ideal case, an cubic bezier starting spanning the gap endpoints is
	set. The angles of the inner "handles" of the curve are determined by
	the tangents at the gap endpoints. The magnitued of the "handle" vectors
	is defined to be the distance to the intersection point between the line formed
	by each handle and the ligature segment associated with the attachment.

	The ligature segment is formed by the joint point and the last ligature point.
	We extend this segment from the joint point up to the intersection with the line
	connecting the pair of gap endpoins. This defines an "extended ligature line segment"
	in which we believe the inner "handles" should be.

	If either bs0 or bs1 are empty, there gap itself is unconstrained. The natural solution
	in this case is to choose the middle point of the branch associated with the empty
	segment. However, there is an important detail to account for when doing this:

    a) If the empty branch segment is at the extremes of the list of segments forming
	   a GSG branch, then choosing the middle point of the branch would leave a pice
	   of gap unfilled.

   @remarks { It return the indices of the first and last point that define the gap. }
*/
void GSG::ComputeInterpolationCurve(const BranchSegment& bs0, bool bIsFirstSeg0,
									const BranchSegment& bs1, bool bIsLastSeg1,
									const LigatureSegment& ls, CubicBezier& cb,
									SkelPtIndex& ptIdx0, SkelPtIndex& ptIdx1)
{
	POINT lp0, lp1, lp2, firstTanPt0, lastTanPt0, firstTanPt1, lastTanPt1;
	int n;

	SkelJoint* pJoint = GetCommonSkelJoint(bs0.m_pBranch, bs1.m_pBranch);

	ASSERT(bs0[bs0.BranchLast()].p == bs1[bs1.BranchFirst()].p);

	//ASSERT(!attchs.empty());

	// Set the tangent for the fist endpoint

	if (bs0.Size() >= 1) // at least we have a valid endpoint
		ptIdx0 = bs0.Last();
	else // the segment is empty. Choose middle point or first point (special case)
		ptIdx0 = (bIsFirstSeg0) ? bs0.BranchFirst() : bs0.BranchSize() / 2;

	AssignPtCoord(cb.p0, bs0[ptIdx0].p);
	bs0.ComputeTangent(ptIdx0, &firstTanPt0, &lastTanPt0);

	// Set the tangent for the second endpoint

	if (bs1.Size() >= 1)
		ptIdx1 = bs1.First();
	else
		ptIdx1 = (bIsLastSeg1) ? bs1.BranchLast() : bs1.BranchSize() / 2;

	AssignPtCoord(cb.p3, bs1[ptIdx1].p);
	bs1.ComputeTangent(ptIdx1, &firstTanPt1, &lastTanPt1);

	// First, build the ligature line...
	// Use the joint to retrieve the attachment information
	//const LigatureSegment& ls = GetEdge(attchs.front()).GetLigatureInfo();

	// Get the line segment determined by the ligature endpoints
	AssignPtCoord(lp0, ls[ls.First()].p);
	AssignPtCoord(lp1, ls[ls.Last()].p);

	// Extend the ligature segment so that it goes up to lower bound
	n = FindLineSegmentIntersection(cb.p0, cb.p3, lp0, lp1, &lp2);
	//ASSERT(n > 0);
	WARNING(n <= 0, "There is no valid line intersection");

	if (n <= 0)
		AssignPtCoord(lp2, pJoint->fp.p);

	(lp0.SqDist(lp2) < lp1.SqDist(lp2)) ? lp0 = lp2 : lp1 = lp2;

	// Set cb.p1 to the intersection between tangent0 and ligature line...
	n = FindLineSegmentIntersection(firstTanPt0, lastTanPt0, lp0, lp1, &cb.p1);

	// Use a heuristic to set ctrt pt if no intersection or bad one
	if ((n != 1 && n != 4) || (cb.p1 == cb.p0))
	{
		//AssignPtCoord(cb.p1, pJoint->fp.p);

		cb.p1 = cb.p0 + (cb.p3 - cb.p0) * 0.5;
	}

	// Set cb.p2 to the intersection between tangent1 and ligature line...
	n = FindLineSegmentIntersection(firstTanPt1, lastTanPt1, lp0, lp1, &cb.p2);

	// Use a heuristic to set ctrt pt if no intersection or bad one
	if ((n != 1 && n != 4) || (cb.p2 == cb.p3))
	{
		//AssignPtCoord(cb.p2, pJoint->fp.p);

		cb.p2 = cb.p0 + (cb.p3 - cb.p0) * 0.5;
	}

	//DBG_MSG6("Filling:", pJoint->fp.p, cb.p0, cb.p1, cb.p2, cb.p3);
}

/*!
	@brief

*/
SkelBranch* GSG::FindSourceBranch(SkelJoint* pJoint, const SkelEdges& branches)
{
	BranchInfoVect biv;
	SkelBranch* pSrcBranch = nil;

	ASSERT(pJoint->degree() >= 3);

	//double angleSum = ComputeObjectAnglesAtJoint(pJoint, branches, biv);
	//ASSERT(biv.size() == branches.size());
	//bool bValidAngleSum = (angleSum >= M_PI - 0.3 && angleSum <= M_PI + 0.3);
	//WARNING1(!bValidAngleSum, "The object angles at the joint don't add up to PI", angleSum);

	bool bValidAngleSum = false;

	const unsigned int N = branches.size() - 1;

	// If we have a standard case of bifurcation (3 branches), two branches
	// would have angles of pi/4 and the remaining one would have on of pi/2.
	// Then, the difference between the first and second largest angles ~= pi/2
	// ie, ~0.78. To allow for some variation, we set it to be 0.6 or more
	if (bValidAngleSum && biv[N].first - biv[N - 1].first > 0.6)
	{
		DBG_C_LOG(DBG_VAL(bValidAngleSum) << DBG_VAL(biv[N].first) << DBG_VAL(biv[N - 1].first))

		pSrcBranch = biv.back().second;
	}
	else
	{
		double jointArea = M_PI * (pJoint->fp.dist * pJoint->fp.dist);

		ComputeAreasAtJoint(pJoint, branches, biv);
		ASSERT(biv.size() == branches.size());

		DBG_C_LOG(DBG_VAL(jointArea) << DBG_VAL(biv[N].first) << DBG_VAL(biv[N - 1].first))

		if (biv[N].first > jointArea && biv[N - 1].first < jointArea)
		{
			pSrcBranch = biv.back().second;
			//DBG_MSG2("There is a most salient branch as source", pJoint->fp.p)
		}
	}

	DBG_C_LOG(DBG_VAL(pSrcBranch))
	//WARNING1(pSrcBranch == nil, "There is a special junction case", pJoint->fp.p);

	return pSrcBranch;
}

/*!
	@brief Creates a GSG_BRANCH node with the junction point as its only data
	point. Then, it makes the new node be the parent of the junction node.

	@return the new root node
*/
leda::node GSG::AddRootBranchNodeToJunctionNode(leda::node v)
{
	ASSERT(GetNodeType(v) == GSG_JUNCTION);
	ASSERT(indeg(v) == 0);

	GSGNode& node = GetNode(v);

	// First, we need to create a real skeletal branch that we can refer to
	SkelBranch* pSrcBranch = NewSkelBranchFromJoint(node.m_pJoint);

	// Now, we can create a branch node from the new skeletal branch
	leda::node r = NewBranchNode(pSrcBranch);

	// Associate the GSG node with the skeletal branch
	pSrcBranch->pEdgeData = r;

	// Finally, the branch node becomes the parent of the junction node
	NewJunctionEdge(r, v, node.m_pJoint); // flows towards the joint

	return r;
}

/*!
	@brief Creates a skeletal branch using the data of a
	sekeletal joint
*/
SkelBranch* GSG::NewSkelBranchFromJoint(SkelJoint* pJoint)
{
	SkelBranch* pBranch = new SkelBranch(nil);

	m_extraSkelEdges.push_back(pBranch); // so that it can be released later

	pBranch->n1 = pBranch->n2 = pJoint;

	sg::FluxPointList& fpl = pBranch->getFluxPoints();
	sg::BoundaryInfoList& bil = pBranch->getBoundaryInfoList();

	fpl.resize(2);
	bil.resize(2);

	fpl[0] = fpl[1] = pJoint->fp;

	if (pJoint->edges[0]->getFirstXYPoint() == pJoint->fp.p)
	{
		bil[0] = bil[1] = pJoint->edges[0]->getFirstBndryInfo();
	}
	else
	{
		bil[0] = bil[1] = pJoint->edges[0]->getLastBndryInfo();
	}

	return pBranch;
}

/*!
	@brief Looks for the middle branch of a crossing case

	A crossing involves 5 branches, with two pairs of branches associated
	to the crossing branches and one middle branch associated with the
	crossing point. Each endpoint of the middle branch must meet
	certain conditions regarding the spokes of the branches that
	flow away from it.

	NOTE: a middle branch has two endpoints n1, n2. It is important that
	this function returns the exact same value when given either n1 or n2.
	This function meets this symmetry proprety ONLY if there is a unique
	middle branch associated with both endpoints. More complicated cases,
	(eg, guitar0008) require to compare the results of the function when
	given both endpoints.
*/
SkelBranch* GSG::FindCrossingMiddleBranch(SkelJoint* pJoint)
{
	SkelBranch* pBranch;
	SkelJoint* pJoint2;
	sg::BoundaryInfoList bil;
	SkelEdges middleBranches;
	unsigned int i;
	bool bIsCrossing;
	double r1, r2;

	DBG_C_LOG(DBG_VAL(pJoint->fp.p) << DBG_VAL(pJoint->degree()));

	if (pJoint->degree() != 3)
	{
		DBG_C_LOG("not a middle branch. degree != 3")
		return NULL;
	}

	// Check all the branches to see which one is a crossing point
	forall_branches(pBranch, pJoint->edges)
	{
		pJoint2 = GetOtherEndpoint(pBranch, pJoint);

		// See if the other endpoint is also a joint with exactly 3 branches
		if (pJoint2->degree() != 3)
			continue;

		// If the current branch is a crossing point, then the branches incident to
		// its other endpoint must have spokes that cross with the disk of pJoint
		GetBoundaryInfoAtJoint(pJoint2, &bil);

		bIsCrossing = true;

		for (i = 0; i < bil.size() && bIsCrossing; i++)
		{
			if (!DoSegmentCircleIntersect(pJoint2->fp.p, bil[i].first.pt, pJoint->fp) ||
				!DoSegmentCircleIntersect(pJoint2->fp.p, bil[i].second.pt, pJoint->fp))
			{
				DBG_C_LOG("not a middle branch. spokes don't intersect disk 1")
				bIsCrossing = false;
			}
		}

		// We need symmetry in the results, so check the same condition on the other joint
		if (bIsCrossing)
		{
			GetBoundaryInfoAtJoint(pJoint, &bil);

			for (i = 0; i < bil.size() && bIsCrossing; i++)
			{
				if (!DoSegmentCircleIntersect(pJoint->fp.p, bil[i].first.pt, pJoint2->fp) ||
					!DoSegmentCircleIntersect(pJoint->fp.p, bil[i].second.pt, pJoint2->fp))
				{
					DBG_C_LOG("not a middle branch. spokes don't intersect disk 2")
					bIsCrossing = false;
				}
			}
		}

		// In addition, the endpoints of a crossing branch should have similar radius
		if (bIsCrossing)
		{
			std::vector<double> alphas1, alphas2;
			BranchPairs brp; // don't really need it

			ComputeTangentAnglesAtJoint(pJoint,  pJoint->edges,  alphas1, brp, pBranch);
			ComputeTangentAnglesAtJoint(pJoint2, pJoint2->edges, alphas2, brp, pBranch);

			if (fabs(alphas1[0] - alphas2[0]) <= M_PI_4)
			{
				DBG_C_LOG("valid angles at both joints " << DBG_VAL(pBranch->size()))

				// Let's check one last thing...
				r1 = fabs(pJoint->fp.dist);
				r2 = fabs(pJoint2->fp.dist);

				if (MIN(r1, r2) / MAX(r1, r2) >= 0.9)
				{
					middleBranches.push_back(pBranch);
				}
				else
				{
					DBG_C_LOG("not a middle branch. invalid radius ratio " << DBG_VAL(r1) << DBG_VAL(r2))
				}
			}
			else
			{
				DBG_C_LOG("not a middle branch. invalid angles " << DBG_VAL(alphas1[0]) << DBG_VAL(alphas2[0]))
			}
		}
	}

	if (middleBranches.empty())
	{
		DBG_C_LOG("there is no middle branch")
		return NULL;
	}
	else if (middleBranches.size() == 1)
	{
		DBG_C_LOG("there is one middle branch")
		return middleBranches.front();
	}

	DBG_C_LOG("there are multiple middle branches")

	// There are multiple middle branches. We have two options, we can either
	// return NULL to indicate that there is no crossing, or we can see which
	// branch is the true middle branch. Here we try the latter approach.
	BranchInfoVect variances;

	ComputeObjectAngleVariance(middleBranches, variances);

	//DBG_MSG2(variances[0].first, variances[1].first)

	// If we have a tie, then we must return NULL because we cannot
	// make a deterministic decision otherwise
	if (variances[0].first == variances[1].first)
	{
		DBG_C_LOG("middle branches are ignored due to a tie")
		return NULL;
	}

	return variances.front().second;
}

bool GSG::IsElbowAttachment(SkelJoint* pJoint, LigatureSegmentList& nonLig,
							   LigatureSegmentList& ligSegs,
							   LigatureSegmentList& semiLigSegs)
{
	if (pJoint->degree() != 3)
		return false;

	if (nonLig.size() == 1 && semiLigSegs.size() == ligSegs.size())
	{
		const LigatureSegment& ls0 = ligSegs.front();
		const LigatureSegment& ls1 = ligSegs.back();

		char s0 = ls0.GetLigatureSide();
		char s1 = ls1.GetLigatureSide();

		if ((!ls0.IsValidCornerDistance(ls0.GetCorner(s0)) ||
			!ls1.IsValidCornerDistance(ls1.GetCorner(s1))) &&
			nonLig.front().IsElbowShape())
		{
			ligSegs = nonLig;
			nonLig = semiLigSegs;
			semiLigSegs.clear();

			DBG_C_LOG("Elbow attachment at " << PRN_JOINT_PTR(pJoint))
			DBG_STREAM("Elbow attachment at " << PRN_JOINT_PTR(pJoint))

			return true;
		}
	}

	return false;
}

/*!
	@brief Checks if the ligature segments rooted at the joint form
	a case of end-to-end attachment.

	\image html IsEndToEndAttachment.gif
*/
bool GSG::IsEndToEndAttachment(SkelJoint* pJoint, LigatureSegmentList& nonLig,
							   LigatureSegmentList& ligSegs,
							   LigatureSegmentList& semiLigSegs)
{
	if (pJoint->degree() != 3)
		return false;

	//DBG_STREAM("Testing EE attachment at " << PRN_JOINT_PTR(pJoint)
	//	<< ": " << nonLig.size() << ", " << ligSegs.size() << ", " << semiLigSegs.size())

	if (nonLig.size() == 1 && semiLigSegs.size() == ligSegs.size())
	{
		const LigatureSegment& ls0 = ligSegs.front();
		const LigatureSegment& ls1 = ligSegs.back();

		// Check if there is a sharer corner or not
		char s0 = ls0.GetLigatureSide();
		char s1 = ls1.GetLigatureSide();

		// See if we have two semiligatures that are not generated by the
		// same boundary corner (see rightmost case in the figure).
		// Label such special cases as EE attachments
		if (ls0.GetCorner(s0).m_pt != ls1.GetCorner(s1).m_pt)
		{
			DBG_C_LOG("EE attachment due to not shared corners at "
				<< PRN_JOINT_PTR(pJoint))

			//DBG_STREAM("EE attachment due to not shared corners at "
			//	<< PRN_JOINT_PTR(pJoint))

			return true;
		}
		else if (ls0.m_dTargetRadius < ls1.m_dTargetRadius &&
			     ls0.m_dTargetRadius / ls1.m_dTargetRadius <= 0.5)
		{
			// in this case, ls0 is clearly the attached branch, and
			// ls1 can be moved to the non-lig segments, so that is merged
			nonLig.splice(nonLig.end(), ligSegs, ++ligSegs.begin());
			semiLigSegs.pop_back();

			return false; // it's clearly not an EE
		}
		else if (ls1.m_dTargetRadius < ls0.m_dTargetRadius &&
			     ls1.m_dTargetRadius / ls0.m_dTargetRadius <= 0.5)
		{
			// in this case, ls1 is clearly the attached branch, and
			// ls0 can be moved to the non-lig segments, so that is merged
			nonLig.splice(nonLig.end(), ligSegs, ligSegs.begin());
			semiLigSegs.pop_front();

			return false; // it's clearly not an EE
		}
		else
		{
			double a0 = ComputeSpokeAngleAtJoint(ls0.m_pBranch, pJoint);
			double a1 = ComputeSpokeAngleAtJoint(ls1.m_pBranch, pJoint);

			// See how much bigger the max radio is wrt the min radio
			double angleRatio = MAX(a0, a1) / MIN(a0, a1);

			// If the max radio is less than twice the size of the min radio
			// and both angles are greater than 45 degrees, then it's an EE
			if (angleRatio < 2 && a0 > M_PI_4 && a1 > M_PI_4)
			{
				DBG_C_LOG("EE attachment: " << a0 * (180 / M_PI) << ", "
					<< a1 * (180 / M_PI) << " at " << PRN_JOINT_PTR(pJoint))

				//DBG_STREAM("EE attachment: " << a0 * (180 / M_PI) << ", "
				//   << a1 * (180 / M_PI) << " at " << PRN_JOINT_PTR(pJoint))

				return true; // it IS an EE
			}
		}
	}
	else if (semiLigSegs.size() == 3)
	{
		LigatureSegmentList::iterator it = ++semiLigSegs.begin();

		DBG_C_LOG("EE 3sm attachment at " << PRN_JOINT_PTR(pJoint)
			<< semiLigSegs.front() << *it << semiLigSegs.back())

		return true;
	}

	return false;
}

/*!
	@brief Determines whether the joint is a crossing or not. If the degree
	of the joint is 3 and us labeled as a crossing, it means that the joint
	is one endpoint of a branch that is seen as the crossing. In this case,
	the	ligature segment lists are updated to include all liagture branches
	from both joins of the crossing branch. The crossing branch itself is
	returnd as a single non-ligature branch.

	Note: In general, both endpoints of a crossing branch should have at least
	one non-ligature branch (other than the middle branch). However, there is
	a special case in which this doesn't apply. This occurs when there is a
	pseudo-crossing that involves two semi-ligature branches, one on each joint
	(eg. a torso, head, and two arms, forming semi-ligature at the shoulders.)
*/
bool GSG::LabelCrossing(SkelJoint* pJoint, LigatureSegmentList& nonLig,
						LigatureSegmentList& ligSegs, LigatureSegmentList& semiLigSegs,
						bool bUseFixedBranch)
{
	// Check a sufficient condition for the joint not being a crossing
	//if (nonLig.size() >= 2 && (semiLigSegs.empty() || pJoint->degree() > 3))
	//	return false;

	DBG_C_LOG_NEW_LINE
	DBG_C_LOG(DBG_VAL(pJoint->fp.p) << DBG_VAL(bUseFixedBranch))

	// See if we have a non-generic case of crossing
	if (pJoint->degree() > 3)
	{
		if (nonLig.size() < 2)
		{
			// All branches are ligature
			ligSegs.splice(ligSegs.end(), nonLig);

			DBG_C_LOG("it's a non-generic crossing")

			return true;
		}
		else
		{
			DBG_C_LOG("not a crossing. degree is > 3 but there are 2+ non-lig segments")
			return false;
		}
	}

	if (true)
		return false;

	// If it is a crossing, there should be a middle branch
	SkelBranch* pBranchX = FindCrossingMiddleBranch(pJoint);

	if (pBranchX == NULL)
	{
		DBG_C_LOG("not a crossing. there is no middle branch")
		return false;
	}

	// Note that the middle branch cannot be broken!
	if (bUseFixedBranch && GetNodeData(pBranchX).HasFixedBranch())
	{
		DBG_C_LOG("not a crossing. there middle branch is broken")
		return false;
	}

	SkelJoint* pJoint2 = GetOtherEndpoint(pBranchX, pJoint);

	// We also need to make sure that the other endpoint has the same middle branch
	if (FindCrossingMiddleBranch(pJoint2) != pBranchX)
	{
		DBG_C_LOG("not a crossing. disagreement about middle branch")
		return false;
	}

	// If we are working with fixed branches, we can also check that the middle branchs
	// is shorter that the other branches (just check number of pts by now)
	// TODO: change all this stuff
	if (bUseFixedBranch)
	{
		unsigned int sz0 = pBranchX->size();
		SkelBranch* pBranch;

		forall_branches(pBranch, pJoint->edges)
		{
			if (pBranch != pBranchX)
			{
				if (bUseFixedBranch && GetNodeData(pBranch).HasFixedBranch())
					pBranch = GetNodeData(pBranch).GetFixedBranch();

				if (pBranch->size() < sz0)
				{
					DBG_C_LOG("not a crossing. middle branch is longer than other branches in joint 1")
					return false;
				}
			}
		}

		forall_branches(pBranch, pJoint2->edges)
		{
			if (pBranch != pBranchX)
			{
				if (bUseFixedBranch && GetNodeData(pBranch).HasFixedBranch())
					pBranch = GetNodeData(pBranch).GetFixedBranch();

				if (pBranch->size() < sz0)
				{
					DBG_C_LOG("not a crossing. middle branch is longer than other branches in joint 2")
					return false;
				}
			}
		}
	}

	// We have a middle branch, and enough evidence to think that the joint is a crossing.
	// Now, let's look at the ligature around both joints of the middle branch...
	LigatureSegmentList nonLig1, ligSegs1, semiLig1;
	LigatureSegmentList nonLig2, ligSegs2, semiLig2;

	// Find ligature and non-ligature branches while excluding the middle branch
	GetLigatureAtJoint(pJoint , nonLig1, ligSegs1, semiLig1, bUseFixedBranch, pBranchX);
	GetLigatureAtJoint(pJoint2, nonLig2, ligSegs2, semiLig2, bUseFixedBranch, pBranchX);

	// In general, if both endpoints have at least one non-ligature branch (other than
	// the middle branch), then it is not a crossing. However, there is a special case
	// in which this doesn't apply. This occurs when there is a pseudo-crossing that
	// involves two semi-ligature branches, one on each joint (eg. a torso, head, and two
	// arms, forming semi-ligature at the shoulders).
	if (!nonLig1.empty() && !nonLig2.empty())
	{
		if (nonLig1.size() <= 2 && semiLig1.size() >= 1 && nonLig2.size() <= 2 && semiLig2.size() >= 1)
		{
			DBG_C_LOG("there is a special case of crossing with semiligature");
		}
		else
		{
			DBG_C_LOG("not a crossing. there is at least one non-lig branch for each joint")
			return false;
		}
	}

	DBG_C_LOG("it's a crossing")

	// Now that we know it's a crossing, we can fill in the branch segment lists
	// with appropriate values
	ligSegs = ligSegs1;
	ligSegs.splice(ligSegs.end(), ligSegs2);

	ligSegs.splice(ligSegs.end(), nonLig1);  // in case there is any
	ligSegs.splice(ligSegs.end(), nonLig2);  // in case there is any

	nonLig.clear();
	nonLig.push_back(LigatureSegment(pJoint, pBranchX));

	semiLigSegs = semiLig1;
	semiLigSegs.splice(semiLigSegs.end(), semiLig2);

	return true;
}

/*!
	@brief Extends the ligature segment information of the edge
	incident on v related to the same "root" joint than that of 'ls'

	@return true if there was an in-edge that was modified and false otherwise
*/
bool GSG::ExtendRootedLigature(leda::node v, const LigatureSegment& ls)
{
	leda::edge e;

	forall_in_edges(e, v)
	{
		GSGEdge& edge = GetEdge(e);

		if (edge.m_pAttachJoint == ls.m_pJoint)
		{
			// we found it...
			edge.ExtendLigature(ls);

			return true;
		}
	}

	// NOTE: it used to be that the orientation of edges incident on a junction
	// varied according to some attributes. Now, all edges have outward direction
	// from junction nodes, and their orientation is only defined when converting
	// the branches to bones. Before, a false return value implied that the ligature
	// was rooted at a bifurcation, and that it could not be true ligature.
	// The decision of whether a ligature segment should be ignored in order to
	// represent bifurcations is now done elsewhere. The idea now is that the decision
	// related to this problem happens at a well defined place and have a clear motivation.

#ifdef _DEBUG
	// If ls.m_pJoint == NULL, it means that the segment ends at a terminal point.
	if (ls.m_pJoint != NULL)
	{
		DBG_STREAM("Possible inversion of edge direction at: "
			<< PRN_JOINT_PTR(ls.m_pJoint))

		ASSERT(false);
	}
#endif

	return false;
}

bool GSG::CheckAxialSmoothness(SkelJoint* pJoint, const LigatureSegmentList& nonLig,
								 const LigatureSegmentList& ligSegs)
{
	BranchSegment bs0(nonLig.front().m_pBranch);
	BranchSegment bs1(nonLig.back().m_pBranch);

	LigatureSegment ls;

	// Find and subtract the gap ligature from each segment
	ls.FindGapLigature(bs0.m_pBranch, pJoint, ligSegs);
	bs0.SubtractLigatureInterval(ls);

	ls.FindGapLigature(bs1.m_pBranch, pJoint, ligSegs);
	bs1.SubtractLigatureInterval(ls);

	// Note: the list of non-ligs must be ordered according to flow
	// such that the segments can be interpolated. Use the same
	// Splice() function than the one in MergeBranchNodes()
	BranchSegmentList bsl;
	BranchSegmentList bslAux;

	bsl.push_back(bs0);
	bslAux.push_back(bs1);

	bsl.Splice(bslAux);

	POINTS pts(0, bsl.Size());

	bsl.PushBackPtCoords(pts);

	EuclideanLineSegmentArray axisFunction;

	PolyLineTLSApprox poly(pts.GetSize() / 2.0, 8);

	poly.Fit(pts);

	// Copy all the line segments
	axisFunction.Resize(poly.m_knots.GetSize());

	POINT endPt0, endPt1;
	double angle;

	for (int i = 0, j = 1; j < poly.m_knots.GetSize(); i++, j++)
	{
		const EuclideanLineSegment& seg0 = poly.m_knots[i].seg;
		const EuclideanLineSegment& seg1 = poly.m_knots[j].seg;

		ASSERT(seg0.p1 == seg1.p0);

		endPt0 = seg0.p0 - seg0.p1;
		endPt1 = seg1.p1 - seg1.p0;

		angle = SignedVectorAngle(endPt0.x, endPt0.y, endPt1.x, endPt1.y);

		if (fabs(angle) <= 2.0)
			return false;
	}

	return true;
}

/*!
	@brief Computes the angle between the skeletal gap enpoints
	and the interpolating point with highest curvature.

	This is used to determine the smoothness of the skeletal gap interpolation.
	If the angle is significantly smaller than PI, then is probably too
    sharp a bend and is not a good branch merge.

	\image html ComputeMaxGapCurvatureAngle.gif
*/
double GSG::ComputeMaxGapCurvatureAngle(SkelJoint* pJoint,
										const LigatureSegmentList& nonLig,
										const LigatureSegmentList& ligSegs)
{
	ASSERT(nonLig.size() == 2 && ligSegs.size() >= 1);

	BranchSegmentList bsl;
	LigatureSegment ls;
	CubicBezier cb;
	SkelPtIndex gapPtIdx0, gapPtIdx1;

	// Find the branch segments that need to be merged. First,
	// create two branch segment with the given broken branches
	// Note: only use the branch info, as the given non-lig segments are
	// just segments rooted at the joint that don't meet the ligature properties
	{
		BranchSegment bs0(nonLig.front().m_pBranch);
		BranchSegment bs1(nonLig.back().m_pBranch);

		// Find and subtract the gap ligature from each segment
		ls.FindGapLigature(bs0.m_pBranch, pJoint, ligSegs);
		bs0.SubtractLigatureInterval(ls);

		ls.FindGapLigature(bs1.m_pBranch, pJoint, ligSegs);
		bs1.SubtractLigatureInterval(ls);

		// Note: the list of non-ligs must be ordered according to flow
		// such that the segments can be interpolated. Use the same
		// Splice() function than the one in MergeBranchNodes()
		BranchSegmentList bslAux;

		bsl.push_back(bs0);
		bslAux.push_back(bs1);

		bsl.Splice(bslAux);
	}

	// Get the first ligature segment to use as reference point
	ls = ligSegs.front();

	ComputeInterpolationCurve(bsl.front(), true, bsl.back(), true, ls,
		cb /*out*/, gapPtIdx0 /*out*/, gapPtIdx1 /*out*/);

	//DBG_MSG6("Testing:", pJoint->fp.p, cb.p0, cb.p1, cb.p2, cb.p3);
	CubicBezierParams cbp(cb);
	double u, k;
	sg::Point pt, pt0, pt1;

	k = cbp.MaximumCurvature(200, &u);
	cbp.GetCurvePoint(u, &pt.x, &pt.y);

	if (fabs(k) > 2)
	{
		DBG_C_LOG("Extreme high curvature found k = " << k
			<< " at " << PRN_JOINT_PTR(pJoint))

		DBG_STREAM("Extreme high curvature found k = " << k
			<< " at " << PRN_JOINT_PTR(pJoint))

		return 0; //maximum bent
	}

	pt0 = bsl.front()[gapPtIdx0].p - pt;
	pt1 = bsl.back()[gapPtIdx1].p - pt;

	double norm0 = pt0.norm();
	double norm1 = pt1.norm();

	// If either norm is approx zero, then it means that the
	// MAX curvature point is a gap endpoint, and so that the
	// interpolation must be pretty flat (ie 180 deg = M_PI)
	if (norm0 <= 1 || norm1 <= 1)
		return M_PI;

	double angle = SignedVectorAngle(pt0, pt1, norm0, norm1);

	//double angle = (norm0 > 0 && norm1 > 0) ?
	//	SignedVectorAngle(pt0, pt1, norm0, norm1) : M_PI;

	return fabs(angle); // get an unsigned angle
}

/*!
	@brief Checks whether the fusion of two branches leads to
	an appropriate medial axis

	@remarks If the non lig segments are not ordered wrt flow, the list
	is reversed.
*/
bool GSG::ValidateBranchMerge(SkelJoint* pJoint,
							  const LigatureSegmentList& nonLig,
							  const LigatureSegmentList& ligSegs)
{
	ASSERT(nonLig.size() == 2 && ligSegs.size() >= 1);

	DBG_C_LOG("Checking branch merge validity: "
		<< PRN_BRANCH_PTR(nonLig.front().m_pBranch)
		<< PRN_BRANCH_PTR(nonLig.back().m_pBranch))

	// Determine the smoothness of the skeletal gap interpolation
	// by computing the angle between the skeletal gap enpoints
	// and the interpolating point with highest curvature. If such an
	// angle is significantly smaller than PI, then is probably too
    // sharp a bend and is not a good merge.
	//if (!CheckAxialSmoothness(pJoint, nonLig, ligSegs))
	//	return false;

	double angle = ComputeMaxGapCurvatureAngle(pJoint, nonLig, ligSegs);

	if (angle <= 2.0 /*M_PI_2*/)
	{
		DBG_C_LOG("Invalid merge. Angle is too small: " << DBG_VAL(angle))
		return false;
	}

	LigatureSegment ls0, ls1;

	// Get the lig segment around the junction assoc with each
	// non-lig segment incident on it
	ls0.FindGapLigature(nonLig.front().m_pBranch, pJoint, ligSegs);
	ls1.FindGapLigature(nonLig.back().m_pBranch, pJoint, ligSegs);

	// Get the endpoints of the skeletal gap
	SkelPt fp0 = ls0[ls0.Target()];
	SkelPt fp1 = ls1[ls1.Target()];

	// Compute maximum "valid" boundary gap size using the radius values of
	// the skeletal gap endpoints
	double maxValidGapSize = 2.2 * MAX(fp0.radius(), fp1.radius());

	// Get the bndry info of each endpoint
	SkelPtBndryInfo bi0 = ls0(ls0.Target());
	SkelPtBndryInfo bi1 = ls1(ls1.Target());

	double dist, maxGapSize = 0;

	std::list<LigatureSegment>::const_iterator it;
	SkelPtCoord pt, pt0, pt1;

	// Find the maximum boundary gap among all attaching branches (generally just 1)
	for (it = ligSegs.begin(); it != ligSegs.end(); it++)
	{
		if (it->IsWholeBranch() && it->GetTargetJointDegree() > 1)
		{
			DBG_C_LOG("Invalid merge. Cannot merge a terminal whole ligature branch")
			return false;
		}

		// First skeletal point for attaching branch '*it'
		pt = (*it)[it->Target()].p;

		// Boundary gap endpoint 0
		pt0 = (pt.sqDist(bi0.first.pt) < pt.sqDist(bi0.second.pt)) ? bi0.first.pt : bi0.second.pt;

		// Boundary gap endpoint 1
		pt1 = (pt.sqDist(bi1.first.pt) < pt.sqDist(bi1.second.pt)) ? bi1.first.pt : bi1.second.pt;

		// Boundary gap distance
		dist = pt0.dist(pt1);

		// Keep only maximum bndry gap
		if (dist > maxGapSize)
			maxGapSize = dist;
	}

	DBG_C_LOG("Merge condition: " << DBG_VAL(maxGapSize) << "' <= ', " << DBG_VAL(maxValidGapSize))

	return (maxGapSize <= maxValidGapSize);
}

/*!
	@brief
	//bool bIgnoreNesting
*/
bool GSG::ValidateAttachment(SkelJoint* pJoint,
							 const LigatureSegmentList& nonLig,
							 const LigatureSegmentList& ligSegs)
{
	ASSERT(nonLig.size() == 2);

	bool bFullLig0 = nonLig.front().IsFullLigature();
	bool bFullLig1 = nonLig.back().IsFullLigature();

	if (bFullLig0 == bFullLig1)
		return true;

	/*if (bFullLig0)
	{
		nonLig.splice(nonLig.end(), ligSegs);
	}*/

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Helper functions

/*!
	@brief Evaluates how the array of flux points should be connected
	in order to avoid repeated points and yield a connected set of points.

	It is assumed that the back of the left array is connected to the front of teh right array.

	This function may return an index outside the right array iff it such an array has size
	equal to 1 and its only point is equal to the last point in the left array.
*/
unsigned int GetFirstConnectedPointIndex(const sg::FluxPointList& fpl, const BranchSegment& bs)
{
	if (fpl.empty())
	{
		return bs.First();
	}
	else if (fpl.back().p.sqDist(bs.FirstPtCoord()) <= 0.01) // pts are approx equals
	{
		return bs.First() + 1;
	}
	else if (bs.Size() == 1) // and they aren't equal!
	{
		return bs.First();
	}
	else if (ArePtsConnected(fpl.back(), bs[bs.First() + 1]))
	{
		return bs.First() + 1;
	}
	else
	{
		return bs.First();
	}
}

/*!
	@brief Evaluates how the array of flux points should be connected
	in order to avoid repeated points and yield a connected set of points.

	It is assumed that the back of the left array is connected to the front of teh right array.

	This function may return an index outside the right array iff it such an array has size
	equal to 1 and its only point is equal to the last point in the left array.
*/
unsigned int GetFirstConnectedPointIndex(const sg::FluxPointList& fpl0,
										 const sg::FluxPointList& fpl1)
{
	if (fpl0.empty())
	{
		return 0;
	}
	else if (fpl0.back().p.sqDist(fpl1.front().p) < 0.01)  // pts are approx equals
	{
		return 1;
	}
	else if (fpl1.size() == 1) // and they aren't equal!
	{
		return 0;
	}
	else if (ArePtsConnected(fpl0.back(), fpl1[1]))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// GSGNode functions

/*!
	Gets the branch segment formed by the non-ligature points
	of a BRANCH node.
*/
BranchSegment GSGNode::GetBonePart() const
{
	ASSERT(m_type == GSG_BRANCH);

	// We assume that if the bone is associated with a broken branch,
	// then that branch has been fixed already
	ASSERT(!HasBrokenBranch());

	BranchSegment bs;

	// Get the fixed branch, or the original non-broken branch
	if (HasFixedBranch())
	{
		bs = BranchSegment(m_pFixedBranch);
	}
	else
	{
		ASSERT(m_segments.size() == 1);
		bs = m_segments.front();
	}

	ASSERT(m_nodeHolder != nil);
	ASSERT(!bs.IsEmpty());

	// We now need to remove the known had and/or tail ligature
	// from the branch to get the bone points. We need the GSG for this.
	GSG* pGSG = (GSG*)graph_of(m_nodeHolder);

	leda::edge e;

	// Visit each incoming node incident on the branch node, retrieve
	// its ligature information, and use it to remove the ligature points
	forall_in_edges(e, m_nodeHolder)
	{
		const LigatureSegment& ls = pGSG->GetEdge(e).GetLigatureInfo();

		if (!ls.IsEmpty())
		{
			bs.SubtractLigatureInterval(ls);

			// It may happen that the ligature removal creates
			// empty bones. If they aren't terminal, this would cause
			// trouble in the creation of the Bone Graph, and so
			// we correct it by keeping the target point of 'ls'
			if (bs.IsEmpty() /*&& !bs.m_pBranch->isTerminal()*/)
			{
				DBG_C_LOG("Fixing empty NON terminal bone at " << bs)

				bs.SetLimits(ls.Target(), ls.Target());
				break;
			}
		}
	}

	ASSERT(!bs.IsEmpty() || bs.m_pBranch->isTerminal());

	return bs;
}

//////////////////////////////////////////////////////////////////////////////////////////
// GSGEdge functions

/*!
	@brief Ensures that the ligature information refers to the latest
	fixed branch in the target GSG node.
*/
void GSGEdge::UpdateLigatureInfo()
{
	if (m_edgeHolder != nil && m_ligature.m_pBranch != NULL)
	{
		GSG* pGSG = (GSG*)graph_of(m_edgeHolder);

		leda::node tgtNode = pGSG->target(m_edgeHolder);
		GSGNode& gsgNode = pGSG->GetNode(tgtNode);

		// Make sure the current branch isn't broken
		if (gsgNode.HasBrokenBranch())
			pGSG->FixBrokenBranch(tgtNode);

		SkelBranch* pBranch = gsgNode.GetCurrentBranch();

		// See if the segment refers to the right branch. Update info if not...
		if (pBranch != m_ligature.m_pBranch)
		{
			// Warning: m_ligature.m_pBranch is most likely not a valid pointer anymore

			const sg::Point& pt = m_ligature.m_pJoint->fp.p;
			const unsigned int ligSz = m_ligature.SizeFromIndices(); // don't call Size()
			const unsigned int braSz = pBranch->size();

			// The ligature must beging from either endpoint of the branch
			ASSERT(pBranch->getFirstXYPoint() == pt || pBranch->getLastXYPoint() == pt);
			ASSERT(ligSz > 0 && ligSz <= braSz);

			// Update the branch pointer
			m_ligature.m_pBranch = pBranch;

			// Update limits if they aren't expressed correctly anymore
			if (pBranch->getFirstXYPoint() == pt && m_ligature.First() != 0)
				m_ligature.SetLimits(0, ligSz - 1);
			else if (pBranch->getLastXYPoint() == pt && m_ligature.Last() != braSz - 1)
				m_ligature.SetLimits(braSz - ligSz, braSz - 1);

			ASSERT(m_ligature.Size() == ligSz); // Size() should work now
		}
	}
}

/*!
	@brief Extends the limits of the ligature segment associated with the edge
	so that in includes the given ligature segment 'ls'
*/
void GSGEdge::ExtendLigature(const LigatureSegment& ls)
{
	UpdateLigatureInfo(); // makes sure that the (fixed-)branch pointer is valid

	if (m_ligature.IsEmpty())
	{
		ASSERT(ls.First() == ls.BranchFirst() ||
			ls.Last() == ls.BranchLast());

		m_ligature = ls;
	}
	else
	{
		m_ligature.ExtendSegment(ls);
	}
}

#ifdef GSG_ENABLE_GRAPH_WIN
void ShowDependencyGraph(GSG::DependencyGraph& dg)
{
	leda::GraphWin gw(dg);

	SkelPtCoord pt;
	GSGNodeType type;
	leda::node v;
	char szCoord[100];

	forall_nodes(v, dg)
	{
		pt = dg.GetJoint(v)->fp.p;
		type = dg.GetType(dg.GetJoint(v));

		sprintf(szCoord, "(%.0f,%.0f)", pt.x, pt.y);
		gw.set_label(v, szCoord);

		if (type == GSG_JUNCTION)
			gw.set_color(v, leda::blue);
		else if (type == GSG_ATTACHMENT)
			gw.set_color(v, leda::green);
		else if (type == GSG_CROSSING)
			gw.set_color(v, leda::red);
		else
			gw.set_color(v, leda::yellow);
	}

	gw.set_node_shape(leda_rectangle_node, true);
	gw.set_node_width(120, true);
	gw.set_node_height(20, true);

	LayoutGraphRandomly(gw);

	gw.message("b=junction g=attachment r=crossing");

	// show the graph
	gw.display();
	gw.edit();
}

void ShowGSG(GSG& g, leda::node redNode/* = nil*/, leda::node blueNode/* = nil*/)
{
	leda::GraphWin gw(g);
	leda::node v;
	GSGNodeType type;

	forall_nodes(v, g)
	{
		type = g.GetNode(v).m_type;

		if (type == GSG_BRANCH)
			gw.set_label(v, "B");
		else if (type == GSG_JUNCTION)
			gw.set_label(v, "J");
		else if (type == GSG_ATTACHMENT)
			gw.set_label(v, "A");
		else if (type == GSG_CROSSING)
			gw.set_label(v, "C");
		else
			gw.set_label(v, "X");
	}

	if (redNode != nil)
		gw.set_color(redNode, leda::red);

	if (blueNode != nil)
		gw.set_color(blueNode, leda::blue);

	/*leda::edge e;

	forall_edges(e, g)
	{
		gw.set_label(e, "(x,y)\n(x,y)");
	}*/

	LayoutGraphRandomly(gw);

	// show the graph
	gw.display();
	gw.edit();
}
#endif