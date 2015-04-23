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
 * $Log: GestureGraph.cpp,v $
 * Revision 1.1  2002/05/21 16:52:19  dmac
 * New file added.
 *
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"


using namespace std;
using namespace dml;
using namespace leda;

//! Helper function for Read(string file_name)
void SkipComments(ifstream& is)
{
	char ch;

	is.get(ch);

	while (ch == '#' || ch == '\n' || ch == ' ')
	{
		// read comments
		if(ch == '#')
			while (ch != '\n')
				is.get(ch);

		is.get(ch);
	}

	is.seekg(-1, ios::cur);
}

/////////////////////////////////////////////////////////////////////////////////
// Init static variables
DAGMatcher* GestureGraph::s_pDAGMatcher = NULL;

/////////////////////////////////////////////////////////////////////////////
// GestureGraph class implementation

void GestureGraph::Clear()
{
	DAG::Clear();

	relationMatrix.Clear();
	hierarchicalMatrix.Clear();
	hierarchyLevels.Clear();
	m_nRootNodeCount = 0;
}

bool GestureGraph::Read(String strFileName)
{
	GGNode* p;
	int n, m, nVertices;
	char szNodeLbl[50];

	ifstream is(strFileName, ios::in);

	if (is.fail())
	{
		cerr << "Error: Cannot open file " << strFileName << endl;
		return false;
	}

	SkipComments(is);

	is >> m_nRootNodeCount;

	SkipComments(is);

	is >> nVertices;

	relationMatrix.Resize(nVertices, nVertices);
	hierarchicalMatrix.Resize(nVertices, nVertices);
	hierarchyLevels.Resize(nVertices);

	SkipComments(is);

	for(n = 0; n < nVertices; n++)
	{
		p = new GGNode;

		is >> p->m_nType >> p->m_dSign >> p->m_dScale >> p->m_nXPos
			>> p->m_nYPos >> p->m_dA >> p->m_dB >> p->m_dOrientation
			>> p->m_dAnisotropy >> p->m_dUnkField;

		p->m_nNodeIndex = n;
		sprintf(szNodeLbl, "%d:%d", n, p->m_nType);
		p->SetNodeLbl(szNodeLbl);

		NewNode(p);
	}

	SkipComments(is);

	for(n = 0; n < nVertices; n++)
		for(m = 0; m < nVertices; m++)
			is >> relationMatrix[n][m];

	SkipComments(is);

	for(n = 0; n < nVertices; n++)
		is >> hierarchyLevels[n];

	SkipComments(is);

	for(n = 0; n < nVertices; n++)
	{
		for (m = 0; m < nVertices; m++)
		{
			is >> hierarchicalMatrix[n][m];

			if (hierarchicalMatrix[n][m] == 1)
				NewEdge(GetNode(n), GetNode(m), 1);
		}
	}

	SetDAGLbl(strFileName);
	m_strObjectName = strFileName;
	ComputeDerivedValues();

	Matrix euclideanDistance(nVertices, nVertices);
  Matrix bearing(nVertices, nVertices);

  BuildMatrices(euclideanDistance, bearing);
  BuildNodeHistograms(euclideanDistance, bearing);

  leda_node_array<int> ord(*this);
  bool isAcyclic = TOPSORT(*this, ord);
  ASSERT(isAcyclic);

	return true;
}

/*!
	@brief Populates the given |V|x|V| matrices with the information stored in the graph

	@param euclideanDistance reference to a |V|x|V| Matrix that will contain
		the euclidean distances
	@param bearing reference to a |V|x|V| Matrix that will contain
		the bearings
*/
void GestureGraph::BuildMatrices(Matrix& euclideanDistance, Matrix& bearing)
{
 leda_node v,u;
 double distance, dif1, dif2;
 int indexV, indexU;

 forall_nodes(v, *this) {
  const GGNode* pV = GetGGNode(v);
  indexV = pV->GetDFSIndex();

  forall_nodes(u, *this){
   const GGNode* pU = GetGGNode(u);
   dif1 = pV->m_nXPos - pU->m_nXPos;
   dif2 = pV->m_nYPos - pU->m_nYPos;

   distance = sqrt(dif1 * dif1 + dif2 * dif2);

   indexU = pU->GetDFSIndex();
   euclideanDistance(indexV + 1, indexU + 1) = distance;
  }
 }
}

/*!
	@brief Populates the histograms of every node

	@param euclideanDistance NEWMAT::Matrix with the euclidean distances
	@param bearing NEWMAT::Matrix with the bearings
*/
void GestureGraph::BuildNodeHistograms(const Matrix& euclideanDistance, const Matrix& bearing)
{
	int indexV, indexU, n, i;
	leda_node v;
	leda_edge e;
	//double sumScales[GetNodeCount()];
	//double sumDistances[GetNodeCount()];

	std::vector<double> sumScales(GetNodeCount());
	std::vector<double> sumDistances(GetNodeCount());

	// First, compute sums of distances and scales to normalize measurements
	forall_nodes(v, *this) {
		const GGNode* pV = GetGGNode(v);
		indexV = pV->GetDFSIndex();

		sumScales[indexV] = pV->m_dScale;
		sumDistances[indexV] = 0;

		forall_inout_edges(e, v) {
			const GGNode* pU = GetGGNode((source(e) == v) ? target(e):source(e));
			indexU = pU->GetDFSIndex();

			sumScales[indexV] += pU->m_dScale;
			sumDistances[indexV] += euclideanDistance(indexV + 1, indexU + 1);
		}

		ASSERT(sumScales[indexV] > 0.0 && sumDistances[indexV] > 0.0);
	}

	// We can now set the info in the nodes using the given matrices
	forall_nodes(v, *this) {
		GGNode* pV = UnsafeGetGGNode(v);

		n = degree(v);
		pV->m_xCoords.Resize(n);
		pV->m_weights.Resize(n);
		pV->m_scales.Resize(n);

		indexV = pV->GetDFSIndex();

		i = 0;
		forall_inout_edges(e, v) {
			const GGNode* pU = GetGGNode((source(e) == v) ? target(e):source(e));
			indexU = pU->GetDFSIndex();

			pV->m_xCoords[i] = euclideanDistance(indexV + 1, indexU + 1) / sumDistances[indexV];
			pV->m_weights[i] = 1.0;
			pV->m_scales[i] = pU->m_dScale / sumScales[indexV];
			i++;
		}
	}
}

//! Extends the behaviour of the same function in the base class.
istream& GestureGraph::Read(istream& is, bool bOnlyDataForMatching /*= false*/)
{
	DAG::Read(is);

  	hierarchyLevels.Read(is);
	hierarchicalMatrix.Read(is);
	relationMatrix.Read(is);
	is.read((char*) &m_nRootNodeCount, sizeof(m_nRootNodeCount));

	return is;
}

//! Extends the behaviour of the same function in the base class.
ostream& GestureGraph::Write(ostream& os) const
{
	DAG::Write(os);

  	hierarchyLevels.Write(os);
	hierarchicalMatrix.Write(os);
	relationMatrix.Write(os);
	os.write((char*) &m_nRootNodeCount, sizeof(m_nRootNodeCount));

	return os;
}

//! Extends the behaviour of the same function in the base class.
void GestureGraph::Print(ostream& os, bool bXMLFormat) const
{
	DAG::Print(os, bXMLFormat);

	// add new stuff here
}

//! Extends the behaviour of the same function in the base class.
DAG& GestureGraph::operator=(const DAG& rhs)
{
	DAG::operator=(rhs);

	const GestureGraph* pRhs = dynamic_cast<const GestureGraph*>(&rhs);

	if (!pRhs)
		THROW_EXCEPTION("Invalid pointer type.");

	hierarchyLevels    = pRhs->hierarchyLevels;
	hierarchicalMatrix = pRhs->hierarchicalMatrix;
	relationMatrix     = pRhs->relationMatrix;
	m_nRootNodeCount   = pRhs->m_nRootNodeCount;

	return *this;
}

/*!
	Relation function for two Shock Graph nodes.
	For now, all nodes of the same type are related.
*/
bool GestureGraph::AreNodesRelated(leda_node u, const DAG& from, leda_node v) const
{
	const GestureGraph& sgFrom = dynamic_cast<const GestureGraph&>(from);

	// add stuff here

	return true;
}

//! Creates a new shock graph object.
DAG* GestureGraph::CreateObject() const
{
	return new GestureGraph;
}

//! Creates a new gesture graph node object.
DAGNodePtr GestureGraph::CreateNodeObject(NODE_LABEL lbl) const
{
	return new GGNode;
}

/*!
	Creates a new GGNode and fill its contents with the current
	information in the stream.
*/
DAGNodePtr GestureGraph::ReadNode(istream& is) const
{
	DAGNodePtr node(new GGNode);

	node->Read(is);

	return node;
}

//! Returns the classs name
String GestureGraph::ClassName() const
{
	return "GestureGraph";
}

/*
TODO: Move this to its own SimilarityMeasurer class

double GestureGraph::NodeAttributeDistance(leda_node u, const DAG& from, leda_node v) const
{
 const GestureGraph& ggFrom = dynamic_cast<const GestureGraph&>(from);

 const GGNode* pU = GetGGNode(u);
 const GGNode* pV = ggFrom.GetGGNode(v);

 //double hist_distance = 0.5;
 //double hist_scale  = 0.5;

 double hist_distance = CompareHistograms(pU->m_xCoords, pU->m_weights,
		pV->m_xCoords, pV->m_weights);

 double hist_scale = CompareHistograms(pU->m_scales, pU->m_weights,
		pV->m_scales, pV->m_weights);

 //cerr << "AVERAGE :" << (hist_distance + hist_scale) / 2.0 << endl;
 //For now, we'll return the average
 return (hist_distance + hist_scale) / 2.0;
}*/

void GestureGraph::SetMatchingAlgorithm()
{
	//TODO: fill in code here
}