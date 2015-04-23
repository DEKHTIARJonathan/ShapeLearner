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
 * $Log: BoundaryCorner.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

/*!
	To control performance, we restrict the number of points that are visited
	when searching for a corner point.
*/
#define MAX_NUM_POINTS_TO_TEST 50

#define MIN_SQ_DISTANCE 4 // ie, approx. 2.83 ~= 2 * sqrt(2)

using namespace dml;

DBG_ONLY(extern LogFile g_dagCreationLog)

//const double BoundaryCorner::s_dBndryCornerMinSqDist = 4;

/*!
	@brief Finds the smallest angle with the boundary interval defined by
	the spokes of the given pair of skeletal points. The maximum
	scale of the "corner" is given by the squared distance 'maxSqDist'.

	@return pCornerInfo is filled in with corner attributes.
*/
void BoundaryCorner::FindCorner(const BoundaryIntervalWithGaps& bi,
								const double& minCornerCosAngle,
								const CornerArea& ca,
								const sg::Point* pRefPt)
{
	BoundaryIntervalWithGaps::Index idx, left, right;
	BPList pts;
	BPIterator firstIt, lastIt;
	sg::Point pt;
	double d, minSqDist, maxSqDist;
	unsigned int n, maxNumPts;
	bool bHaveEnoughtPts;

	ASSERT(bi.IsValid());

	m_cosAngle = -1; // init to widest angle
	m_valid = false; // init to invalid corner

	// Add all pts in bi0 to the list
	if (!bi.GetBoundaryPoints(&pts, bi.First(), bi.Last()))
		return;

	firstIt = pts.begin();
	lastIt  = pts.end();
	lastIt--;

	d = pts.size() * 0.5;
	minSqDist = d * d;

	if (minSqDist < MIN_SQ_DISTANCE)
		minSqDist = MIN_SQ_DISTANCE;

	// Make sure that max dist is >= min dist
	maxNumPts = pts.size();
	maxSqDist = maxNumPts * maxNumPts;

	if (!ca.IsLimitPoint(pts.front()))
	{
		bHaveEnoughtPts = false;

		for (idx = bi.Prev(bi.First()), n = 0; /* true */; bi.MovePrev(idx), n++)
		{
			bi.GetBoundaryPoint(idx, &pt);
			pts.push_front(pt); // push at the FRONT of the list

			if (ca.IsLimitPoint(pt))
				break;

			d = pt.sqDist(*firstIt);

			if (!bHaveEnoughtPts && d >= minSqDist)
			{
				bHaveEnoughtPts = true;

				if (d > maxSqDist)
					maxSqDist = d;
			}

			if (bHaveEnoughtPts && (n >= maxNumPts || d >= maxSqDist))
				break;

			// Make sure that we have enough points to define one arm, and if so
			// check that idx is within bnry pts assoc with current branch
			//if (n >= minNumPts && n >= maxNumPts)
			//	break;
		}
	}

	if (!ca.IsLimitPoint(pts.back()))
	{
		bHaveEnoughtPts = false;

		for (idx = bi.Next(bi.Last()), n = 0; /* true */; bi.MoveNext(idx), n++)
		{
			bi.GetBoundaryPoint(idx, &pt);
			pts.push_back(pt); // push at the BACK of the list

			if (ca.IsLimitPoint(pt))
				break;

			d = pt.sqDist(*lastIt);

			if (!bHaveEnoughtPts && d >= minSqDist)
			{
				bHaveEnoughtPts = true;

				if (d > maxSqDist)
					maxSqDist = d;
			}

			if (bHaveEnoughtPts && (n >= maxNumPts || d >= maxSqDist))
				break;

			// Make sure that we have enough points to define one arm, and if so
			// check that idx is within bnry pts assoc with current branch
			//if (n >= minNumPts && n >= maxNumPts)
			//	break;
		}
	}

	return FindCorner(pts, firstIt, lastIt, minSqDist,	maxSqDist,
		minCornerCosAngle, pRefPt);
}

/*!
	The problem is to walk along the boundary points. This cannot be done
	by walking along the skeletal points and following their spokes, as
	the endpoints of adjacent spokes need not be adjacent.
*/
void BoundaryCorner::FindCorner(const BoundaryIntervalWithGaps& bi0,
								const BoundaryIntervalWithGaps& bi1,
								const double& minCornerCosAngle,
								const sg::Point* pRefPt)
{
	BoundaryIntervalWithGaps::Index idx, br0Idx, br1Idx;
	BPList pts;
	BPIterator firstIt, lastIt;
	sg::Point pt;
	unsigned int n, maxNumPts;
	double d, minSqDist, maxSqDist;
	bool bHaveEnoughtPts;

	ASSERT(bi0.IsValid() && bi1.IsValid());

	//ASSERT(bi0.LastPoint() == bi1.FirstPoint());
	DBG_C_LOG("Searching for corners within " << bi0.FirstPoint() << ", "
		<< bi0.LastPoint() << ", "<< bi1.FirstPoint() << ", " << bi1.LastPoint())

	m_cosAngle = -1; // init to widest angle
	m_valid = false; // init to invalid corner

	// Add all pts in bi0 to the list
	if (!bi0.GetBoundaryPoints(&pts, bi0.First(), bi0.Last()))
		return;

	pts.pop_back();   // remove last point, because will be added again bellow

	// Add all pts in bi1 to the list
	if (!bi1.GetBoundaryPoints(&pts, bi1.First(), bi1.Last()))
		return;

	// We can now extend the chain of points to provide room for the sides
	// of the triangle defined inscribed at a corner. First, save the limits
	// of the interval where the corner must be in.
	ASSERT(!pts.empty());
	firstIt = pts.begin();
	lastIt  = pts.end();
	lastIt--;

	//minNumPts = 2; //(sz0 + sz1) * 0.25;
	//maxNumPts = sz0 + sz1;
	minSqDist = MIN_SQ_DISTANCE;

	// Make sure that max dist is >= min dist
	maxNumPts = pts.size();
	maxSqDist = maxNumPts * maxNumPts;

	// Add poins to the left
	bi0.GetBranchBndryLimits(&br0Idx, &br1Idx);

	bHaveEnoughtPts = false;

	for (idx = bi0.Prev(bi0.First()), n = 0; /* true */; bi0.MovePrev(idx), n++)
	{
		bi0.GetBoundaryPoint(idx, &pt);
		pts.push_front(pt);

		d = pt.sqDist(*firstIt);

		if (!bHaveEnoughtPts && d >= minSqDist)
		{
			bHaveEnoughtPts = true;

			if (d > maxSqDist)
				maxSqDist = d;
		}

		if (bHaveEnoughtPts && (n >= maxNumPts || d >= maxSqDist
			|| idx == br0Idx || idx == br1Idx))
			break;

		// Make sure that we have enough points to define one arm, and if so
		// check that idx is within bnry pts assoc with current branch
		//if (n >= minNumPts && (idx == br0Idx || idx == br1Idx || n >= maxNumPts))
		//	break;
	}

	// Add poins to the left
	bi1.GetBranchBndryLimits(&br0Idx, &br1Idx);

	bHaveEnoughtPts = false;

	for (idx = bi1.Next(bi1.Last()), n = 0; /* true */; bi1.MoveNext(idx), n++)
	{
		bi1.GetBoundaryPoint(idx, &pt);
		pts.push_back(pt);

		d = pt.sqDist(*lastIt);

		if (!bHaveEnoughtPts && d >= minSqDist)
		{
			bHaveEnoughtPts = true;

			if (d > maxSqDist)
				maxSqDist = d;
		}

		if (bHaveEnoughtPts && (n >= maxNumPts || d >= maxSqDist
			|| idx == br0Idx || idx == br1Idx))
			break;

		// Make sure that we have enough points to define one arm, and if so
		// check that idx is within bnry pts assoc with current branch
		//if (n >= minNumPts && (idx == br0Idx || idx == br1Idx || n >= maxNumPts))
		//	break;
	}

	FindCorner(pts, firstIt, lastIt, minSqDist, maxSqDist,
		minCornerCosAngle, pRefPt);
}

/*!
	Finds the point in 'pts' with smallest angle. If 'pRefPt' is given,
	only concave corners wrt to 'pRefPt' are considered.
*/
void BoundaryCorner::FindCorner(BPList& pts, BPIterator left, BPIterator right,
								const double& minSqDist, const double& maxSqDist,
								const double& minCCA, const sg::Point* pRefPt)
{
	BoundaryCorner bc;
	BPIterator it;

	m_cosAngle = -1; // init to widest angle
	m_valid = false; // init to invalid corner

	// See if there is a corner within the left margin [begin,left). Start from the
	// points closer to the 'left' limit. If there is a corner, remove all margin
	// points to its left, in order to avoid having a left "arm" that goes over it
	if (left != pts.begin())
	{
		for (it = left, --it; /* true */; --it)
		{
			if (left->sqDist(*it) >= MIN_SQ_DISTANCE)
			{
				bc.ComputeMinimumAngle(pts, it, MIN_SQ_DISTANCE, maxSqDist, minCCA);

				if (bc.m_cosAngle >= minCCA)
				{
					// Remove pts in the interval [begin(), it)
					pts.erase(pts.begin(), it);
					ASSERT(pts.begin() == it);
					break;
				}
			}

			if (it == pts.begin())
				break;
		}
	}

	// See if there is a corner within the left margin (right,end). Start from the
	// points closer to the 'left' limit. If there is a corner, remove all margin
	// points to its left, in order to avoid having a left "arm" that goes over it
	for (it = right, ++it; it != pts.end(); ++it)
	{
		if (right->sqDist(*it) >= MIN_SQ_DISTANCE)
		{
			bc.ComputeMinimumAngle(pts, it, MIN_SQ_DISTANCE, maxSqDist, minCCA);

			if (bc.m_cosAngle >= minCCA)
			{
				// Remove pts in the interval (it, end())
				it++;

				if (it != pts.end())
					pts.erase(it, pts.end());

				break;
			}
		}
	}

	if (pts.empty())
		return;

	//DBG_C_LOG("Searching for corners within " << *left << ", " << *right)

	// Search for the minimum angle (ie, maximum cos(angle)) within the
	// interval [First(), Last()]
	for (it = left; /* true */; it++)
	{
		bc.ComputeMinimumAngle(pts, it, minSqDist, maxSqDist, minCCA, pRefPt);

		if (bc.m_cosAngle >= this->m_cosAngle)
			*this = bc;

		if (it == right)
			break;

		ASSERT(it != pts.end());
	}
}

/*!
	@brief Compute the minimum angle and boundary point i by inscribing
	a triangle. Each arm of the triangle has a maximim length of sqrt(maxSqDist)
	and a minimum length of sqrt(s_dBndryCornerMinSqDist).

	@return fills the pt, angle, leftArm and rightArm fields of the
            BoundaryCorner structure given by the out parameter pCornerInfo.
*/
void BoundaryCorner::ComputeMinimumAngle(const BPList& pts, const BPIterator& it,
										 const double& minSqDist, const double& maxSqDist,
										 const double& minCornerCosAngle,
										 const sg::Point* pRefPt /* = NULL*/)
{
	std::list< std::pair<sg::Point, double> > rightPoints;

	double cosAlpha, p0Norm2, p0Norm;
	BPIterator left, right;
	sg::Point p0;

	m_cosAngle = -1;
	m_valid    = false;

	// Check that there are enough pts to form the corner's arms
	if (it == pts.begin() || it == pts.end())
		return;

	// Init the corner point
	m_pt = *it;

	// Init for left and right iterators to the origin, ie, the given point at 'it'
	left = right = it;

	// Find all points on the right that provide a valid "arm" endpoint and store them
	for (++right; right != pts.end(); ++right)
	{
		p0 = *right - m_pt;
		p0Norm2 = p0.sqNorm();

		if (p0Norm2 < minSqDist)
			continue;

		// If we have at least one point and the right arm
		// is too longer than the max length allowed, we break
		if (!pts.empty() && p0Norm2 > maxSqDist)
			break;

		rightPoints.push_back(std::make_pair(p0, sqrt(p0Norm2)));
	}

	// Find all points on the left that provide a valid "arm" endpoint and compute
	// the angle between the left and right arms. Save the maximum cos angle (ie, min angle)
	std::list< std::pair<sg::Point, double> >::const_iterator rightPtIt;
	bool bHaveEnoughInfo = false;

	for (--left; /* true */; --left)
	{
		p0 = *left - this->m_pt;
		p0Norm2 = p0.sqNorm();

		if (p0Norm2 >= minSqDist)
		{
			if (bHaveEnoughInfo && p0Norm2 > maxSqDist)
				break;

			bHaveEnoughInfo = true;

			p0Norm = sqrt(p0Norm2);

			for (rightPtIt = rightPoints.begin(); rightPtIt != rightPoints.end(); rightPtIt++)
			{
				cosAlpha = p0.dot(rightPtIt->first) / (p0Norm * rightPtIt->second);

				if (cosAlpha >= m_cosAngle &&
					IsConcaveCorner(pRefPt, m_pt, p0, rightPtIt->first))
				{
					m_cosAngle = cosAlpha;
					m_leftArm  = p0;
					m_rightArm = rightPtIt->first;
				}

				// Record the first (shortest) pair of arms that make the
				// corner a "valid" concave corner
				if (!m_valid && cosAlpha >= minCornerCosAngle &&
					IsConcaveCorner(pRefPt, m_pt, p0, rightPtIt->first))
				{
					m_minLeftArm  = p0;
					m_minRightArm = rightPtIt->first;
					m_valid = true;
				}
			}
		}

		if (left == pts.begin())
			break;
	}
}