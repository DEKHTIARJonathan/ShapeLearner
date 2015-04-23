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

#ifndef __LIGATURE_SEGMENT_LIST_H__
#define __LIGATURE_SEGMENT_LIST_H__

#include "stdafx.h"



namespace dml {
class LigatureSegmentList : public std::list<LigatureSegment>
{
public:
	typedef std::list<LigatureSegment>::iterator iterator;
	typedef std::list<LigatureSegment>::const_iterator const_iterator;

protected:
	void SetConcaveCornerInfo(LigatureSegment& ls0, LigatureSegment& ls1);

public:
	bool FindLigatureSegments(const BranchSegment& bs);

	void FindRootedLigature(SkelEdges& branches, SkelJoint* pJoint);

	//! Copy the branch pointers to the given branch array
	void CopyTo(SkelEdges& branchPtrs) const
	{
		branchPtrs.clear();
		branchPtrs.reserve(size());

		for (const_iterator it = begin(); it != end(); it++)
			branchPtrs.push_back(it->m_pBranch);
	}

	iterator FirstDifferentThan(const SkelBranch* pBranch0,
		                        const SkelBranch* pBranch1 = NULL)
	{
		for (iterator it = begin(); it != end(); it++)
			if (it->m_pBranch != pBranch0 && it->m_pBranch != pBranch1)
				return it;

		return end();
	}
};
} // namespace dml

#endif //__LIGATURE_SEGMENT_LIST_H__
