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

#include "stdafx.h"

using namespace dml;

/*!
	@brief Computes the overlap between the discs defined by two skeleton points.

	@return A value in [0,1] defined as overlap_area / smalles_disc_area.
*/
double BranchWalkInfo::ComputeDiskOverlap(unsigned int i, unsigned int j) const
{
	const SkelPt& fp0 = m_fpl[i];
	const SkelPt& fp1 = m_fpl[j];

	double r0 = fabs(fp0.dist);
	double r1 = fabs(fp1.dist);

	double dx = fp1.p.x - fp0.p.x;
	double dy = fp1.p.y - fp0.p.y;

	double c = sqrt(dx * dx + dy * dy); // distance between centers

	if (c <= r0 + r1) // circles overlap
	{
		double CBA, CBD, CAB, CAD, overlapArea, smallArea;
		double r0r0 = r0 * r0;
		double r1r1 = r1 * r1;
		double cc = c * c;

		CBA = acos((r1r1 + cc - r0r0) / (2 * r1 * c));
		CBD = 2 * CBA;

		CAB = acos((r0r0 + cc - r1r1) / (2 * r0 * c));
		CAD = 2 * CAB;

		overlapArea = 0.5 * CBD * r1r1 - 0.5 * r1r1 * sin(CBD) +
					0.5 * CAD * r0r0 - 0.5 * r0r0 * sin(CAD);

		smallArea = (r0 < r1) ? (M_PI * r0r0) : (M_PI * r1r1);

		return overlapArea / smallArea;
	}

	return 0; // circles do not overlap
}