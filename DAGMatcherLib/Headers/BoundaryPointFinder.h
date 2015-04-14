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
 * $Log: BoundaryPointFinder.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef _BOUNDARY_POINTS_FINDER_H_
#define _BOUNDARY_POINTS_FINDER_H_

#include <vector>
#include "KDTree.h"
#include "BoundaryInterval.h"
#include "BasicUtils.h"

namespace dml {
/*!
	@brief Use an object of this class to find the pair of
	associated boundary points to each skeleton point.

	Since joint points are repeated in each branch, each version of a joint
	point will ba assigned a pair of boundary points that depends on the
	branch the joint is in.
*/
class BoundaryPointFinder
{
	/*!
		Represents the tangent, and CCW tangent angle [0,2PI] of a skeletal
		point, and a pointer to its boundary info, so that it can be be updated
		using the stored tangent and angle info.
	*/
	class AngleInfo
	{
		double m_dAngle;                  //<! CCW tangent angle in [0,2PI]
		sg::BoundaryInfo* m_pBndryInfo;   //<! pointer to bndryinfo to update
		sg::Vector m_tangent;             //<! normalized tangent
		SkelBranch* m_pBranch;            //<! branch that contains bndry info pt

	public:
		void Set(sg::BoundaryInfo* pBndryInfo, const double& angle,
			const sg::Vector& tangent, SkelBranch* pBranch)
		{
			m_pBndryInfo = pBndryInfo;
			m_dAngle = angle;
			m_tangent = tangent;
			m_pBranch = pBranch;
		}

		const double& Angle() const
		{
			return m_dAngle;
		}

		const sg::Vector& Tangent() const
		{
			return m_tangent;
		}

		SkelBranch* Branch() const
		{
			return m_pBranch;
		}

		bool operator<(const AngleInfo& rhs) const
		{
			return m_dAngle < rhs.m_dAngle;
		}

		/*!
			Retrieves the boundary information on the side corresponding
			to the sign of a given object angle.
		*/
		sg::BoundaryPoint& GetBndryPt(const double& objectAngle) const
		{
			return (objectAngle >= 0) ? m_pBndryInfo->first : m_pBndryInfo->second;
		}
	};

	typedef std::vector<AngleInfo> JointAngles;

protected:
	KDTree m_origBndryPts;
	KDTree m_smoothBndryPts;

	bool m_bAreKDTreesBuilt;

protected:
	// Inline functions
	inline int GetValidBoundaryIndex(const SkelBranch* pBranch, char side,
		const sg::BoundaryPoint& bp);

	inline bool TestBoundaryInterval(const int firstIdx, const int lastIdx,
		const int otherIdx0, const int otherIdxN, const int testIndex) const;

	inline void SetBoundaryInterval(const int firstIdx, const int lastIdx, const int otherIdx0,
		const int otherIdxN, BoundaryInterval* pBndryInt) const;

	inline void SetBoundaryInterval(const char side, const sg::BoundaryInfo& bi0,
		const sg::BoundaryInfo& biN, BoundaryInterval* pBndryInt) const;

	inline void SetBoundaryInterval(const SkelBranch* pBranch, const char side,
		BoundaryInterval* pBndryInt);

	inline void EnsureValidInterval(const sg::BoundaryInfo& bi0, sg::BoundaryInfo* pBiN);

	//! Returns true iff the endpoint spokes on the given side don't cross one another
	bool ValidateEndpointSpokes(const SkelBranch* pBranch, char side)
	{
		return !DoSpokesIntersect(pBranch->getFirstXYPoint(),
		                          pBranch->getFirstBndryInfo()[side].pt,
		                          pBranch->getLastXYPoint(),
		                          pBranch->getLastBndryInfo()[side].pt);
	}

	// Non-inline functions

	void FixCrossingEndpointSpokes(SkelBranch* pBranch, char side);

	void FixSpecialCrossingCase(SkelBranch* pBranch, char side);

	sg::BoundaryPoint FindClosestBoundaryPoint(const sg::Point& p0,
		const sg::Point& p1, const BoundaryInterval& bndryInt) const;

	void SetBoundaryInfoAtJoint(SkelJoint* pJoint, const JointAngles& angles);

	void SetBoundaryInfoAtTerminal(const sg::FluxPoint& fp0, sg::Vector tangent,
		bool bDecreasingRadius, const sg::FluxPoint& fp1, const SkelBranch* pBranch,
		sg::BoundaryInfo& bi0);

	void SetInnerBoundaryInfo(const sg::FluxPointList& fpl, const BoundaryInterval& side1,
		const BoundaryInterval& side2, sg::BoundaryInfoList& bil);

	void AssignInnerBoundaryPoints(const sg::FluxPointList& fpl, sg::BoundaryInfoList& bil);

	inline double BoundaryDistance(int nFrom, int nTo) const;

	double SkeletonDistance(const sg::FluxPoint& fp0, const sg::FluxPoint& fp1) const
	{
		double dx = fp1.p.x - fp0.p.x;
		double dy = fp1.p.y - fp0.p.y;

		return sqrt(dx * dx + dy * dy);
	}

	bool BuildKDTrees()
	{
		ASSERT(m_bAreKDTreesBuilt == false);

		return m_bAreKDTreesBuilt = (m_origBndryPts.Build() && m_smoothBndryPts.Build());
	}

	void AssignBoundaryDistanceInfo(const sg::FluxPointList& fpl, const BoundaryInterval& interval,
		char cSide, sg::BoundaryInfoList& bil);

	void AssignAxisDistanceInfo(const sg::FluxPointList& fpl, sg::BoundaryInfoList& bil);

public:
	BoundaryPointFinder(int nMaxBoundaryPoints) : m_origBndryPts(nMaxBoundaryPoints),
		m_smoothBndryPts(nMaxBoundaryPoints)
	{
		m_bAreKDTreesBuilt = false;
	}

	void AssignRadiusValues(const sg::BoundaryInfoList& bil, sg::FluxPointList& fpl);

	void AssignBoundaryPoints(const sg::FluxPointList& fpl, sg::BoundaryInfoList& bil,
		const sg::BoundaryInfo& bi0, const sg::BoundaryInfo& biN);

	void AssignBoundaryPoints(sg::DDSGraph* pDDSGraph);

	void AddBoundaryPoints(std::vector<sg::Point>& pts, unsigned int nSmoothIter)
	{
		unsigned int i, nSize = pts.size();
		//note: size gets incremented by 1 after smoothing, so we save it first

		for (i = 0; i < nSize; i++)
			m_origBndryPts.AddDataPoint(pts[i].x, pts[i].y);

		for (i = 0; i < nSmoothIter; i++)
			sg::smooth_closed_curve_pts(pts);

		// use original size to avoid adding the "duplicated" last endpoint
		for (i = 0; i < nSize; i++)
			m_smoothBndryPts.AddDataPoint(pts[i].x, pts[i].y);
	}

	void GetBoundaryPoint(int i, sg::Point* p) const
	{
		m_origBndryPts.GetDataPoint(i, p->x, p->y);
	}

	KDTree& GetOriginalPoints() { return m_origBndryPts; }

	static double ComputeEndPointOverlap(const sg::FluxPointList& fpl);
};
} // namespace dml

#endif //_BOUNDARY_POINTS_FINDER_H_
