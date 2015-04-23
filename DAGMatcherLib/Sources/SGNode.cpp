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
 * $Log: SGNode.cpp,v $
 * Revision 1.3  2002/06/10 18:44:53  dmac
 * Node's role attribute added.
 *
 * Revision 1.2  2002/05/16 22:50:11  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

#define NOT_TO_COMPARE_PERCENTAGE .15
#define MIN_LIGATURE .7

using namespace std;
using namespace dml;

/////////////////////////////////////////////////////////////////////////////
// ShockInfo class implementation

//! Reads a ShockInfo from the specified stream.
istream& ShockInfo::Read(istream& is)
{
	return is.read((char*)this, sizeof(ShockInfo));
}

//! Writes the ShockInfo to the specified stream.
ostream& ShockInfo::Write(ostream& os) const
{
	return os.write((char*)this, sizeof(ShockInfo));
}

//! Prints the ShockInfo to the specified stream.
void ShockInfo::Print(ostream& os, bool bXMLFormat) const
{
	if (bXMLFormat)
	{
		XML_OPEN_ATTS(os, "point");
		XML_PRINT_ATT(os, xcoord);
		XML_PRINT_ATT(os, ycoord);
		XML_PRINT_ATT(os, radius);
		XML_CLOSE_ATTS(os);
	}
	else
	{
		PRINT_OPEN(os, xcoord);
		PRINT(os, ycoord);
		PRINT(os, radius);
		PRINT(os, type);
		PRINT(os, dir);
		PRINT(os, dr);
		PRINT(os, speed);
		PRINT(os, dr_ds);
		PRINT_CLOSE(os, int(color));
	}
}

/////////////////////////////////////////////////////////////////////////////
// SGNode class implementation

// Begin DAGNode virtual functions

/*!
	Creates a new SGNode with a copy of the information conatines in 'this'.
	This functions is required by SmartPtr<DAGNode*>::operator[].
*/
DAGNode* SGNode::CreateObject() const
{
	return (DAGNode*) new SGNode();
}

void SGNode::Clear()
{
	DAGNode::Clear();

	m_nType       = 0;
	m_nRole = UNK_ROLE;

	m_cEndPt	  = 0;
	m_seg1 		  = 0;
	m_seg2 		  = 0;
	m_strBranchId.Clear();
	m_shocks.Clear();

	m_nEndPt0 = NOTSET_POINT;
	m_nEndPtN = NOTSET_POINT;
}

//! Extends the behaviour of the same function in the base class.
DAGNode& SGNode::operator=(const DAGNode& rhs)
{
	DAGNode::operator=(rhs);

	const SGNode* pRhs = dynamic_cast<const SGNode*>(&rhs);

	if (!pRhs)
		THROW_EXCEPTION("Invalid pointer type.");

	m_nType       = pRhs->m_nType;
	m_nRole		  = pRhs->m_nRole;

	m_cEndPt	  = pRhs->m_cEndPt;
	m_seg1 		  = pRhs->m_seg1;
	m_seg2 		  = pRhs->m_seg2;
	m_strBranchId = pRhs->m_strBranchId;
	m_shocks      = pRhs->m_shocks;

	m_nEndPt0     = pRhs->m_nEndPt0;
	m_nEndPtN     = pRhs->m_nEndPtN;

	return *this;
}

// End DAGNode virtual functions

//! Constructor from data.
SGNode::SGNode(String szId, int nType) : DAGNode(szId)
{
	m_nType = nType;
	m_strBranchId = szId;
	m_cEndPt = '\0';
	m_seg1 = m_seg2 = 0.0;

	for (int i = 0, len = szId.Len(); i < len; i++)
	{
		if (!isdigit(szId[i]))
		{
			m_cEndPt = szId[i];
			break;
		}
	}
}

void SGNode::ComputeDerivedValues()
{
	DAGNode::ComputeDerivedValues();

	m_shocks.ComputeDerivedValues();
}

//! Compute speeds very roughly as consecutive differences
void SGNode::ComputeSpeeds()
{
	for (int i = 1, size = m_shocks.GetSize(); i < size; i++)
		m_shocks[i].speed = m_shocks[i].radius - m_shocks[i-1].radius;

	m_shocks[0].speed = m_shocks[1].speed;
}

//! Extends the behaviour of the same function in the base class.
istream& SGNode::Read(istream& is)
{
	DAGNode::Read(is);

	is.read((char*) &m_nType, sizeof(m_nType));
	is.read((char*) &m_nRole, sizeof(m_nRole));
	is.read((char*) &m_cEndPt, sizeof(m_cEndPt));
	is.read((char*) &m_seg1, sizeof(m_seg1));
	is.read((char*) &m_seg2, sizeof(m_seg2));
	is.read((char*) &m_nEndPt0, sizeof(m_nEndPt0));
	is.read((char*) &m_nEndPtN, sizeof(m_nEndPtN));

	m_strBranchId.Read(is);
	m_shocks.Read(is);

	return is;
}

//! Extends the behaviour of the same function in the base class.
ostream& SGNode::Write(ostream& os) const
{
	DAGNode::Write(os);

	os.write((char*) &m_nType, sizeof(m_nType));
	os.write((char*) &m_nRole, sizeof(m_nRole));
	os.write((char*) &m_cEndPt, sizeof(m_cEndPt));
	os.write((char*) &m_seg1, sizeof(m_seg1));
	os.write((char*) &m_seg2, sizeof(m_seg2));
	os.write((char*) &m_nEndPt0, sizeof(m_nEndPt0));
	os.write((char*) &m_nEndPtN, sizeof(m_nEndPtN));

	m_strBranchId.Write(os);
	m_shocks.Write(os);

	return os;
}

//! Extends the behaviour of the same function in the base class.
void SGNode::Print(ostream& os, bool bXMLFormat) const
{
	DAGNode::Print(os, bXMLFormat);

	if (bXMLFormat)
	{
		XML_PRINT(os, "type", m_nType - 100);
		XML_PRINT(os, "pointCount", GetShockCount());
		m_shocks.Print(os, bXMLFormat);
	}
	else
	{
		PRINT_OPEN(os, m_strBranchId);
		PRINT(os, m_nType);
		PRINT(os, m_nRole);
		PRINT(os, GetShockCount());
		PRINT(os, m_seg1);
		PRINT_CLOSE(os, m_seg2);
	}
}

//! Returns the label that will be displayed in the graph
NODE_LABEL SGNode::GetLblForGraph(bool bExtraInfo /*= true*/) const
{
	if (bExtraInfo)
	{
		char lbl[100];
		sprintf(lbl, "%s:%d", (const char*) GetNodeLbl(), m_nType - 100);

		return lbl;
	}
	else
		return GetNodeLbl();
}

dml::POINTS SGNode::GetVelocityRadiusArray(int& d0, int& dN, bool bReverseOrder /*=false*/) const
{
	int i, nSize = m_shocks.GetSize();
	double s;

	// we don't want to consider those end points that join branches
	// because they're usually outliers wrt to the branch pts.
	ASSERT(m_nEndPt0 != NOTSET_POINT && m_nEndPtN != NOTSET_POINT);

	d0 = (nSize > 3 && m_nEndPt0 == JOINT_POINT) ? 1:0;
	dN = (nSize > 3 && m_nEndPtN == JOINT_POINT) ? 1:0;
	nSize -= d0 + dN;

	POINTS data(nSize);

	if (!bReverseOrder)
	{
		for (i = 0, s = 0.0; i < nSize; i++)
		{
			const ShockInfo& si = m_shocks[i + d0];
			data[i].Set(s += si.speed, si.radius);
		}
	}
	else
	{
		int nLast = nSize - 1 + d0;

		for (i = 0, s = 0.0; i < nSize; i++)
		{
			const ShockInfo& si = m_shocks[nLast - i];
			data[i].Set(s += si.speed, si.radius);
		}
	}

	return data;
}

////////////////////////////////////////////////////////////////////////////////////////////
// ShockBranch

void ShockBranch::Print(ostream &os, bool bXMLFormat) const
{
	if (bXMLFormat)
	{
		for (int i = 0; i < GetSize(); i++)
			operator[](i).Print(os, bXMLFormat);
	}
	else
	{
		for (int i = 0; i < GetSize(); i++)
		{
			os << endl;
			operator[](i).Print(os, bXMLFormat);
		}

		os << endl;
		PRINT_OPEN(os, avgRadius);
		PRINT(os, area);
		PRINT(os, length);
		PRINT(os, avgVelocity);
		PRINT(os, MinR());
		PRINT_CLOSE(os, MaxR());
		//segments.Print(os);
		os << endl;
	}
}

//! Compares the areas of two branches.
double ShockBranch::CompareAreas(const ShockBranch& rhs, const double& scale) const
{
	double a1, a2;

	a1 = Area();
	a2 = rhs.Area() * scale;

	return SIMILARITY(a1, a2);
}

//! Computes normalized square differences over the radii.
double ShockBranch::CompareRadii(const ShockBranch& rhs, const double& scale) const
{
	double a1, a2;

	a1 = AvgRadius();
	a2 = rhs.AvgRadius() * scale;

	return SIMILARITY(a1, a2);
}

/*!
	Helper function for sorting descendently a ShockBranch according
	to the dr_rs values of its elements.
*/
int CompareShockDrDs(const void* elem1, const void* elem2)
{
	ShockInfo* e1 = (ShockInfo*) elem1;
	ShockInfo* e2 = (ShockInfo*) elem2;

	ASSERT(e1 && e2);

	if (e1->dr_ds > e2->dr_ds)
		return 1;
	else if (e1->dr_ds < e2->dr_ds)
		return -1;
	else
		return 0;
}

//! Compares the 85% average velocities.
double ShockBranch::CompareVelocities(const ShockBranch& rhs, const double& scale) const
{
	double avg1, avg2;

	avg1 = AvgVelocity();
	avg2 = rhs.AvgVelocity() * scale;

	return SIMILARITY(avg1, avg2);
}

void ShockBranch::ComputeDerivedValues()
{
	int i, size = GetSize();
	double prevRadius, mean_x;

	area = 0;
	avgRadius = 0;
	avgVelocity = 0;
	length = 0;

	bValuesComputed = true;

	if (size == 0)
		return;
	else if (size == 1)
	{
		avgRadius = operator[](0).radius;
		area = 2 * avgRadius;
		return;
	}

	for (i = 0; i < size; i++)
	{
		const ShockInfo& si = (*this)[i];

		length += si.speed; // speed is delta s
		//avgRadius += si.radius;

		// The area is the sum of the trapezoids
		if (i > 0)
			area += .5 * (prevRadius + si.radius) * si.speed;

		prevRadius = si.radius;
	}

	// Compute the average radius of the brach.
	//avgRadius /= size;

	ASSERT(segments.GetSize() > 0);

	for (i = 0; i < segments.GetSize(); i++)
	{
		const LineSegment& s = segments[i];

		mean_x = (s.p0.x + s.p1.x) / 2.0;
		avgRadius += s.m * mean_x + s.b;
	}

	avgRadius /= segments.GetSize();

	ASSERT(avgRadius >= 0);

	// Compute the average velocity of the branch.
	// Firts, eliminate NOT_TO_COMPARE_PERCENTAGE percentage
	// of the extreme values before computing avg.
	ShockBranch b(*this);
	int min, max, nanCount = 0;

	ASSERT(NOT_TO_COMPARE_PERCENTAGE > 0 && NOT_TO_COMPARE_PERCENTAGE <= 1);

	b.Sort(CompareShockDrDs);

	min = round(size * NOT_TO_COMPARE_PERCENTAGE);
	max = size - min;

	for (i = min; i < max; i++)
	{
		if (isnan(b[i].dr_ds) || !finite(b[i].dr_ds))
		{
			//DBG_MSG("Illegal dr_ds value!!!");
			nanCount++;
			continue;
		}

		avgVelocity += b[i].dr_ds;
	}

	if (avgVelocity != 0)
		avgVelocity /= max - min - nanCount;
}

istream& ShockBranch::Read(istream& is)
{
	SmartArray<ShockInfo>::Read(is);

	segments.Read(is);
	is.read((char*) &dir, sizeof(dir));

	is.read((char*) &bValuesComputed, sizeof(bValuesComputed));
	is.read((char*) &area, sizeof(area));
	is.read((char*) &length, sizeof(length));
	is.read((char*) &avgRadius, sizeof(avgRadius));
	is.read((char*) &avgVelocity, sizeof(avgVelocity));

	return is;
}

ostream& ShockBranch::Write(ostream& os) const
{
	SmartArray<ShockInfo>::Write(os);

	segments.Write(os);
	os.write((char*) &dir, sizeof(dir));

	os.write((char*) &bValuesComputed, sizeof(bValuesComputed));
	os.write((char*) &area, sizeof(area));
	os.write((char*) &length, sizeof(length));
	os.write((char*) &avgRadius, sizeof(avgRadius));
	os.write((char*) &avgVelocity, sizeof(avgVelocity));

	return os;
}

bool ShockBranch::operator==(const ShockBranch& rhs) const
{
	ASSERT(bValuesComputed && rhs.bValuesComputed);

	return AvgRadius() == rhs.AvgRadius();
}

/*!
	@brief Compares the time of formation between two branches

	NOTE that due to an outlier, the max radius of the branch is likely
	to be wrong. However, MaxR() returns the maximum point using the segments instead
	of the actual shock points.
*/
bool ShockBranch::operator>(const ShockBranch& rhs) const
{
	ASSERT(bValuesComputed && rhs.bValuesComputed);

	// If either branch has only one shock, it is likely that this one shock
	// will have some error. So, we must treat this case differently.

	//if (GetSize() == rhs.GetSize() || (GetSize() > 1 && rhs.GetSize() > 1))
		return AvgRadius() > rhs.AvgRadius();
	//else
	//	return MaxR() > rhs.MaxR() || MinR() > rhs.MinR();
}

void ShockBranch::SetSegments(const LineSegmentArray& segs, const double& d)
{
	ASSERT(d >= 0);
	segments = segs;

	// first segment should start at point zero
	if (d > 0.0)
	{
		// Shift all the segments by d
		for(int i = 0; i < segments.GetSize(); i++)
		{
			LineSegment& s = segments[i];
			s.p0.x -= d;
			s.p1.x -= d;
			s.b += s.m * d;
		}
	}
}