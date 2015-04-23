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
 * $Log: PolyBezierApprox.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __CUBIC_BEZIER_H__
#define __CUBIC_BEZIER_H__

namespace dml {

/*!
	@brief Cubic Bezier class
*/
struct CubicBezier
{
	POINT p0;
    POINT p1;
	POINT p2;
	POINT p3;

	CubicBezier& Set(const CubicBezier& cbs)
	{
		p0 = cbs.p0;
		p1 = cbs.p1;
		p2 = cbs.p2;
		p3 = cbs.p3;
		
		return *this;
	}

	double Y0() const { return p0.y; }
    double Y1() const { return p1.y; }

	POINT GetP0() const { return p0; }
	POINT GetP1() const { return p1; }
	POINT GetP2() const { return p2; }
	POINT GetP3() const { return p3; }
    
	CubicBezier& operator=(const CubicBezier& rhs) { return Set(rhs); }

	bool operator==(const CubicBezier& rhs) const
	{
		return (p0 == rhs.p0 && p1 == rhs.p1 && p2 == rhs.p2 && p3 == rhs.p3);
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
};

/*!
	Parameters derived from the four points that define a cubic bezier curve.

	The precomputation of these parameters imprueves the performance when
	computing the cubic bezier function.
*/
class CubicBezierParams
{
	double a0, b0, a1, b1, a2, b2, a3, b3;

	friend struct CubicBezier;
	friend class PolyBezierApprox;

protected:
	void Rasterize(double u, const POINT& endPt, double stepsize, POINTS& pts,
		SmartArray<double>* pParamMap);

public:
	CubicBezierParams(const CubicBezier& cb);

	void GetCurvePoint(const double& u, double* pX, double* pY) const;
	POINT GetTangent(const double& u) const;
	POINT SecondDerivative(const double& u) const;

	double MaximumCurvature(const double& resolution, 
		double* pMaxPtParam = NULL) const;

	double Curvature(const double& u) const
	{
		POINT first = GetTangent(u);
		POINT second = SecondDerivative(u);

		double d = first.x * first.x + first.y * first.y;
		double D = sqrt(d * d * d);

		return (D == 0) ? 1 : fabs(first.x * second.y - first.y * second.x) / D;
	}

	POINTS Rasterize(int nApproxNumPts, SmartArray<double>* pParamMap = NULL,
		double u0 = -1, double u1 = 1);
};

//! Array of cubic bezier segments
typedef SmartArray<CubicBezier> BezierSegmentArray;

} //namespace dml

#endif //__CUBIC_BEZIER_H__
