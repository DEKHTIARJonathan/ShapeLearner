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
 * $Log: LigatureDetection.cpp,v $
 *
 *-----------------------------------------------------------------------*/
#include "DAG.h"
#include "LigatureDetection.h"
#include "BoundaryPointFinder.h"
#include "HelperFunctions.h"
#include "MathUtils.h"

//! Minimum endpoint disk overlap value for a branch to be considered spurious
#define MIN_ENDPT_OVERLAP     0.9

//! Minimum non-ligature segment size considered when merging ligature segments
#define MIN_NON_LIG_SEGMENT_SIZE  1

//! Maximum squared distance from corner used to compute the ratio of spokes within range
#define MAX_SQUARED_DIST_FROM_CORNER  9

//! Minimum ratio of spokes within range for a corner to be valid
#define MIN_SPOKE_WITHIN_RANGE_RATIO  0.5

//! Minimum average angle to achieve enough bending and be classified as an ``elbow''
#define MIN_ELBOW_BENDING_AVG_ANGLE 0.1

using namespace dml;

DBG_ONLY(extern LogFile g_dagCreationLog)

BoundaryPointFinder* LigatureSegment::s_pBndryPtFinder = NULL;

//! Maximum ratio between a ligature segment's smallest and largest endpoint radii
LTDouble LigatureSegment::s_maxRadiusRatio(0.9);

//! Maximum boundary-to-axis ratio that qualifies as ligature
LTDouble LigatureSegment::s_maxCumulativeBAR(0.75);

//! Minimum angle cosine of a concave corner associated with a full-ligature segment
LTDouble LigatureSegment::s_minCornerCosAngle(-0.876818); //cos(2.64)

/*!
	Checks the ligature attributes quite loosly, so that
	anything that barely resembles ligature qualifies.

	Do not check for corner angles.
*/
bool LigatureSegment::HasLigatureLikeAttributes(char side) const
{
	ASSERT(side == '1' || side == '2');

	// Check the radius ratio, which is common to both sides
	if (m_dRadiusRatio > s_maxRadiusRatio.loose)
		return false;

	// Check the attributes of each side
	if (side == '1')
		return (m_cumBAR1.straight <= s_maxCumulativeBAR.loose);
	else
		return (m_cumBAR2.straight <= s_maxCumulativeBAR.loose);
}

/*!
	@brief Returns true iff the given side is a ligature side
*/
bool LigatureSegment::IsLigature(char side) const
{
	ASSERT(side == '1' || side == '2');

	if (m_dRadiusRatio > s_maxRadiusRatio.loose)
		return false;

	if (side == '1')
	{
		if (m_cumBAR1.normal <= s_maxCumulativeBAR.loose
			&& m_corner1.m_valid)
		{
			// We know that all loose thresholds are valid
			// Know, we want that either the tight RR is valid
			// or else, the the tight cos angle one is valid
			return (m_dRadiusRatio <= s_maxRadiusRatio.tight ||
				m_corner1.m_cosAngle >= s_minCornerCosAngle.tight);
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (m_cumBAR2.normal <= s_maxCumulativeBAR.loose
			&& m_corner2.m_valid)
		{
			return (m_dRadiusRatio <= s_maxRadiusRatio.tight ||
				m_corner2.m_cosAngle >= s_minCornerCosAngle.tight);
		}
		else
		{
			return false;
		}
	}
}

/*!
	@brief Computes the boundary-to-axis ratio up to the point in which the circle
	defined by the data of pJoint and the circle defined by the data of a flux point
	in pBranch intersect and the intersection points form an angle of PI wrt the line
	that connects the circles. The flux points are searched by starting from the one
	that es equal to the given joint point. Call IsFullLigature() or IsSemiLigature()
	to find out whether the segment is full or semi ligature.

	Note: spurious branches caused by the AFMM algorithm can result in strange
	conditions that affect the ligature computation. For example, this branches
	tend to be shorter than their true length and may be related to "weak" boundary
	deformations. It is then important to deal with them as a special case. We define
	spurious branch as "a terminal branch whose endpoints greatly overlaps".
	Solution: a spurius branch should still be labeled as ligature but may fail in
	been selected as the ligature branch in cases of joints with multiple ligatures.
	We solve this by setting the m_dTargetRadius of a spurious branch to zero, which
	should make it rank first among all ligature branches.
*/
/*void LigatureSegment::FindRootedLigature(SkelBranch* pBranch, SkelJoint* pJoint)
{
	ASSERT(pBranch->getFirstXYPoint() == pJoint->fp.p ||
		   pBranch->getLastXYPoint() == pJoint->fp.p);

	DBG_C_LOG_NEW_LINE
	DBG_C_LOG(DBG_VAL(pJoint->fp.p) << DBG_VAL(GetOtherEndpoint(pBranch, pJoint)->fp.p))

	m_pBranch = pBranch;
	m_pJoint = pJoint;

	bool bMoveForward = (pBranch->getFirstXYPoint() == pJoint->fp.p);

	BranchWalkInfo wi(pBranch, bMoveForward);

	// Check for the special case of bump branches first
	if (!FindBumpBranch(wi, pBranch, pJoint))
	{
		FindLigature(wi, wi.First());
	}
}*/

/*!
*/
/*bool LigatureSegment::FindBumpBranch(const BranchWalkInfo& wi, SkelBranch* pBranch,
									 SkelJoint* pJoint)
{
	m_bSpuriousBranch = false;

	// Check for "bump branch" pre-conditions
	if (GetOtherEndpoint(pBranch, pJoint)->degree() == 1 &&
		pBranch->n1->fp.p.sqDist(pBranch->n2->fp.p) < pJoint->fp.squaredRadius() &&
		wi.ComputeDiskOverlap(wi.First(), wi.Last()) >= MIN_ENDPT_OVERLAP)
	{
		SetLigatureAttributes(wi, wi.First(), wi.Last(), true);

		if (m_corner1.valid || m_corner2.valid)
		{
			m_dTargetRadius = 0; // so that it ranks first in operator<()
			m_bSpuriousBranch = true;
		}
	}

	return m_bSpuriousBranch;
}*/

/*!
	@brief Finds the endpoins of the ligature gap around a joint

	Let b0 and b0' be the boundary points on either side of the
	ligament segment associated with joint point p0; and let b1
	and b1' be the boundary point associated with the estimated
	ligature segment endpoint s1, as shown in Figure 4c. We
	are interested in knowing skeletal points s2 and s3, with
	corresponding boundary points b2 and b3' on the attachment
	side. The assumption of the model is that the distance from
	b1 to b0, is equal to that between b0 and b2. And similarly,
	the distance from b1' to b0' is said to be equal to that between
	b0' and b3'.
*/
void LigatureSegment::FindGapLigature(SkelBranch* pBranch, SkelJoint* pJoint,
									  const std::list<LigatureSegment>& ligSegs)
{
	ASSERT(pBranch->getFirstXYPoint() == pJoint->fp.p ||
		   pBranch->getLastXYPoint() == pJoint->fp.p);

	m_pBranch = pBranch;
	m_pJoint = pJoint;

	bool bMoveForward = (pBranch->getFirstXYPoint() == pJoint->fp.p);

	BranchWalkInfo wi(pBranch, bMoveForward);

	double maxRadius, maxDist2;

	// Find the maximum radius among all ligature target endpoints (usually just 1)
	std::list<LigatureSegment>::const_iterator it = ligSegs.begin();

	maxRadius = it->m_dTargetRadius;

	for (it++; it != ligSegs.end(); it++)
	{
		if (it->m_dTargetRadius > maxRadius)
			maxRadius = it->m_dTargetRadius;
	}

	// Use max radius to define the size of the skeletal gap
	maxDist2 = maxRadius * maxRadius;

	unsigned int i, j, last;
	std::pair<char,char> sides;
	double bar;

	sides = GetSidesOfSharedBndryPt(pBranch, ligSegs.front().m_pBranch, pJoint);

	i = wi.First();

	// We don't include the endpoint, unless there is no choice
	last = (pBranch->size() > 2) ? (wi.Last() - wi.Inc()) : wi.Last();

	for (j = i + wi.Inc(); j != last; j += wi.Inc())
	{
		if (wi.SquaredDistance(i, j + wi.Inc()) > maxDist2)
			break;

		if (ligSegs.size() == 1)
		{
			wi.ComputeBAR(i, j + wi.Inc(), sides.first, &bar);

			if (bar > 1.5)
				break;
		}
	}

	SetAttributes(wi, i, j);
}

/*!
	@brief Appends the given segment, such that the resulting segment is larger

	It's a function of branch segment now
*/
/*void LigatureSegment::ExtendSegment(const LigatureSegment& ls)
{
	if (m_last + 1 == ls.m_first)
	{
		m_last = ls.m_last;
	}
	else if (m_first == ls.m_last + 1)
	{
		m_first = ls.m_first;
	}
	else if (m_first == ls.m_first && m_last == ls.m_last)
	{
		// there nothing to do
		WARNING(true, "An unexpected head/trail ligature found when extending");
	}
	else
	{
		WARNING(true, "There is a strange error in the ligature segment");
		//ASSERT(false);
	}

	// Add the merged corners (this is needed for debugging purposes only)!
	//m_mergedCorners.AddTail(ls.m_corner1);
	//m_mergedCorners.AddTail(ls.m_corner2);
}*/

/*!
	@brief Determines if the branch segment defines a convex corner.
	A convex corner is produced when the straight continuation of
	the two boundary segments meet at a point that is at distance
	R * SQRT(2) of the begginig of the segment.
*/
bool BranchSegment::IsCornerSegment(bool bForwardDir)
{
	BranchWalkInfo wi(*this, bForwardDir);

	double maxRadius = wi.Radius(wi.First());
	double straightAxisLength = wi.Distance(wi.First(), wi.Last());
	double maxAxisDist = maxRadius * M_SQRT2;

	// Ensure that the sengment is not too long to define a convex corner
	if (straightAxisLength <= maxAxisDist)
	{
		// See where the lines define by each boundary segment intersect
		sg::Point intPt, ptF1, ptL1, ptF2, ptL2;

		ptF1 = wi.BndryPt(wi.First(), '1');
		ptL1 = wi.BndryPt(wi.Last(), '1');

		ptF2 = wi.BndryPt(wi.First(), '2');
		ptL2 = wi.BndryPt(wi.Last(), '2');

		int rv = FindLineSegmentIntersection(ptF1, ptL1, ptF2, ptL2, &intPt);

		if ((rv == 1 /*segments intersect*/|| rv == 5 /*lines intersect*/) &&
			wi.Distance(First(), intPt) <= maxAxisDist /* check length from 'joint'*/)
		{
			// In addition, make sure that the BARs are smaller than 1
			std::pair<double, double> fullBAR;

			wi.ComputeBAR(wi.First(), wi.Last(), &fullBAR);

			return fullBAR.first < 1 && fullBAR.second < 1;
		}
	}

	return false;
}

void LigatureSegment::SetAttributes(const BranchWalkInfo& wi,
									unsigned int first, unsigned int last)
{
	wi.ComputeBAR(first, last, &m_cumBAR1, &m_cumBAR2);

	// Set derived values
	m_dSourceRadius = wi.Radius(first);
	m_dTargetRadius = wi.Radius(last);
	m_dRadiusRatio  = m_dTargetRadius / m_dSourceRadius;

	m_corner1.m_valid = false;
	m_corner2.m_valid = false;

	if (first < last)
	{
		// Ensure that the target point (last) is not the branch endpoint
		if (last == BranchLast() && BranchSize() > 1)
			last = BranchLast() - 1;

		m_first = first;
		m_last  = last;
		m_bDecreasing = true;
	}
	else
	{
		// Ensure that the target point (last) is not the branch endpoint
		if (last == 0 && BranchSize() > 1)
			last = 1;

		m_first = last;
		m_last  = first;
		m_bDecreasing = false;
	}
}

/*!
	@brief Computes the boundary-to-axis ratio up to the point in which the circle
	defined by the data of pJoint and the circle defined by the data of a flux point
	in pBranch intersect and the intersection points form an angle of PI wrt the line
	that connects the circles. The flux points are searched by starting from the one
	idexed by 'first'. Call IsFullLigature() or IsSemiLigature() to find out whether
	the segment is full or semi ligature.
*/
void LigatureSegment::SetLigatureEndpoint(const BranchWalkInfo& wi, unsigned int first)
{
	ASSERT(first != wi.Last());

	const unsigned int i = first;
	const double r0      = wi.Radius(i);
	const double r0r0    = r0 * r0;

	double r1, dist2;
	unsigned int j;

	for (j = i + wi.Inc(); j != wi.Last(); j += wi.Inc())
	{
		r1    = wi.Radius(j + wi.Inc());
		dist2 = wi.SquaredDistance(i, j + wi.Inc());

		// We want a point in which the cos(circle intersection spoke angle) is >= 0
		// since: cosAlpha = (r1 * r1 + dist2 - r0r0) / (2 * r1 * sqrt(dist2));
		// we can simply check whether (r1 * r1 + dist2) >= r0r0.

		// We want the current point j, and not j + inc
		if ((r1 < r0 && (r1 * r1 + dist2) >= r0r0 &&
			(wi.Radius(j) / r0) <= s_maxRadiusRatio.loose)
			|| dist2 >= r0r0)
		{
			//return j;
			break; // j is the endpoint
		}
	}

	SetAttributes(wi, i, j);

	// If it doesn't look like ligature, we set an endpoint that at least
	// meets the BAR requirements. This is required by
	// LigatureSegmentList::SetConcaveCornerInfo, which calls
	// HasLigatureLikeAttributes(), and needs more a permisive
	// assesment of ligature
	if (!HasLigatureLikeAttributes('1') && !HasLigatureLikeAttributes('2'))
	{
		unsigned int prev_k = i;

		for (unsigned int k = i + wi.Inc(); k != j; k += wi.Inc())
		{
			wi.ComputeBAR(i, k, &m_cumBAR1, &m_cumBAR2);

			if (m_cumBAR1.normal >= 0.9 || m_cumBAR2.normal >= 0.9
				|| wi.Radius(prev_k) < wi.Radius(k))
			{
				SetAttributes(wi, i, k);
				break;
			}

			prev_k = k;
		}

		if (HasLigatureLikeAttributes('1') || HasLigatureLikeAttributes('2'))
		{
			DBG_MSG1("Things changed")
		}
	}
}

void LigatureSegment::FindLigature(const BranchWalkInfo& wi, unsigned int first)
{
	SetLigatureEndpoint(wi, first);

	if (HasLigatureLikeAttributes('1'))
		SetCornerInfo('1', CornerArea());

	if (HasLigatureLikeAttributes('2'))
		SetCornerInfo('2', CornerArea());
}

/*!
	Sets the corner info of the ligature segment with the given
	boundary corner.

	It is used during for branch merging.
*/
void LigatureSegment::SetCornerInfo(char side, const BoundaryCorner& bc)
{
	GetCornerReference(side) = bc;

	// Check that the ratio of distance to corner and source radius is valid
	// By checking this, elbow cases wont create ligature. That's not really a bad thing
	// as we are not chequing for elbows anymore (didn't work well). Also, if this
	// condition isn't checked, other problems arise.
	if (bc.m_valid && !IsValidCornerDistance(bc))
		GetCornerReference(side).m_valid = false;
}

/*!
	Finds a concave corner on the given side and within the limits of
	the given corner area. It then sets the appropriate corner info
	of the ligature segment.

	It is used during for branch partitioning.
*/
void LigatureSegment::SetCornerInfo(char side, const CornerArea& ca)
{
	BoundaryIntervalWithGaps bi(m_pBranch, s_pBndryPtFinder->GetOriginalPoints());
	BoundaryCorner& bc = GetCornerReference(side);

	bi.SetLimits(m_first, m_last, side);

	bc.FindCorner(bi, s_minCornerCosAngle.loose, ca);

	// Check that the corner is a concave corner
	if (bc.m_valid)
	{
		bc.m_valid = bc.IsConcaveCorner(GetPtCoord(FindClosestPoint(bc.m_pt)));

		// Check that the ratio of distance to corner and source radius is valid
		// It seems that for the case of branch partitioning, this may
		// not be such a good thing to check (but not sure about it, so test with(out))
		//if (bc.m_valid && !IsValidCornerDistance(bc))
		//	bc.m_valid = false;
	}
}

/*!
	@brief Returns true if the ligature segment forms an ``elbow'' shape
*/
bool LigatureSegment::IsElbowShape() const
{
	if (Source() != BranchFirst() && Source() != BranchLast())
		return false;

	BranchWalkInfo wi(*this, true);

	double dist = wi.GetCumBndryDist('1') + wi.GetCumBndryDist('2');

	sg::BoundaryInfo bi = operator()(Target());
	SkelPtCoord skelPt = GetPtCoord(Target());

	sg::Point v0 = bi.first.pt - skelPt;  // get spoke vector 1 from its endpoint
	sg::Point v1 = bi.second.pt - skelPt; // get spoke vector 2 from its endpoint

	double n0 = v0.norm();
	double n1 = v1.norm();

	// Compute the length of the arc defined by the target spokes and the target radius
	if (n0 > 0 && n1 > 0)
		dist += fabs(SignedVectorAngle(v0, v1, n0, n1)) * m_dTargetRadius;

	// It's elbow if dist is smaller than half the circumference at the source
	return (dist < M_PI * m_dSourceRadius);
}

void LigatureSegment::FindMinMaxDistances(const sg::Point& cornerPt,
										  double* pMinDist, double* pMaxDist) const
{
	ASSERT(!IsEmpty());

	const sg::FluxPointList& fpl = m_pBranch->getFluxPoints();
	double dist2;

	*pMinDist = *pMaxDist = fpl[m_first].p.sqDist(cornerPt);

	for (unsigned int i = m_first + 1; i <= m_last; i++)
	{
		dist2 = fpl[i].p.sqDist(cornerPt);

		if (dist2 < *pMinDist)
			*pMinDist = dist2;
		else if (dist2 > *pMaxDist)
			*pMaxDist = dist2;
	}

	*pMinDist = sqrt(*pMinDist);
	*pMaxDist = sqrt(*pMaxDist);
}

/*!
	Visits all ligature points and returns true is any is the results
	of a skeletal gap interpolation
*/
bool LigatureSegment::HasInterpolatedPoints() const
{
	for (unsigned int i = First(); i <= Last(); i++)
	{
		if (operator()(i).IsInterpolated())
			return true;
	}

	return false;
}

/*!
	@brief Finds the segment of points that begins at the given joint and ends
	at the last point that has a distance to the joint smaller that the radius
	of the joint. This segment can be used as a pseudo ligature at junction points.
*/
void LigatureSegment::FindRootedPseudoLigature(SkelBranch* pBranch, SkelJoint* pJoint)
{
	ASSERT(pBranch->getFirstXYPoint() == pJoint->fp.p ||
		   pBranch->getLastXYPoint() == pJoint->fp.p);

	m_pBranch = pBranch;
	m_pJoint = pJoint;

	bool bMoveForward = (pBranch->getFirstXYPoint() == pJoint->fp.p);

	BranchWalkInfo wi(pBranch, bMoveForward);
	unsigned int i, j;
	double r0r0;

	ASSERT(wi.First() != wi.Last());

	i = wi.First();
	r0r0 = wi.Radius(i) * wi.Radius(i);

	for (j = i + wi.Inc(); j != wi.Last(); j += wi.Inc())
		if (wi.SquaredDistance(i, j + wi.Inc()) >= r0r0)
			break;

	wi.ComputeBAR(i, j, &m_cumBAR1, &m_cumBAR2);
	m_dTargetRadius = wi.Radius(j);
	m_dRadiusRatio = m_dTargetRadius / wi.Radius(i);

	// Save the limits such that first < last
	if (i < j)
	{
		m_first = i;
		m_last  = j;
		m_bDecreasing = true;
	}
	else
	{
		m_first = j;
		m_last  = i;
		m_bDecreasing = false;
	}
}