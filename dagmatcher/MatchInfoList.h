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
 * $Log: MatchInfoList.h,v $
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef __MATCH_INFO_LIST_H__
#define __MATCH_INFO_LIST_H__

namespace dml {
class MatchInfo
{
public:
	double distance;
	int prototype;
	DAGPtr pDag;
	double similarity;

	MatchInfo() { distance = 0; prototype = 0; similarity = 0; }

	MatchInfo(int prot, double dist)
	{
		distance = dist;
		prototype = prot;
	}

	friend bool operator< (const MatchInfo& a, const MatchInfo& b)
	{
		double ap;
		double bp;

		ap = a.distance;
		bp = b.distance;

		if(ap < bp)
			return true;
		else
			return false;
	}

	// The stream operators are implemented only to satisfy LEDA requirements
	friend ostream& operator<<(ostream &os, const MatchInfo& mi) { return os; }
	friend istream& operator>>(istream &is, MatchInfo& mi) { return is; }
};

class MatchInfoList : public leda_list<MatchInfo>
{
	int m_nMaxMatches;
	double m_dSimilarityTau;

	// Stats
	double m_dMinSimilarity;
	double m_dMaxSimilarity;
	int m_nSamplesRead;

public:
	MatchInfoList(int nMaxMatches);
	MatchInfoList(double dSimilarityTau);

	Clear();

	Add();

	GetFirstMatchPos();
	GetNextMatch(pos);

	PrintStats(ostream os = cout);
};
} //namespace dml;

#endif // __MATCH_INFO_LIST_H__
