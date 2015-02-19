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
 * Author: Diego Macrini (dmac@cs.toronto.edu)
 *
 *-----------------------------------------------------------------------*/

#include <iostream>
#include <stdio.h>
#include "memory.h"
#include "DAGNode.h"
#include "HelperFunctions.h"
#include "BasicUtils.h"

using namespace dml;

//! DAGNode constructor
DAGNode::DAGNode(NODE_LABEL lbl)
{
	Clear();
	nodeLbl = lbl;
	color = leda::white;
}

//! Initializes the contents of the object to their original values.
void DAGNode::Clear()
{
	nodeLbl	 = NODE_LBL_DEFAULT_VAL;
	childrenTSV.Clear();

	eigenLbl = -1.0;
	tsvNorm = -1;
	nMass = -1;
	nLevel = -1;
	nDFSIndex = -1;
}

//! Prints the DAGNode to the specified stream.
void DAGNode::Print(std::ostream& os, bool bXMLFormat) const
{
	if (bXMLFormat)
	{
		XML_PRINT_ATT2(os, "index", nDFSIndex);
		XML_PRINT_ATT2(os, "label", nodeLbl);
		XML_PRINT_ATT2(os, "level", nLevel);
		XML_PRINT_ATT2(os, "mass", nMass);
		XML_END_ATTS(os);
	}
	else
	{
		PRINT_OPEN(os, nodeLbl);
		PRINT(os, eigenLbl);
		PRINT(os, tsvNorm);
		PRINT(os, nMass);
		PRINT(os, nLevel);
		PRINT_CLOSE(os, nDFSIndex);

		childrenTSV.Print(os);
	}
}

//! Copies the info of <rhs> into the corresponding member variables.
DAGNode& DAGNode::operator=(const DAGNode& rhs)
{
	nodeLbl		= rhs.nodeLbl;
	childrenTSV = rhs.childrenTSV;

	eigenLbl	= rhs.eigenLbl;
	tsvNorm		= rhs.tsvNorm;
	nMass		= rhs.nMass;
	nLevel		= rhs.nLevel;
	nDFSIndex	= rhs.nDFSIndex;

	color       = rhs.color;

	return *this;
}

/*!
	Reads a DAGNode from the specified stream.

	\pre nodeLbl is a leda_string. Otherwise, this functios has to be modified.
*/
std::istream& DAGNode::Read(std::istream& is)
{
	nodeLbl.Read(is);
	childrenTSV.Read(is);

	is.read((char*) &eigenLbl, sizeof(eigenLbl));
	is.read((char*) &tsvNorm, sizeof(tsvNorm));
	is.read((char*) &nMass, sizeof(nMass));
	is.read((char*) &nLevel, sizeof(nLevel));
	is.read((char*) &nDFSIndex, sizeof(nDFSIndex));
	is.read((char*) &subtreeCost, sizeof(subtreeCost));

	return is;
}

/*!
	Writes the DAGNode to the specified stream.

	\pre nodeLbl is a leda_string. Otherwise, this functios has to be modified.
*/
std::ostream& DAGNode::Write(std::ostream& os) const
{
	nodeLbl.Write(os);
	childrenTSV.Write(os);

	os.write((char*) &eigenLbl, sizeof(eigenLbl));
	os.write((char*) &tsvNorm, sizeof(tsvNorm));
	os.write((char*) &nMass, sizeof(nMass));
	os.write((char*) &nLevel, sizeof(nLevel));
	os.write((char*) &nDFSIndex, sizeof(nDFSIndex));
	os.write((char*) &subtreeCost, sizeof(subtreeCost));

	return os;
}

/*!
	Returns the node's TSV with dimension equal <nDim>. The TSV
	is filled with extra zeros if necessary.

	The TSV of a node is composed of the eigen sums of its adjacent
	nodes and its own eigen sum.

	\param nDim specified the desired dimension. If zero, the current
	TSV dimension is used.
*/
TSV DAGNode::GetTSV(int nDim) const
{
	ASSERT(nDim > 0);

	TSV tsv(nDim);
	int i;

	tsv[0] = eigenLbl;

	for (i = 1; i < nDim && i - 1 < childrenTSV.GetSize(); i++)
		tsv[i] = childrenTSV[i - 1];

	for (; i < nDim; i++)
		tsv[i] = 0;

	// Do not sort it. I want it in a constant position
	// since it is a distinct value from the others.
	// no -> tsv.Sort();

	return tsv;
}

/*!
	Computes the node's TSV norm considering that the TSV
	includes the eigenvalue of the node too.

*/
void DAGNode::ComputeDerivedValues()
{
	double n = childrenTSV.Norm2();

	tsvNorm = sqrt(n * n + eigenLbl * eigenLbl);

	ASSERT(tsvNorm >= 0);
}

NODE_LABEL DAGNode::GetLblForGraph(bool bExtraInfo /*= true*/) const
{
	return GetNodeLbl();
}

leda::color DAGNode::GetColorForGraph() const
{
	//return leda::color(255, 255, 255);
	return GetColor();
}

int DAGNode::GetShapeForGraph() const
{
	return 1; // avoid having to include graphwin.h for leda::ellipse_node
}