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
 * $Log: MGShockGraph.cpp,v $
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#include <iostream>
#include <stdio.h>
#include "BasicUtils.h"
#include "MGShockGraph.h"

extern "C" {
#define BOOL_DEFINED
#include <shockgraph-interface.h>
}

#ifdef XML_READING
#ifndef WIN32
	extern "C" {
	#include <read-xml-skeleton.h>
	#include <read-xml-shockgraph.h>
	}
#else
	typedef void* xmlDocPtr;
	xmlDocPtr read_xml_file(FILE*);
	Graph* get_xml_shockgraph(xmlDocPtr, FILE*);
#endif //_MY_DEBUG
#endif //XML_READING

using namespace dml;

const MGShockGraph::SGPoint& MGShockGraph::SGSegment::operator[](int i) const
{
	return *((SGPoint*)getDLCellAt(m_pShock->points, i)->data);
}

MGShockGraph::SGSegment MGShockGraph::operator[](int i) const
{
	return (Shock*)((Vertex*)getDLCellAt(m_pGraph->vertices, i)->data)->data;
}

const Vertex* MGShockGraph::GetNode(int i) const
{
	return (Vertex*)getDLCellAt(m_pGraph->vertices, i)->data;
}

MGShockGraph::SGSegment MGShockGraph::GetSegment(const Vertex* pNode) const
{
	return (Shock*)pNode->data;
}

const Edge* MGShockGraph::GetEdge(int i) const
{
	return (Edge*)getDLCellAt(m_pGraph->edges, i)->data;
}

#ifdef XML_READING
bool MGShockGraph::ReadFromXMLFile(const char* szFileName)
{
	Empty();

	FILE* file = fopen(szFileName, "r");

	if (file == NULL)
		return false;

	xmlDocPtr doc = (xmlDocPtr) read_xml_file(file);
		
	fclose(file);

	if (doc == NULL)	
		return false;

	m_pGraph = get_xml_shockgraph(doc, stderr);

	return m_pGraph != NULL;
}
#endif //XML_READING

bool MGShockGraph::ComputeFromPPMFile(const char* szFileName, double cutoff, double sigma, double range)
{
	Empty();

	/* Initialize the parameters to the default values
	   One can initialize the parameters oneself, but it is preferable to
	   initialize them and then modify only the relevant ones. */
	ShockParam sp;

	init_shock_param(&sp);

	sp.deriv_cutoff = cutoff;
	sp.deriv_smooth_sigma = sigma;
	sp.deriv_smooth_range = range;
 
	FILE* file = fopen(szFileName, "r");

	if (file == NULL)
		return false;

	m_pGraph = get_shockgraph_ppm(file, &sp);

	fclose(file);

	return m_pGraph != NULL;
}

/*! 
	@brief Takes an object of type Mat2d which is simply an array of
	char's and treats it as the image.
*/
bool MGShockGraph::ComputeFromMemory(Pixmap& image, double cutoff, double sigma, double range)
{
	Empty();

	/* Initialize the parameters to the default values
	   One can initialize the parameters oneself, but it is preferable to
	   initialize them and then modify only the relevant ones. */
	ShockParam sp;

	init_shock_param(&sp);

	sp.deriv_cutoff = cutoff;
	sp.deriv_smooth_sigma = sigma;
	sp.deriv_smooth_range = range;

	Mat2d matrix;

	matrix.rows = image.rows;
	matrix.cols = image.cols;
	matrix.buf = image.data;

	m_pGraph = get_shockgraph(&matrix, &sp);

	return m_pGraph != NULL;
}

ostream& operator<<(ostream& os, const MGShockGraph::SGPoint& p)
{
    os << endl        << "   ";
    os << "x:"        << p.x            << ", ";
    os << "y:"        << p.y            << ", ";
    os << "segments:" << p.segments     << ", ";
    os << "color:"    << (short)p.color << ", ";
    os << "r:"        << p.r            << ", ";
    os << "s:"        << p.s            << ", ";
    os << "dr_ds:"    << p.dr_ds;
    os << endl << endl;

	return os;
}

ostream& operator<<(ostream& os, const MGShockGraph::SGSegment& s)
{
    os << endl;
    os << "shock_type:"   << s.m_pShock->shock_type   << ", ";
    os << "points:";       

    for (int i = 0; i < s.GetPointCount(); i++) 
      os << s[i];

    os << "id:"           << s.m_pShock->id           << ", ";
    os << "cseg1:"        << s.m_pShock->cseg1        << ", ";
    os << "cseg1_length:" << s.m_pShock->cseg1_length << ", ";
    os << "cseg2:"        << s.m_pShock->cseg2        << ", ";
    os << "cseg2_length:" << s.m_pShock->cseg2_length;
    os << endl;

	return os;
}


/*main(int argc, char* argv[])
{
  MGShockGraph sg;

  if (!sg.ReadFromXMLFile(argv[1])) {
    cout << "Cannot open %s file." << argv[1];
    return 0;
  }

  for (int i = 0; i < sg.GetNodeCount(); i++)
     cout << i << '/' << sg.GetNodeCount() << ": " << sg[i];

  cout << "\n\nFINISHED!!!!\n\n";

  return 0;
} */
