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
#ifndef __BONE_GRAPH_CONSTRUCTOR_H__
#define __BONE_GRAPH_CONSTRUCTOR_H__

#include "GeneralizedSkeletalGraph.h"
//#include <LEDA/graph/node_array.h>

namespace dml {
class BoneGraph;

//! Construction parameters for BoneGraph and GeneralizedSkeletalGraph
struct BoneGraphParams : public GSGParams
{
	int nTau1, nSmoothIter, nSpecializeBones, nSubtractLigatureFromJunctions;
	double dMinLeafSaliency, dMinErrorRate, dMinSlope, dMaxYDiff, dMaxAccelChg;
	int nMaxNumSegments;

	BoneGraphParams()
	{
		nTau1 = 0;
		nSmoothIter = 0;
		nSpecializeBones = 1;
		nSubtractLigatureFromJunctions = 0;

		dMinLeafSaliency = 0;
		dMinErrorRate = 2.0;
		dMinSlope = 0.05;
		dMaxYDiff = 0.1;
		dMaxAccelChg = 0.5;

		nMaxNumSegments = 8;
	}
};

struct BoneLigament
{
	BranchSegment boneSegment;
	LigatureSegment ligamentSegment;
	leda::node bgNode;

	BGElement::AxisFunction axisFunction;
	BGElement::RadiusFunction radiusFunction;

	BoneLigament() { bgNode = nil; }

	void SetAxisAndRadiusFunctions(const BoneGraphParams& params);
	bool SetCornerInfo(const BoneGraphParams& params, LigatureSegment* pLigSeg);
};

class BoneLigamentList : public std::list<BoneLigament>
{
public:
	typedef std::list<BoneLigament>::iterator iterator;
	typedef std::list<BoneLigament>::const_iterator const_iterator;

	BoneLigamentList(const LigatureSegmentList& lsl,
		const BranchSegment& nodeBS);

	void RemoveCornerSegments(const BoneGraphParams& params);

	void SubpartitionBones(const BoneGraphParams& params);

	iterator RemoveLigature(iterator it);
};

/*!
	@brief The construction of a bone graph from a skeleton graph
	is a complex operation that deserves its own class.

	In particular, there are several classes and objects that are only
	related to the construction of a bone graph and do not need to be
	around after the bone graph is created.

	A bone graph has 2 types of entitities: bones (nodes) and ligaments (edges).
	A ligament has zero saliency if it only connects bones and small saliency
	if it also describes boundary points.
*/
class BoneGraphConstructor
{
	typedef std::pair<leda::node, BranchSegment> BoneInfo;
	typedef std::vector<BoneInfo> BoneArray;

	struct BAPosition
	{
		unsigned int pos;		//!< Bone index in a bone array
		bool valid;				//!< true iff bone is in the array
		unsigned int subIndex;  //!< Position within the bone

		BAPosition() { }

		BAPosition(unsigned int p, bool v, unsigned int s)
		{
			pos      = p;
			valid    = v;
			subIndex = s;
		}
	};

protected:
	GeneralizedSkeletalGraph m_genSkelGraph;

	// Bone graph creation parameters
	BoneGraph* m_pBoneGraph;                        //!< Holds the new bone graph
	BoneGraphParams m_params;                       //!< Construction parameters
	int m_nLastIndexUsed;                           //!< Last node index used
	double m_dTotalArea;                            //!< Total shape area of the bone graph

	leda::node_map<BoneArray>   m_gsgNodeToBones;
	std::vector<leda::edge> m_nestedLigatureEdges;

protected:

public:
	BoneGraphConstructor(BoneGraph* pBoneGraph, const BoneGraphParams& bgparams);

	bool CreateBoneGraph(SkeletalGraph* pSkeletalGraph);

	void Print(std::ostream& os = std::cout) const { }

	static void ComputeAxisFunction(const POINTS& dataPts,
		const BoneGraphParams& params,
		BGElement::AxisFunction& axisFunction) { }

	static void ComputeRadiusFunction(const POINTS& dataPts,
		const BoneGraphParams& params,
		BGElement::RadiusFunction& radiusFunction) { }
};
} //namespace dml

#endif //__BONE_GRAPH_CONSTRUCTOR_H__
