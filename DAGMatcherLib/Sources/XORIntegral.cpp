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

#include "XORIntegral.h"

using namespace dml;

double XORIntegral::FunctionSimilarity(const double& slopeSigma) const
{
	double w, a, b, areaDiff, sim, totalSimilarity = 0;
	double m0, m1, dm, area, areaSum, slopeSim;

	const double L = MAX(m_pts0.back().x, m_pts1.back().x)
		- MIN(m_pts0.front().x, m_pts1.front().x);

	ASSERT(L > 0);

	const_iterator itB = begin();
	const_iterator itA = itB++;

	for (; itB != end(); itA++, itB++)
	{
		w = itB->x - itA->x;
		a = itA->Dy(*itB); // get hight wrt next side
		b = itB->Dy(*itA); // get hight wrt previous side

		ASSERT(w >= 0 && a >= 0 && b >= 0);

		// Compute the area of the DIFF trapezoid
		areaDiff = w * (a + b) / 2;

		if (HasFunctionOverlap(*itA, *itB))
		{
			// Sum the areas of each function's trapezoid
			areaSum = (w / 2) * (itA->y0 + itB->y0 + itA->y1 + itB->y1);

			m0 = (itB->y0 - itA->y0) / w;
			m1 = (itB->y1 - itA->y1) / w;

			dm = fabs(m0 - m1);

			//sigma = 0.15;

			if ((m0 >=0 && m1 >= 0) || (m0 <= 0 && m1 <= 0))
			{
				slopeSim = exp(-(dm * dm) / (2 * slopeSigma * slopeSigma));
			}
			else
			{
				//slopeSim = 0;
				slopeSim = exp(-(dm * dm) / (2 * slopeSigma * slopeSigma));
			}

			area = slopeSim * areaDiff + (1 - slopeSim) * areaSum;

			sim = 1 - area / areaSum;

			totalSimilarity += sim * w / L;
		}
	}

	return totalSimilarity;
}