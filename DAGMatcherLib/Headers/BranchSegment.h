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

#ifndef __BRANCH_SEGMENT_H__
#define __BRANCH_SEGMENT_H__

#include "stdafx.h"

namespace dml {
class LigatureSegment;

/*!
	@brief Represents a branch segment in a SkeletalGraph. In addition,
	it allows for efficiently inverting the direction of the flux points
	in the segment.

	The inversion of the flux points is a common operation needed when
	splicing mutiple segments. All segments in a list of segments must
	have their flux point flowing in the same direction, and so the inversion
	of some segments is necessary.

	An empty segment is represented by m_first == m_pBranch->size() or one
	with m_pBranch == NULL.

	The poins on the branch m_pBranch that are outside the segment are said to
	be on the left or on the right margin. See operator[]() for details.

	NOTE: the sides of the boundary information of an inverted branch segment
	are inverted when using operator()() because the change of flow direction means
	that tangents were computed starting at opposite points.
*/
class BranchSegment
{
	friend class BranchSegmentList;
	friend class BranchWalkInfo;
	friend struct CompareBranchSegment;

protected:
	SkelPtIndex m_first, m_last;
	bool m_bInverted;                  //!< needed for splicing of flux points

public:
	SkelBranch* m_pBranch;

	//! Initializes branch pointer to NULL
	BranchSegment() { m_pBranch = NULL; m_bInverted = false; }

	BranchSegment(SkelBranch* pBranch, SkelPtIndex first, SkelPtIndex last, bool inverted = false)
	{
		m_pBranch  = pBranch;
		m_first    = first;
		m_last     = last;
		m_bInverted = inverted;
	}

	BranchSegment(SkelBranch* pBranch)
	{
		ASSERT(pBranch->size() > 0);

		m_pBranch  = pBranch;
		m_first    = 0;
		m_last     = pBranch->size() - 1;
		m_bInverted = false;
	}

	void SetBranchSegment(SkelBranch* pBranch, SkelPtIndex first, SkelPtIndex last, bool inverted = false)
	{
		m_pBranch  = pBranch;
		m_first    = first;
		m_last     = last;
		m_bInverted = inverted;
	}

	void SetLimits(SkelPtIndex first, SkelPtIndex last)
	{
		ASSERT(first <= last);

		ASSERT((m_bInverted && first > 0  && last <= m_pBranch->size()) ||
			  (!m_bInverted && first >= 0 && last  < m_pBranch->size()));

		m_first = first;
		m_last  = last;
	}

	void SetEmpty()
	{
		if (m_pBranch)
			m_first = m_pBranch->size();
	}

	//! Tells whether the "natural" flow of the branch is inverted
	bool IsInverted() const { return m_bInverted; }

	/*!
		Iterates from left-to-right or right-to-left, depending on the value of 'inverted'.
		From the left the indices are in [0, size-1] and from the right they are in [1, size].
		Usage:
              BranchSegment bs;

		      // iterating across points INSIDE the segment:

		      for (SkelPtIndex i = bs.First(); i <= bs.Last(); i++)
		      {
		         ...bs[i]...
		      }

	          // iterating across points OUTSIDE the segment (eg, left margin):

		      for (SkelPtIndex i = bs.LeftMarginFirst(); i < bs.LeftMarginSize(); i++)
		      {
		         ...bs[i]...
		      }
	*/
	const sg::FluxPoint& operator[](SkelPtIndex i) const
	{
		ASSERT((m_bInverted && i > 0 && i <= m_pBranch->size()) ||
			  (!m_bInverted && i >= 0 && i < m_pBranch->size()));

		return m_pBranch->getFluxPoint((m_bInverted) ? (m_pBranch->size() - i) : i);
	}

	void PushBackPtCoords(SmartArray<POINT>& pts) const
	{
		POINT pt;

		for (SkelPtIndex i = First(); i <= Last(); i++)
		{
			pts.AddTail(AssignPtCoord(pt, operator[](i).p));
		}
	}

	/*!
		Same as operator[], but returning a BoundaryInfo reference.
		Note: If the segment is "inverted", the boundary side must be swapped
		and the cumulative axis and bndry distances must be inverted too.
	*/
	sg::BoundaryInfo operator()(SkelPtIndex i) const
	{
		if (m_bInverted)
		{
			const sg::BoundaryInfo& bN = m_pBranch->getLastBndryInfo();
			sg::BoundaryInfo bi;

			bi = m_pBranch->getBoundaryInfo(m_pBranch->size() - i);

			bi.cumAxisDist         = bN.cumAxisDist - bi.cumAxisDist;
			bi.first.cumBndryDist  = bN.first.cumBndryDist - bi.first.cumBndryDist;
			bi.second.cumBndryDist = bN.second.cumBndryDist - bi.second.cumBndryDist;

			ASSERT(bi.cumAxisDist >= 0 && bi.first.cumBndryDist >= 0 && bi.second.cumBndryDist >= 0);

			bi.SwapSides(); // and finally, we swap the sides

			return bi;
		}
		else
			return m_pBranch->getBoundaryInfo(i);
	}

	//! Returns first point from the left or the right, depending on the value of 'inverted'
	SkelPtIndex First() const { return (m_bInverted) ? (m_pBranch->size() - m_last) : m_first; }

	//! Returns last point from the left or the right, depending on the value of 'inverted'
	SkelPtIndex Last() const  { return (m_bInverted) ? (m_pBranch->size() - m_first) : m_last; }

	//! Returns the first point of the branch instead of the segment's first
	SkelPtIndex BranchFirst() const { return (m_bInverted) ? 1 : 0; }

	//! Returns the last point of the branch instead of the segment's last
	SkelPtIndex BranchLast() const  { return (m_bInverted) ? m_pBranch->size() : m_pBranch->size() - 1; }

	//! Returs the number of points within the interval (checks valid branch size)
	SkelPtIndex Size() const
	{
		if (m_pBranch == NULL)
			return 0;

		ASSERT(m_last >= m_first || m_first == m_pBranch->size());

		return (m_first == m_pBranch->size()) ? 0 : (m_last + 1 - m_first);
	}

	//! Returs the number of points by subtracting the last and first indices (no checks)
	SkelPtIndex SizeFromIndices() const
	{
		return (m_last + 1 - m_first);
	}

	//! Returns the size of the branch instead of the segment's size
	SkelPtIndex BranchSize() const  { return m_pBranch->size(); }

	//! Returns the first flux point in the inteval
	SkelPt FirstPoint() const        { return operator[](First()); }

	//! Returns the first flux point coordinate in the inteval
	SkelPtCoord FirstPtCoord() const { return FirstPoint().p; }

	//! Returns the last flux point in the inteval
	SkelPt LastPoint() const        { return operator[](Last()); }

	//! Returns the last flux point coordinate in the inteval
	SkelPtCoord LastPtCoord() const { return LastPoint().p; }

	//! Returns the x-y coordinate of the i'th skeletal point
	SkelPtCoord GetPtCoord(SkelPtIndex i) const
	{
		return operator[](i).p;
	}

	/*!
		Returns the x-y coordinate of the boundary point mapped
	    by the spoke on side 'side' of the i'th skeletal point
	*/
	SkelPtCoord GetBoundaryPt(SkelPtIndex i, char side) const
	{
		return operator()(i)[side].pt;
	}

	//! Returns the joint pt associated with the first branch point
	const SkelJoint* FirstJoint() const
	{
		return (m_bInverted) ? GetLastJoint(m_pBranch) : GetFirstJoint(m_pBranch);
	}

	//! Returns the joint pt associated with the last branch point
	const SkelJoint* LastJoint() const
	{
		return (m_bInverted) ? GetFirstJoint(m_pBranch) : GetLastJoint(m_pBranch);
	}

	//! Returns the degree of the first endpoint of the branch
	unsigned int BranchFirstPtDegree() const { return FirstJoint()->degree(); }

	//! Returns the degree of the last endpoint of the branch
	unsigned int BranchLastPtDegree() const  { return LastJoint()->degree(); }

	//! First point of the left margin. Undefined if segment is empty.
	unsigned int LeftMarginFirst() const { return (m_bInverted) ? 1 : 0; }

	//! First point of the right margin. Undefined if segment is empty.
	unsigned int RightMarginFirst() const { return Last() + 1; }

	//! Size of the left margin. Undefined if segment is empty.
	unsigned int LeftMarginSize() const { return (m_bInverted) ? (m_pBranch->size() - m_last - 1) : m_first; }

	//! Size of the right margin. Undefined if segment is empty.
	unsigned int RightMarginSize() const { return (m_bInverted) ? m_first : (m_pBranch->size() - m_last - 1); }

	//! Returns true if the segment is empty and false otherwise
	bool IsEmpty() const { return Size() == 0; }

	//! Returns true iff the segment has the same size than the branch
	bool IsWholeBranch() const { return Size() == m_pBranch->size(); }

	//! Returns true if branch point index 'i' is inside the segment and false otherwise
	bool IsInside(SkelPtIndex i) const
	{
		ASSERT(i >= 0 && i < BranchSize());

		if (IsEmpty())
			return false;

		return i >= m_first && i <= m_last;
	}

	void ComputeEndpointTangent(bool bFirstPt, POINT* p0, POINT* p1) const;
	void ComputeTangent(SkelPtIndex ptIdx, POINT* p0, POINT* p1) const;
	double ComputeArea(bool bSubtractAreaFirstPt, bool bSubtractAreaLastPt) const;

	//! Srinks a branch segment so as to ignore the ligature interval.
	void SubtractLigatureInterval(const LigatureSegment& ls);

	bool IsCornerSegment(bool bForwardDir);

	//! Finds the closest point in the segment to the given point
	SkelPtIndex FindClosestPoint(const sg::Point& pt) const
	{
		double minSqDist = -1, sqDist;
		SkelPtIndex minIdx;

		ASSERT(!IsEmpty());

		for (SkelPtIndex i = First(); i <= Last(); i++)
		{
			sqDist = operator[](i).p.sqDist(pt);

			if (minSqDist == -1 || sqDist < minSqDist)
			{
				minSqDist = sqDist;
				minIdx = i;
			}
		}

		return minIdx;
	}

	//! Merges two adjacent segments such that the resulting segment is larger
	void ExtendSegment(const BranchSegment& bs)
	{
		ASSERT(!IsInverted());

		if (m_last + 1 == bs.m_first)
		{
			m_last = bs.m_last;
		}
		else if (m_first == bs.m_last + 1)
		{
			m_first = bs.m_first;
		}
		else if (m_first == bs.m_first && m_last == bs.m_last)
		{
			// there nothing to do
			WARNING(true, "branch segments are equal");
		}
		else
		{
			WARNING(true, "branch segments are not adjacent");
			//ASSERT(false);
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const BranchSegment& bs)
	{
		return os << PRN_BRANCH_PTR(bs.m_pBranch)
			<< ":[" << bs.m_first << "--" << bs.m_last << "]";
	}
};

//! Comparison function for ligature segments
struct CompareBranchSegment : std::binary_function<BranchSegment, BranchSegment, bool>
{
	bool operator()(const BranchSegment& bs0, const BranchSegment& bs1) const
	{
		if (bs0.m_first < bs1.m_first)
			return true;
		else if (bs0.m_first > bs1.m_first)
			return false;

		return (bs0.m_last < bs1.m_last);
	}
};
} // namespace dml

#endif //__BRANCH_SEGMENT_H__
