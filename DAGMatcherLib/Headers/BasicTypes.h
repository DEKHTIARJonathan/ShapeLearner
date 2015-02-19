#ifndef _BASIC_TYPES_H_
#define _BASIC_TYPES_H_

#include "BasicUtils.h"
#include "FluxSkeleton/include/Point.h"

namespace dml {
//! Dimensions of an object's silhouette
struct ShapeDims
{
	double xmin, xmax, ymin, ymax;
};

//! Loose and tight parameter interval of type double
struct LTDouble
{
	double loose, tight;

	void Set(const double& l, const double& t)
	{
		loose = l;
		tight = t;
	}

	void Set(const double& v)
	{
		loose = v;
		tight = v;
	}

	LTDouble() { }

	LTDouble(const double& l, const double& t) { Set(l, t); }

	LTDouble(const double& v) { Set(v); }

	LTDouble Cos(bool bConvertToRadians) const;
};

//! X-Y Point class with handy member functions
struct POINT
{
	double x, y;
	POINT() {x = -1; y = -1; }
	POINT(const double& xx, const double& yy)    { x = xx; y = yy; }
	void Set(const double& xx, const double& yy) { x = xx; y = yy; }

	friend std::ostream& operator<<(std::ostream& os, const POINT& p)
	{
		//os << "(x:" << p.x << ", y:" << p.y << ")";
		return os << p.x << ", " << p.y;
	}

	POINT& operator=(const POINT& rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	POINT& operator=(const sg::Point& rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	bool operator==(const POINT& rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const POINT& rhs) const { return !operator==(rhs); }

	POINT operator/(const double& k) const  { return POINT(x / k, y / k); }
	POINT operator*(const double& k) const  { return POINT(x * k, y * k); }

	POINT operator-(const POINT& rhs) const { return POINT(x - rhs.x, y - rhs.y); }
	POINT operator+(const POINT& rhs) const { return POINT(x + rhs.x, y + rhs.y); }

	void operator-=(const POINT& rhs) { x -= rhs.x; y -= rhs.y; }
	void operator+=(const POINT& rhs) { x += rhs.x; y += rhs.y; }

	//! Computes the L2 norm of the point
	double L2() const { return sqrt(x * x + y * y); }

	double SqNorm() const { return x * x + y * y; }

	//! Computes the Euclidean distance between the points
	double L2(const POINT& p1) const
	{
		double dx = x - p1.x;
		double dy = y - p1.y;
		return sqrt(dx * dx + dy * dy);
	}

	//! Rounds the coordinates of the point
	void Round()
	{
		x = (x < 0) ? -(int)(0.5 - x) : (int)(x + 0.5);
		y = (y < 0) ? -(int)(0.5 - y) : (int)(y + 0.5);
	}

	void GetRoundedCoords(int* pXCoord, int* pYCoord) const
	{
		*pXCoord = (x < 0) ? -(int)(0.5 - x) : (int)(x + 0.5);
		*pYCoord = (y < 0) ? -(int)(0.5 - y) : (int)(y + 0.5);
	}

	//! Computes the squared Euclidean distance between the points
	double SqDist(const POINT& rhs) const
	{
		double dx = x - rhs.x;
		double dy = y - rhs.y;
		return dx * dx + dy * dy;
	}

	//! Computes the squared Euclidean distance between the points
	double SqDist(const double& rhs_x, const double& rhs_y) const
	{
		double dx = x - rhs_x;
		double dy = y - rhs_y;
		return dx * dx + dy * dy;
	}

	//! Returns true iff the rounded coordinates of both points are equal
	bool RoundedIsEqual(const POINT& rhs) const
	{
		int this_x, this_y, rhs_x, rhs_y;

		GetRoundedCoords(&this_x, &this_y);
		rhs.GetRoundedCoords(&rhs_x, &rhs_y);

		return (this_x == rhs_x && this_y == rhs_y);
	}

	//! dot product. The dot product between perpendicular vectors is zero.
	double Dot(const POINT& b) const { return x * b.x + y * b.y; }

	/*!
		@brief perp product. The perp product betwen parallel vectores is zero.

	    The perp product is the dot product between a perpendicular vector
	    to (*this) and vector b.
	*/
	double Perp(const POINT& b) const { return x * b.y - y * b.x; }
};

//! The POINT class is essentially a vector class.
typedef POINT VECTOR;

//! Simple X-Y integer coordinates class
struct XYCoord
{
	int x, y;

	bool operator==(const XYCoord& rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}

	bool operator!=(const XYCoord& rhs) const
	{
		return !operator==(rhs);
	}

	void operator=(const XYCoord& rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}

	void operator=(const POINT& pt)
	{
		x = (int)round(pt.x);
		y = (int)round(pt.y);
	}

	//! Computes the squared distance between the points
	int SqDist(const XYCoord& rhs) const
	{
		int dx = x - rhs.x;
		int dy = y - rhs.y;

		return dx * dx + dy * dy;
	}
};
} // namespace dml

#endif //_BASIC_TYPES_H_
