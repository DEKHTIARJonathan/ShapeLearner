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
 * $Log: ShockGraph.h,v $
 * Revision 1.3  2002/06/10 18:37:33  dmac
 * New class that handles the node distance function added.
 *
 * Revision 1.2  2002/05/16 22:50:11  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef __SHOCK_GRAPH_H__
#define __SHOCK_GRAPH_H__

#include "DAG.h"
#include "SGNode.h"
#include "SkeletalGraph.h"

namespace dml {
//! ShockGraph parameters for construction
struct ShockGraphParams
{
	int nSlipt3s;
	double dMinSlope, dMinError, dMaxYDiff, dMaxAccelChg;

	ShockGraphParams() { memset(this, 0, sizeof(ShockGraphParams)); }
};

/*!
	@brief A shock graph is a shape representation in the form of
	a graph with node type SGNode and endge type double.

	@see DAG, SGNode, DAGNode.
*/
class ShockGraph: public DAG
{
protected:
	enum BRANCH_DIR {IN_DIR, OUT_DIR};

	SkeletalGraph* m_pSkeleton;

	int m_nLastIndexUsed;
	ShockGraphParams m_compParams;

	static DAGMatcher* s_pDAGMatcher; //!< Matching algorithm for shock graphs

protected:
	const char* GetNextIdx();

	bool ComputeSGFromDDSGraph(sg::DDSGraph* sk);

	SGNode* CopyNodeInfo(sg::DDSEdge* ddsEdge, leda_node u, SGNode* pNode);
	void LabelEndPoints(SGNode* pNode, sg::FluxPointList& fpl, sg::DDSNode* n1, sg::DDSNode* n2);
	SGNode* GroupShockPoints(leda_node u, SGNode* pNode);
	SGNode* SplitNode(leda_node v, SGNode* pNode, int nEnd, LineSegment& leftSeg, LineSegment& rightSeg);
	void ConnectNodes(leda_node u, SGNode* pNode, sg::DDSEdge* e, sg::DDSNode* n);

	void RevertEdge(leda_edge pxEdge);

	void Insert4sAnd2s();
	void TryRelabel3As2or4(leda_node v);
	void DirectEdgesAcordingToTime();
	void FinalizeSGConstruction();

	void ComputeNodeRole(leda_node v);

	SGNode* SplitSGNode(SGNode* pNode, leda_d_array<long, leda_node>& map);

public:
	ShockGraph();
	~ShockGraph();

	bool Create(const ImageInfo& imgInfo, const ShockGraphParams& sgparams,
		const SkeletalGraphParams& skelparams);

	bool Create(SkeletalGraph* pSkeleton, const ShapeDims& dims, String strLbl,
		const ShockGraphParams& sgparams);

	int GetBranchDir(leda_node u, leda_node wrtV) const;
	const ShockInfo& GetJointPoint(leda_node u, leda_node v) const;

	int NodeType(leda_node v) const { return GetSGNode(v)->m_nType; }
	NODE_ROLE NodeRole(leda_node v) const { return GetSGNode(v)->GetNodeRole(); }
	int NodeLength(leda_node v) const {	return GetSGNode(v)->m_shocks.GetSize(); }

	GRAMMAR_RULE GetRewriteRule(leda_node r) const;

	bool CheckChildNodeTypes(leda_node r, SmartArray<int> validTypes,
		const ShockInfo* pInvJointPt = NULL) const;

	leda_node CheckGrammar(leda_node root = nil) const;

	// DAG virtual functions
	virtual DAG& operator=(const DAG& rhs);
	virtual ShockGraph& operator=(const ShockGraph& rhs);

	virtual void Clear();

	// DAG pure virtual functions
	virtual DAGMatcher* GetMatchingAlgorithm() const { return s_pDAGMatcher; }

	virtual void SetMatchingAlgorithm();
	virtual bool AreNodesRelated(leda_node g1Node, const DAG& g2, leda_node g2Node) const;
	virtual DAG* CreateObject() const;
	virtual DAGNodePtr CreateNodeObject(NODE_LABEL lbl) const;
	virtual DAGNodePtr ReadNode(std::istream& is) const;
	virtual String ClassName() const;

	virtual std::istream& Read(std::istream& is, bool bOnlyDataForMatching = false);
	virtual std::ostream& Write(std::ostream& os) const;
	virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;
   virtual bool ReadFromXMLFile(const char* szFileName, bool bCompEigenLbl = true) ;

	//! Returns the stroed skeletal graph or NULL if it does not exist
	virtual const SkeletalGraph* GetSkeleton() const { return m_pSkeleton; }

	//! Releases the ownership of the stored skeletal graph (if it exists)
	virtual SkeletalGraph* ReleaseSkeleton()
	{
		SkeletalGraph* pSkel = m_pSkeleton;
		m_pSkeleton = NULL;
		return pSkel;
	}

	//void SetSkeleton(SkeletalGraph* pSkeleton) { m_pSkeleton = pSkeleton; }

	const SGNode* GetSGNode(leda_node v) const { return (const SGNode*)(const DAGNode*)GetNode(v); }
	SGNode* UnsafeGetSGNode(leda_node v) { return (SGNode*)GetSGNode(v); }
};
} //namespace dml

#endif //__SHOCK_GRAPH_H__
