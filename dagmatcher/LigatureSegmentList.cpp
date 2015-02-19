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

#include "LigatureSegmentList.h"
#include "HelperFunctions.h"

//! Minimum ligature segment size considered
#define MIN_LIG_SEGMENT_SIZE    2

using namespace dml;

DBG_ONLY(extern LogFile g_dagCreationLog)

/*!
	@brief Sets the ligature information of each branch incident to a joint point.

	Every ligature segment has as its origin the joint point. The ligature information
	is set for all branches, regardless of whether they do contain ligature points or not.
	To inquery the type of ligature represented by each LigatureSegment element, call the
	functions IsLigature(), IsFullLigature(), and IsSemiLigature().

	@return a list of LigatureSegment objects of size equal to the number of given branches.
*/
void LigatureSegmentList::FindRootedLigature(SkelEdges& branches, SkelJoint* pJoint)
{
	LigatureSegment ls;
	bool bMoveForward;

	ASSERT(branches.size() >= 3);
	ASSERT(branches.size() == pJoint->degree());

	// Make sure that the list is empty
	clear();

	// Init the joint pointer for all ligature segments
	ls.m_pJoint = pJoint;

	// Init all ligature segments
	for (unsigned int i = 0; i < branches.size(); i++)
	{
		ls.m_pBranch = branches[i];

		ASSERT(ls.m_pBranch->getFirstXYPoint() == pJoint->fp.p ||
		   ls.m_pBranch->getLastXYPoint() == pJoint->fp.p);

		bMoveForward = (ls.m_pBranch->getFirstXYPoint() == pJoint->fp.p);

		// Construct a branch walk info object to traverse the branch
		BranchWalkInfo wi(ls.m_pBranch, bMoveForward);

		ls.SetLigatureEndpoint(wi, wi.First());

		push_back(ls);
	}

	// Set the concave corner of each LigatureSegment object in the list. This is done by
	// finding the corner that "may" be associated with each pairs of adjacent
	// branches. Assume that the branches are sorted, such that adjacent elements
	// in the array correspond to adjacent branches, and that the first and
	// last elements are also adjacent branches (as if it were a circular array).
	// This property is enforced by BoundaryPointFinder::AssignBoundaryPoints()

	// Set the concave corner for the pair of first and last branches
	SetConcaveCornerInfo(front(), back());

	// Set the concave corner of all other pairs of adjacent branches
	iterator it1 = begin();
	iterator it0 = it1++;

	for (; it1 != end(); it0++, it1++)
		SetConcaveCornerInfo(*it0, *it1);
}

/*!
	@brief Finds all ligature segments within the given branch segment. A heading
	or trailing ligature segtent is considered to be a "rooted" ligature, and
	so its associated joint is set. Otherwise, this joint is set to NULL.

	@param minLigSegSize minimum size of a ligatiure segment (3 is a good value here)
	@param minNonLigSegSize minimum size of a non-ligature segment (3 is a good value here)
*/
bool LigatureSegmentList::FindLigatureSegments(const BranchSegment& bs)
{
	clear();

	if (bs.Size() < 2 || bs[bs.First()].p.dist(bs[bs.Last()].p) < 1)
		return false;

	BranchWalkInfo wi(bs, true);
	LigatureSegment ls, bestLigSeg, mergeLigSeg;
	unsigned int i;
	bool bOverlappingState;
	bool bFirstPass = true;

	// Init the ligature segments' branch pointer
	ls.m_pBranch = bs.m_pBranch;
	mergeLigSeg.m_pBranch = bs.m_pBranch;

	DBG_C_LOG_BEGIN_GROUP
	//DBG_C_LOG(DBG_VAL(minLigSegSize) << DBG_VAL(minNonLigSegSize))
	//DBG_PRINT2(ls.m_pBranch->n1->fp.p.x, ls.m_pBranch->n2->fp.p.x)

	CornerArea ca1, ca2;

	if (bs.First() != bs.BranchFirst())
	{
		ca1.SetFirst(bs(bs.First())['1'].pt);
		ca2.SetFirst(bs(bs.First())['2'].pt);
	}

	if (bs.Last() != bs.BranchLast())
	{
		ca1.SetLast(bs(bs.Last())['1'].pt);
		ca2.SetLast(bs(bs.Last())['2'].pt);
	}

	while (true)
	{
		// Keep track of the overlaps between ligature segments. If the beggining of
		// a 'candidate' ligature segment is within the limits of a previously found
		// ligature, then there is overlap bettwen the segments.
		bOverlappingState = false;

		// Eveluate every point in the given branch segment as a potential start of
		// a ligature segment
		for (i = wi.First(); i != wi.Last(); i += wi.Inc())
		{
			// Check for a change in the overlapping state from true to 'out of range'
			if (bOverlappingState && !bestLigSeg.IsInside(i))
			{
				// Flush current best ligature segment
				push_back(bestLigSeg);

				// Update the overlapping state to false
				bOverlappingState = false;
			}

			// Evaluate the ligature segment that begins at point i
			//ls.FindLigature(wi, i);
			ls.SetLigatureEndpoint(wi, i);

			// Ignore segments that are too small
			if (ls.Size() < MIN_LIG_SEGMENT_SIZE)
				continue;

			// If there is overlap, check whether the segment is worth considering
			if (bOverlappingState)
			{
				if (ls.m_last <= bestLigSeg.m_last // it's fully contained in best segment
					|| ls.m_dRadiusRatio > bestLigSeg.m_dRadiusRatio // worse ratio
					|| (ls.m_dRadiusRatio == bestLigSeg.m_dRadiusRatio &&
						ls.Size() <= bestLigSeg.Size()) // same ratio but shorter segment
					)
				{
					// Skip the consideration of this "potential" ligature segment
					continue;
				}
			}

			// Compute the conrner information of the segment
			if (ls.HasLigatureLikeAttributes('1'))
				ls.SetCornerInfo('1', ca1);

			if (ls.HasLigatureLikeAttributes('2'))
				ls.SetCornerInfo('2', ca2);

			// Check if it's ligature, and if so update the best segment
			if (ls.IsLigature())
			{
				bestLigSeg = ls;
				bOverlappingState = true; // simply do this even if already true
			}
		}

		// Flush the possibly remaining best segment
		if (bOverlappingState)
		{
			push_back(bestLigSeg);
		}

		// Reverse direction if it's the first pass or finish otherwise
		if (bFirstPass)
		{
			wi.ReverseDirection();

			bFirstPass = false;
		}
		else
		{
			break;
		}
	}

	sort(CompareBranchSegment());

	// Check for strange overlaps of segments (can happen in some weird cases)
	for (iterator it0, it1 = begin(); it1 != end(); )
	{
		it0 = it1++; // increment it1 for next iteration here

		if (it1 != end() && it0->Last() >= it1->First())
		{
			DBG_C_LOG("WARNING: there is a weird overlap of ligature segments")
			erase(it1);
			erase(it0);
			it1 = begin(); // start again
		}
		// If there is no overlap, but last and first leave no
		// point in between, we have an empty bone
		else if (it1 != end() && it0->Last() + 1 == it1->First())
		{
			DBG_C_LOG("WARNING: ligature segments define an empty bone")

			// Define a non-empty bone by shrinking either the non-terminal segment
			// or, if both are non-terminal, the longest one.
			bool terminal0 = (it0->First() == it0->BranchFirst());
			bool terminal1 = (it1->Last() == it1->BranchLast());

			if (it0->Size() <= 1)
			{
				erase(it0);
				it1 = begin(); // start again
			}
			else if (it1->Size() <= 1)
			{
				erase(it1);
				it1 = begin(); // start again
			}
			else
			{
				ASSERT(it0->Size() > 1 && it1->Size() > 1);

				if (terminal0 && !terminal1)
					it1->m_first++;
				else if (!terminal0 && terminal1)
					it0->m_last--;
				else if (it0->Size() < it1->Size())
					it1->m_first++;
				else
					it0->m_last--;
			}
		}
	}

	if (!empty())
	{
		// If the first point of the segment is a joint, there may be an instability in the labeling that
		// can be easily corrected by ignoring these labels.
		// First, remove small non-lig segments at the front of the list...
		if (bs.First() == bs.BranchFirst() && front().m_first != bs.BranchFirst() &&
			front().DecreasesForward() && bs.BranchFirstPtDegree() > 1)
		{
			BranchSegment nonLig(bs.m_pBranch, bs.First(), front().m_first - 1);

			if (nonLig.Size() == 1 || nonLig.IsCornerSegment(true))
			{
				front().m_first = bs.First();
				DBG_C_LOG("WARNING: heuristicaly ignoring front non-lig joint point")
			}
		}

		// ...then, remove small non-lig segments at the back of the list
		if (bs.Last() == bs.BranchLast() && back().m_last != bs.BranchLast() &&
			back().DecreasesBackwards() && bs.BranchLastPtDegree() > 1)
		{
			BranchSegment nonLig(bs.m_pBranch, back().m_last + 1, bs.Last());

			if (nonLig.Size() == 1 || nonLig.IsCornerSegment(false))
			{
				back().m_last = bs.Last();
				DBG_C_LOG("WARNING: heuristicaly ignoring back non-lig joint point")
			}
		}

		// Finally, ignore whole ligature branches
		if (front().m_first == bs.First() && front().m_last == bs.Last())
		{
			clear();
			DBG_C_LOG("WARNING: heuristicaly ignoring whole lig branch")
		}
		else if (front().m_first == bs.BranchFirst())
		{
			if (GetFirstJoint(bs.m_pBranch)->degree() > 1)
				front().m_pJoint = GetFirstJoint(bs.m_pBranch); // rooted ligature
			else
				pop_front(); // lig ends at terminal point
		}
		else if (back().m_last == bs.BranchLast())
		{
			if (GetLastJoint(bs.m_pBranch)->degree() > 1)
				back().m_pJoint = GetLastJoint(bs.m_pBranch); // rooted ligature
			else
				pop_back(); // lig ends at terminal point
		}
	}

	DBG_C_LOG(DBG_VAL(size()))

	return !empty();
}

void LigatureSegmentList::SetConcaveCornerInfo(LigatureSegment& ls0, LigatureSegment& ls1)
{
	// It must be true that the lig segs meet at their source points
	ASSERT(ls0[ls0.Source()].p == ls0.m_pJoint->fp.p);
	ASSERT(ls1[ls1.Source()].p == ls1.m_pJoint->fp.p);
	ASSERT(ls0.m_pJoint->fp.p == ls1.m_pJoint->fp.p);

	std::pair<char,char> sides;

	sides = GetSidesOfSharedBndryPt(ls0.m_pBranch, ls1.m_pBranch, ls0.m_pJoint);

	// First, determine whether we should bother looking for a corner. ie, the
	// ligature segment attributes must look like a valid ligature segment
	bool isLig0 = ls0.HasLigatureLikeAttributes(sides.first);
	bool isLig1 = ls1.HasLigatureLikeAttributes(sides.second);

	DBG_C_LOG_NEW_LINE

	DBG_C_LOG("Branches: " << PRN_BRANCH_PTR(ls0.m_pBranch)
		<< ", " << PRN_BRANCH_PTR(ls1.m_pBranch))

	DBG_C_LOG("Ligature statuts: segment0 = " << isLig0 << " segment1 = " << isLig1)

	// If either one of the segments is a potential (semi) ligature, we search for
	// the concave corner that should exist to confirm the presense of ligature
	if (isLig0 || isLig1)
	{
		BoundaryIntervalWithGaps bi0(ls0.m_pBranch,
			LigatureSegment::s_pBndryPtFinder->GetOriginalPoints());

		BoundaryIntervalWithGaps bi1(ls1.m_pBranch,
			LigatureSegment::s_pBndryPtFinder->GetOriginalPoints());

		// Make sure that we have valid intervals
		ASSERT(bi0.IsValid() && bi1.IsValid());

		//DBG_PRINT2(ls0.DecreasesForward(), ls1.DecreasesForward())

		// Set limits, sides, an the direction in which boundary points must
		// be traversed. One of the ligature segments must arrive at the joint
		// point and the other must depart from the branch joint
		SkelPtIndex jointIdx0 = GetEndpointIndex(ls0.m_pBranch, ls0.m_pJoint);
		ASSERT(jointIdx0 == ls0.m_first || jointIdx0 == ls0.m_last);
		bool bInvertIndices0 = (jointIdx0 != ls0.m_last); // invert if doens't arrive at joint

		bi0.SetLimits(ls0.m_first, ls0.m_last, sides.first, bInvertIndices0);

		SkelPtIndex jointIdx1 = GetEndpointIndex(ls1.m_pBranch, ls1.m_pJoint);
		ASSERT(jointIdx1 == ls1.m_first || jointIdx1 == ls1.m_last);
		bool bInvertIndices1 = (jointIdx1 != ls1.m_first);// invert if doens't depart from joint

		bi1.SetLimits(ls1.m_first, ls1.m_last, sides.second, bInvertIndices1);

		//unsigned int sz0 = bi0.BoundaryPointCount(bi0.First(), bi0.Last());
		//unsigned int sz1 = bi1.BoundaryPointCount(bi1.First(), bi1.Last());
		//DBG_PRINT2(sz0, sz1)

		//DBG_PRINT2(ls0.DecreasesForward(), !ls1.DecreasesForward())
		//DBG_PRINT2(bInvertIndices0, bInvertIndices1)

		//ASSERT(bi0.GetBoundaryPoint(bi0.Last()) == bi1.GetBoundaryPoint(bi1.First()));

		BoundaryCorner bc;

		// Use one of the corner objects to search for a corner and check
		// its concavity wrt the joint point (ie, ligature source)
		bc.FindCorner(bi0, bi1, LigatureSegment::s_minCornerCosAngle.loose,
			&ls0.m_pJoint->fp.p);

		ls0.SetCornerInfo(sides.first, bc);
		ls1.SetCornerInfo(sides.second, bc);

		DBG_C_LOG("Corner found: " << bc)
	}
}