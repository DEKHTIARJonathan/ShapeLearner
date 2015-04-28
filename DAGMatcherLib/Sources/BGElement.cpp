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

/////////////////////////////////////////////////////////////////////////////
// BGElement class implementation

//! Copies all the attributes of a bone graph element
BGElement& BGElement::operator=(const BGElement& rhs)
{
	m_simpleAttributes = rhs.m_simpleAttributes;
	m_axisFunction     = rhs.m_axisFunction;
	m_radiusFunction   = rhs.m_radiusFunction;
	m_shocks           = rhs.m_shocks;

	return *this;
}

//! Sets all attributes to zero
void BGElement::Clear()
{
	memset(&m_simpleAttributes, 0, sizeof(m_simpleAttributes));
	m_axisFunction.Clear();
	m_radiusFunction.Clear();
	m_shocks.Clear();
}

//! Reads the value of all attributes
std::istream& BGElement::Read(std::istream& is)
{
	is.read((char*) &m_simpleAttributes, sizeof(m_simpleAttributes));

	m_axisFunction.Read(is);
	m_radiusFunction.Read(is);
	m_shocks.Read(is);

	return is;
}

//! Writes the value of all attributes
std::ostream& BGElement::Write(std::ostream& os) const
{
	os.write((char*) &m_simpleAttributes, sizeof(m_simpleAttributes));

	m_axisFunction.Write(os);
	m_radiusFunction.Write(os);
	m_shocks.Write(os);

	return os;
}

//! Prints the value of all attributes
void BGElement::Print(std::ostream& os, bool bXMLFormat) const
{
	if (bXMLFormat)
	{
		XML_PRINT(os, "type", Type());
		XML_PRINT(os, "length", Length());
		XML_PRINT(os, "averageRadius", AvgRadius());
		XML_PRINT(os, "saliency", Saliency());
		XML_PRINT(os, "pointCount", NumPoints());

		for (int i = 0; i < m_shocks.Size(); i++)
		{
			m_shocks[i].Print(os, bXMLFormat);
		}
	}
	else
	{
		PRINT_OPEN(os, Type());
		PRINT(os, Length());
		PRINT(os, AvgRadius());
		PRINT(os, Saliency());
		PRINT(os, FirstShockPoint().pt);
		PRINT(os, LastShockPoint().pt);
		PRINT_CLOSE(os, NumPoints());
	}
}

//! Returns an ordered array of velocity-radius values
dml::POINTS BGElement::GetVelocityRadiusArray(bool bReverseOrder /*=false*/) const
{
	int nSize = m_shocks.GetSize();
	dml::POINTS data(nSize);

	if (!bReverseOrder)
	{
		for (int i = 0; i < nSize; i++)
			data[i].Set(m_shocks[i].axisDist, m_shocks[i].radius);
	}
	else
	{
		const double maxDist = m_shocks[nSize - 1].axisDist;

		for (int i = 0, j = nSize - 1; i < nSize; i++, j--)
			data[i].Set(maxDist - m_shocks[j].axisDist, m_shocks[j].radius);
	}

	return data;
}

//! Returns the distance and point on the bonepoint array closest to 'pt'
double BGElement::FindClosestPoint(const dml::POINT& pt, ShockPoint* bp) const
{
	ASSERT(!m_shocks.IsEmpty());

	double dist2, minDist2;
	int minPtIdx;

	for (int i = 0; i < m_shocks.Size(); i++)
	{
		dist2 = pt.SqDist(m_shocks[i].pt);

		if (i == 0 || dist2 < minDist2)
		{
			minDist2 = dist2;
			minPtIdx = i;
		}
	}

	*bp = m_shocks[minPtIdx];

	return sqrt(minDist2);
}

//! Finds the point in the bonepoint array with greatest value
double BGElement::GetMaximumRadius() const
{
	double maxRad = 1;

	for (int i = 0; i < m_shocks.Size(); i++)
	{
		if (m_shocks[i].radius > maxRad)
			maxRad = m_shocks[i].radius;
	}

	return maxRad;
}

//! Returns a color based on the saliency value, with 1 = black and 0 = white
leda::color BGElement::SaliencyColor() const
{
	unsigned char c = (char)(255 * (1 - Saliency()));

	return leda::color(c, c, c);
}

/*!
	@brief Copies the radius function and subtracts d from the x-coordinate of
	all segments, such that the first element starts at zero.

	d must be greater or equal to zero.
*/
void BGElement::SetRadiusFunction(const LineSegmentArray& segs, const double& d)
{
	ASSERT(d >= 0);
	m_radiusFunction = segs;

	// first segment should start at point zero
	if (d > 0.0)
	{
		// Shift all the segments by d
		for(int i = 0; i < m_radiusFunction.GetSize(); i++)
		{
			LineSegment& s = m_radiusFunction[i];
			s.p0.x -= d;
			s.p1.x -= d;
			s.b += s.m * d;
		}
	}
}

/*!
	@brief Reverses the shock point array and the radius function
*/
void BGElement::ReverseDataPoints()
{
	ASSERT(m_shocks.IsEmpty() || m_shocks[0].axisDist == 0);
	ASSERT(m_radiusFunction.IsEmpty() || m_radiusFunction[0].p0.x == 0);

	if (!m_shocks.IsEmpty())
	{
		// This cannot be a reference!!!
		const double maxAxisDist = m_shocks.GetTail().axisDist;

		ASSERT(maxAxisDist >= 0);

		// Reverse point order...
		m_shocks = m_shocks.Reverse();

		// ...and change the axis distance so that it starts from zero
		for (int i = 0; i < m_shocks.Size(); i++)
			m_shocks[i].axisDist = maxAxisDist - m_shocks[i].axisDist;

		ASSERT(m_shocks[0].axisDist == 0);
	}

	// ... and reverse the slope of each line segment, while also shifting
	// it along the x-axis to reflect the changes in axis distance
	if (!m_radiusFunction.IsEmpty())
	{
		// This cannot be a reference!!!
		const double maxRadFunDist = m_radiusFunction.GetTail().p1.x;

		ASSERT(maxRadFunDist >= 0);

		// Reverse line segment order...
		m_radiusFunction = m_radiusFunction.Reverse();

		for (int i = 0; i < m_radiusFunction.Size(); i++)
			m_radiusFunction[i].Reverse(maxRadFunDist);

		ASSERT(m_radiusFunction[0].p0.x == 0);
	}

	if (!m_axisFunction.IsEmpty())
	{
		// Reverse line segment order for the axis function...
		m_axisFunction = m_axisFunction.Reverse();

		// ... and reverse the parameterization of each line segment
		for (int i = 0; i < m_axisFunction.Size(); i++)
			m_axisFunction[i].Reverse();
	}
}

//! Computes all values that depend entirely on other attributes of the object
void BGElement::ComputeDerivedValues()
{
	m_simpleAttributes.m_dLength = 0;
	m_simpleAttributes.m_dAvgRadius = 0;

	if (m_shocks.IsEmpty())
		return;

	m_shocks[0].axisDist = 0; // up to point (not including it)

	double radiusSum = m_shocks[0].radius;

	for (int i = 1; i < m_shocks.Size(); i++)
	{
		ShockPoint& bp = m_shocks[i];

		bp.axisDist = bp.pt.L2(m_shocks[i - 1].pt);

		WARNING1(bp.axisDist > 1.5, "Shock points are not connected", bp.axisDist);

#ifdef _DEBUG
		if (bp.axisDist > 1.5)
		{
			DBG_PRINT2(bp.pt, m_shocks[i - 1].pt)
		}
#endif

		// Axis distance is cumulative, so we must add the previus value
		bp.axisDist += m_shocks[i - 1].axisDist;

		// Also sum radius values
		radiusSum += bp.radius;
	}

	// The lenght is simply the axis distance of the last point plus 1.
	// This is use to get an endpoint position equal to 1.0 if attachment
	// is at the last point (ie, at lastPt.axisDist)
	m_simpleAttributes.m_dLength = m_shocks.GetTail().axisDist + 1;

	m_simpleAttributes.m_dAvgRadius = radiusSum / m_shocks.Size();

	/*if (m_radiusFunction.IsEmpty())
	{
		m_simpleAttributes.m_dAvgRadius = m_shocks[0].radius;
	}
	else
	{
		double mean_x, radiusSum = 0;

		for (int i = 0; i < m_radiusFunction.Size(); i++)
		{
			const LineSegment& s = m_radiusFunction[i];

			mean_x     = (s.p0.x + s.p1.x) / 2.0;
			radiusSum += s.m * mean_x + s.b;
		}

		m_simpleAttributes.m_dAvgRadius = radiusSum / m_radiusFunction.Size();
	}*/

	ASSERT(m_simpleAttributes.m_dAvgRadius > 0);
}

/*!
	@brief
*/
/*void BGElement::WarpShockPoints(int shockCoordIdx, const NEWMAT::Matrix& H)
{
	const int numPts = NumPoints();
	NEWMAT::Matrix P(3, numPts);
	int i, ipp;

	for (i = 0, ipp = 1; i < numPts; i++, ipp++)
	{
		P(1, ipp) = m_shocks[i].X(shockCoordIdx);
		P(2, ipp) = m_shocks[i].Y(shockCoordIdx);
		P(3, ipp) = 1;
	}

	P = H * P;

	for (i = 0, ipp = 1; i < numPts; i++, ipp++)
	{
		m_shocks[i].X(shockCoordIdx) = P(1, ipp) / P(3, ipp);
		m_shocks[i].Y(shockCoordIdx) = P(2, ipp) / P(3, ipp);
	}
}*/

/*!
	@brief
*/
/*void BGElement::WarpShockPoints(const NEWMAT::Matrix& H)
{
	for (int i = 0; i < 3; i++)
		WarpShockPoints(i, H);
}*/

/*!
	@brief Computes tangent vactore at the given shock point

	The tangent is approximated by the secant line that goes throw
	the two immediate neighbours of the given point, if it's not
	and endpoint. Otherwise, a totalk least square is computed using
	the points close to the endpoint.

	The tangent is directed from the point ptIndex + 1 to the point
	ptIdx - 1.
*/
dml::POINT BGElement::ComputeTangent(int ptIdx) const
{
	ASSERT(ptIdx >= 0 && ptIdx < m_shocks.Size());
	ASSERT(m_shocks.Size() >= 2);

	int j, nPts;

	if (ptIdx == 0) // first endpoint
	{
		nPts = MIN(m_shocks.Size(), 5);
		j = 0;
	}
	else if (ptIdx == m_shocks.Size() - 1) // last endpoint
	{
		nPts = MIN(m_shocks.Size(), 5);
		j = m_shocks.Size() - nPts;
	}
	else // there are at least 3 pts
	{
		nPts = 3;
		j = ptIdx - 1;
	}

	POINT tangentVector;

	if (nPts <= 3) // just connect endpoints
	{
		tangentVector = m_shocks[j].pt - m_shocks[j + nPts - 1].pt;
	}
	else // do a real fitting of the points
	{
		POINTS pts(nPts);

		for (int i = 0; i < nPts; i++, j++)
			pts[i] = m_shocks[j].pt;

		double a, b, c; // coefficient of the line: a * x + b * y + c = 0
		LinearTotalLeastSquares(pts, nPts, a, b, c);

		tangentVector = GetClosestPointOnLine(a, b, c, pts[0]) -
			GetClosestPointOnLine(a, b, c, pts[nPts - 1]);
	}

	return tangentVector;
}

const ShockPoint& BGElement::FindClosestPoint(const dml::POINT& pt, bool bStartFromFirst,
								  const double& minSqDist) const
{
	if (bStartFromFirst)
	{
		for (int i = 0; i < m_shocks.Size(); i++)
		{
			if (pt.SqDist(m_shocks[i].pt) >= minSqDist)
				return m_shocks[i];
		}

		return m_shocks.GetTail();
	}
	else
	{
		for (int i = m_shocks.Size() - 1; i >= 0; i--)
		{
			if (pt.SqDist(m_shocks[i].pt) >= minSqDist)
				return m_shocks[i];
		}

		return m_shocks.GetHead();
	}
}