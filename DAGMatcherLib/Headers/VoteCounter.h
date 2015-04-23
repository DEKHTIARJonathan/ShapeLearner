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
 * $Log: VoteCounter.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef _VOTECOUNTER_H_
#define _VOTECOUNTER_H_

#include "stdafx.h"

namespace dml {
typedef SmartArray<double> RealVector;

class QueryRange : public RealVector
{
	double commonDelta;

public:
	QueryRange(double delta, int dim) : RealVector(dim)
	{
		Set(delta);
		commonDelta = delta;
	}

	QueryRange(const RealVector& rangeVector) : RealVector(rangeVector)
	{
		commonDelta = 0;
	}

	QueryRange(const QueryRange& range) : RealVector(range)
	{
		commonDelta = range.commonDelta;
	}

	void operator=(const QueryRange& range)
	{
	    RealVector::operator=(range);
	    commonDelta = range.commonDelta;
	}

	bool IsInBounds(const double& x, const double& min, const double& max) const
	{
		return (x >= min && x <= max);
	}

	void GetDimBounds(const double& x, int dim, double& min, double& max) const
	{
		double delta = x * (*this)[dim];
		min = x - delta;
		max = x + delta;
	}

	double GetDimDelta() const
	{
		return commonDelta;
	}

	bool DoPtsOverlap(const TSV pt1, const TSV pt2) const;
};

///////////////////////////////////////////////////////////////////////////////////
// Helper classes for the MOOVC algorithm

/*!
	The user of he MOOVC algorithm does not need to use the following
	classes enclosed inside the MOOVC namespace. The user only needs
	to call ComputeVotesForModelGraphs, which is declared outside such namepsace.
*/
namespace MOOVC {
struct VOTE
{
	leda_node queryNode;   //!< query node that may vote for multiple model nodes
	int nQueryGraph;	   //!< ID of the query graph

	int nModelNode;   //!< model node that recives a vote from a given query node
	int nModelGraph;  //!< ID of the model graph

	double dWeight;   //!< the vote weight that sasfies 0 <= weight <= 1

	VOTE()
	{
		dWeight = 0.0;
	}

	void operator+=(const double& w)
	{
		dWeight += w;
	}

	static int Compare(const void* elem1, const void* elem2)
	{
		double a = ((const VOTE*)elem1)->dWeight, b = ((const VOTE*)elem2)->dWeight;
		return a > b ? -1:(a < b ? 1:0);
	}
};

class VoteBalance
{
	TSV queryNodeTSV;	//!< TSV of the the subgraph rooted at the query node
	int nQueryNodeLbl;	//!< Label of the node
	double dQueryNodeRelMass; //!< Relative mass of the subgraph rooted at the query node
	int nTSVDim;	//!< Max dimension off all TSV in the database
	double dAlpha; 	//!< Convexity parameter for the query rel mass vs. model node rel mass

public:

	VoteBalance(int nTSVDimension, double dConvexityParam = 0.5)
	{
		nTSVDim = nTSVDimension;
		dAlpha = dConvexityParam;
	}

	void SetQueryNode(const DAGPtr ptrDag, leda_node v)
	{
		queryNodeTSV = ptrDag->GetNodeTSV(v, nTSVDim);
		nQueryNodeLbl = ptrDag->NodeType(v);
		dQueryNodeRelMass = (1 - dAlpha) * ptrDag->GetNodeTSVNorm(v) / ptrDag->GetTotalTSVSum();
	}

	const TSV& GetQueryNodeTSV() const
	{
		return queryNodeTSV;
	}

	double Weight(const DAGSearchRecordEx& modelNodeInfo)
	{
		if (modelNodeInfo.nodeLbl != nQueryNodeLbl)
			return 0;

		double dModelNodeRelMass = dAlpha * modelNodeInfo.nodeTSV.Norm2() / modelNodeInfo.totalVote;
		double dNormDiff = Norm2(queryNodeTSV - modelNodeInfo.nodeTSV);

		return (dQueryNodeRelMass + dModelNodeRelMass) / (1 + dNormDiff);
	}
};

/*!
	A bounded bin is a sorted sequence, <sorted_data, other_inf>, whose length
	is at all times smaller or equal to nMaxSize.
*/
class BoundedBin : public leda_p_queue<double, int>
{
	int nMaxSize;
public:
	BoundedBin(int sz = 0) { SetMaxSize(sz); }
	void SetMaxSize(int sz) { nMaxSize = sz; }
	int GetMaxSize() const { return nMaxSize; }

	void AddVote(VOTE v)
	{
		ASSERT(size() <= nMaxSize);

		// Keep the size <= nMaxSize
		if (size() == nMaxSize)
		{
			pq_item min = find_min(); //O(1)

			if (prio(min) < v.dWeight)
				del_item(min); //O(log nMaxSize)
			else
				return; // nothing to insert
		}

		insert(v.dWeight, v.nModelNode); //O(log nMaxSize)
	}

	void Clear()
	{
		clear();
	}
};

class PartitionBins
{
	  SmartArray<BoundedBin> bins;
	  double dCumulativeVote;

	  // Ideally, the number of nodes in the model would be know.
	  // This should be save in the NN info. Temporarely we use a fixed-size map.
	  static SmartArray<leda_node> modelNodeMap;

public:
	  void AddVote(int idxElem, VOTE v)
	  {
		  bins[idxElem].AddVote(v);
		  dCumulativeVote += v.dWeight;
	  }

	  int GetNodeCount() const { return bins.GetSize(); }
	  int GetBinBound(int idxBin) const { return bins[idxBin].GetMaxSize(); }
	  double GetCumulativeVote() const { return dCumulativeVote; }

	  void SetPartitionSize(int nNodeCount)
	  {
		  bins.Resize(nNodeCount, false);
		  dCumulativeVote = 0;

		  for (int i = 0; i < nNodeCount; i++)
		  {
			  bins[i].Clear();
			  bins[i].SetMaxSize(nNodeCount);
		  }
	  }

	  void CreateVoteGraph(LEDA_GRAPH<int, double>& g) const;
	  double CompPartitionVote() const;
};

class Partition : public SmartArray<leda_node>
{
public:
	int NodeCount() const { return GetSize(); }
	void AddElement(leda_node v) { AddTail(v); }
};

class PartitionArray : public SmartArray<Partition>
{
	const DAG* pDag;

public:
	PartitionArray(const DAG* pOverlapGraph)
	{
		pDag = pOverlapGraph;
	}

	void SetSize(int sz)
	{
		Resize(sz, false);
	}

	void AddElement(int idxPart, leda_node v)
	{
		(*this)[idxPart].AddElement(v);
	}
};

class ModelBins
{
	SmartArray<double> modelVotes;
	SmartArray<PartitionBins> partBins;
	double dMinTotalModelVote;
	int nModelsAboveMinVote;

public:
	ModelBins(int nNumModels, double dMinModelVote)
	{
		modelVotes.Resize(nNumModels, true); //init to zero!
		partBins.Resize(nNumModels, false);
		dMinTotalModelVote = dMinModelVote;
		nModelsAboveMinVote = 0;
	}

	void AddVote(int idxModel, const double& vote)
	{
		double& currModelWeight = modelVotes[idxModel];
		bool flag = currModelWeight < dMinTotalModelVote;

		ASSERT(currModelWeight >= 0.0);

		currModelWeight += vote;

		if (flag && currModelWeight >= dMinTotalModelVote)
			nModelsAboveMinVote++;

		WARNING(currModelWeight > 1.01, currModelWeight);
	}

	void AddPartitionVotes(Partition& part, const DAGPtr ptrQueryDag, DAGDatabase& modelDB,
		const QueryRange& range, double dQueryNodeRelMass);

	SmartArray<VOTE> GetRankedVotes() const;

	ModelBins& operator=(const ModelBins& rhs)
	{
		modelVotes = rhs.modelVotes;
		partBins = rhs.partBins;

		return *this;
	}
};

/*!
 	@brief Helper function for the MOOVC algorithm. It computes a node
  	overlapping graph, that can be delivered to FindPartitions() to partition
   the query graph into groups of overlapping nodes.
*/
DAGPtr ComputeOverlappingNodeGraph(const DAGPtr ptrQueryDag, const QueryRange& r);

/*!
 	@brief Helper function for the MOOVC algorithm.
*/
PartitionArray FindPartitions(const DAGPtr ptrOverlapDag);
} // end namespace MOOVC

typedef SmartArray<MOOVC::VOTE> NNVoteArray;

/*!
	@brief The actual MOOVC algorithm. Returns a descendent sorted list model graph.
*/
NNVoteArray ComputeVotesForModelGraphs(const DAGPtr ptrQueryDag,
	DAGDatabase& modelDB, const QueryRange& r, double dQueryNodeRelMass, double dMinSimilarity);
} //namespace dml

#endif //_VOTECOUNTER_H_
