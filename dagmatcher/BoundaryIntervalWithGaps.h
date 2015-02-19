/**------------------------------------------------------------------------
 * @file
 *
 * Copyright 2007 by the University of Toronto (UT).
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
#ifndef __BOUNDARY_INTERVAL_WITH_GAPS_H__
#define __BOUNDARY_INTERVAL_WITH_GAPS_H__

#include "SkeletalGraph.h"
#include "BoundaryInterval.h"
#include "KDTree.h"
#include "BoundaryPointFinder.h"

namespace dml {
/*!
	@brief Objects of this class are used for walking around a closed countour
	that *may* have gaps in it. In contrast, BoundaryInterval objects can only
	traverse a countour without gaps.

	The gaps are defined in the list of BoundarySegment objects associated with
	each skeletal branch. Every BoundarySegment object, bs, has an associated list of
	pairs of "original" boundary points and "gap" boundary points segments. These segments
	are ordered wrt ther associated skeletal points, ie, from the first branch point to
	the last. A skeletal point is associated with two boundary points, one for each side.
	Each branch has a pair of equal sized arrays of skeletal points	and an boundary
	info objects.

	Gap boundary points can be seen as "shortcuts" connecting origial bounadry points. In
	order to differenciate between original boundary points and gap boundary points, a
	"generic" boundary point is idexed by a pair of integers (index, subindex). If 'index'
	is greated or equal to zero, then the boundary point belongs to the "original" contour
	of the shape. Otherwise, the point is a gap point.

	When defining an interval of boundary points in the functions SetBoundaryIntervals() and
	UpdateBoundarySubintervals() [now deprecated], the function GetBoundaryIndex() must
	be used to convert an index pair (index, subindex) to a single integer index over
	the original boundary. That is, regardless of whether (index, subindex) refer to
	an "original" or a "gap" point, the GetBoundaryIndex() always returns an index to
	and original point, which in the case of an input gap point will correspond to its
	closest original boundary point. This function is necessary becasue the ordering
	direction of intervals can only be defined wrt original boundary points.

	In order to simplify the iteration over boundary points, each "generic" boundary point
	is indexed by an object of type Index. The boundary points are assumed to represent
	a closed contour, and so the successor of the last boundary point is the first
	boundary point, and the predecessor of the first boundary point is the last boundary
	point. The functions Pred() and Succ() account for this (this is true also for the
	correponding member functions of the BoundayInterval class).

	Finally, an object of type Index has an associate direction of iteration that is either
	foreward or backwards. When it is set to foreward, the successor of a given index
	correponds to the nex boundary point in the direction of the skeletal point of the
	input branch. In turn, a boundary interval is associated with a ordering direction that
	specifies how the boundary points should be visited such that the successor of a point
	is closer to the successor of its associated skeletal branch point. If the natural
	integer successor agrees with the boundary point successor (ie, index++ is the successor),
	then the intevral is said to have a clockwise ordering.
*/
class BoundaryIntervalWithGaps
{
public:
	/*!
		@brief Index over boundary points (bndryPointIdx >= 0)
		or gap points (bndryPointIdx <= 0).
	*/
	struct Index
	{
		char branchSide;
		int bndrySegmentIdx, bndryPointIdx;
		bool moveForward;

		Index() { branchSide = 0; moveForward = true; }

		bool IsNull() const
		{
			return (branchSide == 0);
		}

		//! Sets the index to a standard and impossible value
		void SetNull()
		{
			branchSide = 0;
			bndrySegmentIdx = -1;
			bndryPointIdx = -1;
			moveForward = true;
		}

		void Set(char bs, int bsi, int bpi, bool mf = true)
		{
			branchSide = bs;
			bndrySegmentIdx = bsi;
			bndryPointIdx = bpi;
			moveForward = mf;
		}

		void Set(const sg::BoundaryInfo& bi, char side, bool mf = true)
		{
			const sg::BoundaryPoint& bp = bi[side];

			Set(side, bp.subindex, bp.index, mf);
		}

		bool operator==(const Index& rhs) const
		{
			return (bndryPointIdx == rhs.bndryPointIdx &&
				bndrySegmentIdx == rhs.bndrySegmentIdx &&
				branchSide == rhs.branchSide);
		}

		bool operator!=(const Index& rhs) const
		{
			return !operator==(rhs);
		}
	};

protected:
	const SkelBranch* m_pBranch;           //!< the interval is given wrt a branch

	const sg::BoundarySegmentLists& m_bsl; //!< boundary segments of the branch
	KDTree& m_bndryPts;                    //!< all points in the shape's countour

	BoundaryInterval m_bndryIntervalSide1; //!< all points of the branch interval on side 1
	BoundaryInterval m_bndryIntervalSide2; //!< all points of the branch interval on side 2
	bool m_moveClockWise[2];               //!< the CW or CCW order of bndry pts on sides 1 and 2

	Index m_first, m_last;                 //!< first and last points of the interval
	BoundaryInterval m_bndrySubinterval;   //!< Subinterval of boundary points [m_first, m_last]

	bool m_bIsValidInterval;

protected:
	//! Moves the index to the first point in segment 'newBndrySegIdx'
	void MoveIdxToFirstPointInSegment(Index& idx, int newBndrySegIdx) const
	{
		const sg::BoundarySegments& bss = m_bsl[idx.branchSide];
		const char side = idx.branchSide;

		ASSERT(newBndrySegIdx >= 0 && newBndrySegIdx <= (int) bss.size());

		idx.bndrySegmentIdx = newBndrySegIdx;

		if (newBndrySegIdx == bss.size()) // in between end points
		{
			int newBndryIdx = NextBndryPoint(side, GetLastIdxInBranch(side));

			MoveInBetweenSides(idx, newBndryIdx);
		}
		else // its a valid segment index
		{
			ASSERT(bss[newBndrySegIdx].start >= 0 || bss[newBndrySegIdx].start == -1);

			// if '.start' == -1, it's first gap pt, else it's first bndry pt
			idx.bndryPointIdx = bss[newBndrySegIdx].start;
		}
	}

	//! Moves the index to the last point in segment 'newBndrySegIdx'
	void MoveIdxToLastPointInSegment(Index& idx, int newBndrySegIdx) const
	{
		const sg::BoundarySegments& bss = m_bsl[idx.branchSide];
		const char side = idx.branchSide;

		ASSERT(newBndrySegIdx == -1 ||
			(newBndrySegIdx >= 0 && newBndrySegIdx < (int) bss.size()));

		idx.bndrySegmentIdx = newBndrySegIdx;

		if (newBndrySegIdx == -1) // in between start points
		{
			int newBndryIdx = PrevBndryPoint(side, GetFirstIdxInBranch(side));

			MoveInBetweenSides(idx, newBndryIdx);
		}
		else // its a valid segment index
		{
			const sg::BoundarySegment& bs = bss[newBndrySegIdx];
			const int sz = (int)bs.gapPts.size();

			idx.bndryPointIdx = (bs.gapPts.empty()) ? bs.end : -sz;
		}
	}

	/*!
		@brief Assigns 'newBndryIdx' as the new index only if it is located
		outside the limits of both branch sides. If 'newBndryIdx' falls within
		the limits of either branch side, the index idx is set to the appropriate
		within-branch endpoint index.

		When the index goes from one side to another, the walking direction of
		the index is reversed, so that the index can continue cycling and eventually
		reach it starting point.
	*/
	void MoveInBetweenSides(Index& idx, int newBndryIdx) const
	{
		char currSide = idx.branchSide;
		char otherSide = GetOtherSide(idx.branchSide);

		// There are only two valid "in-between-endpoints" indices...
		//ASSERT(idx.bndrySegmentIdx == -1 ||
		//	idx.bndrySegmentIdx == m_bsl[currSide][idx.bndrySegmentIdx].gapPts.size());

		// ..and they determine whether we look at the first or the last branch point
		unsigned int skelEndpt = (idx.bndrySegmentIdx == -1) ? 0 : m_pBranch->size() - 1;

		// See if it entered in the limits of either side of the branch...
		if (GetBndryInt(otherSide).Includes(newBndryIdx)) // it did (other side)
			idx.Set(m_pBranch->getBoundaryInfo(skelEndpt), otherSide, !idx.moveForward);
		else if (GetBndryInt(currSide).Includes(newBndryIdx)) // it did (same side)
			idx.Set(m_pBranch->getBoundaryInfo(skelEndpt), currSide, idx.moveForward);
		else // it didn't
			idx.bndryPointIdx = newBndryIdx;
	}

	//! Gets the opposite side to the given side
	/*char GetOtherSide(char side) const
	{
		ASSERT(side == '1' || side == '2');

		return (side == '1') ? '2' : '1';
	}*/

	//! Gets the boundary interval associated with the given side
	BoundaryInterval& GetBndryInt(char side)
	{
		ASSERT(side == '1' || side == '2');

		return (side == '1') ? m_bndryIntervalSide1 : m_bndryIntervalSide2;
	}

	//! Gets the boundary interval associated with the given side
	const BoundaryInterval& GetBndryInt(char side) const
	{
		ASSERT(side == '1' || side == '2');

		return (side == '1') ? m_bndryIntervalSide1 : m_bndryIntervalSide2;
	}

	//! Gets the closest boundary point to the given point
	int GetClosestBoundaryIndex(const sg::Point& pt)
	{
		double epsilon = 0;
		int n = m_bndryPts.RangeSearch(pt.x, pt.y, 2, &epsilon, 2, 1);

		ASSERT(n > 0);
		return m_bndryPts.GetNNIndex(0);
	}

	//! Find the index of the boundary point associated with the given indices
	int GetBoundaryIndex(char side, int subindex, int index)
	{
		if (index >= 0) // we already have it
			return index;
		else // it's a gap point, find closest boundary point to it
			return GetClosestBoundaryIndex(m_bsl[side][subindex][-index - 1]);
	}

	int GetBoundaryIndex(const sg::BoundaryInfo& bi, char side)
	{
		return GetBoundaryIndex(side, bi[side].subindex, bi[side].index);
	}

	/*!
		@brief Gets the last index in the boundary point interval defined by
		the boundary segment bs.
	*/
	int GetLastIdxInSegment(const sg::BoundarySegment& bs) const
	{
		ASSERT(bs.end >= 0);

		return bs.end;
	}

	/*!
		@brief Gets the first index in the boundary point interval defined by
		the boundary segment bs.
	*/
	int GetFirstIdxInSegment(const sg::BoundarySegment& bs) const
	{
		ASSERT(bs.start >= 0);

		return bs.start;
	}

	/*!
		@brief Gets the last boundary point of the branch, while accounting for
		the clock wise and counter clock wise ordering of points.
	*/
	int GetLastIdxInBranch(char side) const
	{
		if (m_moveClockWise[side - '1'])
			return GetBndryInt(side).Last();
		else
			return GetBndryInt(side).First();
	}

	/*!
		@brief Gets the first boundary point of the branch, while accounting for
		the clock wise and counter clock wise ordering of points.
	*/
	int GetFirstIdxInBranch(char side) const
	{
		if (m_moveClockWise[side - '1'])
			return GetBndryInt(side).First();
		else
			return GetBndryInt(side).Last();
	}

	bool SetIntervalDirectionWithOuterPoint(char side, int first,
		int last, int outPtIdx);

	bool SetIntervalDirectionWithInnerPoint(char side, int first,
		int last, int inBndryPtIdx);

	bool SetIntervalDirectionUsingContext(char side, int first, int last);

	bool SetIntervalDirection(const sg::BoundaryInfoList& bil, char side);

	/*!
		Forms valid boundary intervals for all the points on each branch side
		and determines whether the boundary points, on eithr side of the branch,
		should be visited clock-wise or counter clock-wise, when moving
		from the first branch point to the last branch point.
	*/
	void SetBoundaryIntervals()
	{
		const sg::BoundaryInfoList& bil = m_pBranch->getBoundaryInfoList();

		bool bValid1 = SetIntervalDirection(bil, '1');
		bool bValid2 = SetIntervalDirection(bil, '2');

		m_bIsValidInterval = (bValid1 && bValid2);
	}

	//! Sets the boundary subinterval with the information in m_first and m_last
	void UpdateBoundarySubinterval()
	{
		ASSERT(m_first.branchSide == m_last.branchSide);

		const char side = m_first.branchSide;
		const int first = GetBoundaryIndex(side, m_first.bndrySegmentIdx, m_first.bndryPointIdx);
		const int last = GetBoundaryIndex(side, m_last.bndrySegmentIdx, m_last.bndryPointIdx);

		m_bndrySubinterval.Set(m_bndryPts.Size(), first, last);

		if ((m_moveClockWise[side - '1'] && m_bndrySubinterval.IsGreaterThan(first, last)) ||
			(!m_moveClockWise[side - '1'] && m_bndrySubinterval.IsSmallerThan(first, last)))
		{
			m_bndrySubinterval.Swap();
		}

		ASSERT(m_bndrySubinterval.Size() <= GetBndryInt(side).Size());
	}

	//! Gets the next boundary point after i in the order defined by m_moveClockWise
	int NextBndryPoint(char side, int i) const
	{
		return (m_moveClockWise[side - '1']) ?
			GetBndryInt(side).Succ(i) : GetBndryInt(side).Pred(i);
	}

	//! Moves to the next boundary point in the order defined by m_moveClockWise
	void MoveToNextBndryPoint(Index& idx) const
	{
		idx.bndryPointIdx = NextBndryPoint(idx.branchSide, idx.bndryPointIdx);
	}

	//! Gets the previous boundary point after i in the order defined by m_moveClockWise
	int PrevBndryPoint(char side, int i) const
	{
		return (m_moveClockWise[side - '1']) ?
			GetBndryInt(side).Pred(i) : GetBndryInt(side).Succ(i);
	}

	//! Moves to the previous boundary point in the order defined by m_moveClockWise
	void MoveToPrevBndryPoint(Index& idx) const
	{
		idx.bndryPointIdx = PrevBndryPoint(idx.branchSide, idx.bndryPointIdx);
	}

	void Succ(Index& idx) const;
	void Pred(Index& idx) const;

public:
	BoundaryIntervalWithGaps(const SkelBranch* pBranch, KDTree& kdt)
		: m_bsl(pBranch->getBoundarySegments()), m_bndryPts(kdt)
	{
		m_pBranch = pBranch;

		SetBoundaryIntervals();
	}

	BoundaryIntervalWithGaps(const BoundaryIntervalWithGaps& rhs)
		: m_bsl(rhs.m_bsl), m_bndryPts(rhs.m_bndryPts)
	{
		m_pBranch            = rhs.m_pBranch;
		m_bndryIntervalSide1 = rhs.m_bndryIntervalSide1;
		m_bndryIntervalSide2 = rhs.m_bndryIntervalSide2;
		m_moveClockWise[0]   = rhs.m_moveClockWise[0];
		m_moveClockWise[1]   = rhs.m_moveClockWise[1];
		m_first              = rhs.m_first;
		m_last               = rhs.m_last;
		m_bndrySubinterval   = rhs.m_bndrySubinterval;
		m_bIsValidInterval   = rhs.m_bIsValidInterval;
	}

	bool IsValid() const
	{
		return m_bIsValidInterval;
	}

	/*!
		Sets the boundary interval from a pair of branch point indices i <= j

		If the boundary points are to be visited from the one assciated with
		skeletal point j to the one associated with skeletal point i, ie, inverted
		wrt the branch points, then bInverted must be set to true.
	*/
	void SetLimits(unsigned int i, unsigned int j, char side, bool bInverted = false)
	{
		ASSERT(i <= j);

		// Set both indices and assume a forward motion from first to last
		// which does not imply a foward motion wrt bndry point indices!
		if (bInverted)
		{
			m_first.Set(m_pBranch->getBoundaryInfo(j), side, false);
			m_last.Set(m_pBranch->getBoundaryInfo(i), side, false);
		}
		else
		{
			m_first.Set(m_pBranch->getBoundaryInfo(i), side, true);
			m_last.Set(m_pBranch->getBoundaryInfo(j), side, true);
		}

		//UpdateBoundarySubinterval();
	}

	//! Sets the boundary interval from a pair of Index objects
	void SetLimits(const Index& idx0, const Index& idx1)
	{
		ASSERT(idx0.branchSide == idx1.branchSide);

		m_first = idx0;
		m_last = idx1;

		//UpdateBoundarySubinterval();
	}

	//! Get the indices of the bndry pts referred by the first and last branch pts
	void GetBranchBndryLimits(Index* p0, Index* p1) const
	{
		ASSERT(m_first.branchSide == m_last.branchSide);

		p0->Set(m_pBranch->getFirstBndryInfo(), m_first.branchSide);
		p1->Set(m_pBranch->getLastBndryInfo(), m_last.branchSide);
	}

	//! Sets 'pt' to the boundary point referenced by 'idx'
	void GetBoundaryPoint(const Index& idx, sg::Point* pt) const
	{
		if (idx.bndryPointIdx >= 0)
			m_bndryPts.GetDataPoint(idx.bndryPointIdx, pt->x, pt->y);
		else
			*pt = m_bsl[idx.branchSide][idx.bndrySegmentIdx][-idx.bndryPointIdx - 1];
	}

	//! Less efficient way of retrieving the point referenced by 'idx'
	sg::Point GetBoundaryPoint(const Index& idx) const
	{
		sg::Point pt;

		GetBoundaryPoint(idx, &pt);

		return pt;
	}

	//! Gets the first boundary point index
	const Index& First() const { return m_first; }

	//! Gets the last boundary point index
	const Index& Last() const { return m_last; }

	//! Shortcut for getting the first boundary point
	sg::Point FirstPoint() const { return GetBoundaryPoint(First()); }

	//! Shortcut for getting the last boundary point
	sg::Point LastPoint() const { return GetBoundaryPoint(Last()); }

	//! Updates 'idx' with its successor
	void MoveNext(Index& idx) const
	{
		(idx.moveForward) ? Succ(idx) : Pred(idx);
	}

	//! Updates 'idx' with its predecessor
	void MovePrev(Index& idx) const
	{
		(idx.moveForward) ? Pred(idx) : Succ(idx);
	}

	//! Retrieves the successor of 'currIdx'
	Index Next(const Index& currIdx) const
	{
		Index idx = currIdx; // clone the index first

		MoveNext(idx);

		return idx;
	}

	//! Retrieves the predecessor of 'currIdx'
	Index Prev(const Index& currIdx) const
	{
		Index idx = currIdx; // clone the index first

		MovePrev(idx);

		return idx;
	}

	//! Returns true if idx is in between the limits and false otherwise
	bool Includes(const Index& idx) const
	{
		if (idx.bndryPointIdx >= 0) // it's a boundary point
		{
			m_bndrySubinterval.Includes(idx.bndryPointIdx);
		}
		else // it's a gap point
		{
			ASSERT(idx.bndrySegmentIdx >= 0);

			if (idx.bndrySegmentIdx > m_first.bndrySegmentIdx &&
				idx.bndrySegmentIdx < m_last.bndrySegmentIdx)
			{
				return true;
			}
			else if (idx.bndrySegmentIdx == m_first.bndrySegmentIdx)
			{
				return (idx.bndryPointIdx <= m_first.bndryPointIdx); //(signs are inverted)
			}
			else if (idx.bndrySegmentIdx == m_last.bndrySegmentIdx)
			{
				return (idx.bndryPointIdx >= m_last.bndryPointIdx); //(signs are inverted)
			}
			else
			{
				return false;
			}
		}
	}

	/*!
		Adds the boundary points in the specified interval, [First(), Last()]
		by default, to the given list of points.
	*/
	bool GetBoundaryPoints(std::list<sg::Point>* pPts,
		const Index& first, const Index& last) const
	{
		sg::Point pt;

		const unsigned int maxNumPts = int(m_bndryPts.Size() / 2.0);
		int unsigned numPts = 0;

		for (Index idx = first; ; MoveNext(idx))
		{
			GetBoundaryPoint(idx, &pt);
			pPts->push_back(pt);

			if (idx == last)
				return true;

			if (++numPts >= maxNumPts)
			{
				WARNING(numPts >= maxNumPts, "There are too many boundary points");
				return false;
			}
		}

		return false;
	}

	/*!
		Counts the number of points in the specified interval, [First(), Last()]
		by default.
	*/
	unsigned int BoundaryPointCount(const Index& first,
		const Index& last) const
	{
		unsigned int n = 1;

		//numPts = m_bndryPts.Size();
		for (Index idx = first; idx != last; MoveNext(idx))
		{
			n++;
		}

		return n;
	}
};
} //namespace dml

#endif //__BOUNDARY_INTERVAL_WITH_GAPS_H__
