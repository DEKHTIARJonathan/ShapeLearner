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

#include "BoundaryIntervalWithGaps.h"

using namespace dml;

/*!
	Determines the direction of iteration along the boundary points
	such that walking from the spoke endpoints of the first and last
	spoke of a branch on side A doesn't visit the spoke endpoints on
	side B.
*/
bool BoundaryIntervalWithGaps::SetIntervalDirection(
	const sg::BoundaryInfoList& bil, char side)
{
	const char oppSide = GetOtherSide(side);

	const int first = GetBoundaryIndex(bil.front(), side);
	const int last  = GetBoundaryIndex(bil.back(), side);

	const int oppFirst = GetBoundaryIndex(bil.front(), oppSide);
	const int oppLast  = GetBoundaryIndex(bil.back(), oppSide);

	if (first != last)
	{
		int outBndryPtIdx = -1;
		int inBndryPtIdx = -1;

		// See if oppFirst is diffirent from firts and last
		if (oppFirst != first && oppFirst != last) // it is
		{
			outBndryPtIdx = oppFirst;
		}
		// See if oppLast is diffirent from firts and last
		else if (oppLast != first && oppLast != last) // it is
		{
			outBndryPtIdx = oppLast;
		}
		// Try and find a unique inner/outer point to define the interval
		else
		{
			unsigned int i;

			// First, try finding an outer point (ie inner pt on opposite side)
			i = GetUniqueInnerBoundaryPoint(m_pBranch, oppSide);

			if (i > 0)
			{
				outBndryPtIdx = GetBoundaryIndex(bil[i], oppSide);
			}
			else
			{
				// Next, try finding an inner point (ie inner pt on given side)
				i = GetUniqueInnerBoundaryPoint(m_pBranch, side);

				if (i > 0)
					inBndryPtIdx = GetBoundaryIndex(bil[i], side);
			}
		}

		if (outBndryPtIdx >= 0)
		{
			return SetIntervalDirectionWithOuterPoint(side, first, last, outBndryPtIdx);
		}
		else if (inBndryPtIdx >= 0)
		{
			return SetIntervalDirectionWithInnerPoint(side, first, last, inBndryPtIdx);
		}
	}

	return SetIntervalDirectionUsingContext(side, first, last);
}

/*!
	Determines the direction of iteration along the boundary points
	such that walking from the spoke endpoints of the first and last
	spoke of a branch on side A doesn't visit the spoke endpoints on
	side B.
*/
bool BoundaryIntervalWithGaps::SetIntervalDirectionWithOuterPoint(
	char side, int first, int last, int outPtIdx)
{
	BoundaryInterval& bndryInt = GetBndryInt(side);
	bool& moveClockwise = (side == '1') ? m_moveClockWise[0] : m_moveClockWise[1];

	// Set the interval associated with the first side
	bndryInt.Set(m_bndryPts.Size(), first, last);

	if (bndryInt.Inside(outPtIdx))
	{
		bndryInt.Swap();
		moveClockwise = false;

		if (bndryInt.Inside(outPtIdx))
			return false;
	}
	else
	{
		moveClockwise = true;
	}

	return true;
}

/*!
	Determines the direction of iteration along the boundary points
	such that walking from the spoke endpoints of the first and last
	spoke of a branch on side A doesn't visit the spoke endpoints on
	side B.
*/
bool BoundaryIntervalWithGaps::SetIntervalDirectionWithInnerPoint(
	char side, int first, int last, int inBndryPtIdx)
{
	BoundaryInterval& bndryInt = GetBndryInt(side);
	bool& moveClockwise = (side == '1') ? m_moveClockWise[0] : m_moveClockWise[1];

	// Set the interval associated with the first side
	bndryInt.Set(m_bndryPts.Size(), first, last);

	if (!bndryInt.Inside(inBndryPtIdx))
	{
		bndryInt.Swap();
		moveClockwise = false;

		if (!bndryInt.Inside(inBndryPtIdx))
			return false;
	}
	else
	{
		moveClockwise = true;
	}

	return true;
}

/*!
	Determines the direction of iteration along the boundary points
	such that walking from the spoke endpoints of the first and last
	spoke of a branch on side A doesn't visit the spoke endpoints on
	side B.
*/
bool BoundaryIntervalWithGaps::SetIntervalDirectionUsingContext(
	char side, int first, int last)
{
	BoundaryInterval& bndryInt = GetBndryInt(side);
	bool& moveClockwise = (side == '1') ? m_moveClockWise[0] : m_moveClockWise[1];

	// Set the interval associated with the first side
	bndryInt.Set(m_bndryPts.Size(), first, last);

	if (first == last)
	{
		sg::Point succPt, predPt, intPt;

		m_bndryPts.GetDataPoint(bndryInt.Succ(last), succPt.x, succPt.y);
		m_bndryPts.GetDataPoint(bndryInt.Pred(first), predPt.x, predPt.y);

		const sg::FluxPointList& fpl = m_pBranch->getFluxPoints();

		int retVal = FindLineSegmentIntersection(fpl.front().p, predPt,
			fpl.back().p, succPt, &intPt);

		// If spokes intersect (retVal=1), or a line intersects a spoke (retVal=3
		// or retVal=4), move CCW. Otherwise (retVal=0 or retVal = 5), move CW.
		moveClockwise = (retVal == 0 || retVal == 5);
	}
	else
	{
		sg::Point succPt, predPt;

		m_bndryPts.GetDataPoint(bndryInt.Succ(first), succPt.x, succPt.y);
		m_bndryPts.GetDataPoint(bndryInt.Pred(last), predPt.x, predPt.y);

		const sg::FluxPointList& fpl = m_pBranch->getFluxPoints();

		// If moving CW, it should be true that the successor of first
		// and the predecessor of last are endpoints of crossing spokes
		moveClockwise = DoSpokesIntersect(fpl.front().p, predPt,
			fpl.back().p, succPt);
	}

	return true;
}

/*!
	@brief Moves 'idx' to its successor.

	It always moves forward, regardless of whether idx.moveForward
	is true or false.
*/
void BoundaryIntervalWithGaps::Succ(Index& idx) const
{
	const sg::BoundarySegments& bss = m_bsl[idx.branchSide];

	// Advance idx so that the next call returns the successor

	if (bss.empty()) // there are no gaps, the successor is trivial
	{
		MoveToNextBndryPoint(idx);
	}
	else if (idx.bndrySegmentIdx == -1 || idx.bndrySegmentIdx == bss.size())
	{
		// idx is in between the end points
		MoveInBetweenSides(idx, NextBndryPoint(idx.branchSide, idx.bndryPointIdx));
	}
	else // we are within the bndary pts of a branch with gaps
	{
		// Analyze the current bndry segment to find a valid successor
		const sg::BoundarySegment& bs = bss[idx.bndrySegmentIdx];

		// See whether we are on a bndry pt or a gap pt...
		if (idx.bndryPointIdx >= 0) // it's a boundary pt
		{
			// The succ is either a bndry pt or a gap pt...
			if (idx.bndryPointIdx == GetLastIdxInSegment(bs))
			{
				if (!bs.gapPts.empty())     // if there are gap points...
					idx.bndryPointIdx = -1; // ...succ is first gap pt
				else // ...no gap pts, go to (next segment, first point)
					MoveIdxToFirstPointInSegment(idx, idx.bndrySegmentIdx + 1);
			}
			else // succ is a boundary pt
				MoveToNextBndryPoint(idx);
		}
		else //it's a gap pt (i < 0)
		{
			unsigned int i = -idx.bndryPointIdx - 1; // convert to valid gap index

			ASSERT(i < bs.gapPts.size());

			if (i + 1 < bs.gapPts.size()) // not last pt in gap
				idx.bndryPointIdx--;      // move ahead by one (signs are reversed)
			else
				MoveIdxToFirstPointInSegment(idx, idx.bndrySegmentIdx + 1);
		}
	}
}

/*!
	@brief Moves 'idx' to its predecessor.

	It always moves backward, regardless of whether idx.moveForward
	is true or false.
*/
void BoundaryIntervalWithGaps::Pred(Index& idx) const
{
	const sg::BoundarySegments& bss = m_bsl[idx.branchSide];

	// Move back idx so that the next call returns the predecessor

	if (bss.empty()) // there are no gaps, the predecessor is trivial
	{
		MoveToPrevBndryPoint(idx);
	}
	else if (idx.bndrySegmentIdx == -1 || idx.bndrySegmentIdx == bss.size())
	{
		// idx is in between the end points
		MoveInBetweenSides(idx, PrevBndryPoint(idx.branchSide, idx.bndryPointIdx));
	}
	else // we are within the bndary pts of a branch with gaps
	{
		// Analyze the current bndry segment to find a valid successor
		const sg::BoundarySegment& bs = bss[idx.bndrySegmentIdx];

		// See whether we are on a bndry pt or a gap pt...
		if (idx.bndryPointIdx >= 0) // it's a boundary pt
		{
			if (idx.bndryPointIdx == GetFirstIdxInSegment(bs))
				MoveIdxToLastPointInSegment(idx, idx.bndrySegmentIdx - 1);
			else // pred is a boundary pt
				MoveToPrevBndryPoint(idx);
		}
		else //it's a gap pt (i < 0)
		{
			unsigned int i = -idx.bndryPointIdx - 1; // convert to valid gap index

			ASSERT(i < bs.gapPts.size());

			if (i > 0) // not first pt in gap
				idx.bndryPointIdx++;      // move back by one (signs are reversed)
			else // it's the first gap pt...
			{
				// The pred is either a bndry pt or a gap pt...
				if (bs.start >= 0 && bs.end >= 0) // ...valid limits, there are bndry pts
					idx.bndryPointIdx = bs.end;
				else // ...no bndry pts, go to (prev segment, last point)
					MoveIdxToLastPointInSegment(idx, idx.bndrySegmentIdx - 1);
			}
		}
	}
}