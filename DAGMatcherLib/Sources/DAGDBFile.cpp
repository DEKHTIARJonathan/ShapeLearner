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
 * $Log: DAGDBFile.cpp,v $
 * Revision 1.1  2002/05/21 16:51:43  dmac
 * New file added.
 *
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

// Graph Ids used in the database
#define UNK_GRAPH        0
#define SHOCK_GRAPH      1
#define GESTURE_GRAPH    2
#define BONE_GRAPH       3

using namespace dml;

int DAGDBFile::GetClassID(const String& strClassName) const
{
	if (strClassName == "ShockGraph")
		return SHOCK_GRAPH;
	else if (strClassName == "GestureGraph")
		return GESTURE_GRAPH;
	else if (strClassName == "BoneGraph")
		return BONE_GRAPH;

	DBG_MSG2("Warning: Unknown graph class:", strClassName);
	return UNK_GRAPH;
}

DAG* DAGDBFile::CreateDAGObj(int nClassID) const
{
	switch (nClassID)
	{
		case SHOCK_GRAPH:
			return new ShockGraph;
		case GESTURE_GRAPH:
			return new GestureGraph;
		case BONE_GRAPH:
			return new BoneGraph;
	}

	DBG_MSG2("Warning: Unknown class ID:", nClassID);
	return NULL;
}

//! Overload std::fstream::open
void DAGDBFile::open(const char* szName, ios_base::openmode nMode)
{
	ASSERT(!IsDBModified());

	// append cannot be specified because we need to overwrite
	// the tail info!
	if (nMode & ios_base::app)
	{
		nMode &= ~ios_base::app;
		nMode |= ios_base::out;
	}

	nMode |= ios_base::in; // We always need to read the tail info

	m_strFileName = szName;
	std::fstream::open(szName, nMode);

	if (!fail())
	{
		if (!ReadTailInfo())
		{
			if (!(nMode & ios_base::out))
			{
				std::fstream::close();

				nMode |= ios_base::out;
				std::fstream::open(szName, nMode);
			}

			ShowMsg("Warning: the database is corrupted. Trying to fix it...");

			if (RebuildTailInfo())
			{
				clear(); // clears all error flags
				ShowMsg("...the database was fixed successfully!");
			}
			else
				THROW_EXCEPTION("Cannot fix corrupted database.");
		}
	}
}

//! Overload std::fstream::close
void DAGDBFile::close()
{
	if (IsDBModified())
		WriteTailInfo();

	m_nDBSize = 0;
	SetDBModified(false);
	m_strFileName.Clear();
	DBInfo().Clear();
	m_dagInfo.Clear();

	std::fstream::close();
}

bool DAGDBFile::ReadTailInfo()
{
	ASSERT(is_open());
	ASSERT(!IsDBModified());

	MoveToEnd();
	m_nDBSize = tellg();

	if (m_nDBSize > 0)
	{
		MoveTo(m_nDBSize - sizeof(DBINFO));

		read((char*) m_pDBInfo, sizeof(DBINFO));

		ASSERT(gcount() == sizeof(DBINFO));

		if (!DBInfo().IsValid())
			return false;

		MoveTo(DBInfo().m_nTailOffset);
		m_dagInfo.Read(*this);
	}

	return true;
}

void DAGDBFile::WriteTailInfo()
{
	seekp(DBInfo().m_nTailOffset);

	DBInfo().MakeValid();

	m_dagInfo.Write(*this);

	write((char*) m_pDBInfo, sizeof(DBINFO));
	flush();

	SetDBModified(false);
}

DAGPtr DAGDBFile::ReadDAG(int nDagId, bool bOnlyDataForMatching /*= false*/)
{
	ASSERT(nDagId < m_dagInfo.GetSize());

	DAGPtr ptrDag;
	DAGINFO info = m_dagInfo[nDagId];

	ASSERT(info.nOffset < DBInfo().m_nTailOffset);
	MoveTo(info.nOffset);

	ptrDag = CreateDAGObj(info.nType);
	ASSERT(!ptrDag.IsNull());

	try {
		ptrDag->Read(*this, bOnlyDataForMatching);
	}
	catch(ExceptionInfo e)
	{
		e.Print();
		return (DAG*)NULL;
	}

	ptrDag->SetDAGId(nDagId);

	return ptrDag;
}

DAGPtr DAGDBFile::ReadDAG(streamoff nDagOffset, const String& strClassName)
{
	DAGPtr ptrDag;

	ptrDag = CreateDAGObj(GetClassID(strClassName));

	if (!ptrDag.IsNull())
	{
		ASSERT(nDagOffset < DBInfo().m_nTailOffset);
		MoveTo(nDagOffset);

		try {
			ptrDag->Read(*this);
		}
		catch(ExceptionInfo e)
		{
			e.Print();
			return (DAG*)NULL;
		}
	}

	return ptrDag;
}

bool DAGDBFile::AddDAG(const DAG* pDag, bool bReadSaveTailInfo)
{
	DAGINFO info;

	if (bReadSaveTailInfo)
		ReadTailInfo();

	info.nType = GetClassID(pDag->ClassName());
	ASSERT(info.nType != UNK_GRAPH);

	info.nOffset = DBInfo().m_nTailOffset;
	seekp(DBInfo().m_nTailOffset);

	pDag->Write(*this);
	DBInfo().m_nTailOffset = tellp();

	m_dagInfo.AddTail(info);

	if (pDag->GetMaxTSVDimension() > DBInfo().m_nMaxTSVDimension)
		DBInfo().m_nMaxTSVDimension = pDag->GetMaxTSVDimension();

	SetDBModified(true);
	flush();

	if (bReadSaveTailInfo)
		WriteTailInfo();

	return true;
}

bool DAGDBFile::RebuildTailInfo()
{
	String strClassName;
	streampos pos;
	DAGPtr ptrDag;
	DAGINFO dagInfo;

	MoveToEnd();
	DBInfo().m_nTailOffset = tellg();
	DBInfo().m_nMaxTSVDimension = 0;

	MoveToBeg();

	while(!fail())
	{
		pos = tellg();
		strClassName.Read(*this);

		ptrDag = ReadDAG(pos, strClassName);

		if (ptrDag.IsNull())
			break;

		dagInfo.nOffset = pos;
		dagInfo.nType = GetClassID(strClassName);

		m_dagInfo.AddTail(dagInfo);

		if (ptrDag->GetMaxTSVDimension() > DBInfo().m_nMaxTSVDimension)
			DBInfo().m_nMaxTSVDimension = ptrDag->GetMaxTSVDimension();
	}

	return SetDBModified(m_dagInfo.GetSize() > 0);
}