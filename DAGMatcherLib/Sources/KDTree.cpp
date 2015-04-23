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
 * $Log: KDTree.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

using namespace dml;

KDTree::KDTree(int nMaxDataPoints)
{
	m_nMaxPts = nMaxDataPoints;
	m_nPts = 0;
	m_nNNPtsReturned = 0;

	m_queryPt = annAllocPt(DIM);	     		// allocate 1 query point structure
	m_dataPts = annAllocPts(m_nMaxPts, DIM);	// allocate datapoint storage space

	m_kdTree    = NULL;
	m_indices   = NULL;
	m_distances = NULL;
}

KDTree::~KDTree()
{
	delete[] m_indices;
	delete[] m_distances;
	delete m_kdTree;
	annDeallocPt(m_queryPt);
	annDeallocPts(m_dataPts);
	//annClose(); => deletes static variables in the ANNkd_tree class. do not call it.
}

/*!
	@brief Creates KD-tree for efficient nearest neighbour search
*/
bool KDTree::Build()
{
	ASSERT(m_kdTree == NULL);

	if (m_kdTree == NULL)
	{
		// Build the tree
		m_kdTree = new ANNkd_tree(m_dataPts, m_nPts, DIM);

		if (m_kdTree != NULL)
		{
			// Allocated space for query results
			m_indices   = new ANNidx[m_nPts];
			m_distances = new ANNdist[m_nPts];
		}
	}

	return m_kdTree != NULL;
}

/*bool KDTree::Rebuild(newdatapoints)
{
	ASSERT(m_bIsKDTreeBuilt == true);

	delete m_kdTree;

	// Build new tree
	m_kdTree = new ANNkd_tree(m_dataPts, m_nPts, DIM);

	return (m_bIsKDTreeBuilt = (m_kdTree != NULL));
}*/

/*!
	@brief Finds the boundary points that are closest to (x,y). It returns
	at least 'atLeast' number of points. For efficiency, it uses a given
	range to restrict the initial search. The parameter epsilon controls
	the error margin for the range. 'epsilon' is an in/out variable and can
	be set to zero (recommended). If the minimum number of points is not reached,
	then 'epsilon' is incremented by 'epsilonIncrement'.

	Ideally, one wants to set range such that the do-loop in this function is done
	only once. However, in some weird cases, setting an appropriate range is difficult
	and so multiple iterations may be needed.

	@return the number of points found within range and '*epsilon' incremented by
		'epsilonIncrement' at least once.
*/
int KDTree::RangeSearch(const double& x, const double& y, const double& range,
						double* epsilon, const double& epsilonIncrement, int atLeast)
{
	const double rangeSquared = range * range;
	int n;

	//DBG_ONLY(int nIter = 0)

	do
	{
		//DBG_ONLY(nIter++)

		n = RangeSearch(x, y, rangeSquared + *epsilon);

		*epsilon += epsilonIncrement; // must end up incremented
	} while (n < atLeast && n < m_nPts);

	// TODO: this happens too often. change the thing with tangents
	/*DBG_MSG_IF("Multiple iterations for skeleton point required", nIter > 1)
	DBG_PRINT_IF(nIter, nIter > 1)
	DBG_PRINT_IF(m_queryPt[0], nIter > 1)
	DBG_PRINT_IF(m_queryPt[1], nIter > 1)*/

	return n;
}