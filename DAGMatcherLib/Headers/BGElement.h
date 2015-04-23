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
#ifndef _BG_ELEMENT_H_
#define _BG_ELEMENT_H_

#include "stdafx.h"

namespace dml {
/*!
	@brief Represents the basic attributes of both nodes and
	edges of a BoneGraph.

	@see BoneGraph.
*/
class BGElement
{
	friend class BoneGraphConstructor;
	friend class BoneGraphView;
	friend class BGDistMeasurer1;
	friend class BGEdge; //!< needed to merge node and edges

public:
	enum ElementType {BACK_BONE, SIMPLE_BONE, ROOT_BONE, CORNER_BONE,
		LIGAMENT, SUBLIGAMENT, BONE_PART, BONE_PART_TYPE_2,
		BONE_PART_TYPE_4, SIMPLE_ROOT_BONE};

	//typedef BezierSegmentArray AxisFunction;
	typedef EuclideanLineSegmentArray AxisFunction;
	typedef LineSegmentArray RadiusFunction;

	struct SimpleAttributes
	{
		ElementType m_nType;
		double m_dSaliency;
		double m_dLength;
		double m_dAvgRadius;
		int m_nSlopeDir;      //!< Can either be -1, 0 , or 1 (ie, decreasing, constant, or increasing)

		SimpleAttributes() { }

		SimpleAttributes(ElementType type)
		{
			m_nType = type;
			m_dSaliency = m_dLength = m_dAvgRadius = -1;
			m_nSlopeDir = 0;
		}
	};

protected:
	SimpleAttributes m_simpleAttributes;
	AxisFunction     m_axisFunction;
	RadiusFunction   m_radiusFunction;
	ShockPoints      m_shocks;

public:
	BGElement(ElementType type) : m_simpleAttributes(type)
	{
		// nothing else to do
	}

	BGElement(const BGElement& sb)
	{
		BGElement::operator=(sb);
	}

	BGElement& operator=(const BGElement& rhs);

	//! Creates a new object of type bone graph element
	BGElement* CreateObject() const
	{
		return new BGElement(BACK_BONE);
	}

	void Clear();
	void ReverseDataPoints();
	void ComputeDerivedValues();

	void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;
	std::istream& Read(std::istream& is);
	std::ostream& Write(std::ostream& os) const;

	void SetType(ElementType type)
	{
		m_simpleAttributes.m_nType = type;
	}

	void SetSlopeDir(int dir)
	{
		m_simpleAttributes.m_nSlopeDir = dir;
	}

	int SlopeDir() const
	{
		return m_simpleAttributes.m_nSlopeDir;
	}

	int SlopeDir(bool bReverse) const
	{
		int dir = m_simpleAttributes.m_nSlopeDir;
		ASSERT(dir == 0 || dir == 1 || dir == -1);

		return (bReverse && dir != 0) ? -dir : dir;
	}

	ElementType Type() const
	{
		return m_simpleAttributes.m_nType;
	}

	const double& Saliency() const
	{
		//return 1;
		//ASSERT(m_simpleAttributes.m_dSaliency >= 0 &&
		//	   m_simpleAttributes.m_dSaliency <= 1);

		WARNING1(m_simpleAttributes.m_dSaliency < 0 || m_simpleAttributes.m_dSaliency > 1,
			"Wrong saliency value", m_simpleAttributes.m_dSaliency);

		if (m_simpleAttributes.m_dSaliency < 0 || m_simpleAttributes.m_dSaliency > 1)
			return 0;

		return m_simpleAttributes.m_dSaliency;
	}

	double Length() const
	{
		ASSERT(m_simpleAttributes.m_dLength >= 0);

		return m_simpleAttributes.m_dLength;
	}

	double AvgRadius() const
	{
		//ASSERT(m_simpleAttributes.m_dAvgRadius > 0);

		WARNING1(m_simpleAttributes.m_dAvgRadius < 0,
			"Wrong average radius", m_simpleAttributes.m_dAvgRadius);

		return m_simpleAttributes.m_dAvgRadius;
	}

	POINTS GetVelocityRadiusArray(bool bReverseOrder = false) const;

	void SetRadiusFunction(const LineSegmentArray& segs, const double& d);

	void CopyPoint(int shockPtIdx, int shockPtSubIdx, POINT* pPoint) const
	{
		pPoint->x = m_shocks[shockPtIdx].X(shockPtSubIdx);
		pPoint->y = m_shocks[shockPtIdx].Y(shockPtSubIdx);
	}

	POINT ComputeTangent(int ptIdx) const;

	const ShockPoint& FindClosestPoint(const POINT& pt, bool bStartFromFirst,
		const double& minSqDist) const;

	//void WarpShockPoints(int shockCoordIdx, const NEWMAT::Matrix& H);
	//void WarpShockPoints(const NEWMAT::Matrix& H);

	LineSegmentArray GetRadiusFunction(bool bFlipOrientation = false) const
	{
		if (bFlipOrientation)
		{
			// Reverse line segment order...
			LineSegmentArray revRF = m_radiusFunction.Reverse();

			const double maxXCoord = m_radiusFunction.GetTail().p1.x;

			// ... and reverse the slope of each line segment, while also shifting
			// it along the x-axis to reflect the changes in axis distance
			for (int i = 0; i < revRF.Size(); i++)
				revRF[i].Reverse(maxXCoord);

			return revRF;
		}
		else
		{
			return m_radiusFunction;
		}
	}

	//int FlowsFrom() const                   { return m_simpleAttributes.m_nFlowsFrom; }
	int NumPoints() const                   { return m_shocks.Size(); }

	bool IsEmpty() const
	{
		return m_shocks.IsEmpty();
	}

	int GetLastPointPosition() const
	{
		return (m_shocks.IsEmpty()) ? 0 : m_shocks.Size() - 1;
	}

	const ShockPoints& GetShockPoints() const
	{
		return m_shocks;
	}

	const ShockPoint& FirstShockPoint() const
	{
		ASSERT(!m_shocks.IsEmpty());
		return m_shocks.GetHead();
	}

	const ShockPoint& LastShockPoint() const
	{
		ASSERT(!m_shocks.IsEmpty());
		return m_shocks.GetTail();
	}

	const ShockPoint& GetShockPoint(int i) const
	{
		return m_shocks[i];
	}

	double ClosestSqDistanceToPoint(const POINT& pt) const
	{
		ASSERT(!IsEmpty());

		double minSqDist = m_shocks[0].pt.SqDist(pt);
		double sqDist;

		for (int i = 1; i < m_shocks.Size(); i++)
		{
			sqDist = m_shocks[i].pt.SqDist(pt);

			if (sqDist < minSqDist)
				minSqDist = sqDist;
		}

		return minSqDist;
	}

	double FindClosestPoint(const POINT& pt, ShockPoint* bp) const;
	double GetMaximumRadius() const;
	leda::color SaliencyColor() const;
};

} //namespace dml

#endif //_BG_ELEMENT_H_
