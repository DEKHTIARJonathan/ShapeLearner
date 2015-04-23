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
#ifndef __BOUNDARY_INTERVAL_H__
#define __BOUNDARY_INTERVAL_H__

#include <list>
#include "SkeletalGraph.h"

namespace dml {
/*!
	@brief Defines an interval of the boundary curve associated
	with a DDSGraph.

	The successor direction is always in increasing order. So, for example,
	for a boundary of length 200, the interval [10, 100] defines the points
	{10, 11, ..., 99, 100}, while the interval [100, 10] defines the point
	{100, 101, ..., 199, 0, 1, ..., 10}.

	Then, the interval is defined such that the "first" point can reach the "last"
	point by ONLY visiting interval ("inside") point via multiple calls to the successor
	operation. Similarly, the predecessor operation would connect the first and last
	point without visiting ANY interval point.

	@note Use IsGreaterThan() and IsSmallerThan() to compare indices, or to iterate
	through the enttire interval, do

	for (int i = bndryInt.First(); ; i = bndryInt.Succ(i))
	{
		//...
		if (i == bndryInt.Last())
			break;
	}
*/
class BoundaryInterval
{
	int length, start, end;

public:
	BoundaryInterval() { }
	BoundaryInterval(int len, int s, int e) { length = len; start = s; end = e; }
	void Set(int len, int s, int e)         { length = len; start = s; end = e; }

	void SetFirst(int s)            { start = s; }
	void SetLast(int e)             { end = e; }
	void SetLimits(int s, int e)    { start = s; end = e; }
	void SetBoundaryLength(int len) { length = len; }

	int GetBndryLength() const { return length; }

	//! Return true iff 'i' is within the boundary interval (including endpoints)
	bool Includes(int i) const
	{
		ASSERT(i >= 0);

		if (end < start)
			return (i >= start && i < length) || (i >= 0 && i <= end);
		else
			return (i >= start && i <= end);
	}

	//! Return true iff 'i' is within the boundary interval (excluding endpoints)
	bool Inside(int i) const
	{
		return (i == start || i == end) ? false : Includes(i);
	}

	void Swap() { int aux = start; start = end; end = aux; }
	int First() const { return start; }
	int Last() const  { return end; }

	/*!
		Checks that the indices idx0 and idx1 are not INSIDE the interval. If
		they are, the interval is swapped.

		@return true iff the given indices are not inside the interval.
	*/
	bool MakeExclude(int idx0, int idx1)
	{
		if (Inside(idx0) || Inside(idx1))
		{
			Swap();

			// If swap didn't fix things, the interval is invalid
			if (Inside(idx0) || Inside(idx1))
				return false;
		}

		return true;
	}

	int Size() const
	{
		return (end < start) ? (length - start + end + 1) : (end - start + 1);
	}

	int Pred(int i) const
	{
		ASSERT(i >= 0 && i < length);

		return (i == 0) ? length - 1 : i - 1;
	}

	int Succ(int i) const
	{
		ASSERT(i >= 0 && i < length);

		return (i == length - 1) ? 0 : i + 1;
	}

	//! Computes (a > b) for indices a and b using the interval's information
	bool IsGreaterThan(int a, int b) const
	{
		return ((a >= start && b >= start) || (a < start && b < start)) ? (a > b) : (a < b);
	}

	//! Computes (a < b) for indices a and b using the interval's information
	bool IsSmallerThan(int a, int b) const
	{
		return ((a >= start && b >= start) || (a < start && b < start)) ? (a < b) : (a > b);
	}

	friend std::ostream& operator<<(std::ostream& os, const BoundaryInterval& bi)
	{
		return os << "0:" << bi.length-1 << "->[" << bi.start << ',' << bi.end << ']';
	}
};

/*!
	@brief TwoBranchBndryInterval class. Defines an interval of the boundary
	curve associated with two adjacent branches in a DDSGraph.
*/
struct TBBInterval
{
	SkelBranch *pBranch0, *pBranch1;
	SkelPtBndryInfo extPt0, intPt0, extPt1, intPt1;
	BoundaryInterval firstSideInt, secondSideInt;

	TBBInterval() { }

	TBBInterval(SkeletalGraph* pSG, SkelBranch* p0, SkelBranch* p1)
	{
		pBranch0 = p0;
		pBranch1 = p1;
		SetLimits((int) pSG->GetBoundarySize());
	}

	void SetLimits(int bndryLength);
	int Side(const SkelBranch* pBranch, const SkelJoint* pJoint) const;
	bool IsValid() const;
};

/*!
	@brief List of TwoBranchBndryInterval's.
*/
class TBBIntervalList : public std::list<TBBInterval>
{
	int m_nBndrySize;

public:
	typedef std::list<TBBInterval>::iterator iterator;

	TBBIntervalList(SkeletalGraph* pSkeletalGraph)
	{
		m_nBndrySize = (int) pSkeletalGraph->GetBoundarySize();
		ASSERT(m_nBndrySize > 0);
	}

	void FindAllIntervals(BranchIt iBranch, BranchIt iEnd);
};
} //namespace dml

#endif //__BOUNDARY_INTERVAL_H__
