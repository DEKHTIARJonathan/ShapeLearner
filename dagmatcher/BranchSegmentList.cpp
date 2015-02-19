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

#include "BranchSegmentList.h"
#include "HelperFunctions.h"

using namespace dml;

DBG_ONLY(extern LogFile g_dagCreationLog)

/*!
	@brief Moves the elements of 'bsl' to the front or back of the list. The
	resulting list has all skeleton points in all segments flowing in the
	same direction. The flow direction is set be be that of (*this). This means
	that only the flow direction of 'bsl' may be changed.

	@return either 'f' or 'b', indicating whether bsl was spliced at the front or
	        at the back.

	Note: some segments need to be "inverted" when concatenation occurrs. However,
	the skeleton data cannot be modified and so instead, a flag is set in each
	segment to indicate whether the flux points are inverted or not. To simpify the
	iteration through possibly inverted flux points, the operator[] of BranchSegment
	is overloaded and can be used together with the special functions First() and Last().
*/
BranchGapInfo* BranchSegmentList::Splice(BranchSegmentList& bsl)
{
	char pos;

	// Create handy aliases
	const SkelPtCoord& b0p0 = FirstXYPoint();
	const SkelPtCoord& b0pN = LastXYPoint();

	const SkelPtCoord& b1p0 = bsl.FirstXYPoint();
	const SkelPtCoord& b1pN = bsl.LastXYPoint();

	// Flux point indices must end up ordered from either the left or the right.
	// There are 4 cases -> N...0~N...0; N...0~0...N; 0...N~0...N; 0...N~N...0

	if (b0p0 == b1pN) // case: N...0~N...0
	{
		splice(begin(), bsl);
		pos = 'f';
	}
	else if (b0p0 == b1p0) // case: N...0~0...N
	{
		bsl.InvertFlowDirection();
		splice(begin(), bsl);
		pos = 'f';
	}
	else if (b0pN == b1p0) // case: 0...N~0...N
	{
		splice(end(), bsl);
		pos = 'b';
	}
	else // case: 0...N~N...0
	{
		ASSERT(b0pN == b1pN);

		bsl.InvertFlowDirection();
		splice(end(), bsl);
		pos = 'b';
	}

	// Add a placeholder for the gap info at either the front or the back,
	// and save the pointer to it. Then, splice the two gap info lits
	BranchGapInfo* pGapInfo;

	if (pos == 'f')
	{
		m_branchGaps.push_front(BranchGapInfo());
		pGapInfo = &m_branchGaps.front();
		m_branchGaps.splice(m_branchGaps.begin(), bsl.m_branchGaps);
	}
	else
	{
		m_branchGaps.push_back(BranchGapInfo());
		pGapInfo = &m_branchGaps.back();
		m_branchGaps.splice(m_branchGaps.end(), bsl.m_branchGaps);
	}

	ASSERT(m_branchGaps.size() == size() - 1);

	return pGapInfo;
}