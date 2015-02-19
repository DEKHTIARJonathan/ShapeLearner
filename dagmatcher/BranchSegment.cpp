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
#include "DAG.h"
#include "BranchSegment.h"
#include "LigatureSegment.h"
#include "BoundaryPointFinder.h"
#include "HelperFunctions.h"
#include "MathUtils.h"

using namespace dml;

DBG_ONLY(extern LogFile g_dagCreationLog)

/*!
	@brief Shrinks a branch segment so as to ignore the TRAILING ligature interval
	defined by [first, last]. It is assumed that the ligature interval refers to
	either the beginning or ending of the branch.

	The subtraction of an interval may create an empty branch segment. An empty segment
	is represented by m_first == m_pBranch->size().

	Note: it is possible to have a whole semi-ligature branch. Such a branch would leave an
	empty branch segment the first time the ligature interval is removed. Thus, when called for
	removing ligature on the other side of the branch there would be nothing to remove.
*/
void BranchSegment::SubtractLigatureInterval(const LigatureSegment& ls)
{
	ASSERT(m_pBranch == ls.m_pBranch); // both refer to the same branch
	ASSERT(ls.IsEmpty() || ls.m_first <= ls.m_last); // it is a well-defined interval

	if (ls.IsEmpty())
		return; // empty ligature interval. There is nothing to subtract.

	if (IsEmpty())
		return; // empty branch. There is nothing to subtract from.

	if (m_first == ls.m_first && ls.m_last < m_last)
	{
		m_first = ls.m_last + 1;
	}
	else if (m_first == ls.m_first && ls.m_last >= m_last)
	{
		m_first = m_pBranch->size(); // ie, an empty segment
	}
	else if (m_last == ls.m_last && ls.m_first > m_first)
	{
		m_last = ls.m_first - 1;
	}
	else if (m_last == ls.m_last && ls.m_first <= m_first)
	{
		//ASSERT(m_last == ls.m_last && ls.m_first <= m_first);

		m_first = m_pBranch->size(); // ie, an empty segment
	}
	else
	{
		// TODO_ fix me. seahorse54
		WARNING(true, "An unexpected case of ligature subtraction.");
	}

	ASSERT((m_first >= 0 && m_last >= m_first && m_last < m_pBranch->size())
		 || m_first == m_pBranch->size());
}

/*
	@brief Computes the tangent of the axis function at either the first or
	the last point of the segment using a linear total least squares approach.
*/
void BranchSegment::ComputeEndpointTangent(bool bFirstPt, POINT* p0, POINT* p1) const
{
	int nPts = MIN(Size(), 4);
	POINTS pts(nPts);

	SkelPtIndex j = (bFirstPt) ? First() : (Last() + 1 ) - nPts;

	for (int i = 0; i < nPts; i++, j++)
		AssignPtCoord(pts[i], (*this)[j].p);

	double a, b, c; // coefficient of the line: a * x + b * y + c = 0
	LinearTotalLeastSquares(pts, nPts, a, b, c);

	*p0 = GetClosestPointOnLine(a, b, c, pts[0]);
	*p1 = GetClosestPointOnLine(a, b, c, pts[nPts - 1]);

	DBG_MSG2(p0->L2(pts[0]), fabs(a * pts[0].x + b * pts[0].y + c))
	DBG_MSG2(p1->L2(pts[nPts - 1]), fabs(a * pts[nPts - 1].x + b * pts[nPts - 1].y + c))
}

/*
	@brief Computes the tangent of the axis function at the given point
	ptIdx. If the point is inside the branch segment, only points within
	the segment are considered when computing the tangent. Otherwise,
	the immediate neighbours of the point are considered regardless of whether
	they are inside or outside the segment.
*/
void BranchSegment::ComputeTangent(SkelPtIndex ptIdx, POINT* p0, POINT* p1) const
{
	ASSERT(ptIdx >= BranchFirst() && ptIdx <= BranchLast());
	ASSERT(BranchSize() >= 2);

	SkelPtIndex j;
	int nPts;

	if (Size() > 1 && ptIdx >= First() && ptIdx <= Last()) // only use pts inside the segment
	{
		//nPts = MIN(Size(), 3);

		if (ptIdx == First())          // left endpoint
		{
			nPts = MIN(Size(), 5);
			j = First();
		}
		else if (ptIdx == Last())
		{
			nPts = MIN(Size(), 5);
			j = (Last() + 1 ) - nPts;  // right endpoint
		}
		else
		{
			nPts = 3;
			j = ptIdx - 1; // there are at least 3 pts, so it's fine
		}
	}
	else // use pts outside the segment. branch has at least 2 pts
	{
		j = (ptIdx > BranchFirst()) ? ptIdx - 1 : ptIdx;
		nPts = (j < ptIdx && ptIdx < BranchLast()) ? 3 : 2;
	}

	if (nPts <= 3) // just connect endpoints
	{
		AssignPtCoord(*p0, (*this)[j].p);
		AssignPtCoord(*p1, (*this)[j + nPts - 1].p);
	}
	else // do a real fitting of the points
	{
		POINTS pts(nPts);

		for (int i = 0; i < nPts; i++, j++)
			AssignPtCoord(pts[i], (*this)[j].p);

		double a, b, c; // coefficient of the line: a * x + b * y + c = 0
		LinearTotalLeastSquares(pts, nPts, a, b, c);

		*p0 = GetClosestPointOnLine(a, b, c, pts[0]);
		*p1 = GetClosestPointOnLine(a, b, c, pts[nPts - 1]);
	}
}

/*!
	@brief Computes the area of the branch segment.

	If bSubtractAreaFirstPt is true, the disk area of the first point of the
	segment is subtracted from the total area.

	If bSubtractAreaLastPt is true, the disk area of the last point of the
	segment is subtracted from the total area.
*/
double BranchSegment::ComputeArea(bool bSubtractAreaFirstPt, bool bSubtractAreaLastPt) const
{
	if (IsEmpty())
		return 0;

	return BranchSegmentArea(m_pBranch->getFluxPoints(), m_pBranch->getBoundaryInfoList(),
		First(), bSubtractAreaFirstPt, Last(), bSubtractAreaLastPt);
}