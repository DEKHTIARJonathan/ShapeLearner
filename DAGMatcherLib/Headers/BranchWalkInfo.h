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

#ifndef __BRANCH_WALK_INFO_H__
#define __BRANCH_WALK_INFO_H__

#include "stdafx.h"

namespace dml {
//! Cumulative boundary-to-axis ratio on each side of a medial axis segment
struct CumulativeBAR
{
	double normal;
	double straight;
};

/*!
	@brief Wraps the information needed to walk along the points of a branch
	in either forward or backwards direction. The branch information must
	remain constant while the walking takes place.
*/
class BranchWalkInfo
{
	const sg::FluxPointList& m_fpl;
	const sg::BoundaryInfoList& m_bil;

	unsigned int m_first, m_last;
	int m_inc;

public:
	BranchWalkInfo(SkelBranch* pBranch, bool bForward)
		: m_fpl(pBranch->getFluxPoints()), m_bil(pBranch->getBoundaryInfoList())
	{
		ASSERT(pBranch->size() >= 2); // otherwise, reverse walking isn't possible
		ASSERT(m_fpl.size() == m_bil.size());

		if (bForward)
		{
			m_first    = 0;
			m_last     = pBranch->size() - 1;
			m_inc      = 1;
		}
		else
		{
			m_first    = pBranch->size() - 1;
			m_last     = 0;
			m_inc      = -1;
		}
	}

	BranchWalkInfo(const BranchSegment& bs, bool bForward)
		: m_fpl(bs.m_pBranch->getFluxPoints()), m_bil(bs.m_pBranch->getBoundaryInfoList())
	{
		// Ensure that reverse walking is possible
		ASSERT((bForward && bs.Size() > 0) ||
			(!bForward && bs.Size() > 1));

		ASSERT(m_fpl.size() == m_bil.size());

		if (bForward)
		{
			m_first    = bs.m_first;
			m_last     = bs.m_last;
			m_inc      = 1;
		}
		else
		{
			m_first    = bs.m_last;
			m_last     = bs.m_first;
			m_inc      = -1;
		}
	}

	//! Returns the increment {-1, 1} to advance a point index along the branch
	int Inc() const           { return m_inc; }

	//! Returns the first point index of the branch wrt the currenct walking direction
	unsigned int First() const { return m_first; }

	//! Returns the last point index of the branch wrt the currenct walking direction
	unsigned int Last() const { return m_last; }

	unsigned int Size() const
	{
		ASSERT((m_inc > 0 && m_first <= m_last) || (m_inc < 0 && m_last <= m_first));

		return (m_inc > 0) ? (m_last - m_first + 1) : (m_first - m_last + 1);
	}

	//! Returns the size from point i to the end of the segment
	unsigned int SizeToLast(unsigned int i) const
	{
		ASSERT((m_inc > 0 && i <= m_last) || (m_inc < 0 && i >= m_last));

		return (m_inc > 0) ? m_last - i : i - m_last;
	}

	sg::Point BndryPt(unsigned int i, char side) const
	{
		return m_bil[i][side].pt;
	}

	//! Returns the radius of point 'i'
	double Radius(unsigned int i) const
	{
		return m_fpl[i].radius();
	}

	SkelPtCoord PtCoord(unsigned int i) const
	{
		return m_fpl[i].p;
	}

	SkelPtCoord FirstPtCoord() const
	{
		return m_fpl.front().p;
	}

	SkelPtCoord LastPtCoord() const
	{
		return m_fpl.back().p;
	}

	double SquaredDistance(unsigned int i, unsigned int j) const
	{
		return m_fpl[i].p.sqDist(m_fpl[j].p);
	}

	double Distance(unsigned int i, unsigned int j) const
	{
		return sqrt(SquaredDistance(i, j));
	}

	double SquaredDistance(unsigned int i, const sg::Point& p) const
	{
		return m_fpl[i].p.sqDist(p);
	}

	double Distance(unsigned int i, const sg::Point& p) const
	{
		return sqrt(SquaredDistance(i, p));
	}

	bool IsGreaterThan(unsigned int i, unsigned int j)
	{
		return (m_inc > 0) ? (i > j) : (i < j);
	}

	double SquaredSpokesDistance(unsigned int i) const
	{
		return m_bil[i].first.pt.sqDist(m_bil[i].second.pt);
	}

	//! Reverses the current walking direction
	void ReverseDirection()
	{
		std::swap(m_first, m_last);
		m_inc *= -1;
	}

	double GetCumAxisDist(unsigned int i, unsigned int j) const
	{
		if (i < j)
			return m_bil[j].cumAxisDist - m_bil[i].cumAxisDist;
		else
			return m_bil[i].cumAxisDist - m_bil[j].cumAxisDist;
	}

	double GetCumBndryDist(unsigned int i, unsigned int j, char side) const
	{
		if (i < j)
			return m_bil[j][side].cumBndryDist - m_bil[i][side].cumBndryDist;
		else
			return m_bil[i][side].cumBndryDist - m_bil[j][side].cumBndryDist;
	}

	double GetCumBndryDist(char side) const
	{
		return GetCumBndryDist(m_first, m_last, side);
	}

	//! Returns the boundary-to-axis ration on either side of the branch segment [i, j]
	void ComputeBAR(unsigned int i, unsigned int j, CumulativeBAR* pCBAR1, CumulativeBAR* pCBAR2) const
	{
		double cad, sad, cbd1, cbd2;

		ASSERT(i != j);

		if (i < j)
		{
			cad = m_bil[j].cumAxisDist - m_bil[i].cumAxisDist;

			cbd1 = m_bil[j].first.cumBndryDist - m_bil[i].first.cumBndryDist;
			cbd2 = m_bil[j].second.cumBndryDist - m_bil[i].second.cumBndryDist;
		}
		else
		{
			cad = m_bil[i].cumAxisDist - m_bil[j].cumAxisDist;

			cbd1 = m_bil[i].first.cumBndryDist - m_bil[j].first.cumBndryDist;
			cbd2 = m_bil[i].second.cumBndryDist - m_bil[j].second.cumBndryDist;
		}

		pCBAR1->normal = cbd1 / cad;
		pCBAR2->normal = cbd2 / cad;

		sad = m_fpl[j].p.dist(m_fpl[i].p); // straight axis distance

		pCBAR1->straight = cbd1 / sad;
		pCBAR2->straight = cbd2 / sad;
	}

	//! Returns the boundary-to-axis ration on either side of the branch segment [i, j]
	void ComputeBAR(unsigned int i, unsigned int j, std::pair<double, double>* pBAR) const
	{
		double cad;

		ASSERT(i != j);

		if (i > j)
			std::swap(i, j);

		cad = m_bil[j].cumAxisDist - m_bil[i].cumAxisDist;

		pBAR->first  = (m_bil[j].first.cumBndryDist - m_bil[i].first.cumBndryDist) / cad;
		pBAR->second = (m_bil[j].second.cumBndryDist - m_bil[i].second.cumBndryDist) / cad;
	}

	//! Returns the boundary-to-axis ration on the given side of the branch segment [i, j]
	void ComputeBAR(unsigned int i, unsigned int j, char side, double* pBAR) const
	{
		double cad;

		ASSERT(i != j);
		ASSERT(side == '1' || side == '2');

		if (i > j)
			std::swap(i, j);

		cad = m_bil[j].cumAxisDist - m_bil[i].cumAxisDist;

		*pBAR  = (m_bil[j][side].cumBndryDist - m_bil[i][side].cumBndryDist) / cad;
	}

	double ComputeDiskOverlap(unsigned int i, unsigned int j) const;
};
} // namespace dml

#endif //__BRANCH_WALK_INFO_H__
