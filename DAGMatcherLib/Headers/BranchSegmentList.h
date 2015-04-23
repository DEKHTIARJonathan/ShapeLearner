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

#ifndef __BRANCH_SEGMENT_LIST_H__
#define __BRANCH_SEGMENT_LIST_H__

#include "stdafx.h"

namespace dml {
/*!
	A gap is formed at an attachment point beween a souce branch and N target
	branches, for N >= 1. Each attachment i = 1,...,N is associated with
	an edge in a GSG. The BranchGapInfo is the set of N edges associated with
	a gap or attachment point.
*/
typedef std::vector<leda::edge> BranchGapInfo;

/*!
	A branch can have any number of gaps, whci in turn can be associated with
	a strictly positive number of attachments. The BranchGapList is the ordered
	list of gaps along a branch.
*/
typedef std::list<BranchGapInfo> BranchGapList;

/*!
	@brief Its simply a STL list of BranchSegment with the additional
	functionality for splicing two lists.
*/
class BranchSegmentList : public std::list<BranchSegment>
{
	BranchGapList m_branchGaps;

protected:
	//! Inverts all skeleton points in all segements by setting their inverted flag
	void InvertFlowDirection()
	{
		for (iterator it = begin(); it != end(); it++)
			it->m_bInverted = !it->m_bInverted;

		reverse();
		m_branchGaps.reverse();
	}

	/*!
		Gets the xy coordinate of the first skeleton point in the BRANCH
		associated with the first branch segment while accounting for the
		'inverted' flag of the segment.

		NOTE: this is NOT the first point of the first SEGMENT.
	*/
	const sg::Point& FirstXYPoint() const
	{
		const BranchSegment& b0 = front();
		return (b0.m_bInverted) ? b0.m_pBranch->getLastXYPoint():b0.m_pBranch->getFirstXYPoint();
	}

	/*!
		Gets the xy coordinate of the last skeleton point in the BRANCH
		associated with the last branch segment while accounting for the
		'inverted' flag of the segment.

		NOTE: this is NOT the last point of the last SEGMENT.
	*/
	const sg::Point& LastXYPoint() const
	{
		const BranchSegment& bN = back();
		return (bN.m_bInverted) ? bN.m_pBranch->getFirstXYPoint():bN.m_pBranch->getLastXYPoint();
	}

public:
	typedef std::list<BranchSegment>::iterator iterator;
	typedef std::list<BranchSegment>::const_iterator const_iterator;

	BranchGapInfo* Splice(BranchSegmentList& bsl);

	unsigned int GapSize() const       { return m_branchGaps.size(); }
	BranchGapList::iterator GapBegin() { return m_branchGaps.begin(); }
	BranchGapList::iterator GapEnd()   { return m_branchGaps.end(); }

	/*!
		@brief Subtracts the ligature segment from either the first
		or last element in the list of branch segments.
	*/
	void SubtractLigatureInterval(const LigatureSegment& ls)
	{
		if (front().m_pBranch == ls.m_pBranch)
			front().SubtractLigatureInterval(ls);
		else
			back().SubtractLigatureInterval(ls);
	}

	/*!
		@brief Adds up the size of each BranchSegment in the list
	*/
	unsigned int Size() const
	{
		unsigned int sz = 0;

		for (const_iterator it = begin(); it != end(); it++)
			sz += it->Size();

		return sz;
	}

	/*!
		@brief Adds up the total size of each BRANCH in the list
	*/
	unsigned int TotalBranchSize() const
	{
		unsigned int sz = 0;

		for (const_iterator it = begin(); it != end(); it++)
			sz += it->m_pBranch->size();

		return sz;
	}

	/*!
		@brief Returns the endpoints of the barcnh segment list.

		The pointers to the skeletal joint endpoints are set such that
		*ppEndpt1 is a pointer to the endpoint associated with the FIRST
		point of the first segment in the list, and *ppEndpt2 points to the
		endpoint associated with the LAST point of the last segment in the list.
	*/
	void GetEndpoints(SkelJoint** ppEndpt1, SkelJoint** ppEndpt2) const
	{
		ASSERT(!empty());

		if (size() == 1)
		{
			const BranchSegment& bs = front();

			if (bs[bs.BranchFirst()].p == bs.m_pBranch->n1->fp.p)
			{
				*ppEndpt1 = bs.m_pBranch->n1;
				*ppEndpt2 = bs.m_pBranch->n2;
			}
			else
			{
				*ppEndpt1 = bs.m_pBranch->n2;
				*ppEndpt2 = bs.m_pBranch->n1;
			}
		}
		else
		{
			const_iterator it0 = begin();
			const_reverse_iterator itN = rbegin();

			SkelBranch* b0 = (it0)->m_pBranch;
			SkelBranch* bN = (itN)->m_pBranch;

			SkelJoint* j1 = GetCommonSkelJoint(b0, (++it0)->m_pBranch);
			SkelJoint* j2 = GetCommonSkelJoint(bN, (++itN)->m_pBranch);

			*ppEndpt1 = (b0->n1 != j1) ? b0->n1 : b0->n2;
			*ppEndpt2 = (bN->n1 != j2) ? bN->n1 : bN->n2;
		}
	}

	void PushBackPtCoords(SmartArray<POINT>& pts) const
	{
		for (const_iterator it = begin(); it != end(); it++)
			it->PushBackPtCoords(pts);
	}
};
} // namespace dml

#endif //__BRANCH_SEGMENT_LIST_H__
