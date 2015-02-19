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

#ifndef __EUCLIDEAN_LINE_SEGMENT_H__
#define __EUCLIDEAN_LINE_SEGMENT_H__

#include "MathUtils.h"
#include "SmartArray.h"
#include "BasicTypes.h"

namespace dml {
struct EuclideanLineSegment
{
    POINT p0;	//!< segment endpoint 0
    POINT p1;	//!< segment endpoint 1

    EuclideanLineSegment() {  }

	/*!
		The segment class cannot be virtual (it would affect read/write operations)
		However, the Set() function needs to be virtual. The solution is to defer
		it's "virtuality" to the PiecewiseApprox class that deals with it.
	*/
    EuclideanLineSegment& Set(const EuclideanLineSegment& s)
    {
    	p0 = s.p0;
    	p1 = s.p1;

		return *this;
    }

	/*!
		Set the segment by providing the slope-intercept form of the line
		and the first and last point of the segment
	*/
	void Set(const POINT& firstPt, const POINT& lastPt)
	{
    	p0 = firstPt;
    	p1 = lastPt;
	}

    EuclideanLineSegment& operator=(const EuclideanLineSegment& rhs)
	{
		return Set(rhs);
	}

    double GetLen() const { return p0.L2(p1); }

    double Y(const double& x) const
	{
		if (x == p0.x)
			return p0.y;
		else if (x == p1.x)
			return p1.y;
		else
			ASSERT(false);

		return 0; /*m * x + b;*/
	}

    double Y0() const { return Y(p0.x); }
    double Y1() const { return Y(p1.x); }

	const double& X0() const { return p0.x; }
	const double& X1() const { return p1.x; }

	POINT GetP0() const { return POINT(p0.x, Y(p0.x)); }
	POINT GetP1() const { return POINT(p1.x, Y(p1.x)); }

	//! Returns a vector tangent to the segment
	POINT GetTangent() const { return POINT(p1.x - p0.x, p1.y - p0.y); }

	void Reverse()
	{
		std::swap(p0, p1);
	}

	//! Returns true if x >= X0() and x <= X1()
	bool ContainsX(const double& x) const
	{
		return (x >= X0() && x <= X1());
	}

	/*!
		Find the x coordinate at which the line segments cross.
		@precondition the lines segments must cross at exactly one point
	*/
	double CrossingPoint(const EuclideanLineSegment& ls) const
	{
		ASSERT(false);
		/*ASSERT(m != ls.m);

		double cx = (ls.b - b) / (m - ls.m);

		ASSERT(ContainsX(cx) && ls.ContainsX(cx));
		ASSERT(fabs(Y(cx) - ls.Y(cx)) < 0.0001);

		return cx;*/
	}

    std::istream& Read(std::istream& is)
    {
		is.read((char*)this, sizeof(*this));
		return is;
    }

    std::ostream& Write(std::ostream& os) const
    {
		os.write((char*)this, sizeof(*this));
		return os;
    }

	std::ostream& Print(std::ostream& os) const
    {
    	return os << "[" << p0 << "," << p1 << "]";
    }

	friend std::ostream& operator<<(std::ostream& os, const EuclideanLineSegment& s)
	{
		return s.Print(os);
	}
};

//! Array of Euclidean line segments
typedef SmartArray<EuclideanLineSegment> EuclideanLineSegmentArray;
} //namespace dml

#endif //__EUCLIDEAN_LINE_SEGMENT_H__
