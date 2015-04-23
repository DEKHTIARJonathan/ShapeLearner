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

#ifndef __PARTNER_BRANCH_H__
#define __PARTNER_BRANCH_H__

#include "stdafx.h"

namespace dml {
/*!
	A branch B is the parner of branch A if both branches
	are incident on joint node J and, B can be rotated, wrt J,
	to be aligned with A without going over other adjacent branches.

	The order in the array of branches of a joint node is defind such
	that parner branches ara adjacent in the array (assuming circularity
	by endpoint adjacency). This property is enforced by the function
	BoundaryPointFinder::AssignBoundaryPoints.

	Each branch endpoint can define either two or zero parner branches.
	When there are two parner branches, B1 and B2, each one of them can be
	identified by the side of branch A with which they share the spoke
	that emanates from the common joint.

	@see BoundaryPointFinder::AssignBoundaryPoints
*/
class PartnerBranch
{
public:
	SkelBranch* m_pSrcBranch; //!< The branch given to set this object
	SkelJoint* m_pSrcJoint;   //!< The joint given to set this object
	char m_srcSide;           //!< The given branch side to set this object

	SkelBranch* m_pBranch;    //!< The parner branch of m_pSrcBranch
	char m_side;              //!< Side of the partner branch with shared spoke

	PartnerBranch() { m_pBranch = NULL; }

	//! The the parner branch attributes
	void Set(SkelBranch* pBranch, char side)
	{
		m_pBranch    = pBranch;
		m_side       = side;
	}

	//! Set the attributes of the given source branch
	void SetSourceInfo(SkelJoint* pSrcJoint, SkelBranch* pSrcBranch, char srcSide)
	{
		m_pSrcJoint  = pSrcJoint;
		m_pSrcBranch = pSrcBranch;
		m_srcSide    = srcSide;
	}

	/*!
		Finds the adjacent branch on in the joint node's array that
		has a spoke whose endpoint is equal to the endpoint of the spoke
		of branch 'pSrcBranch' on side 'side' at joint point 'pJoint'.
	*/
	bool FindPartnerBranchOf(SkelBranch* pSrcBranch, SkelJoint* pJoint, char side)
	{
		ASSERT(pJoint->degree() > 1);

		// Save the given parameters
		SetSourceInfo(pJoint, pSrcBranch, side);

		// Get the boundary info of the given branch at the given joint point
		SkelPtIndex i = GetEndpointIndex(pSrcBranch, pJoint);
		const sg::BoundaryPoint& bp = pSrcBranch->getBoundaryInfo(i)[side];

		unsigned int idx, predIdx, succIdx;

		// Find the index of the branch in the node's array
		idx = FindIncidentBranchIndex(pJoint, pSrcBranch);

		ASSERT(idx >= 0 && idx < pJoint->degree());

		// Get the predecessor branch assuming circular adjacency
		predIdx = (idx == 0) ? pJoint->degree() - 1 : idx - 1;

		// Get the succesor branch assuming circular adjacency
		succIdx = (idx == pJoint->degree() - 1) ? 0 : idx + 1;

		SkelBranch* adjBr[2] = {pJoint->getEdge(predIdx),
							 	pJoint->getEdge(succIdx)};

		// See which of the two branch has the shared spoke
		for (SkelBranch** ppBr = adjBr; ppBr <= adjBr + 1; ppBr++)
		{
			i = GetEndpointIndex(*ppBr, pJoint);

			const sg::BoundaryInfo& bi = (*ppBr)->getBoundaryInfo(i);

			// Check both side of the branch
			if (bi.first.pt == bp.pt)
			{
				Set(*ppBr, '1');
				return true;
			}
			else if (bi.second.pt == bp.pt)
			{
				Set(*ppBr, '2');
				return true;
			}
		}

		return false;
	}

	/*!
	*/
	sg::BoundaryPoint& GetSourceBoundaryPoint()
	{
		SkelPtIndex i = GetEndpointIndex(m_pSrcBranch, m_pSrcJoint);

		return m_pSrcBranch->getBoundaryInfoList()[i][m_srcSide];
	}

	/*!
	*/
	sg::BoundaryPoint& GetFirstBoundaryPoint()
	{
		SkelPtIndex i = GetEndpointIndex(m_pBranch, m_pSrcJoint);

		return m_pBranch->getBoundaryInfoList()[i][m_side];
	}

	/*!
	*/
	sg::BoundaryPoint& GetLastBoundaryPoint()
	{
		SkelPtIndex i = GetOtherEndpointIndex(m_pBranch, m_pSrcJoint);

		return m_pBranch->getBoundaryInfoList()[i][m_side];
	}

	/*!
	*/
	int GetFirstBoundaryPointIndex() const
	{
		ASSERT(m_pBranch != NULL);

		SkelPtIndex i = GetEndpointIndex(m_pBranch, m_pSrcJoint);

		return m_pBranch->getBoundaryInfo(i)[m_side].index;
	}

	/*!
	*/
	int GetLastBoundaryPointIndex() const
	{
		ASSERT(m_pBranch != NULL);

		SkelPtIndex i = GetOtherEndpointIndex(m_pBranch, m_pSrcJoint);

		return m_pBranch->getBoundaryInfo(i)[m_side].index;
	}

	/*!
		@brief Replaces the spoke endpoint shared by the partner branch
		and the source branch.
	*/
	void MoveSharedSpoke(const sg::BoundaryPoint& newBndryPt)
	{
		// Make sure that nothing has changed in src and parner
		ASSERT(GetSourceBoundaryPoint().pt == GetFirstBoundaryPoint().pt);

		GetSourceBoundaryPoint() = newBndryPt;

		GetFirstBoundaryPoint() = newBndryPt;
	}
};
} // namespace dml

#endif //__PARTNER_BRANCH_H__
