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
 * $Log: MGShockGraph.h,v $
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef __MG_SHOCKGRAPH_H__
#define __MG_SHOCKGRAPH_H__

extern "C" {
#include <Shock.h>
#include <Branches.h>
#include <Graphs.h>
}

namespace dml {
struct Pixmap
{
	int rows;
	int cols;
	char* data;
    const char* name;

    Pixmap() { name = "pixmap"; }
};

/*!
	\brief This class is a wrapper for the class Graph used by the shock graph detection algorithm.

	The purpose of this class is to provide a nice object oriented interface for the
	class Graph used at McGill to represent a shock graph.
*/
class MGShockGraph : public Graph
{
	Graph* m_pGraph;	//!< Ponter to the warpped Graph object.

public:
	//! Wrapper for the class BPoint used by the shock graph detection algorithm.
	class SGPoint : public BPoint
	{
	public:

		friend ostream& operator<<(ostream& stream, const SGPoint& p );
	};

	//! Wrapper for the class Shock used by the shock graph detection algorithm.
	class SGSegment
	{
		Shock* m_pShock;	//!< Ponter to the warpped Shock object.

	public:

		const SGPoint& operator[](int i) const;
		friend ostream& operator<<(ostream& os, const SGSegment& s);

		SGSegment(Shock* pShock)  { m_pShock = pShock; }
		int GetPointCount() const { return (m_pShock && m_pShock->points) ? m_pShock->points->ncells:0; }

		const char* GetId() const { return m_pShock->id; }
		int GetType() const	      { return m_pShock->shock_type; }

		void GetContourLength(double& seg1, double& seg2) const
		{
			seg1 = m_pShock->cseg1_length;
			seg2 = m_pShock->cseg2_length;
		}
	};

public:

	SGSegment operator[](int i) const;
	SGSegment GetSegment(const Vertex* pNode) const;
	const Vertex* GetNode(int i) const;
	const Edge* GetEdge(int i) const;

#ifdef XML_READING
	bool ReadFromXMLFile(const char* szFileNumber);
#endif //XML_READING
	bool ComputeFromPPMFile(const char* szFileName,
		double cutoff, double sigma, double range);

	bool ComputeFromMemory(Pixmap& image,
		double cutoff, double sigma, double range);

	MGShockGraph(Graph* pGraph = NULL)  { m_pGraph = pGraph; }
	~MGShockGraph() { Empty(); }

	int GetNodeCount() const { return (m_pGraph) ? m_pGraph->vertices->ncells:0; }
	int GetEdgeCount() const { return (m_pGraph) ? m_pGraph->edges->ncells:0; }

	void Empty()  { if (m_pGraph) { freeGraph(m_pGraph); m_pGraph = NULL; } }
};
} //namespace dml

#endif //__MG_SHOCKGRAPH_H__
