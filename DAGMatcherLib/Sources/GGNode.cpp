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
 * $Log: GGNode.cpp,v $
 * Revision 1.2  2002/06/14 23:13:03  dmac
 * Matching algorithm updated.
 *
 * Revision 1.1  2002/05/21 16:52:19  dmac
 * New file added.
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

using namespace std;
using namespace dml;

/////////////////////////////////////////////////////////////////////////////
// GGRelation class implementation

//! Reads a GGRelation from the specified stream.
istream& GGRelation::Read(istream& is)
{
	is.read((char*) this, sizeof(*this));

	return is;
}

//! Writes the GGRelation to the specified stream.
ostream& GGRelation::Write(ostream& os) const
{
	os.write((char*) this, sizeof(*this));

	return os;
}

//! Prints the ShockInfo to the specified stream.
void GGRelation::Print(ostream& os, bool bXMLFormat) const
{
	os << "(type: " << m_nType << ", scale: " << m_dScaleRatio
		<< ", orient: " << m_dOrient << ", dist1: " << m_dDist1
		<< ", dist2: " << m_dDist2 << ", bearing: " << m_dBearing << ')' << endl;
}

/*!
	@todo these pair of functions should cal read and write, but we
	still need a version of read that reads in text format for
	reading the GG from a text file.
*/

istream& dml::operator>>(istream& is, GGRelation& rel)
{
	is >> rel.m_nType >> rel.m_dScaleRatio >> rel.m_dOrient
		>> rel.m_dDist1 >> rel.m_dDist2 >> rel.m_dBearing;

	return is;
}

ostream& dml::operator<<(ostream& os, const GGRelation& rel)
{
	os << rel.m_nType << rel.m_dScaleRatio << rel.m_dOrient
		<< rel.m_dDist1 << rel.m_dDist2 << rel.m_dBearing;

	return os;
}

/////////////////////////////////////////////////////////////////////////////
// GGNode class implementation

// Begin DAGNode virtual functions

/*!
	Creates a new GGNode with a copy of the information contained in 'this'.
	This functions is required by SmartPtr<DAGNode*>::operator[].
*/
DAGNode* GGNode::CreateObject() const
{
	return (DAGNode*) new GGNode();
}

void GGNode::Clear()
{
	DAGNode::Clear();

	m_nType = 0;
	m_dSign = 0;
	m_dScale = 0;
	m_nXPos = 0;
	m_nYPos = 0;
	m_dA = 0;
	m_dB = 0;
	m_dOrientation = 0;
	m_dAnisotropy = 0;
	m_dUnkField = 0;
	m_nNodeIndex = 0;

	m_xCoords.Clear();
	m_weights.Clear();
	m_scales.Clear();
}

//! Extends the behaviour of the same function in the base class.
DAGNode& GGNode::operator=(const DAGNode& rhs)
{
	DAGNode::operator=(rhs);

	const GGNode* pRhs = dynamic_cast<const GGNode*>(&rhs);

	if (!pRhs)
		THROW_EXCEPTION("Invalid pointer type.");

	m_nType = pRhs->m_nType;
	m_dSign = pRhs->m_dSign;
	m_dScale = pRhs->m_dScale;
	m_nXPos = pRhs->m_nXPos;
	m_nYPos = pRhs->m_nYPos;
	m_dA = pRhs->m_dA;
	m_dB = pRhs->m_dB;
	m_dOrientation = pRhs->m_dOrientation;
	m_dAnisotropy = pRhs->m_dAnisotropy;
	m_dUnkField = pRhs->m_dUnkField;
	m_nNodeIndex = pRhs->m_nNodeIndex;

	m_xCoords = pRhs->m_xCoords;
	m_weights = pRhs->m_weights;
	m_scales = pRhs->m_scales;

	return *this;
}

//! Extends the behaviour of the same function in the base class.
istream& GGNode::Read(istream& is)
{
	DAGNode::Read(is);

	is.read((char*) &m_nType, sizeof(m_nType));
	is.read((char*) &m_dSign, sizeof(m_dSign));
	is.read((char*) &m_dScale, sizeof(m_dScale));
	is.read((char*) &m_nXPos, sizeof(m_nXPos));
	is.read((char*) &m_nYPos, sizeof(m_nYPos));
	is.read((char*) &m_dA, sizeof(m_dA));
	is.read((char*) &m_dB, sizeof(m_dB));
	is.read((char*) &m_dOrientation, sizeof(m_dOrientation));
	is.read((char*) &m_dAnisotropy, sizeof(m_dAnisotropy));
	is.read((char*) &m_dUnkField, sizeof(m_dUnkField));
	is.read((char*) &m_nNodeIndex, sizeof(m_nNodeIndex));

	m_xCoords.Read(is);
	m_weights.Read(is);
	m_scales.Read(is);

	return is;
}

//! Extends the behaviour of the same function in the base class.
ostream& GGNode::Write(ostream& os) const
{
	DAGNode::Write(os);

	os.write((char*) &m_nType, sizeof(m_nType));
	os.write((char*) &m_dSign, sizeof(m_dSign));
	os.write((char*) &m_dScale, sizeof(m_dScale));
	os.write((char*) &m_nXPos, sizeof(m_nXPos));
	os.write((char*) &m_nYPos, sizeof(m_nYPos));
	os.write((char*) &m_dA, sizeof(m_dA));
	os.write((char*) &m_dB, sizeof(m_dB));
	os.write((char*) &m_dOrientation, sizeof(m_dOrientation));
	os.write((char*) &m_dAnisotropy, sizeof(m_dAnisotropy));
	os.write((char*) &m_dUnkField, sizeof(m_dUnkField));
	os.write((char*) &m_nNodeIndex, sizeof(m_nNodeIndex));

	m_xCoords.Write(os);
	m_weights.Write(os);
	m_scales.Write(os);

	return os;
}

//! Extends the behaviour of the same function in the base class.
void GGNode::Print(ostream& os, bool bXMLFormat) const
{
	DAGNode::Print(os, bXMLFormat);

	os << endl;

	PRINT(os, m_nType);
	PRINT(os, m_dSign);
	PRINT(os, m_dScale);
	PRINT(os, m_nXPos);
	PRINTN(os, m_nYPos);
	PRINT(os, m_dA);
	PRINT(os, m_dB);
	PRINT(os, m_dOrientation);
	PRINT(os, m_dAnisotropy);
	PRINT(os, m_dUnkField);
	PRINTN(os, m_nNodeIndex);

	os << "x-coords: "; m_xCoords.Print(os); os << ", ";
	os << "weigths: "; m_weights.Print(os); os << ", ";
	os << "scales: "; m_scales.Print(os); os << endl;
}