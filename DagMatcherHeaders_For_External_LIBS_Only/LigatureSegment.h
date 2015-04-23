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

#ifndef __LIGATURE_SEGMENT_H__
#define __LIGATURE_SEGMENT_H__

#include "BranchSegment.h"
#include "BranchWalkInfo.h"
#include "BoundaryCorner.h"

namespace dml {
class BoundaryPointFinder;

/*!
	@brief Represents the ligature segment of a branch in a SkeletalGraph.

	The limits of a ligature segment are always defined such that First() <= Last()
	Thus, First() needs not be the origin of the ligature. When m_bDecreasing is
	true, then First() is the actual origin, and when is false, then Last() is the
	origin. In general, a ligature segment will have a radius funtion that decreases
	from its source to its target endpoints. However, this is not enforces, and the
	m_bDecreasing simply states whether the indices where inverted or not in order
	to meet the condition First() <= Last().
*/
class LigatureSegment : public BranchSegment
{
	friend class LigatureSegmentList;

public:
	static BoundaryPointFinder* s_pBndryPtFinder;

	static LTDouble s_maxRadiusRatio;
	static LTDouble s_maxCumulativeBAR;
	static LTDouble s_minCornerCosAngle;

	SkelJoint* m_pJoint;
	CumulativeBAR m_cumBAR1, m_cumBAR2;
	BoundaryCorner m_corner1, m_corner2;

	double m_dSourceRadius;   //!< radius of the endpoint with largest radius
	double m_dTargetRadius;   //!< radius of the endpoint with smalles radius
	double m_dRadiusRatio;    //!< computed as m_dTargetRadius / sourceRadius
	bool   m_bDecreasing;     //!< true iff the radius decreases from m_first to m_last

	LigatureSegment()
	{
		m_pJoint = NULL;
	}

	LigatureSegment(SkelJoint* pJoint, SkelBranch* pBranch, SkelPtIndex first, SkelPtIndex last)
		: BranchSegment(pBranch, first, last)
	{
		m_pJoint = pJoint;
	}

	LigatureSegment(SkelJoint* pJoint, SkelBranch* pBranch) : BranchSegment(pBranch)
	{
		m_pJoint = pJoint;
	}

	//void ExtendSegment(const LigatureSegment& ls);

	void SetLigatureEndpoint(const BranchWalkInfo& wi, unsigned int first);

	void FindLigature(const BranchWalkInfo& wi, unsigned int first);

	void SetAttributes(const BranchWalkInfo& wi, unsigned int first, unsigned int last);

	bool HasLigatureLikeAttributes(char side) const;

	//! Returns true iff at least one side has ligature like attributes
	bool HasLigatureLikeAttributes() const
	{
		return (HasLigatureLikeAttributes('1') ||
			    HasLigatureLikeAttributes('2'));
	}

	void FindGapLigature(SkelBranch* pBranch, SkelJoint* pJoint,
		const std::list<LigatureSegment>& ligSegs);

	void FindRootedPseudoLigature(SkelBranch* pBranch, SkelJoint* pJoint);

	bool HasInterpolatedPoints() const;

	bool IsRootedLigature() const { return m_pJoint != NULL; }

	bool IsLigature(char side) const;

	//! It's true if both sides are ligature, and false otherwise
	bool IsFullLigature() const
	{
		bool bIsLig = (IsLigature('1') && IsLigature('2'));

		if (!bIsLig && m_corner1.m_valid && m_corner2.m_valid)
			return (HasLigatureLikeAttributes('1') && HasLigatureLikeAttributes('2'));

		return bIsLig;
	}

	//! It's true if exactly one side is ligature, and false otherwise
	bool IsSemiLigature() const
	{
		bool bIsLig1 = IsLigature('1');
		bool bIsLig2 = IsLigature('2');

		return ((bIsLig1 && !bIsLig2) || (!bIsLig1 && bIsLig2));
	}

	//! It's true if the segment is full or semi ligature
	bool IsLigature() const
	{
		return (IsFullLigature() || IsSemiLigature());
	}

	//! Returns the side that is ligature (segment must be a semi-ligature)
	char GetLigatureSide() const
	{
		// Ensure that either side '1' or side '2' is ligature
		ASSERT(IsSemiLigature());

		return IsLigature('1') ? '1' : '2';
	}

	bool IsElbowShape() const;
	void FindMinMaxDistances(const sg::Point& corner, double* pMinDist, double* pMaxDist) const;

	void SetCornerInfo(char side, const BoundaryCorner& bc);
	void SetCornerInfo(char side, const CornerArea& ca);

	/*!
		Computes the distance to corner as the smallest distance from a ligature
		point to the corner on the specified side.
	*/
	double GetDistanceToCorner(const BoundaryCorner& bc) const
	{
		double d, minDist;

		ASSERT(m_first <= m_last);
		ASSERT(!bc.IsNull() && bc.m_valid);

		// Use squared distances to find minimum
		minDist = GetPtCoord(m_first).sqDist(bc.m_pt);

		for (SkelPtIndex i = m_first + 1; i <= m_last; i++)
		{
			d = GetPtCoord(i).sqDist(bc.m_pt);

			if (d < minDist)
				minDist = d;
		}

		// Get the true distance by computer the squared root of it
		return sqrt(minDist);
	}

	//! Checks that the ratio of distance to corner to source radius is valid
	bool IsValidCornerDistance(const BoundaryCorner& bc) const
	{
		return ((GetDistanceToCorner(bc) / m_dSourceRadius)
			<= s_maxRadiusRatio.loose);
	}

	//! Compares the target radii of two ligature segments
	bool operator<(const LigatureSegment& rhs) const
	{
		return m_dTargetRadius < rhs.m_dTargetRadius;
	}

	//! Returns true iff radii decreases from first to last
	bool DecreasesForward() const { return m_bDecreasing; }

	//! Returns true iff radii decreases from last to first
	bool DecreasesBackwards() const { return !m_bDecreasing; }

	//! Returns the root of the ligament. ie, the endpoint with largest radius
	unsigned int Source() const
	{
		return (m_bDecreasing) ? m_first : m_last;
	}

	//! Returns the target of the ligament. ie, the endpoint with smallest radius
	unsigned int Target() const
	{
		return (m_bDecreasing) ? m_last : m_first;
	}

	//! Returns the degree of the joint at the source end of the branch
	unsigned int GetSourceJointDegree() const
	{
		return m_pJoint->degree();
	}

	//! Returns the degree of the joint at the non-source end of the branch
	unsigned int GetTargetJointDegree() const
	{
		return GetOtherEndpoint(m_pBranch, m_pJoint)->degree();
	}

	//! Returns the ligature endpoinst such that radius(i) > radius(j)
	void GetLigatureEndpoints(unsigned int* i, unsigned int* j) const
	{
		if (m_bDecreasing)
		{
			*i = m_first;
			*j = m_last;
		}
		else
		{
			*i = m_last;
			*j = m_first;
		}
	}

	//! Get a reference to the corner info on the given side
	BoundaryCorner& GetCornerReference(char side)
	{
		ASSERT(side == '1' || side == '2');

		return (side == '1') ? m_corner1 : m_corner2;
	}

	//! Get the corner info on the specified boundary side
	const BoundaryCorner& GetCorner(char side) const
	{
		ASSERT(side == '1' || side == '2');

		return (side == '1') ? m_corner1 : m_corner2;
	}

	friend std::ostream& operator<<(std::ostream& os, const LigatureSegment& ls)
	{
		os << "\n([" << ls.m_first << ls.m_pBranch->getFluxPoint(ls.m_first).p
			<< "--" << ls.m_last << ls.m_pBranch->getFluxPoint(ls.m_last).p
			<< "]\n"
			<< "radius [" << ls.m_pBranch->getFluxPoint(ls.m_first).radius()
			<< "--" << ls.m_pBranch->getFluxPoint(ls.m_last).radius()
			<< "], decreasing: " << ls.m_bDecreasing
			<< ", ratio: " << ls.m_dRadiusRatio
			<< ")\n"
			<< "side 1: normal BAR: " << ls.m_cumBAR1.normal
			<< ", straight BAR: " << ls.m_cumBAR1.straight
			<< ", cos angle: " << ls.m_corner1.m_cosAngle
			<< ", valid: " << ls.m_corner1.m_valid
			<< ", bndry pt: " << ls.m_corner1.m_pt
			<< "\n"
			<< "side 2: normal BAR: " << ls.m_cumBAR2.normal
			<< ", straight BAR: " << ls.m_cumBAR2.straight
			<< ", cos angle: " << ls.m_corner2.m_cosAngle
			<< ", valid: " << ls.m_corner2.m_valid
			<< ", bndry pt: " << ls.m_corner2.m_pt
			<< "\n";

		return os;
	}
};
} // namespace dml

#endif //__LIGATURE_SEGMENT_H__
