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
 * $Log: BoundaryCorner.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __BOUNDARY_CORNER_H__
#define __BOUNDARY_CORNER_H__

#include "stdafx.h"

namespace dml {
/*!
	Define the first and last point of a boundary segment that can
	be visited when searching for a boundary corner.

	If no limit is specified, all boundary point are consider potential
	corners or corner arm's endpoints.
*/
struct CornerArea
{
	sg::Point m_first, m_last;
	bool m_bHasFirst, m_bHasLast;

	CornerArea()
	{
		m_bHasFirst = false;
		m_bHasLast = false;
	}

	void SetFirst(const sg::Point& pt)
	{
		m_first = pt;
		m_bHasFirst = true;
	}

	void SetLast(const sg::Point& pt)
	{
		m_last = pt;
		m_bHasLast = true;
	}

	bool IsLimitPoint(const sg::Point& pt) const
	{
		return ((m_bHasFirst && m_first == pt) || (m_bHasLast && m_last == pt));
	}
};

/*!
	Represents a boundary corner and the function to find it
	along a given boundary countour.

	The convexity/concavity of a corner can be evaluated with the function
	IsConcaveCorner().
*/
class BoundaryCorner
{
	//static const double s_dBndryCornerMinSqDist;

	typedef std::list<sg::Point> BPList; //<! Boundary Point List
	typedef BPList::iterator BPIterator; //<! Boundary Point Iterator

protected:

	void FindCorner(BPList& pts, BPIterator left, BPIterator right,
		const double& minSqDist, const double& maxSqDist,
		const double& minCCA, const sg::Point* pRefPt);

	void ComputeMinimumAngle(const BPList& pts, const BPIterator& it,
		const double& minSqDist, const double& maxSqDist,
		const double& minCornerCosAngle, const sg::Point* pRefPt = NULL);

public:
	double m_cosAngle;    //<! cosine of the minimum angle at 'pt'
	bool m_valid;         //<! states whether the corner is <= a given max angle

	sg::Point m_pt;           //<! corner point
	sg::Point m_leftArm;      //<! left arm that forms the minimum angle
	sg::Point m_rightArm;     //<! right arm that forms the minimum angle
	sg::Point m_minLeftArm;   //<! shortest left arm that forms a valid concave corner
	sg::Point m_minRightArm;  //<! shortest right arm that forms a valid concave corner

	BoundaryCorner()
	{
		Clear();
	}

	/*!
		Sets 'cosAngle' to HUGE_VAL (ie, MAX double) and 'm_valid' to false.
		The cos() funtions has {[-1,1]} as its codomain, and so HUGE_VAL is
		always outside such a set.
	*/
	void Clear()
	{
		m_cosAngle = HUGE_VAL;
		m_valid = false;
	}

	//! Retrievs the endpoint of the left arm that forms the corner
	sg::Point LeftEndpoint() const
	{
		return sg::Point(m_pt.x + m_leftArm.x, m_pt.y + m_leftArm.y);
	}

	//! Retrievs the endpoint of the left arm that forms the corner
	sg::Point RightEndpoint() const
	{
		return sg::Point(m_pt.x + m_rightArm.x, m_pt.y + m_rightArm.y);
	}

	//! Returns true iff 'm_cosAngle' is equal to HUGE_VAL
	bool IsNull() const
	{
		return (m_cosAngle == HUGE_VAL);
	}

	//! Returns true iff 'm_valid' is true
	bool IsValid() const
	{
		return m_valid;
	}

	void FindCorner(const BoundaryIntervalWithGaps& bi,
		const double& minCornerCosAngle, const CornerArea& ca,
		const sg::Point* pRefPt = NULL);

	void FindCorner(const BoundaryIntervalWithGaps& bi0,
		const BoundaryIntervalWithGaps& bi1,
		const double& minCornerCosAngle, const sg::Point* pRefPt);

	friend std::ostream& operator<<(std::ostream& os, const BoundaryCorner& bc)
	{
		os << "(" << bc.m_pt << ", " << bc.m_cosAngle << ", " << bc.m_valid << ")"
			<< "[" << bc.m_leftArm << ", " << bc.m_rightArm << "]";

		return os;
	}

	/*!
		Checks that the given boundary corner is concave wrt the
		given point skeletal point 'skelPt' using the left and right
		arm information.

		\image html IsConcaveCorner.gif
	*/
	static bool IsConcaveCorner(const sg::Point& skelPt, const sg::Point& corPt,
		const sg::Point& leftArmPt, const sg::Point& rightArmPt)
	{
		sg::Point intPt;

		int rv = FindLineSegmentIntersection(skelPt, corPt,
			leftArmPt + corPt, rightArmPt + corPt, &intPt);

		// rv = 2 means that the segments overlap over several points
		ASSERT(rv != 2);

		// rv == 4 means that the second segments is intersected by the first line,
		// which generaly is the case of concave corners, but not always (see fig (c))
		// distance from intersection pt to corner must be smaller than to skel pt
		if (rv == 4)
			return (intPt.sqDist(corPt) < intPt.sqDist(skelPt));

		//  rv = 1 means that both segments intersect, the corner is NOT concave
		return (rv != 1);
	}

	/*!
		Checks that the given boundary corner 'bc' is concave wrt the
		given point skeletal point 'skelPt'
	*/
	bool IsConcaveCorner(const sg::Point& SkelPt) const
	{
		return IsConcaveCorner(SkelPt, m_pt, m_leftArm, m_rightArm);
	}

	/*!
		If pSkelPt is not NULL, it checks that the given boundary corner
		is concave wrt the given point skeletal point 'skelPt'. Otherwise,
		it returns true.
	*/
	static bool IsConcaveCorner(const sg::Point* pSkelPt, const sg::Point& corPt,
		const sg::Point& leftArmPt, const sg::Point& rightArmPt)
	{
		if (pSkelPt)
			return IsConcaveCorner(*pSkelPt, corPt, leftArmPt, rightArmPt);

		return true;
	}
};
} // namespace dml

#endif //__BOUNDARY_CORNER_H__
