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
 * $Log: DAG.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

#ifdef USE_TEMPLATE_BASED_MWBM_FUNCTION
#include <LEDA/graph/templates/mwb_matching.h>
using namespace std; // Fixes an error in the next LEDA header scale_weights.h line 35
#include <LEDA/graph/scale_weights.h>
#else
#include <LEDA/graph/mwb_matching.h>
#endif

//#include <LEDA/graphwin.h>

#include <stack>
#include <float.h>
#include "HelperFunctions.h"
#include "Exceptions.h"
#include "BasicUtils.h"

#ifdef WIN32
#define isnan  _isnan
#define finite _finite
#endif

#define ROOT_NODE_LEVEL			0
#define ROOT_NODE_DFS_INDEX		0

using namespace dml;
using namespace leda;

bool DAG::s_bDbgMode = false;
DAG::ConstructParams DAG::s_constructParams;

#ifdef _DEBUG
LogFile g_dagCreationLog("dagCreation.log", &DAG::s_bDbgMode);
#endif

/*!
	@brief Visits all the node in DFS order and computes they derived values from the
	info in the graph.

	It sets the cost of the subtree rooted at v, and that of all its sub-subtrees.

	@return the last DFS index + 1. i.e. The corresponding next DFS index.
*/
int DAG::ComputeNodesInfo(leda::node v, int nLevel, int nDFSIndex)
{
	DAGNodePtr& ptrNode = GetNode(v);

	// Default level is -1. Loopy nodes should have their greatest height
	if (ptrNode->GetLevel() < nLevel)
		ptrNode->SetLevel(nLevel);

	if (ptrNode->GetDFSIndex() == -1)
		ptrNode->SetDFSIndex(nDFSIndex++);

	nLevel++;

	leda::node u;
	double stCost = ptrNode->GetCost();

	ASSERT(stCost > 0);

	forall_adj_nodes(u, v)
	{
		nDFSIndex = ComputeNodesInfo(u, nLevel, nDFSIndex);

		stCost += GetSubtreeCost(u);
	}

	ptrNode->SetSubtreeCost(stCost);

	return nDFSIndex;
}

void DAG::ResetNodesInfo()
{
	leda::node v;

	forall_nodes(v, *this)
	{
		GetNode(v)->SetLevel(-1);
		GetNode(v)->SetDFSIndex(-1);
	}
}

void DAG::ComputeTransiveClosure()
{
	leda::node u, v;
	int i, n = GetNodeCount();

	m_transClosMat.Resize(n, n, true);

#if LEDA_VERSION == 43
	leda::graph g = TRANSITIVE_CLOSURE(*this);

	// Translate the transitive closure graph into a matrix form
	forall_nodes(v, g)
	{
		i = GetNodeDFSIndex(v);

		forall_adj_nodes(u, v)
			m_transClosMat[i][GetNodeDFSIndex(u)] = 1;
	}
#else
	LEDA_GRAPH<leda::node,leda::edge> g = TRANSITIVE_CLOSURE(*this);

	// Translate the transitive closure graph into a matrix form
	forall_nodes(v, g)
	{
		i = GetNodeDFSIndex(g[v]);

		forall_adj_nodes(u, v)
			m_transClosMat[i][GetNodeDFSIndex(g[u])] = 1;
	}
#endif
}

void DAG::ComputeNodesMass()
{
	leda::node v;

	ASSERT(GetNodeCount() * GetNodeCount() == m_transClosMat.GetSize());

	/*forall_nodes(v, *this)
		GetNode(v)->SetMass(m_transClosMat.RowSum(GetNodeDFSIndex(v)));*/

	SmartArray<int> ones(m_transClosMat.NRows());
	ones.Set(1);

	SmartArray<int> relMasses = m_transClosMat * (m_transClosMat * ones);

	forall_nodes(v, *this)
		GetNode(v)->SetMass(relMasses[GetNodeDFSIndex(v)]);
}

/*!
	\brief Compute all the derived values in the graph.

	The derived values are those which depend on other values and don't need
	to be saved. These values are in fact actually read and saved but only in
	order to improve computation performance.
*/
void DAG::ComputeDerivedValues()
{
	SmartArray<leda::node> roots = GetAllRoots();
	int i, nDFSIndex = ROOT_NODE_DFS_INDEX;
	leda::node v;
	leda::edge e;

	ASSERT(roots.GetSize() > 0); // There has to be at least one root

	ResetNodesInfo(); // Make sure the node info is clean

	m_dDAGCost = 0; // init the total cost of the DAG

	for (i = 0; i < roots.GetSize(); i++)
	{
		nDFSIndex = ComputeNodesInfo(roots[i], ROOT_NODE_LEVEL, nDFSIndex);

		// Sum the cost of each root
		m_dDAGCost += GetSubtreeCost(roots[i]);
	}

	ComputeTransiveClosure();
	ComputeNodesMass();

	m_nCumulativeMass = 0;
	m_nMaxBFactor = 0;
	m_dTotalTSVSum = 0;

	for (i = 0; i < roots.GetSize(); i++)
	{
		v = roots[i];
		ComputeTSVs(v);
		m_nCumulativeMass += GetNodeMass(v);
	}

	forall_nodes(v, *this)
	{
		GetNode(v)->ComputeDerivedValues();

		m_dTotalTSVSum += GetNodeTSVNorm(v);

		if (m_nMaxBFactor < outdeg(v))
			m_nMaxBFactor = outdeg(v);
	}

	forall_edges(e, *this)
	{
		GetEdge(e)->ComputeDerivedValues(e);
	}

	SetObjectNameAndViewNumber(GetDAGLbl());
}

/*!
	This function separates a subtree rooted at <v> and returs it
	in the <subg> parameter.

	The transitive closure matrix and the cumulative mass of the entire
	graph is retained in both subgraphs.

	\param v root of the subtree.
	\param bRecomputeTSVs controls whether the TSVs that remains in
	the original graph should be recomputed.
*/
DAGPtr DAG::SplitSubGraph(leda::node v, bool bRecomputeTSVs)
{
	DAGPtr subg = CreateObject();

	// if v is not a root...
	if (indeg(v) > 0)
	{
		// Create a map for the parents of nodes with more
		// than one parent
		DynamicNodeMap nodeParentMap;

		SplitSubGraph(v, subg, nodeParentMap, nil);

		// Now it is safe to eliminate those nodes in the
		// subtre that have multiple parents
		forall_defined(v, nodeParentMap)
			del_node(v);

		subg->m_transClosMat = GetTransClosMat();
		subg->m_nCumulativeMass = m_nCumulativeMass;

		if (bRecomputeTSVs)
		{
			SmartArray<leda::node> roots = GetAllRoots();
			for (int i = 0; i < roots.GetSize(); i++)
				ComputeTSVs(roots[i]);
		}
	}
	else
	{
		*subg = *this;
		del_all_nodes();
	}

	return subg;
}

/*!
	Recursive function called by SplitSubGraph(leda::node, bool).
	It recursively adds a subgraph to the graph pointed by <ptrDag>
	and then it deletes the subgraph from the original graph.

	Note: if the desired subtree contains nodes with multiple parents,
	we need special care. There are two cases, (a) more than one of the
	multiple parents of a node are also contained in the subtree; (b) only
	one of the node's parents is in the subtree. Case (b) is not a problem,
	but case (a) requires keeping track of the multiple parents and avoding
	deleting these nodes before traversing all their insident edges.
*/
leda::node DAG::SplitSubGraph(leda::node v, DAGPtr& ptrDag, DynamicNodeMap& nodeParentMap, leda::edge inEdge)
{
	leda::node a, b;
	leda::edge e;
	double w;
	bool bMultiParents;

	ASSERT(inEdge == nil || target(inEdge) == v);

	if (nodeParentMap.defined(v))
	{
		bMultiParents = true; // it either has OR had multiple parents
		a = nodeParentMap[v];
	}
	else
	{
		bMultiParents = indeg(v) > 1; // it has multiple parents
		a = ptrDag->NewNode(inf(v));

		if (bMultiParents)
			nodeParentMap[v] = a;
	}

	forall_adj_edges(e, v)
	{
		w = inf(e);
		b = SplitSubGraph(target(e), ptrDag, nodeParentMap, e);
		ptrDag->NewEdge(a, b, w);
	}

	// NOTE: nodes with multiple parents will be eliminated at the end,
	// but the edge that brought us to the node must be eliminated now.
	if (!bMultiParents)
		del_node(v);
	else if (inEdge != nil)
		del_edge(inEdge);

	return a;
}

//! Create an edge between vertices <i> and <j> of weight <dist>.
void DAG::AddEdge(Matrix& adj, leda::edge e, int srcNodeIdx, int tgtNodeIdx) const
{
	double w = GetEdgeWeight(e);

	adj(srcNodeIdx, tgtNodeIdx) = w;
	adj(tgtNodeIdx, srcNodeIdx) = -w;
}

/*!
	\brief Computes eigen-sum over a given adjacency matrix.

	We are supposed to compute the $\delta(T_i) - 1$ largest absolute eigenvalues,
	where $\delta(T_i)$ is the degree of the root node of the tree $T_i$.

	The justification for summing the $\delta(T_i) - 1$ largest absolute eigenvalues follows:

	a) the largest absolute eigenvalues are the most informative of the subgraph structure.

	b) by summing $\delta(T_i) - 1$ elements we normalize the sum according to the local
	   complexity of the subgraph root.

	According to the Overton and Womersley's Theorem, for the sum of the first
	k eigenvalues of a symmetric matrix A, the following semidefinite programming
	characterization holds:

	\[\lambda_1(A) + \hdots + \lamda_k(A) = max A \dot U  s.t. trace(U) = k for 0 \le U \le I.\]

	However, for our particular case, we can't use this theorem because A is antisymmetric.
	Therefore, for this implementation, we compute the SVD to obtain the eigenvalues and then
	all the values are summed up, instead of only the $\delta(T_i) - 1$ largest ones.

	\todo Find out the impact of summing all the eigenvalues instead of the $k$ largest ones.5s3l1

*/
double DAG::ComputeEigenSum(const Matrix& adj, int nVals)
{
	using namespace NEWMAT;

	int n = adj.Nrows();
	Matrix U(n, n);
	DiagonalMatrix D(n);
	double td = 0.0;
	//double eigenVals[n];

	//nVals *= 2; // Values will be duplicated, so step will be 2.
	//ASSERT(nVals <= n);

	SVD(adj, D, U);

	for (int i = 1; i <= nVals; i++)
		td += D(i, i);

	return td;
}

double DAG::ComputeLaplacian(const Matrix& adj)
{
	using namespace NEWMAT;

	int n = adj.Nrows(), i, j;
	SymmetricMatrix L(n);
	ColumnVector T(n);

	for (i = 1; i <= n; i++)
	{
		T(i) = 0;

		for (j = 1; j <= n; j++)
			T(i) += fabs(adj(i, j));
	}

	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
		{
			if (i == j)
				L(i, j) = 1;
			else if (adj(i, j) != 0)
				L(i, j) = -1 / (sqrt(T(i) * T(j)));
			else
				L(i, j) = 0;
		}
	}

	DiagonalMatrix D(n);

	Jacobi(L, D);

	SortAscending(D);

	return fabs(D(1,1));
}

/*!
	Computes all the nodes' TSV's.
*/
double DAG::ComputeTSVs(leda::node root)
{
	int j = 1, n = GetNodeMass(root);
	ASSERT(n > 0);

	NodeIndexMap loopyNodeMap;
	Matrix adj(n, n);
	adj = 0.0;

	return ComputeTSVs(root, j, adj, loopyNodeMap);
}

/*!
	step 1: for all adjacent nodes w of the given root node, if w has
	in-degree grater that 1 (multiple parents) and has already been visited,
	bHasLoopyChild is set to true so that a new adj matrix will be computed,
	in which v is the root and use it to compute v's TSV. Note that
	if j is the root of the matrix (i = j = 1), we don't care whether v has
	loopy children or not. Otherwise we would enter in an endless loop.

	step 2: add the edges to the adj matrix. Edges immediately below
	a loopy	node must be added to the adj matrix, but those below them
	do not because they have already been added.

	step 3: compute v's eigen sum if necessary.
*/
double DAG::ComputeTSVs(leda::node v, int& j, Matrix& adj, NodeIndexMap& loopyNodeMap)
{
	using namespace NEWMAT;

	leda::node w;
	leda::edge e;
	double s;
	int i = j;
	bool bDefined, bHasLoopyChild = false;

	TSV& tsv = GetNodeTSV(v);
	tsv.Resize(outdeg(v), true);

	forall_adj_edges(e, v)
	{
		w = target(e);
		bDefined = indeg(w) > 1 ? loopyNodeMap.defined(w):false;

		if (bDefined)
		{
			bHasLoopyChild = true;
			AddEdge(adj, e, i, loopyNodeMap[w]);
			s = GetEigenLbl(w);
		}
		else
		{
			AddEdge(adj, e, i, ++j);

			if (indeg(w) > 1)
				loopyNodeMap[w] = j; // loopy node visited

			s = ComputeTSVs(w, j, adj, loopyNodeMap);
		}

		tsv.Add(s);
	}

	// If v has loopy children and it isn't the root of the adj matrix
	// we'll compute its TSV sum by creating a new adj mat rooted at v.
	if (bHasLoopyChild && i > 1)
		s = ComputeTSVs(v);
	else
	{
		s = ComputeEigenSum(adj.SubMatrix(i, j, i, j), outdeg(v));
		//s = ComputeLaplacian(adj.SubMatrix(i, j, i, j));

		SetEigenLbl(v, s);
		tsv.Sort();
	}

	return s;
}

leda::node DAG::GetNode(int nIndex)
{
	const leda::list<leda::node>& nodeList = all_nodes();
	leda::list_item it = nodeList.get_item(nIndex);

	return nodeList[it];
}

leda::node DAG::GetNode(NODE_LABEL lbl)
{
	leda::node v;

	forall_nodes(v, *this)
	{
		if (GetNodeLbl(v) == lbl)
			return v;
	}

	return nil;
}

//! Returns the first root in the DAG. i.e. the first node v that has indeg(v) = 0
leda::node DAG::GetFirstRootNode() const
{
	leda::node v;

	forall_nodes(v, *this)
		if (indeg(v) == 0)
			return v;

	return nil;
}

//! Returns the next root in the DAG. i.e. the first SUCCESSOR of node v that has indeg(v) = 0
leda::node DAG::GetNextRootNode(leda::node v) const
{
	do {
		v = succ_node(v);
	} while (v != nil && indeg(v) != 0);

	return v;
}

//! Returns a SmartArray with all the graph's root nodes.
SmartArray<leda::node> DAG::GetAllRoots() const
{
	SmartArray<leda::node> roots;
	leda::node v;

	forall_nodes(v, *this)
		if (indeg(v) == 0)
			roots.AddTail(v);

	return roots;
}

/////////////////////////////////////////////////////////////////////////////////////
// Virtual functions

//! Assignment operator for the DAG class. Must be overwritten in the derived classes.
DAG& DAG::operator=(const DAG& rhs)
{
	Clear();

	DAG_BASE_CLASS::operator=(rhs);

	m_nFileOffset = rhs.m_nFileOffset;
	m_nDAGId      = rhs.m_nDAGId;
	m_strGraphLbl = rhs.m_strGraphLbl;

	// Derived values
	m_nMaxBFactor     = rhs.m_nMaxBFactor;
	m_dTotalTSVSum    = rhs.m_dTotalTSVSum;
	m_nViewNumber0    = rhs.m_nViewNumber0;
	m_nViewNumber1    = rhs.m_nViewNumber1;
	m_strObjectName   = rhs.m_strObjectName;
	m_transClosMat    = rhs.m_transClosMat;
	m_nCumulativeMass = rhs.m_nCumulativeMass;
	m_dDAGCost        = rhs.m_dDAGCost;

	// Display parameters
	m_dims = rhs.m_dims;

	return *this;
}

//! It sets to zero all the DAG's member variables
void DAG::Clear()
{
	DAG_BASE_CLASS::clear();

	m_nFileOffset = 0;
	m_nDAGId = 0;
	m_strGraphLbl.Clear();

	// Derived values
	m_nMaxBFactor = 0;
	m_dTotalTSVSum = 0;
	m_nViewNumber0 = 0;
	m_nViewNumber1 = 0;
	m_strObjectName.Clear();
	m_transClosMat.Clear();
	m_nCumulativeMass = 0;

	// Display parameters
	m_dims.xmin = m_dims.xmax = 0;
	m_dims.ymin = m_dims.ymax = 0;
}

/*!
	Print out vertices, their labels and eigenlabels, and the
	adjacency structure of the graph.
*/
void DAG::Print(std::ostream& os, bool bXMLFormat) const
{
	leda::node v;
	leda::edge e;

	if (bXMLFormat)
	{
		//XML_PRINT(os, "label", GetDAGLbl());
		XML_PRINT(os, "objectName", GetObjName());
		XML_PRINT(os, "viewNumber", GetViewNumber());

		forall_nodes(v, *this)
		{
			XML_OPEN_ATTS(os, "node");

			XML_PRINT_ATT2(os, "parentCount", indeg(v));
			XML_PRINT_ATT2(os, "childCount", outdeg(v));

			GetNode(v)->Print(os, bXMLFormat);

			//XML_PRINT(os, "childNodeIndex", GetNodeDFSIndex(target(e)));

			XML_CLOSE(os, "node");
		}

		forall_edges(e, *this)
		{
			XML_OPEN_ATTS(os, "edge");
			XML_PRINT_ATT2(os, "source", GetNodeDFSIndex(source(e)));
			XML_PRINT_ATT2(os, "target", GetNodeDFSIndex(target(e)));
			XML_END_ATTS(os);

			GetEdge(e)->Print(os, bXMLFormat);

			XML_CLOSE(os, "edge");
		}
	}
	else
	{
		os << "\nDAG label: " << GetDAGLbl() << std::endl;
		os << "Object: " << GetObjName() << ", view: " << GetViewNumber() << std::endl;

		forall_nodes(v, *this)
		{
			os << "\nNode:" << index(v) << std::endl;
			GetNode(v)->Print(os, bXMLFormat);
		}
	}
}

//! Prints the set of all TSVs in the graph.
void DAG::PrintTSVs(std::ostream& os /*=cout*/) const
{
	os << "\nDAG's adj matrix eigen values:\n";

	os << "\n\n--------------- TSV Set ---------------\n";

	leda::node v;

	forall_nodes(v, *this)
	{
		os << "\nNode " << GetNodeLbl(v) << ": ";
		GetNodeTSV(v).Print(os);
	}

	os << "\n---------------------------------------\n";
}

//! Reads a DAG from the specified stream.
std::istream& DAG::Read(std::istream& is, bool bOnlyDataForMatching /*= false*/)
{
	int n, i;
	leda::node v, s, t;
	leda::map<long, leda::node> map;
	//double w;
	String strClassName;

	Clear();

	// Save the offset where we are about to read from
	m_nFileOffset = is.tellg();

	strClassName.Read(is);

	if (ClassName() != strClassName) {
		DBG_MSG2("Reading DAG from offset:", m_nFileOffset);
		THROW_EXCEPTION("Incorrect DAG class");
	}

	m_strGraphLbl.Read(is);

	is.read((char*) &n, sizeof(n));

	for(i = 0; i < n; i++)
	{
		is.read((char*) &v, sizeof(v));
		map[(long)v] = NewNode(ReadNode(is));
	}

	is.read((char*) &n, sizeof(n));

	for(i = 0; i < n; i++)
	{
		is.read((char*) &s, sizeof(s));
		is.read((char*) &t, sizeof(t));
		//is.read((char*) &w, sizeof(w));
		NewEdge(map[(long)s], map[(long)t], ReadEdge(is));
	}

	// Read derived values
	is.read((char*) &m_nMaxBFactor, sizeof(m_nMaxBFactor));
	is.read((char*) &m_dTotalTSVSum, sizeof(m_dTotalTSVSum));
	is.read((char*) &m_nViewNumber0, sizeof(m_nViewNumber0));
	is.read((char*) &m_nViewNumber1, sizeof(m_nViewNumber1));

	m_strObjectName.Read(is);

	is.read((char*) &m_dDAGCost, sizeof(m_dDAGCost));

	m_transClosMat.Read(is);

	is.read((char*) &m_nCumulativeMass, sizeof(m_nCumulativeMass));
	ASSERT(m_nCumulativeMass >= GetNodeCount());

	// Read basic display parameters
	is.read((char*) &m_dims, sizeof(m_dims));

	return is;
}

//! Writes the DAG to the specified stream.
std::ostream& DAG::Write(std::ostream& os) const
{
	leda::node v, s, t;
	leda::edge e;
	//double w;

	// Save the offset where we are about to write to ???
	//m_nFileOffset = os.tellp();

	ClassName().Write(os);
	m_strGraphLbl.Write(os);

	int n = GetNodeCount();

	os.write((char*) &n, sizeof(n));

	forall_nodes(v, *this)
	{
		os.write((char*) &v, sizeof(v));
		os << inf(v);
	}

	n = GetEdgeCount();

	os.write((char*) &n, sizeof(n));

	forall_edges(e, *this)
	{
		s = source(e);
		t = target(e);
		//w = inf(e);
		os.write((char*) &s, sizeof(s));
		os.write((char*) &t, sizeof(t));
		//os.write((char*) &w, sizeof(w));
		os << inf(e);
	}

	// Write derived values
	os.write((char*) &m_nMaxBFactor, sizeof(m_nMaxBFactor));
	os.write((char*) &m_dTotalTSVSum, sizeof(m_dTotalTSVSum));
	os.write((char*) &m_nViewNumber0, sizeof(m_nViewNumber0));
	os.write((char*) &m_nViewNumber1, sizeof(m_nViewNumber1));

	m_strObjectName.Write(os);

	os.write((char*) &m_dDAGCost, sizeof(m_dDAGCost));

	m_transClosMat.Write(os);
	os.write((char*) &m_nCumulativeMass, sizeof(m_nCumulativeMass));

	// Write basic display parameters
	os.write((char*) &m_dims, sizeof(m_dims));

	return os;
}

//! Delete and edge between vertices <i> and <j>.
void DAG::DelEdge(Matrix& adj, int srcNodeIdx, int tgtNodeIdx) const
{
	adj(srcNodeIdx, tgtNodeIdx) = 0.0;
	adj(tgtNodeIdx, srcNodeIdx) = 0.0;
}

/*!
	@brief Deletes the subtree rooted at <v>

	\todo This function does not support some kinds of DAGs. More specifically,
	DAGs with loops are not allowed. This must be fixed in the future.
*/
void DAG::DeleteSubDAG(leda::node v)
{
	leda::node u;

	forall_adj_nodes(u, v)
		DeleteSubDAG(u);

	del_node(v);
}

/*!
	@brief Prints the DAG's adjacency matrix

	FIXME It only works where there is one single root node
*/
void DAG::PrintAdjMatrix(std::ostream& os)
{
	int j = 1, n = GetNodeCount();
	NodeIndexMap loopyNodeMap;
	Matrix adj(n, n);

	adj = 0.0;
	ComputeTSVs(GetFirstRootNode(), j, adj, loopyNodeMap);

	os << "\nAdjacency matrix:\n";
	os << std::setw(1) << std::setprecision(0) << adj << std::setprecision(6) << std::endl;
}

/*
	Depth First Search
*/
leda::node DAG::DFSGetNext(leda::node v, EdgeList& l, bool bVisitNode) const
{
	leda::edge e;

	ASSERT(v != nil);

	if (bVisitNode)
		forall_adj_edges(e, v)
			l.append(e);

	return l.empty() ? nil:target(l.pop_back());
}

/*
	Breadth First Search
*/
leda::node DAG::BFSGetNext(leda::node v, EdgeList& l, bool bVisitNode) const
{
	leda::edge e;

	ASSERT(v != nil);

	if (bVisitNode)
		forall_adj_edges(e, v)
			l.append(e);

	return l.empty() ? nil:target(l.pop_front());
}

/*!
	@brief Uses the file path to extract the object name and view
	number.
*/
void DAG::SetObjectNameAndViewNumber(const String& path)
{
	DirWalker dw;

	dw.ParseFileName(path);
	dw.ParseFileNameParams();

	if (dw.params.size() == 1)
	{
		m_strObjectName = dw.params.begin()->first.c_str();
		m_nViewNumber0  = dw.params.begin()->second;
	}
	else if (dw.params.size() > 1)
	{
		DirWalker::iterator it = dw.params.begin();

		m_strObjectName.SetEqual(it->first.c_str(), it->second);

		for (++it; it != dw.params.end(); ++it)
		{
			if (it->first == "az")
				m_nViewNumber0 = it->second - 180;
			else if (it->first == "el")
				m_nViewNumber1 = it->second - 90;
			else
				ShowError1("Unknown file name param:", it->first);
		}
	}
	else
	{
		m_strObjectName = path;
	}
}

void DAG::GetClassAndModel(char* szClassName, int* pModelId) const
{
	DirWalker::SplitPrefixAndNumber(GetObjName(), szClassName, pModelId);
}

std::string DAG::GetNodeMapString() const
{
	DAGMatcher* pMatcher = GetMatchingAlgorithm();
	NodeMatchMap nodeMap;
	std::stringstream ss;
	leda::node v;
	bool first = true;

	// Get the node correspondences associated with the last call to DAG::Similarity()
	pMatcher->GetNodeMap(nodeMap);

	ASSERT(this == &pMatcher->G1());

	ss << "{";

	forall_nodes(v, *this)
	{
		const NodeMatchCodomain& nmc = nodeMap[v];

		if (first)
			first = false;
		else
			ss << ";";

		ss << GetNode(v)->GetLblForGraph(false).c_str() << ",";

		ss << pMatcher->G2().GetNode(nmc.node)->GetLblForGraph(false).c_str() << ",";

		ss << nmc.similarity;
	}

	ss << "}";

	return ss.str();
}