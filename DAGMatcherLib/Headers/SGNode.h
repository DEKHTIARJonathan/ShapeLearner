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
#ifndef __SG_NODE_H__
#define __SG_NODE_H__

#include "stdafx.h"

#define  SHOCK_base 100
#define  SHOCK_2or3 100
#define  SHOCK_1	101
#define  SHOCK_2	102
#define  SHOCK_3	103
#define  SHOCK_4	104
#define  ROOT		105
#define  TEST_NODE	106

namespace dml {
enum NODE_ROLE {UNK_ROLE = 0, BIRTH, PROTRUSION, UNION, DEATH, DUMMY};
enum ENDPT_TYPE {NOTSET_POINT, TERMINAL_POINT, JOINT_POINT, SPLIT_POINT};
enum GRAMMAR_RULE {INV_RULE = 0, RULE_1, RULE_2, RULE_3, RULE_4, RULE_5,
		   RULE_6, RULE_7, RULE_8, RULE_9,RULE_10};

//! Information of a particular shock.
struct ShockInfo
{
	double xcoord;	//!< X coordinate of branch points.
	double ycoord;	//!< Y coordinate of branch points.
	double radius;	//!< Radii at the branch points.
	double speed;	//!< Velocities at the branch points.
	double dr_ds;
	char color;
	double dr;		//!< Delta radius. Diff in radius with that of the previous point
	int type;
	int dir;		//!< Direction: increasing=1, decreasing=-1, constant=0

	ShockInfo() { xcoord = ycoord = radius = speed = dr_ds = dr = 0; color = 0; type = dir = 0; }

	/*virtual*/ std::istream& Read(std::istream& is);
	/*virtual*/ std::ostream& Write(std::ostream& os) const;
	void Print(std::ostream &os, bool bXMLFormat) const;

	friend std::ostream& operator<<(std::ostream& os, const ShockInfo& shock) { return shock.Write(os); }
	friend std::istream& operator>>(std::istream& is, ShockInfo& shock)		{ return shock.Read(is); }

	bool operator==(const ShockInfo& rhs) const
	{
		return xcoord == rhs.xcoord && ycoord == rhs.ycoord;
	}

	bool operator!=(const ShockInfo& rhs) const
	{
		return !operator==(rhs);
	}
};

class ShockBranch : public SmartArray<ShockInfo>
{
	LineSegmentArray segments;
	int dir;

	bool bValuesComputed;
	double area;
	double length;
	double avgRadius;
	double avgVelocity;

public:
	enum BRANCH_DIR {FORWARD, BACKWARD};

	ShockBranch()
	{
		dir = 2; //diff than 0, 1, -1

		bValuesComputed = false;
		area = length = avgRadius = avgVelocity = 0;
	}

	void Clear()
	{
		SmartArray<ShockInfo>::Clear();

		segments.Clear();
		dir = 2; //diff than 0, 1, -1

		bValuesComputed = false;
		area = length = avgRadius = avgVelocity = 0;
	}

	ShockBranch(const ShockBranch& rhs)
	{
		operator=(rhs);
	}

	ShockBranch& operator=(const ShockBranch& rhs)
	{
		SmartArray<ShockInfo>::operator=(rhs);

		segments		= rhs.segments;
		dir				= rhs.dir;

		bValuesComputed = rhs.bValuesComputed;
		area			= rhs.area;
		length			= rhs.length;
		avgRadius		= rhs.avgRadius;
		avgVelocity 	= rhs.avgVelocity;

		return *this;
	}

	ShockBranch& operator=(const SmartArray<ShockInfo>& rhs)
	{
		SmartArray<ShockInfo>::operator=(rhs);
		return *this;
	}

	bool operator==(const ShockBranch& rhs) const;
	bool operator!=(const ShockBranch& rhs) const { return !operator==(rhs); }

	bool operator>(const ShockBranch& rhs) const;
	bool operator<=(const ShockBranch& rhs) const { return !operator>(rhs); }

	bool operator<(const ShockBranch& rhs) const { return operator<=(rhs) && operator!=(rhs); }
	bool operator>=(const ShockBranch& rhs) const { return !operator<(rhs); }

	void GetEndPtRadius(double& r0, double& rN) const
	{
		if (segments.GetSize() > 0) // Use segment' end pt, because the don't have outliers
		{
			r0 = segments.GetHead().Y0();
			rN = segments.GetTail().Y1();
		}
		else
		{
			r0 = GetHead().radius;
	    	rN = GetTail().radius;
		}
	}

	const double& MinR() const
	{
		double r0, rN;
		GetEndPtRadius(r0, rN);
	    return r0 < rN ? r0:rN;
	}

	const double& MaxR() const
	{
		double r0, rN;
		GetEndPtRadius(r0, rN);
	    return r0 > rN ? r0:rN;
	}

	void SetDir(int d) { dir = d; }
	void SetSegments(const LineSegmentArray& segs, const double& d);
	LineSegmentArray GetSegments() const { return segments; }

	int GetDir(BRANCH_DIR d) const
	{
		ASSERT(dir == 0 || dir == 1 || dir == -1);
		return (d == FORWARD || dir == 0) ? dir:-dir;
	}

	std::istream& Read(std::istream& is);
	std::ostream& Write(std::ostream& os) const;
	void Print(std::ostream &os = std::cout, bool bXMLFormat = false) const;

	void ComputeDerivedValues();

	double CompareAreas(const ShockBranch& rhs, const double& scale) const;
	double CompareRadii(const ShockBranch& rhs, const double& scale) const;
	double CompareVelocities(const ShockBranch& rhs, const double& scale) const;

	double Area() const 		{ ASSERT(bValuesComputed); return area; }
	double Length() const 		{ ASSERT(bValuesComputed); return length; }
	double AvgRadius() const	{ ASSERT(bValuesComputed); return avgRadius; }
	double AvgVelocity() const	{ ASSERT(bValuesComputed); return avgVelocity; }
};

/*!
\brief Represents a branch/node of a shock graph.

 \todo At this moment, the type of a branch is not 1..4 but instead the set of numbers
 that the shock graph detection algorithm returns. SHOCK_base=100, SHOCK_2or3=100,
 SHOCK_1=101, SHOCK_2=102, SHOCK_3=103, SHOCK_4=104, ROOT=105 (values defined in Shock.h).

  \see DAGNode, ShockGraph.

*/
class SGNode : public DAGNode
{
public:
	int m_nType;				//!< Shock branch type (1, 2, 3, 4).
	char m_cEndPt;				//!< Whether is 3a, 3b, and so on (vals: 0, 'a', 'b')
	double m_seg1, m_seg2;		//!< Contour segment's lengths
	String m_strBranchId;		//!< Id to distinguish from others of this kind.
	ShockBranch m_shocks;		//!< array of shocks that belong to the branch (branch points).
	NODE_ROLE m_nRole;
	ENDPT_TYPE m_nEndPt0, m_nEndPtN;

	virtual DAGNode* CreateObject() const;

	// Operations
public:

	SGNode() { }
	SGNode(const SGNode &sb)		{ SGNode::operator=(sb); }
	SGNode(String szId, int nType);

	void GetContourLength(double& seg1, double& seg2) const { seg1 = m_seg1; seg2 = m_seg2; }
	void SetContourLength(double& seg1, double& seg2) { m_seg1 = seg1; m_seg2 = seg2; }

	virtual void ComputeDerivedValues();

	void ComputeSpeeds();
	int GetShockCount() const { return m_shocks.GetSize(); }

	int GetType() const { return m_nType; }
	char GetEndPt() const { return m_cEndPt; }
	String GetBranchId() const { return m_strBranchId; }
	int GetBranchDir(ShockBranch::BRANCH_DIR d) const { return m_shocks.GetDir(d); }
	LineSegmentArray GetSegments() const { return m_shocks.GetSegments(); }

	const ShockInfo& operator[](int i) const { return m_shocks[i]; }
	int GetSize() const { return m_shocks.GetSize(); }

	POINTS GetVelocityRadiusArray(int& d0, int& dn, bool bReverseOrder = false) const;

	void SetNodeType(int nType) { m_nType = nType; }

	NODE_ROLE GetNodeRole() const { return m_nRole; }
	void SetNodeRole(NODE_ROLE nRole) { m_nRole = nRole; }

	virtual void Clear();
	virtual DAGNode& operator=(const DAGNode& rhs);
	virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;
	virtual std::istream& Read(std::istream& is);
	virtual std::ostream& Write(std::ostream& os) const;

	virtual NODE_LABEL GetLblForGraph(bool bExtraInfo = true) const;
};
} // namespace dml

#endif //__SG_NODE_H__
