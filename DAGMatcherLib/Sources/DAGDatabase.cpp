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
 * $Log: DAGDatabase.cpp,v $
 * Revision 1.3  2002/06/13 21:48:48  dmac
 * Unknown change.
 *
 * Revision 1.2  2002/05/16 22:48:56  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"


#ifdef USE_TEMPLATE_BASED_MWBM_FUNCTION
#include <LEDA/graph/templates/mwb_matching.h>
using namespace std; // Fixes an error in the next LEDA header scale_weights.h line 35
#include <LEDA/graph/scale_weights.h>
#endif


/*!
	Compares the similarity value of each MatchInfo in descending order.
	If both objects have equal similarity, then their offset position
	in the database file is used to sort them (in ascending order), so
	that the results are deterministic and clear.
*/

int leda::compare(const dml::MatchInfo& a, const dml::MatchInfo& b)

{
	if (a.dSimilarity > b.dSimilarity)
		return -1;
	else if(a.dSimilarity < b.dSimilarity)
		return 1;
	else if (a.nDAGOffset < b.nDAGOffset)
		return -1;
	else if (a.nDAGOffset > b.nDAGOffset)
		return 1;
	else
		return 0;
}

using namespace std;
using namespace dml;
using namespace leda;

struct DBGINFO
{
	fstream		os;

	const DAG*	pDag;
	double		KorRange;
	double		modelSimWeight;

	leda_node	imgNode;
	double		imgNodeNorm;
	double		imgNodeTotalNorm;

	int			modelID;
	int			modelNode;
	double		modelNodeNorm;
	double		modelNodeTotalNorm;

	double		diffNorm;
	double		vote;
	double		maxVote;
	double		currVote;

	void SetCommonData(const DAG* p, double k, double w) {
		pDag = p; KorRange = k; modelSimWeight = w;
	}

	void SetImgNodeInfo(leda_node v, double n, double t) {
		imgNode = v; imgNodeNorm = n; imgNodeTotalNorm = t;
	}

	void SetModelNodeInfo(int id, int v, double n, double t) {
		modelID = id; modelNode = v; modelNodeNorm = n; modelNodeTotalNorm = t;
	}

	void SetVoteInfo(double d, double v, double cv) {
		diffNorm = d; vote = v; currVote = cv;
		maxVote = (1 - modelSimWeight) * imgNodeNorm / imgNodeTotalNorm +
			modelSimWeight * modelNodeNorm / modelNodeTotalNorm;
	}

	void Print();
} dbg;

void DBGINFO::Print()
{
	static bool bFirstTime = true;

	if (bFirstTime)
	{
		bFirstTime = false;

		os.open("voting.log", ios::trunc | ios::out);

		os << pDag->GetDAGLbl() << endl
			<< "k or range = " << KorRange << endl
			<< "weight of model similarity = " << modelSimWeight << endl;
	}

	os  << "\nModel ID: " << modelID
		<< "\nImg node: " <<  pDag->GetNodeLbl(imgNode) << ", norm: " << imgNodeNorm << ", total norm: " << imgNodeTotalNorm
		<< "\nMod node: " <<  modelNode << ", norm: " << modelNodeNorm << ", total norm: " << modelNodeTotalNorm
		<< "\nDiff norm: " << diffNorm
		<< "\nVote: " << vote << ", max vote: " << maxVote << ", curr max vote: " << currVote
		<< endl << flush;
}

void VoteList::AddVote(Vote& newVote)
{
	//leda_list_item it;

	if (dagId == -1)
		dagId = newVote.dagId;

	ASSERT(dagId == newVote.dagId);

	if (newVote.vote < 0.001)
		return;

	int imgNode = index(newVote.imgNode);
	leda_seq_item itu = modelNodes.lookup(newVote.modelNode);
	leda_seq_item itv = imgNodes.lookup(imgNode);

	leda_node u, v;

	if (itu == nil) {
		u = bipGraph.new_node(newVote.modelNode);
		modelNodes.insert(newVote.modelNode, u);
		modelNodeSet.push(u);
	}
	else
		u = modelNodes[itu];

	if (itv == nil) {
		v = bipGraph.new_node(imgNode);
		imgNodes.insert(imgNode, v);
		imgNodeSet.push(v);
	}
	else
		v = imgNodes[itv];

	/* The array access operator A[e] checks its precondition (A must be valid for e).
		The check can be turned off by compiling with the flag -DLEDA_CHECKING_OFF. */

	votes[bipGraph.new_edge(u, v, 1.0)] = newVote.vote;

	totalVote += newVote.vote;
}

double VoteList::GetTotalVote(const DAG& dag, const double& minTotalVote)
{
	if (totalVote < minTotalVote)
		return 0;

	leda_list_item it;
	double total = 0;

	//ASSERT(Is_Bipartite(bipGraph, modelNodeSet, imgNodeSet));

	//MWA_SCALE_WEIGHTS(bipGraph, votes); // See Notes.txt
	//leda_list<leda_edge> bestVotes = MWMCB_MATCHING(bipGraph, modelNodeSet, imgNodeSet, votes);

#ifdef USE_TEMPLATE_BASED_MWBM_FUNCTION
	scale_weights(bipGraph, votes, 3.0);
	leda_list<leda_edge> bestVotes = MAX_WEIGHT_BIPARTITE_MATCHING(bipGraph, votes);
#else
	MWBM_SCALE_WEIGHTS(bipGraph, votes); // See Notes.txt
	leda_list<leda_edge> bestVotes = MAX_WEIGHT_BIPARTITE_MATCHING(bipGraph, votes);
#endif //USE_TEMPLATE_BASED_MWBM_FUNCTION

	forall_items(it, bestVotes)
		total += votes[bestVotes[it]];

	return (total >= minTotalVote) ? total:0.0;
}

bool DAGDatabase::Open(const char* szName, ios_base::openmode nMode)
{
	m_bIndexOpened = false;
	m_dagFile.clear();
	m_dagFile.open(szName, nMode);

	return !m_dagFile.fail();
}

bool DAGDatabase::OpenIndex()
{
	char szIdxFileName[MAX_PATH_SIZE];

	DirWalker::ChangeFileExt(GetFileName(), "idx", szIdxFileName);

	m_bIndexOpened = m_index.Open(szIdxFileName);

	if (m_bIndexOpened)
	{
		int dif = DirWalker::CompareLastModifDate(GetFileName(), szIdxFileName);

		if (dif > 0)
		{
			m_index.Close();
			remove(szIdxFileName);
			m_bIndexOpened = false;
		}
	}

	if (!m_bIndexOpened)
	{
		DAGPtr ptrDag;
		int i, nMaxDim = 0;
		int nDAGCount = GetDAGCount();

		nMaxDim = m_dagFile.GetMaxTSVDimension();
		m_bIndexOpened = m_index.Create(szIdxFileName, nMaxDim);

		// Create index
		if (m_bIndexOpened && nMaxDim > 0)
		{
			cout << "Creating index of " << nMaxDim << " dimensions... " << flush;

			try {
				for (i = 0; i < nDAGCount; i++)
				{
					ptrDag = m_dagFile.ReadDAG(i, true);
					m_index.Add(ptrDag->GetDAGId(), ptrDag);
				}
			} catch(ExceptionInfo e) {
				e.Print();
				return false;
			}

			cout << "The DB index has beed created!" << endl;
		}
		else
			cerr << "\nERROR: Cannot create index (max dim: " << nMaxDim << ")\n";
	}

	return m_bIndexOpened;
}

/*bool DAGDatabase::ReadObjClass(DAG* pDAGReader, const char* szObjName)
{
	String strLbl;

	MoveToBeg();

	while (Next(pDAGReader))
	{
		if (pDAGReader->GetObjName() == szObjName)
		{
			DAGPtr ptr = pDAGReader->CreateObject();
			*ptr = *pDAGReader;
			m_dagList.Append(ptr);
		}
		else if (m_dagList.GetSize() > 0)
			break;
	}

	return true;
}*/

/*bool DAGDatabase::ReadAll(DAG* pDAGReader, int nMaxCount)
{
	MoveToBeg();

	while (nMaxCount-- && Next(pDAGReader))
	{
		DAGPtr ptr = pDAGReader->CreateObject();
		*ptr = *pDAGReader;
		m_dagList.Append(ptr);
	}

	return true;
}*/

int DAGDatabase::GetSimilar(const DAG& dag, leda_array<MatchInfo>& matched,
							double range, const double& w, const double& minSimilarity)
{
	ASSERT(m_bIndexOpened);
	ASSERT_UNIT_INTERVAL(w);
	ASSERT(minSimilarity >= 0);

	if (DAG::IsDbgMode())
		dbg.SetCommonData(&dag, range, w);

	// Range is used to choose the method to compute the votes
	// If it is negative, we use the old method. In turn, if abs(range)
	// is greater that 1, we use a K NN-search instead of a range search.
	if (range < 0)
	{
		range = -range; // we use the absolute value

		TSV nodeTSV;
		double nodeTSVL2, targetNodeSim, modelNodeSim, dagTotalVote, diffNorm;
		leda_node v;
		int id, offset, i, size, nodeLbl, lbl;
		SmartArray<DAGSearchRecordEx> kClosest;
		leda_h_array<int, VoteList> viewBins(0, 1024);
		Vote nodeVote(&dag);

		// Compute max total vote for the given dag
		dagTotalVote = dag.GetTotalTSVSum();

		forall_nodes(v, dag)
		{
			nodeTSVL2 = dag.GetNodeTSVNorm(v);

			if (nodeTSVL2 == 0)
				continue;

			nodeTSV = dag.GetNodeTSV(v, m_index.GetDim());
			nodeLbl = dag.NodeType(v);

			// Get the closest neighbours of the node
			if (range >= 1)
				kClosest = m_index.GetKClosest(dag, v, (int)range);
			else
				kClosest = m_index.GetClosest(dag, v, range);

			size = kClosest.GetSize();

			//For each neighbour perform object model count
			for(i = 0; i < size; i++)
			{
				lbl = kClosest[i].nodeLbl;

				if (nodeLbl != lbl)
					continue;

				id = kClosest[i].id;
				offset = kClosest[i].offset;

				// Must weight each vote...

				if (DAGMatcher::GetMatchParams().nUseNewVoteWeightFunc)
				{
					// NEW VOTE WEIGHTING FUNCTION
					targetNodeSim = (1 - w) * nodeTSVL2 / dagTotalVote;
					modelNodeSim = w * kClosest[i].nodeTSV.Norm2() / kClosest[i].totalVote;
					diffNorm = Norm2(nodeTSV - kClosest[i].nodeTSV);

					nodeVote.vote = (targetNodeSim + modelNodeSim) / (1 + diffNorm);
				}
				else
				{
					// OLD VOTE WEIGHTING FUNCTION
					nodeVote.vote = nodeTSVL2 / (1 + Norm2(nodeTSV - kClosest[i].nodeTSV));
				}

				// Save the weighted vote

				if (nodeVote.vote > 0)
				{
					nodeVote.dagId = id;
					nodeVote.dagOffset = offset;
					nodeVote.imgNode = v;
					nodeVote.modelNode = kClosest[i].node;
					viewBins[id].AddVote(nodeVote);
				}

				if (DAG::IsDbgMode())
				{
					dbg.SetImgNodeInfo(v, nodeTSVL2, dagTotalVote);
					dbg.SetModelNodeInfo(id, kClosest[i].node, kClosest[i].nodeTSV.Norm2(), kClosest[i].totalVote);
					dbg.SetVoteInfo(diffNorm, nodeVote.vote, 0.0);

					dbg.Print();
				}
			}
		}

		// Discard the prototypes that aren't similar enough
		i = viewBins.size();
		double sim;

		matched.resize(i);

		if (i > 0)
		{
			// Return prototypes ordered by number of votes
			i = 0;

			forall_defined(id, viewBins)
			{
				VoteList& vl = viewBins[id];
				// Use one-to-one mapping or the old bad idea of summing all the votes?
				sim = (DAGMatcher::GetMatchParams().nUseMOOVC) ? vl.GetTotalVote(dag, minSimilarity):vl.SumAllVotes();
				offset = vl.GetDAGFileOffset();
				matched[i++].Set(id, offset, sim);
			}

			matched.sort();
		}
	}
	else
	{
		QueryRange rangeVector(range, dag.GetMaxTSVDimension());

		NNVoteArray votes = ComputeVotesForModelGraphs(&dag, *this, rangeVector, w, minSimilarity);

		matched.resize(votes.GetSize());

		// Copy the result to the matched array
		for (int i = 0; i < votes.GetSize(); i++)
			matched[i].Set(votes[i].nModelGraph, 0, votes[i].dWeight);
	}

	return matched.size();
}

const DAGPtr DAGDatabase::ChooseDAG() const
{
	// Choose a random dag
	srand( (unsigned)time( NULL ));
	int nTarget = rand() % m_dagList.size();

	const DAGPtr pTargetDag = m_dagList[m_dagList.get_item(nTarget)];

	return pTargetDag;
}

void DAGDatabase::ShowDAGList(ostream& os)
{
	const int DAGID = 15;
	const int OBJNAME = 45;
	leda_list_item it;

	os.flags(ios::left);
	os << endl << setw(DAGID) << "DAG ID" << setw(OBJNAME) << "DAG Label" << endl;

	forall_items(it, m_dagList)
	{
		os << endl;
		os << setw(DAGID)   << m_dagList[it]->GetDAGId();
		os << setw(OBJNAME) << m_dagList[it]->GetDAGLbl();
	}

	os << endl << flush;
}

void DAGDatabase::ListDAGs(ostream& os)
{
	typedef std::list< std::pair<int, int> > Boundaries;

	const int DAGID = 15;
	const int OBJNAME = 25;
	const int VIEWNUM = 10;
	const int MAXTSVD = 10;
	int nDAGCount = GetDAGCount();
	DAGPtr ptrDag;

	os.flags(ios::left);
	os << '\n' << setw(DAGID) << "DAG ID"
		<< setw(OBJNAME) << "Obj Name"
		<< setw(VIEWNUM) << "View0"
		<< setw(VIEWNUM) << "View1"
		<< setw(MAXTSVD) << "Nodes" << endl;
		//<< setw(MAXTSVD) << "Max TSV Dim" << endl;

	int view0, view1;
	char className0[MAX_PATH_SIZE], className1[MAX_PATH_SIZE];
	int modelId, firstId;
	Boundaries classBoundaries;

	for (int i = 0; i < nDAGCount; i++)
	{
		ptrDag = m_dagFile.ReadDAG(i, true);

		ptrDag->GetViewNumbers(&view0, &view1);

		os << endl;
		os << setw(DAGID)   << i;
		os << setw(OBJNAME) << ptrDag->GetObjName();
		os << setw(VIEWNUM) << view0; //ptrDag->GetViewNumber();
		os << setw(VIEWNUM) << view1; //ptrDag->GetViewNumber();
		os << setw(MAXTSVD) << ptrDag->GetNodeCount();
		//os << setw(MAXTSVD) << ptrDag->GetMaxTSVDimension();

		ptrDag->GetClassAndModel(className1, &modelId);

		os << className1;

		if (i == 0)
		{
			strcpy(className0, className1);
		}
		else if (strcmp(className0, className1) || i + 1 == nDAGCount)
		{
			firstId = (classBoundaries.empty()) ?
				0 : classBoundaries.back().second + 1;

			classBoundaries.push_back(make_pair(firstId, i - 1));

			strcpy(className0, className1);
		}
	}

	os << endl << "Total number of objects and views: " << nDAGCount << endl;

	/*
	// Show the start and end indices of each group of consecutive DAGs
	// of the same class
	Boundaries::const_iterator it = classBoundaries.begin();

	for (; it != classBoundaries.end(); it++)
	{
		os << it->first << ", " << it->second << "; ";
	}*/

	os << endl;
}