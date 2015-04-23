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

#include "DAG.h"

#define MINDIST 0
#define MAXDIST 1

using namespace std;
using namespace dml;
using namespace leda;

typedef SGRadialSimilarityMeasurer SGRSM;

//////////////////////////////////////////////////////////////////////////////
// SGRSM

void SGRSM::LogResults(leda::node v1, leda::node v2,
					   const ModelFit& m1, const ModelFit& m2,
					   dml::POINTS g1Pts, dml::POINTS g2Pts,
					   bool bReverseOrder) const
{
	static bool bFirstTime = true;
	static fstream log;

	//Log the results if debug mode. In any case, empty the log file
	if (bFirstTime)
	{
		bFirstTime = false;
		log.open("nodedist.m", ios::out | ios::trunc);
	}
	//else
	//	log.open("nodedist.m", ios::out | ios::app);

	int d0, dN;

	const SGNode* pNode1 = m_pG1->GetSGNode(v1);
	const SGNode* pNode2 = m_pG2->GetSGNode(v2);

	// If reverted, put them back in place before displaying
   	if (bReverseOrder)
   	{
   		g1Pts = pNode1->GetVelocityRadiusArray(d0, dN, false);
   		g2Pts = pNode2->GetVelocityRadiusArray(d0, dN, false);
   	}

   	log << "disp('" << m_pG1->GetDAGLbl() << "');\n";
   	log << "disp('" << m_pG2->GetDAGLbl() << "');\n";

   	log << "disp('" << pNode1->GetNodeLbl()
   		<< " vs " << pNode2->GetNodeLbl()
   		<< " [DFS indices: " << m_pG1->GetNodeDFSIndex(v1) << " vs " << m_pG2->GetNodeDFSIndex(v2) << "]');\n";
   	log << "% SEARCH CODE: " << m_pG1->GetViewNumber() << pNode1->GetNodeLbl()
   	 	<< m_pG2->GetViewNumber() << pNode2->GetNodeLbl() << "\n";

   	m1.Plot(log, g2Pts, pNode2->GetSegments());

   	log << "disp('" << m_pG1->GetDAGLbl() << "');\n";
   	log << "disp('" << m_pG2->GetDAGLbl() << "');\n";

   	log << "disp('" << pNode2->GetNodeLbl()
   		<< " vs " << pNode1->GetNodeLbl()
   		<< " [DFS indices: " << m_pG2->GetNodeDFSIndex(v2) << " vs " << m_pG1->GetNodeDFSIndex(v1) << "]');\n";

   	m2.Plot(log, g1Pts, pNode1->GetSegments());

   	//log << "disp('Avg error: " << error << "');\n";
}

/*!
	Use m_pG1, m_pG2, v1, v2, pNode1, pNode2
*/
double SGRSM::ComputeNodeDistance(leda::node v1, leda::node v2, const ParamIndices& parInds,
								  double* pSimilarity /*= NULL*/) const
{
	if (!DAGMatcher::GetMatchParams().nCompareNodes)
	{
		return 0;
	}

	/*if (m_pG1->GetViewNumber() == 79 && m_pG2->GetViewNumber() == 6 &&
		m_pG1->GetNodeDFSIndex(v1) == 1 && m_pG2->GetNodeDFSIndex(v2) == 1)
	{
		cout << "\n" << pNode1->GetNodeLbl() << "\n" << pNode2->GetNodeLbl() << "\n";
	}*/
	const SGNode* pNode1 = m_pG1->GetSGNode(v1);
	const SGNode* pNode2 = m_pG2->GetSGNode(v2);

	int n1Type = m_pG1->NodeType(v1);
	int n2Type = m_pG2->NodeType(v2);

	// If label is different, do not compare nodes
	if (n1Type != n2Type)
		return MAXDIST;
	else if (n1Type == ROOT || n2Type == ROOT)
		return n1Type == n2Type ? MINDIST:MAXDIST;
	else if (pNode1->m_shocks.GetSize() == 1 && pNode2->m_shocks.GetSize() == 1)
	{
		double r1 = pNode1->m_shocks.GetHead().radius;
		double r2 = pNode2->m_shocks.GetHead().radius;

		return 1 - (MIN(r1, r2) / MAX(r1, r2));
	}
	else if (pNode1->m_shocks.GetSize() == 1 || pNode2->m_shocks.GetSize() == 1)
	{
		if (pNode1->m_shocks.GetSize() > 4 || pNode2->m_shocks.GetSize() > 4)
			return MAXDIST; // if it is too long, it's unlikely that they are the same

		double r1 = pNode1->m_shocks.AvgRadius();
		double r2 = pNode2->m_shocks.AvgRadius();
		double l1 = pNode1->m_shocks.Length();
		double l2 = pNode2->m_shocks.Length();

		double rdiff = 1 - (MIN(r1, r2) / MAX(r1, r2));
		double lendiff = 1 - (MIN(l1, l2) / MAX(l1, l2));

		double dist = 0.85 * rdiff + 0.15 * lendiff;

		ASSERT_UNIT_INTERVAL(dist);

		return dist;
	}

	ASSERT(pNode1->GetSegments().GetSize() > 0 && pNode2->GetSegments().GetSize() > 0);

	// We are dealing with same label, non-root nodes, with more that 1 shock point
	// so...
	leda_node par1 = m_pG1->GetFirstParent(v1);
	leda_node par2 = m_pG2->GetFirstParent(v2);
	int n1Dir, n2Dir;

	if (m_pG1->NodeType(par1) != ROOT && m_pG2->NodeType(par2) != ROOT)
	{
		n1Dir = m_pG1->GetBranchDir(v1, par1);
		n2Dir = m_pG2->GetBranchDir(v2, par2);

		// If the nodes' directions wrt their parents differ,
		// we can say they directions don't match
		if (n1Dir != n2Dir)
			return MAXDIST;
	}

	// Now, given that their directions match, we need to make
	// sure that we get them right. The may go from 0 to N or form N to 0.
	n1Dir = pNode1->GetBranchDir(ShockBranch::FORWARD);
	n2Dir = pNode2->GetBranchDir(ShockBranch::FORWARD);
	bool bReverseOrder = ( n1Dir != n2Dir );

	// what if dir == 0 but there is a slope? We should still reverse it.
	// THIS IS A TO DO

	int d0, dN;
	POINTS g1Pts = pNode1->GetVelocityRadiusArray(d0, dN, bReverseOrder);
	POINTS g2Pts = pNode2->GetVelocityRadiusArray(d0, dN, bReverseOrder);

// 	if(!( pNode2->GetSegments().GetTail().p1.y == (bReverseOrder ? g2Pts.GetHead().y:g2Pts.GetTail().y) ))
// 	{
// 		DBG_VAL(g1Pts.GetSize())
// 		DBG_VAL(g2Pts.GetSize())
// 		DBG_VAL(pNode1->GetShockCount())
// 		DBG_VAL(pNode2->GetShockCount())
// 		DBG_VAL(n1Dir)
// 		DBG_VAL(n2Dir)
// 		DBG_VAL(pNode2->GetSegments().GetTail().p0.y)
// 		DBG_VAL(pNode2->GetSegments().GetTail().p1.y)
// 		DBG_VAL(pNode2->GetSegments().GetHead().p0.y)
// 		DBG_VAL(pNode2->GetSegments().GetHead().p1.y)
// 		DBG_VAL(pNode1->GetSegments().GetTail().p0.y)
// 		DBG_VAL(pNode1->GetSegments().GetTail().p1.y)
// 		DBG_VAL(pNode1->GetSegments().GetHead().p0.y)
// 		DBG_VAL(pNode1->GetSegments().GetHead().p1.y)
// 		DBG_VAL(g2Pts.GetHead().y)
// 		DBG_VAL(g2Pts.GetTail().y)
// 		DBG_VAL(g1Pts.GetHead().y)
// 		DBG_VAL(g1Pts.GetTail().y)
//
// 		for (int k = 0; k < pNode2->GetShockCount(); k++)
// 			cerr << (*pNode2)[k].radius << ", ";
// 		cerr << endl;
// 	}

	ASSERT( pNode1->GetSegments().GetHead().p0.y == (bReverseOrder ? g1Pts.GetTail().y:g1Pts.GetHead().y) || g1Pts.GetSize() <= 3 );
	ASSERT( pNode1->GetSegments().GetTail().p1.y == (bReverseOrder ? g1Pts.GetHead().y:g1Pts.GetTail().y) || g1Pts.GetSize() <= 3 );
	ASSERT( pNode2->GetSegments().GetHead().p0.y == (bReverseOrder ? g2Pts.GetTail().y:g2Pts.GetHead().y) || g2Pts.GetSize() <= 3 );
	ASSERT( pNode2->GetSegments().GetTail().p1.y == (bReverseOrder ? g2Pts.GetHead().y:g2Pts.GetTail().y) || g2Pts.GetSize() <= 3 );

	// Let's fit the model of node one to the data of node 2 and viceversa
	ModelFit m1(2), m2(2); // 2 is max diff in lenght

	double e1 = m1.Fit(g1Pts, pNode2->GetSegments());

	if (e1 >= INFINITY)
		return MAXDIST;

	double e2 = m2.Fit(g2Pts, pNode1->GetSegments());

	if (e2 >= INFINITY)
		return MAXDIST;

	if (DAG::IsDbgMode())
		LogResults(v1, v2, m1, m2, g1Pts, g2Pts, bReverseOrder);

	/*double e, s;

	for (int i = 0; i < pNode2->GetSegments().GetSize(); i++)
	{
		m1.GetSegmentError(i, &e, &s);
		DBG_SHOW(e)
		DBG_SHOW(s)
	}*/

	// We have non-infinite errors, so we must normalize them
	double avge1 = (e1 / g1Pts.GetSize()) / pNode1->m_shocks.AvgRadius();
	double avge2 = (e2 / g2Pts.GetSize()) / pNode2->m_shocks.AvgRadius();
	double error = (avge1 + avge2) / 2.0;

	if (error >= MAXDIST)
		return MAXDIST;

	double minL = MIN(pNode1->m_shocks.Length(), pNode2->m_shocks.Length());
	double maxL = MAX(pNode1->m_shocks.Length(), pNode2->m_shocks.Length());

	double dist = 0.85 * error + 0.15 * (1 - minL / maxL);

	ASSERT_UNIT_INTERVAL(dist);

	return dist;
}

///////////////////////////////////////////////////////////////////////////////////////
//