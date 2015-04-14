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

#ifndef __DAG_H__
#define __DAG_H__

#include <fstream>
#include <string>

#include "BasicTypedefs.h"

#include "DAGNode.h"
#include "DAGEdge.h"
#include "DMLString.h"


#include <Newmat\newmatap.h>
#include <Newmat\newmatio.h>

#include "SkeletalGraph.h"
#include "DAGMatcher.h"

namespace dml {
typedef NEWMAT::Matrix Matrix;

typedef leda::GRAPH<DAGNodePtr, DAGEdgePtr> DAG_BASE_CLASS;

/*!
	\brief This class represents a specialized directed acyclic graph (DAG).
	It derives from	the generic GRAPH<> class in the LEDA library.

	DAG is an abstract class derived from the parameterized LEDA graph.
	This class can only be used by deriving a new class from it and overwriting
	its abstract functions.

	The information contained in each node is a pointer to a DAGNode, which
	in turn is another abstract class. In order to handle the destruction of the
	objects referenced by these pointers, the class DAGNodePtr was created to keep
	track of the references to DAGNode's and delete them when no pointer points to
	them anymore.

	The information in each edge is a 'double' that represents its weigth.

	/see DAGNode and DAGNodePtr.
*/

class DAG : public DAG_BASE_CLASS
{
public:
	struct ConstructParams
	{
		int nUseDirNameAsObjName;

		ConstructParams() { nUseDirNameAsObjName = 1; }
	};

private:
	String m_strGraphLbl;            //!< Graph label. e.g. the source image file name.
	int m_nFileOffset;                 //!< file offset where this DAG is located.
	int m_nDAGId;                      //!< Id of the DAG in the DB. Is transitory so mustn't be saved.
	int m_nCumulativeMass;             //!< Sum of all the nodes' masses

	// Derived values
	int m_nMaxBFactor;                 //!< Maximum branching factor of the DAG.
	double m_dTotalTSVSum;             //!< Sum of all the node's TSV magnitudes.
	SmartMatrix<int> m_transClosMat;   //!< The adjacency matrix of the transitive closure graph
	double m_dDAGCost;                 //!< Som of all node and edge costs

public:
	static bool s_bDbgMode;
	static ConstructParams s_constructParams;

protected:
	int m_nViewNumber0;
	int m_nViewNumber1;
	String m_strObjectName;

	ShapeDims m_dims;                 //!< Basic display parameters: Shape dimensions

protected:
	TSV& GetNodeTSV(leda::node v)               { return operator[](v)->GetTSV(); }
	void SetEigenVals(const Matrix& adj);

public:
	const TSV& GetNodeTSV(leda::node v) const   { return inf(v)->GetTSV(); }

public:
// Virtual functions:
	virtual ~DAG() { /* default virtual destructor */ }
	virtual DAG& operator=(const DAG& rhs);
	virtual void Clear();
	virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;
	virtual std::istream& Read(std::istream& is, bool bOnlyDataForMatching = false);
	virtual std::ostream& Write(std::ostream& os) const;
	virtual void ComputeDerivedValues();
	virtual void SetObjectNameAndViewNumber(const String& path);

	virtual bool IsRootNode(leda::node v) const { return indeg(v) == 0; }

	//! Reads a double edge weight
	virtual DAGEdgePtr ReadEdge(std::istream& is)
	{
		DAGEdgePtr edge(new DAGEdge);
		edge->Read(is);
		return edge;
	}

// Pure virtual functions:
	virtual DAGMatcher* GetMatchingAlgorithm() const = 0;
	virtual void SetMatchingAlgorithm() = 0;
	virtual bool AreNodesRelated(leda::node g1Node, const DAG& g2, leda::node g2Node) const = 0;
	virtual DAGNodePtr CreateNodeObject(NODE_LABEL lbl) const = 0;
	virtual DAGNodePtr ReadNode(std::istream& is) const = 0;
	virtual String ClassName() const = 0;
	virtual int NodeType(leda::node v) const = 0;

	/*!
		@brief Creates an empty object of a DAG-derived class. To clone a DAG do
		   pNewObj = dag.CreateObject(); *pNewObj = dag; ...; delete pNewObj;
	*/
	virtual DAG* CreateObject() const = 0;

#ifdef XML_READING
	virtual bool ReadFromXMLFile(const char* szFileName, bool bCompEigenLbl = true) = 0;
#endif //XML_READING

// Simple inline functions:
	DAG()
	{
		// Init all default values
		Clear();
	}

	leda::node NewNode(DAGNodePtr ptr)
	{
		return DAG_BASE_CLASS::new_node(ptr);
	}

	leda::edge NewEdge(leda::node u, leda::node v, double dVal = DEFAULT_DAG_EDGE_WEIGHT)
	{
		return DAG_BASE_CLASS::new_edge(u, v, new DAGEdge(dVal));
	}

	leda::edge NewEdge(leda::node u, leda::node v, DAGEdgePtr ptr)
	{
		return DAG_BASE_CLASS::new_edge(u, v, ptr);
	}

	void GetClassAndModel(char* szClassName, int* pModelId) const;

	void DelEdge(leda::edge e)                       { del_edge(e); }
	void DelNode(leda::node v)                       { del_node(v); }

	int GetNodeCount() const                         { return number_of_nodes(); }
	int GetEdgeCount() const                         { return number_of_edges(); }
	double GetEdgeWeight(leda::edge e) const         { return inf(e)->GetWeight(); }
	NODE_LABEL GetNodeLbl(leda::node v) const        { return inf(v)->GetNodeLbl(); }
	double GetEigenLbl(leda::node v) const           { return inf(v)->GetEigenLbl(); }

	//! Gets the sum of all the nodes' masses
	int GetCumulativeMass() const                    { return m_nCumulativeMass; }

	bool IsEmpty() const                             { return GetNodeCount() == 0; }

	TSV GetNodeTSV(leda::node v, int nDim) const     { return inf(v)->GetTSV(nDim); }
	double GetNodeTSVNorm(leda::node v) const        { return inf(v)->GetTSVNorm(); }

	const DAGNodePtr GetNode(leda::node v) const     { return inf(v); }
	DAGNodePtr& GetNode(leda::node v)                { return operator[](v); }

	//! Slow version. Linear time.
	leda::node GetNode(int nIndex);

	//! Slow version. Linear time.
	leda::node GetNode(NODE_LABEL lbl);

	int GetNodeInDegree(leda::node v) const          { return indeg(v); }
	int GetNodeOutDegree(leda::node v) const         { return outdeg(v); }

	const DAGEdgePtr GetEdge(leda::edge e) const     { return inf(e); }
	DAGEdgePtr& GetEdge(leda::edge e)                { return operator[](e); }

	void SetNodeLbl(leda::node v, NODE_LABEL lbl)    { operator[](v)->SetNodeLbl(lbl); }
	void SetEigenLbl(leda::node v, double lbl)       { operator[](v)->SetEigenLbl(lbl); }

	int GetNodeIndex(leda::node v) const             { return index(v); }

	leda::color GetNodeColor(leda::node v) const     { return inf(v)->GetColor(); }
	void SetNodeColor(leda::node v, leda::color c)   { operator[](v)->SetColor(c); }

	String GetDAGLbl() const                         { return m_strGraphLbl; }
	void SetDAGLbl(const char* szLbl)                { m_strGraphLbl = szLbl; }

	int GetDAGId() const                             { return m_nDAGId; }
	void SetDAGId(int nId)                           { m_nDAGId = nId; }

	//! Gets the sum of all node and edge costs
	const double& GetDAGCost() const                 { return m_dDAGCost; }

	double GetNodeCost(leda::node v) const           { return GetNode(v)->GetCost(); }

	double GetSubtreeCost(leda::node v) const        { return GetNode(v)->GetSubtreeCost(); }

	//! Gets the const of the DAG minus that of the subtree rooted at v
	double GetSubtreeComplementCost(leda::node v) const
	{
		return GetDAGCost() - GetSubtreeCost(v);
	}

	int GetFileOffset()	const                        { return m_nFileOffset; }

	int GetMaxTSVDimension() const                   { return m_nMaxBFactor + 1; }
	double GetTotalTSVSum() const                    { return m_dTotalTSVSum; }
	int GetViewNumber() const                        { return m_nViewNumber0; }

	void GetViewNumbers(int* pNum0, int* pNum1) const
	{
		*pNum0 = m_nViewNumber0;
		*pNum1 = m_nViewNumber1;
	}

	const String& GetObjName() const                 { return m_strObjectName; }

	void SetViewNumber(int n)                        { m_nViewNumber0 = n; }
	void SetObjName(char *str)                       { m_strObjectName = str; }

	int GetNodeMass(leda::node v) const              { return GetNode(v)->GetMass(); }
	int GetNodeLevel(leda::node v) const             { return GetNode(v)->GetLevel(); }
	int GetNodeDFSIndex(leda::node v) const          { return GetNode(v)->GetDFSIndex(); }
	NODE_LABEL GetNodeDFSLbl(leda::node v) const     { return GetNode(v)->GetDFSLbl(); }

	leda::node GetFirstParent(leda::node v) const    { return source(first_in_edge(v)); }
	leda::node GetSecondParent(leda::node v) const   { return source(in_succ(first_in_edge(v))); }

	leda::node GetFirstChild(leda::node v) const     { return target(first_adj_edge(v)); }
	leda::node GetSecondChild(leda::node v) const    { return target(adj_succ(first_adj_edge(v))); }

	void DeleteSubDAG(leda::node v);

	virtual double ComputeTSVs(leda::node root);
	double ComputeTSVs(leda::node v, int& j, Matrix& adj, NodeIndexMap& loopyNodeMap);
	//double ComputeTSVs(leda::node v, Matrix& adj);
	int AddTreeToMatrix(leda::node r, int i, Matrix& adj);

// Static functions:
	static double ComputeEigenSum(const Matrix& adj, int nVals);
	static double ComputeLaplacian(const Matrix& adj);
	static void ComputeHeights(const Matrix& adj, Matrix& m);

// Non-virtual const functions:
	void AddEdge(Matrix& adj, leda::edge e, int sourceNodeLbl, int targetNodeLbl) const;
	void DelEdge(Matrix& adj, int sourceNodeLbl, int targetNodeLbl) const;
	leda::node GetFirstRootNode() const;
	leda::node GetNextRootNode(leda::node v) const;
	SmartArray<leda::node> GetAllRoots() const;
	void PrintTSVs(std::ostream& os = std::cout) const;
	double NodeTSVSimilarity(leda::node u, const DAG& from, leda::node v) const;
	void PrintAdjMatrix(std::ostream& os = std::cout);
	leda::node DFSGetNext(leda::node v, EdgeList& l, bool bVisitNode = true) const;
	leda::node BFSGetNext(leda::node v, EdgeList& l, bool bVisitNode = true) const;

	const SmartMatrix<int>& GetTransClosMat() const { return m_transClosMat; }

// Non-virtual non-const functions:
	DAGPtr SplitSubGraph(leda::node v, bool bRecomputeTSVs = false);

	leda::node SplitSubGraph(leda::node v, DAGPtr& ptrDag,
		DynamicNodeMap& nodeParentMap, leda::edge inEdge);

	void ComputeTransiveClosure();
	int ComputeNodesInfo(leda::node v, int nLevel, int nDFSIndex);
	void ResetNodesInfo();
	void ComputeNodesMass();

// Functions for DAGView class (make them return NULL/0 if not interested in implementing them)
	virtual const SkeletalGraph* GetSkeleton() const = 0;
	double xmin() const   { return m_dims.xmin; }
	double xmax() const   { return m_dims.xmax; }
	double ymin() const   { return m_dims.ymin; }
	double ymax() const   { return m_dims.ymax; }
	double Width() const  { return xmax() - xmin(); }
	double Height() const { return ymax() - ymin(); }
	const ShapeDims& GetDims() const { return m_dims; }

// Basic stream functions
	friend std::ostream& operator<<(std::ostream &os, const DAG& dag)  { return dag.Write(os); }
	friend std::istream& operator>>(std::istream &is, DAG& dag)        { return dag.Read(is); }

	//! Lower case version of Read()
	std::istream& read(std::istream& is) { return Read(is); }

	//! Lower case version of Write()
	std::ostream& write(std::ostream& os) const { return Write(os); }

private:
	//! Base class' function that should not be called.
	leda::node new_node(DAGNodePtr ptr = (DAGNode*)NULL)		{ return NewNode(ptr); }

	//! Base class' function that should not be called.
	leda::edge new_edge(leda::node u, leda::node v, double dVal = 0.0) { return NewEdge(u, v, dVal); }

public:
	static void EnterDbgMode()                  { s_bDbgMode = true; }
	static void LeaveDbgMode()                  { s_bDbgMode = false; }
	static bool IsDbgMode()	                    { return s_bDbgMode; }

	/*!
		True if there is a path from node 'a' to node 'v'

		Note: A node is considered an ancestor of itself.
	*/
	bool IsAncestor(leda::node a, leda::node v) const
	{
		ASSERT_GRAPH2(this, a, v);

		return (m_transClosMat[GetNodeDFSIndex(a)][GetNodeDFSIndex(v)] != 0);
	}

	/*!
		True if there is a path from node 'v' to node 'd'

		Note: A node is considered a descendant of itself.
	*/
	bool IsDescendant(leda::node d, leda::node v) const
	{
		ASSERT_GRAPH2(this, d, v);

		return (m_transClosMat[GetNodeDFSIndex(v)][GetNodeDFSIndex(d)] != 0);
	}

	/*!
		True if node 'v' is a child of a parent of node 'u'
	*/
	bool IsSibling(leda::node v, leda::node u) const
	{
		ASSERT_GRAPH2(this, u, v);

		leda::edge e, ee;
		leda::node p;

		const int idxV = GetNodeDFSIndex(v);

		// See if v is a child of any parent of the matched node u
		// for all parents of u...
		forall_in_edges(e, u)
		{
			p = source(e); // p is a parent of node u

			forall_out_edges(ee, p)
				if (target(ee) == v)
					return true;
		}

		return false;
	}

	/*!
		True if node 'v' is a descendant of a parent of node 'u'
	*/
	bool IsSiblingOrSiblingDescendant(leda::node v, leda::node u) const
	{
		ASSERT_GRAPH2(this, u, v);

		leda::edge e;
		leda::node p;

		// See if v is a descendant of any parent of the matched node u
		forall_in_edges(e, u)
		{
			p = source(e); // p is a parent of node u

			// Deal with the special case of v being a parent node
			if (p == v)
				return false;
			else if (IsDescendant(v, p))
				return true;
		}

		return false;
	}

	//! Finds the first edge in the path that goes from 'v0' to 'v1'
	leda::edge FindFirstEdgeInPath(leda::node v0, leda::node v1) const
	{
		ASSERT_GRAPH2(this, v0, v1);

		leda::edge e;

		forall_out_edges(e, v0)
			if (IsAncestor(target(e), v1))
				return e;

		return nil;
	}

	//! Finds the last edge in the path that goes from 'v0' to 'v1'
	leda::edge FindLastEdgeInPath(leda::node v0, leda::node v1) const
	{
		ASSERT_GRAPH2(this, v0, v1);

		leda::edge e;

		forall_in_edges(e, v1)
			if (IsDescendant(source(e), v0))
				return e;

		return nil;
	}

	//! Gets the relative level of 'v' wrt 'u' defined as: rel_level = level(v) - level(u)
	int GetRelativeNodeLevel(leda::node u, leda::node v) const
	{
		ASSERT_GRAPH2(this, u, v);

		return GetNodeLevel(v) - GetNodeLevel(u);
	}

	//! Computes the similarity between two DAGs
	double Similarity(const DAG& model) const
	{
		return GetMatchingAlgorithm()->Match(*this, model);
	}

	//! Computes the similarity between two DAGs and their node correspondeces
	double Similarity(const DAG& model, NodeMatchMap& nodeMap) const
	{
		double sim = GetMatchingAlgorithm()->Match(*this, model);

		GetMatchingAlgorithm()->GetNodeMap(nodeMap);

		return sim;
	}

    //! Gets the node correspondences associated with the last call to DAG::Similarity()
	std::string GetNodeMapString() const;

	//! Computes the similarity between the attributes of node u in *this and node v in g2
	double NodeAttributeSimilarity(leda::node u, const DAG& g2, leda::node v,
		NodeMatchInfoPtr ptrMatchInfo = NULL)
	{
		GetMatchingAlgorithm()->InitGraphs(this, &g2);

		return GetMatchingAlgorithm()->NodeAttributeSimilarity(u, v, ptrMatchInfo);
	}

	/*!
		Computes the attribute and structural similarity between
		node u in *this and node v in g2
	*/
	double NodeSimilarity(leda::node u, const DAG& g2, leda::node v,
		NodeMatchInfoPtr ptrMatchInfo = NULL) const
	{
		GetMatchingAlgorithm()->InitGraphs(this, &g2);

		return GetMatchingAlgorithm()->NodeSimilarity(u, v, ptrMatchInfo);
	}
};
} //namespace dml

#endif //__DAG_H__
