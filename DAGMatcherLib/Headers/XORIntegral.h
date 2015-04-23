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

#ifndef __XOR_INTEGRAL_H__
#define __XOR_INTEGRAL_H__

#include "stdafx.h"

namespace dml {
/*
	Side of trapezoid at position X of a given pair of functions. The side
	has a height |y0-y1|.

	\image html TrapezoidSide.gif
*/
struct TrapezoidSide
{
	double x, y0, y1;

	TrapezoidSide() { }

	/*!
		Values of functions 0 and 1 at x. The functions must be positive, or -1 to
		indicate that there is no overlap at x. Only one function can have y == -1 at x.
	*/
	TrapezoidSide(const double& x_coord, const double& y0_coord, const double& y1_coord)
	{
		// The function must be positive, or -1 to indicate that there is no overlap at x
		ASSERT(y0_coord >= 0 || y0_coord == -1);
		ASSERT(y1_coord >= 0 || y1_coord == -1);

		// Only one function can have y == -1 at x
		ASSERT(y0_coord != -1 || y1_coord != -1);

		x  = x_coord;
		y0 = y0_coord;
		y1 = y1_coord;
	}

	//! Returns true iff both y0 and y1 are defined
	bool HasOverlap() const
	{
		return (y0 != -1 && y1 != -1);
	}

	/*!
		Computes the length of the side as a function of a given adjacent side.
		If the side or the given adjacent side has a y = -1, ie, a missing
		function value, then the y of the non missing function is used.

		\image html TrapezoidSide_Dy.gif
	*/
	double Dy(const TrapezoidSide& adjTS) const
	{
		if (y0 == -1 || adjTS.y0 == -1)
			return y1;
		else if (y1 == -1 || adjTS.y1 == -1)
			return y0;
		else
			return fabs(y0 - y1);
	}
};

/*!
	@brief Represents the non-overlaping area below two functions.
*/
class XORIntegral : public std::list<TrapezoidSide>
{
	typedef std::vector<POINT> ControlPoints;

	ControlPoints m_pts0, m_pts1;

	void CreateTrapezoids()
	{
		ASSERT(empty());
		ASSERT(!m_pts0.empty() && !m_pts1.empty());

		ControlPoints::const_iterator it0 = m_pts0.begin();
		ControlPoints::const_iterator it1 = m_pts1.begin();

		// Add heading non-overlapping points. Ensure that
		// only one of the two foor-loops is executed.
		if (it0->x < it1->x)
		{
			for (; it0 != m_pts0.end() && it0->x < it1->x; it0++)
				Add(it0->x,  it0->y, -1);
		}
		else if (it1->x < it0->x)
		{
			for (; it1 != m_pts1.end() && it1->x < it0->x; it1++)
				Add(it1->x, -1, it1->y);
		}

		// Add overlapping points
		while (it0 != m_pts0.end() && it1 != m_pts1.end())
		{
			if (it0->x == it1->x)
			{
				Add(it0->x, it0->y, it1->y);
				it0++;
				it1++;
			}
			else if (it0->x < it1->x)
			{
				Add(it0->x, it0->y, *it1);
				it0++;
			}
			else // ie (it0->x > it1->x)
			{
				Add(it1->x, *it0, it1->y);
				it1++;
			}
		}

		// Add trailing non-overlapping points
		// (only one of the two for-loops should be executed)
		for (; it0 != m_pts0.end(); it0++)
			Add(it0->x,  it0->y, -1);

		for (; it1 != m_pts1.end(); it1++)
			Add(it1->x, -1, it1->y);
	}

	void Add(const double& x, const double& y0, const double& y1)
	{
		push_back(TrapezoidSide(x, y0, y1));
	}

	void Add(const double& x, const double& y0, const POINT& pt1)
	{
		ASSERT(y0 >= 0);    // we assume that the functions are positive
		ASSERT(x != pt1.x); // call other Add function if x == pt1.x
		ASSERT(!empty() && back().y1 != -1); // there must be a prev pt on f1
		ASSERT(x > back().x); // pts are added in ascending order of x-coord

		double y1 = GetLinePoint(x, back().x, back().y1, pt1.x, pt1.y);

		if (back().y0 >= 0)
			CheckCrossingAndAdd(x, y0, y1);
		else
			Add(x, y0, y1);
	}

	void Add(const double& x, const POINT& pt0, const double& y1)
	{
		ASSERT(y1 >= 0);    // we assume that the functions are positive
		ASSERT(x != pt0.x); // call other Add function if x == pt0.x
		ASSERT(!empty() && back().y0 != -1); // there must be a prev pt on f0
		ASSERT(x > back().x); // pts are added in ascending order of x-coord

		double y0 = GetLinePoint(x, back().x, back().y0, pt0.x, pt0.y);

		if (back().y1 >= 0)
			CheckCrossingAndAdd(x, y0, y1);
		else
			Add(x, y0, y1);
	}

	void CheckCrossingAndAdd(const double& x, const double& y0, const double& y1)
	{
		ASSERT(!empty() && back().y0 >= 0 && back().y1 >= 0);

		TrapezoidSide ts(x, y0, y1);
		POINT midPt;

		// See if there is a crossing
		if (HasCrossing(back(), ts, &midPt))
			push_back(TrapezoidSide(midPt.x, midPt.y, midPt.y));

		push_back(ts);
	}

	static void SetControlPoint(POINT& pt, const double& x, const double& y)
	{
		WARNING2(y <= -2, "Strange negative y-coordinate", x, y);
		pt.Set(x, (y >= 0) ? y : 0);
	}

	static void GetControlPoints(ControlPoints& pts, const LineSegmentArray& f)
	{
		if (f.Size() == 0)
		{
			pts.clear();
		}
		else
		{
			pts.resize(f.Size() + 1);

			// Add first point
			SetControlPoint(pts.front(), f.GetHead().X0(), f.GetHead().Y0());

			// Make sure that we don't have rounding errors with the first x coord
			if (pts.front().x > -0.001 && pts.front().x < 0.001)
				pts.front().x = 0;

			// Add middle points
			for (int i = 1; i < f.Size(); i++)
			{
				const LineSegment& ls0 = f[i - 1];
				const LineSegment& ls1 = f[i];

				ASSERT(ls0.X1() == ls1.X0());

				SetControlPoint(pts[i], ls0.X1(), (ls0.Y1() + ls1.Y0()) / 2.0);
			}

			// Add last point
			SetControlPoint(pts.back(), f.GetTail().X1(), f.GetTail().Y1());
		}
	}

	static double GetLinePoint(const double& x, const double& x0, const double& y0,
		const double& x1, const double& y1)
	{
		double m = (y1 - y0) / (x1 - x0);
		double b = y1 - m * x1;

		return m * x + b;
	}

	static bool HasCrossing(const TrapezoidSide& a, const TrapezoidSide& b, POINT* p)
	{
		if ((a.y0 > a.y1 && b.y0 < b.y1) || (a.y0 < a.y1 && b.y0 > b.y1))
		{
			double dx = b.x - a.x;

			double m0 = (b.y0 - a.y0) / dx;
			double b0 = b.y0 - m0 * b.x;

			double m1 = (b.y1 - a.y1) / dx;
			double b1 = b.y1 - m1 * b.x;

			p->x = (b1 - b0) / (m0 - m1);
			p->y = m0 * p->x + b0;

			ASSERT(p->x + 0.001 > a.x && p->x < b.x  + 0.001);
			ASSERT(fabs(p->y - (m1 * p->x + b1)) < 0.0001);

			return true;
		}

		return false;
	}

	static double FunctionArea(const ControlPoints& pts)
	{
		double area, totalArea = 0;

		if (!pts.empty())
		{
			ControlPoints::const_iterator itB = pts.begin();
			ControlPoints::const_iterator itA = itB++;

			for (; itB != pts.end(); itA++, itB++)
			{
				ASSERT(itB->x > itA->x);

				// Compute the area of the trapezoid
				area = (itB->x - itA->x) * (itA->y + itB->y) / 2;

				ASSERT(area >= 0);

				totalArea += area;
			}
		}

		return totalArea;
	}

public:
	typedef std::list<TrapezoidSide>::const_iterator const_iterator;

	XORIntegral(const LineSegmentArray& func0, const LineSegmentArray& func1)
	{
		ASSERT(func0.Size() > 0 && func1.Size() > 0);

		GetControlPoints(m_pts0, func0);
		GetControlPoints(m_pts1, func1);

		CreateTrapezoids();
	}

	//! ts0 and ts1 must be adjacent sides
	bool HasFunctionOverlap(const TrapezoidSide& ts0, const TrapezoidSide& ts1) const
	{
		return (ts0.HasOverlap() && ts1.HasOverlap());
	}

	/*!
		Computes the area under the function that is
		uniquely represented by one of the functions.

		If height is the distance between the parallel sides a and b,
		the	area of each trapezoind is: A = h (a + b) / 2. In our case,
		the parallel sides are vertical, so the height is really the width,
		and the area is written A = w (a + b) / 2, with w = x_{i+1} - x_{i},
		a = |y1_{i+1} - y0_{i+1}| and b = |y1_{i} - y0_{i}|.

		There is a special case when the function don't overlap. In that case,
		either y1 == -1 or y0 == -1, and the area is A = w (yk_{i+1} + yk_{i}) / 2,
		where k = {0,1}, s.t. yk != -1.

		\image html XORIntegral_Area.gif
	*/
	double XORArea() const
	{
		double w, a, b, areaDiff, totalArea = 0;
		//double f0area, f1area, maxFuncArea;
		double m0, m1, dm, area, areaSum, slopeSim, sigma;

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

				if ((m0 >=0 && m1 >= 0) || (m0 <= 0 && m1 <= 0))
				{
					sigma = 0.5;
					slopeSim = exp(-(dm * dm) / sigma);
				}
				else
				{
					slopeSim = 0;
				}

				area = slopeSim * areaDiff + (1 - slopeSim) * areaSum;
			}
			else
			{
				area = areaDiff;
			}

			totalArea += area;

			/*f0area = w * (itA->y0 + itB->y0) / 2;
			f1area = w * (itA->y1 + itB->y1) / 2;

			maxFuncArea = MAX(f0area, f1area);
			ASSERT(area <= maxFuncArea);

			if (area > 0)
				totalArea += (area) * (1 + area / maxFuncArea);*/
		}

		return totalArea;
	}

	double FunctionSimilarity(const double& slopeSigma) const;

	double FunctionArea(int i) const
	{
		ASSERT(i == 0 || i == 1);

		return FunctionArea((i == 0) ? m_pts0 : m_pts1);
	}

	static double FunctionArea(const LineSegmentArray& piecewiseLinearFunc)
	{
		ControlPoints ctrlPts;

		GetControlPoints(ctrlPts, piecewiseLinearFunc);

		return FunctionArea(ctrlPts);
	}
};
} //namespace dml

#endif //__XOR_INTEGRAL_H__