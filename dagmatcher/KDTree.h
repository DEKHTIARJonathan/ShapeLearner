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
 * $Log: KDTree.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef _KD_TREE_H_
#define _KD_TREE_H_

//#include <vector>
#include <ann_1.1/include/ANN/ANN.h>
#include "BasicUtils.h"

namespace dml {
/*!
	@brief Wrapper class for the structures and algorithms in the ANN library
*/
class KDTree
{
protected:
	int m_nPts;                 //!< actual number of data points
	int m_nMaxPts;              //!< maximum number of data points
	int m_nNNPtsReturned;		//!< number of NN points returned by the last search

	ANNpointArray m_dataPts;    //!< data points
	ANNpoint m_queryPt;         //!< query point
	ANNkd_tree* m_kdTree;       //!< search structure

	enum {DIM = 2};             //!< assume 2-dimension points

public:
	ANNidx* m_indices;          //!< array of indices for points in m_dataPts
	ANNdist* m_distances;       //!< array of NN squared distances for points in m_dataPts

public:
	KDTree(int nMaxDataPoints);
	~KDTree();

	bool Build();
	int Size() const { return m_nPts; }
	int RangeSearch(const double& x, const double& y, const double& range,
		double* epsilon, const double& epsilonIncrement, int atLeast);

	//!< Finds all points within range 'range' from position x,y. Use GetNN*() to get the points.
	int RangeSearch(const double& x, const double& y, const double& range)
	{
		ASSERT(m_kdTree != NULL);

		m_queryPt[0] = x;
		m_queryPt[1] = y;

		m_nNNPtsReturned = m_kdTree->annkFRSearch(m_queryPt, range,
			m_nPts, m_indices, m_distances);

		return m_nNNPtsReturned;
	}

	// Functions that index over the m_indices array are identified by the 'NN' in their name
	ANNpoint GetNNPoint(int i)  { ASSERT(i >= 0 && i < m_nNNPtsReturned); return m_dataPts[m_indices[i]]; }
	ANNidx GetNNIndex(int i)    { ASSERT(i >= 0 && i < m_nNNPtsReturned); return m_indices[i]; }
	double GetNNDistance(int i) { ASSERT(i >= 0 && i < m_nNNPtsReturned); return sqrt(m_distances[i]); }
	double GetNNSquaredDistance(int i) { ASSERT(i >= 0 && i < m_nNNPtsReturned); return m_distances[i]; }

	double GetNNXCoord(int i) { return GetNNPoint(i)[0]; }
	double GetNNYCoord(int i) { return GetNNPoint(i)[1]; }

	// Functions that index over the m_dataPts array are identified by the 'Data' in their name
	ANNpoint GetDataPoint(ANNidx idx)
	{
		ASSERT(idx >= 0 && idx < m_nPts);
		return m_dataPts[idx];
	}

	void GetDataPoint(ANNidx idx, double&x, double& y) const
	{
		ASSERT(idx >= 0 && idx < m_nPts);
		x = m_dataPts[idx][0];
		y = m_dataPts[idx][1];
	}

	double GetDataXCoord(int i) { return GetDataPoint(i)[0]; }
	double GetDataYCoord(int i) { return GetDataPoint(i)[1]; }

	void AddDataPoint(const double& x, const double& y)
	{
		ASSERT(m_kdTree == NULL);
		ASSERT(m_nPts < m_nMaxPts);

		m_dataPts[m_nPts][0] = x;
		m_dataPts[m_nPts][1] = y;
		m_nPts++;
	}

	/*void AddDataPoints(const std::vector& points)
	{
		ASSERT(m_kdTree == NULL);
		ASSERT(m_nPts + points.size() <= m_nMaxPts);

		std::copy(points.begin(), points.end(), m_dataPts + m_nPts);
		m_nPts += points.size();

		//
		for (int k = 0; k < m_nSmoothContourIterations; k++)
			smooth_closed_curve_pts(contour_pts);
	}*/

	/*!
		@brief Computes the straight distance from point nFrom on the boundary
		to point nTo on the boundary.
	*/
	double DataPointDistance(int nFrom, int nTo) const
	{
		ASSERT(nFrom >= 0 && nFrom < m_nPts);
		ASSERT(nTo >= 0 && nTo < m_nPts);

		double dx = m_dataPts[nTo][0] - m_dataPts[nFrom][0];
		double dy = m_dataPts[nTo][1] - m_dataPts[nFrom][1];

		return sqrt(dx * dx + dy * dy);
	}
};
} // namespace dml

#endif //_KD_TREE_H_
