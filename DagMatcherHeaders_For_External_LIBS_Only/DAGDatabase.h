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
 * $Log: DAGDatabase.h,v $
 * Revision 1.2  2002/05/16 22:48:56  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef _DAG_DATABASE_H_
#define _DAG_DATABASE_H_

#include <iostream>
#include <string>
#include <LEDA/core/list.h>
#include <LEDA/core/sortseq.h>
#include <LEDA/core/array.h>

#include "DAGSearchDatabase.h"
#include "DAGDBFile.h"

namespace dml {
class MatchInfo;
}

#ifdef WIN32
namespace leda {
int compare(const dml::MatchInfo& a, const dml::MatchInfo& b);
}
#endif

namespace dml {
#ifndef WIN32
int compare(const dml::MatchInfo& a, const dml::MatchInfo& b);
#endif

class DAGList : public leda_list<DAGPtr>
{
	int nMaxDim;

public:
	DAGList() { nMaxDim = 0; }
	int GetMaxDim() const { return nMaxDim; }
	//void SetMaxDim(int nDim) { nMaxDim = nDim; }
	void Append(DAGPtr pDAG)
	{
	   int n = ((const DAGPtr)pDAG)->GetMaxTSVDimension();

	   if (n > nMaxDim)
		 nMaxDim = n;

	  append(pDAG);
	}
	int GetSize() const { return size(); }
};

class MatchInfo
{
public:
    int nDAGId;
	int nDAGOffset;
	double dSimilarity;
	DAGPtr ptrDAG;
	int nQueryDBId;
	int nModelDBId;
	std::string nodeMatches;

	MatchInfo()
	{
		nDAGId = 0;
		nDAGOffset = 0;
		dSimilarity = 0;
	}

	void operator=(const MatchInfo& rhs)
	{
		nDAGId      = rhs.nDAGId;
		nDAGOffset  = rhs.nDAGOffset;
		dSimilarity = rhs.dSimilarity;
		ptrDAG      = rhs.ptrDAG;
		nQueryDBId  = rhs.nQueryDBId;
		nModelDBId  = rhs.nModelDBId;
		nodeMatches = rhs.nodeMatches;
	}

	void Set(int id, int offset, double similarity,
		int queryDBId = -1, int modelDBId = -1)
	{
		nDAGId = id;
		nDAGOffset = offset;
		dSimilarity = similarity;
		nQueryDBId = queryDBId;
		nModelDBId = modelDBId;
		nodeMatches.clear();
	}

	void Set(DAGPtr ptr, double similarity,
		int queryDBId = -1, int modelDBId = -1)
	{
		ptrDAG = ptr;
		dSimilarity = similarity;
		nDAGOffset = ((const DAG*)ptr)->GetFileOffset();
		nDAGId = ((const DAG*)ptr)->GetDAGId();
		nQueryDBId = queryDBId;
		nModelDBId = modelDBId;
		nodeMatches.clear();
	}

	void Set(DAGPtr ptr, double similarity, const std::string& nodeCorr)
	{
		Set(ptr, similarity);

		nodeMatches = nodeCorr;
	}

	const char* GetDAGLbl() const
	{
		return ptrDAG->GetDAGLbl();
	}

#ifdef WIN32
	friend int leda::compare(const MatchInfo& a, const MatchInfo& b);
#else
	friend int compare(const MatchInfo& a, const MatchInfo& b);
#endif

	// The stream operators are implemented only to satisfy LEDA requirements
	friend std::ostream& operator<<(std::ostream &os, const MatchInfo& mi) { return os; }
	friend std::istream& operator>>(std::istream &is, MatchInfo& mi) { return is; }
};

struct Vote
{
	int modelNode;
	leda_node imgNode;
	double vote;
	int dagId;
    int dagOffset;
    const DAG* pDag;

	Vote(const DAG* pImgDag)
	{
		pDag = pImgDag; modelNode = 0; imgNode = nil; vote = 0;
	}

	void operator=(const Vote& v)
	{
		modelNode = v.modelNode;
		imgNode   = v.imgNode;
		vote      = v.vote;
		dagOffset = v.dagOffset;
		dagId 	  = v.dagId;
	}
};

class VoteList //: public leda_list<Vote>
{
	int dagId;		//!< DAG's ID.
	int dagOffset; 	//!< In case we need direct acces to the record. Not used anymore.
	LEDA_GRAPH<int, double> bipGraph;
	leda_list<leda_node> modelNodeSet; // Smaller set in general
	leda_list<leda_node> imgNodeSet; // Larger in general
	leda_edge_map<double> votes;

	leda_sortseq<int, leda_node> modelNodes;
	leda_sortseq<int, leda_node> imgNodes;

	double totalVote;

public:
	VoteList(int prot = -1) { dagId = -1; dagOffset = -1; totalVote = 0; }
	void AddVote(Vote& vote);
	double GetTotalVote(const DAG& dag, const double& minTotalVote);
	double SumAllVotes() const { return totalVote; }
	int GetDAGFileOffset() const { return dagOffset; }
};

typedef SmartPtr<DAGList> DAGListPtr;

class DAGDatabase
{
	DAGDBFile			m_dagFile;
	DAGSearchDatabase	m_index;
	bool				m_bIndexOpened;

public:
	DAGList				m_dagList;

public:
	DAGDatabase() {	m_bIndexOpened = false; }

	//! Returns true iff the file exists and has non-zero size
	bool CheckFileExist(const char* szName)
	{
		bool bFileExist;

		// Set 'ate' (at end) mode so that tellg > 0 if file
		// is not empty
		bFileExist = (Open(szName, std::ios_base::in |
			std::ios_base::binary | std::ios_base::ate)
			&& m_dagFile.tellg() > 0);

		Close();

		return bFileExist;
	}

	/*!
		Creates a file with writing privileges. If bCheckExist is
		true, the operation fails if the file already exists.
	*/
	bool Create(const char* szName, bool bCheckExist = true)
	{
		if (!CheckFileExist(szName))
		{
			return Open(szName, std::ios_base::out | std::ios_base::binary
				| std::ios_base::trunc); //noreplace
		}
		else if (!bCheckExist) // open the existing file
		{
			return Open(szName, std::ios_base::out | std::ios_base::binary);
		}

		return false;
	}

	bool Open(const char* szName, std::ios_base::openmode nMode = std::ios_base::in
		| std::ios_base::binary | std::ios_base::ate);

	bool OpenIndex();

	void Clear()
	{
		if (m_dagFile.is_open())
			m_dagFile.close();

		if (m_bIndexOpened)
		{
			m_index.Close();
			m_bIndexOpened = false;
		}

		m_dagList.clear();
	}

	void Close() { Clear(); }

	int GetDAGCount() const { return m_dagFile.GetDAGCount(); }
	String GetFileName() const { return m_dagFile.GetName(); }

	DAGPtr ReadDAG(int nDagId, bool bOnlyDataForMatching = false)
	{
		return m_dagFile.ReadDAG(nDagId, bOnlyDataForMatching);
	}

	DAGPtr ReadDAG(std::streamoff nDagOffset, const String& strClassName)
	{
		return m_dagFile.ReadDAG(nDagOffset, strClassName);
	}

	bool AddDAG(const DAG* pDag, bool bReadSaveTailInfo = false)
	{
		return m_dagFile.AddDAG(pDag, bReadSaveTailInfo);
	}

	bool ReadObjView(DAG* pDAG, const char* szDAGLabel);
	bool ReadObjView(DAG* pDAG, int nRRN);
	bool ReadObjClass(DAG* pDAGReader, const char* szObjName);
	bool ReadAll(DAG* pDAGReader, int nMaxCount = -1);

	int GetSimilar(const DAG& dag, leda_array<MatchInfo>& matched, double range,
		const double& w, const double& minSimilarity);

	int GetMaxTSVDimension() const
	{
		return m_index.GetDim();
	}

	const DAGPtr ChooseDAG() const;
	void ShowDAGList(std::ostream& os = std::cout);
	void ListDAGs(std::ostream& os = std::cout);

	DAGSearchDatabase& GetIndexDB() { ASSERT(m_bIndexOpened); return m_index; }
};
} //namespace dml

#endif //_DAG_DATABASE_H_
