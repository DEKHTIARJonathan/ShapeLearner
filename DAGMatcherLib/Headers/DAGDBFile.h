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
 * $Log: DAGDBFile.h,v $
 * Revision 1.2  2002/06/14 23:13:03  dmac
 * Matching algorithm updated.
 *
 * Revision 1.1  2002/05/21 16:51:43  dmac
 * New file added.
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef _DAG_DB_FILE_H_
#define _DAG_DB_FILE_H_

#include "stdafx.h"

#define GROW_FACTOR	20

namespace dml {
class DAGDBFile : public std::fstream
{
	struct DAGINFO
	{
		int nType;
		std::streampos nOffset;

		friend std::istream& operator>>(std::istream& is, DAGINFO& x)
		{
			is.read((char*)&x, sizeof(x));
			return is;
		}

		friend std::ostream& operator<<(std::ostream& os, const DAGINFO& x)
		{
			os.write((char*)&x, sizeof(x));
			return os;
		}
	};

	class DBINFO
	{
		int m_nErrorCheck;

	public:
		int m_nMaxTSVDimension;
		std::streampos m_nTailOffset; // Leave it as the last field if possible.

		DBINFO() { Clear(); }

		void Clear()
		{
			m_nMaxTSVDimension = 0;
			m_nTailOffset = 0;
			m_nErrorCheck = -1;
		}

		bool IsValid()
		{
			return m_nErrorCheck == (m_nMaxTSVDimension + m_nTailOffset);
		}

		void MakeValid()
		{
			m_nErrorCheck = m_nMaxTSVDimension + (int)m_nTailOffset;
		}
	};

	int m_nDBSize;
	String m_strFileName;

	SmartArray<DAGINFO> m_dagInfo;
	DBINFO* m_pDBInfo;
	bool* m_pbModified;

public:
	DAGDBFile() : m_dagInfo(0, GROW_FACTOR)
	{
		m_nDBSize = 0;
		m_pbModified = new bool(false);
		m_pDBInfo = new DBINFO;
	}

	~DAGDBFile() { close(); delete m_pDBInfo; delete m_pbModified; }

	int GetDAGCount() const { return m_dagInfo.GetSize(); }
	String GetName() const { return m_strFileName; }
	int GetClassID(const String& strClassName) const;
	DAG* CreateDAGObj(int nClassID) const;

	void MoveTo(std::streamoff off) { clear(); seekg(off, std::ios::beg);	}
	void MoveToBeg() { MoveTo(0); }
	void MoveToEnd() { clear(); seekg(0, std::ios::end);	}

	//! Overload fstream::open
	void open(const char* szName, ios_base::openmode nMode);

	//! Overload fstream::close
	void close();

	bool ReadTailInfo();
	void WriteTailInfo();
	DAGPtr ReadDAG(int nDagId, bool bOnlyDataForMatching = false);
	DAGPtr ReadDAG(std::streamoff nDagOffset, const String& strClassName);
	bool AddDAG(const DAG* pDag, bool bReadSaveTailInfo = false);
	bool RebuildTailInfo();

	bool IsDBModified() const { return *m_pbModified; }
	bool SetDBModified(bool bNewVal) { return *m_pbModified = bNewVal; }

	const DBINFO& DBInfo() const { return *m_pDBInfo; }
	DBINFO& DBInfo() { return *m_pDBInfo; }

	int GetMaxTSVDimension() const { return DBInfo().m_nMaxTSVDimension; }
};
} // namespace dml

#endif //_DAG_DB_FILE_H_
