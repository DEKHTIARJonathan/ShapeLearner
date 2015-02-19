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
 * $Log: PiecewiseApprox.cpp,v $
 *
 *
 *-----------------------------------------------------------------------*/
#include <stdlib.h>
#include <iostream>
#include "ModelFit.h"
#include "HelperFunctions.h"
#include "BasicUtils.h"

using namespace dml;

double ModelFit::Fit(const POINTS& vertices, const LineSegmentArray& segs)
{
	ASSERT(segs.GetSize() >= 1);
	ASSERT(vertices.GetSize() > 1);

	int i;
	m_minLineLen.Resize(segs.GetSize());
	m_minDataLen.Resize(vertices.GetSize());

	// See if the data can possibly be fit with the model given the constraints (m_minLenCoeff)
	m_minDataLen[0] = 0;

	for (i = 1; i < m_minDataLen.GetSize(); i++)
		m_minDataLen[i] = m_minDataLen[i - 1] + vertices[i].L2(vertices[i - 1]);

	m_minLineLen[0] = segs[0].GetLen() / m_minLenCoeff;

	for (i = 1; i < m_minLineLen.GetSize(); i++)
		m_minLineLen[i] = m_minLineLen[i - 1] + (segs[i].GetLen() / m_minLenCoeff);

	m_totalDataLen = m_minDataLen.GetTail();
	m_totalMinLineLen = m_minLineLen.GetTail();
	m_totalMaxLineLen = m_totalMinLineLen * m_minLenCoeff * m_minLenCoeff;

	if (m_totalDataLen < m_totalMinLineLen || m_totalDataLen > m_totalMaxLineLen)
	{
		m_points = vertices; // for display purposes. see plot.
		return INFINITY;
	}

	// There is enought room, so we have work to do
	int dim = segs.GetSize() + 1; //(int)ceil(segs.GetSize() / 2.0);
	m_points = vertices;
	m_segments = segs;
	m_minerrors.Resize(dim, dim);

	MEMDATA d = Min(0, segs.GetSize(), 0, m_points.GetSize() - 1);

	// Modify segments
	UpdateSegments(0, segs.GetSize(), 0, m_points.GetSize() - 1);

	return d.GetMinError();
}

void ModelFit::UpdateSegments(int ls, int le, int ps, int pe)
{
	if (le - ls == 1)
	{
		m_segments[ls].p0 = m_points[ps];
		m_segments[ls].p1 = m_points[pe];
	}
	else
	{
		const MEMDATA& d = m_minerrors[ls][le][ps][pe];

		/*if(d.IsEmpty())
		{
			DBG_VAL(ls)
			DBG_VAL(le)
			DBG_VAL(ps)
			DBG_VAL(pe)
		}*/

		ASSERT(!d.IsEmpty());

		//int li = (int)ceil(ls + ((le - ls) / 2.0));//(int)floor(le / 2.0);
		int li = int((le + ls) / 2.0);

		UpdateSegments(ls, li, ps, d.GetIdx());
		UpdateSegments(li, le, d.GetIdx(), pe);
	}
}

/*!
	Cumulated lengths, without including the last line
*/
MINMAX ModelFit::GetMinMaxLen(int ls, int li, int le, int ps, int pe) const
{
	double minSegLen, d;
	int i, j;

	/*if (!(li > ls && li < le))
	{
		DBG_MSG("\nThis should not happen\n")
		DBG_VAL(ls)
		DBG_VAL(li)
		DBG_VAL(le)
	}*/
	ASSERT(li > ls && li < le);

	d = (ls == 0) ? 0:m_minLineLen[ls - 1];
	minSegLen = m_minLineLen[li - 1] - d;

	for (i = ps + 1; i < pe && (m_minDataLen[i] - m_minDataLen[ps]) < minSegLen; i++);

	if(i == pe)
		i--;

	d = (li == 0) ? 0:m_minLineLen[li - 1];
	minSegLen = m_minLineLen[le - 1] - d;

	for (j = pe - 1; j > ps && j > i && (m_minDataLen[pe] - m_minDataLen[j]) < minSegLen; j--);

	if (i > j)
	{
		DBG_PRINT5(ps, pe, m_totalDataLen, m_totalMinLineLen, m_totalMaxLineLen)
		DBG_PRINT5(i, j, ls, li, le)
		i = j;
	}

	ASSERT(i <= j);

	return MINMAX(i, j);
}

MEMDATA ModelFit::Min(int ls, int le, int ps, int pe)
{
	if (m_minerrors[ls][le].GetSize() == 0)
	{
		int dim = m_points.GetSize(); //(int)ceil(m_points.GetSize() / 2.0);
		m_minerrors[ls][le].Resize(dim, dim);
	}

	MEMDATA& d = m_minerrors[ls][le][ps][pe];

	if (d.IsEmpty())
	{
		if(le - ls == 1)
			d.Set(CompLSError(ls, ps, pe), pe);
		else
		{
			double m, minerror = INFINITY;
			int i = pe; //In case it never enters the for loop
			int li = int((le + ls) / 2.0); //(int)floor(le / 2.0);

			MINMAX mm = GetMinMaxLen(ls, li, le, ps, pe);

			for (int pi = mm.min; pi <= mm.max; pi++)
			{
				m = Min(ls, li, ps, pi) + Min(li, le, pi, pe);

				if (m < minerror)
				{
					minerror = m;
					i = pi;
				}
			}

			d.Set(minerror, i);
		}
	}

	return d;
}

double ModelFit::CompLSError(int ls, int ps, int pe) const
{
	const LineSegment& s = m_segments[ls];
	double e = 0;

	for (int i = ps; i <= pe; i++)
		e += fabs(m_points[i].y - (s.m * m_points[i].x + s.b));

	return e;
}

void ModelFit::GetSegmentError(int s, double* error, double* scaleFactor) const
{
	/*const MEMDATA& d = m_minerrors[s][s + 1][m_segments[s].p0][m_segments[s].p1];
	ASSERT(!d.IsEmpty());

	*error = d.GetMinError();

	double oldLen = (s == 0) ? m_minLineLen[s]:m_minLineLen[s] - m_minLineLen[s - 1];
	*scaleFactor = m_segments[s].GetLen() / (oldLen * m_minLenCoeff);*/
}

void ModelFit::Plot(std::ostream& os, const POINTS& points2, const LineSegmentArray& segments2) const
{
	if (m_points.GetSize() > 0)
	{
		os << "\n% DATA 1...\na = ";
		m_points.Print(os, "\n");
		os << ";\nplot(a(:,1),a(:,2),'xb');\nhold on;\n";
	}

	if (points2.GetSize() > 0)
	{
		os << "\n% DATA 2...\nb = ";
		points2.Print(os, "\n");
		os << ";\nplot(b(:,1),b(:,2),'or');\n";

		if (m_points.GetSize() == 0)
			os << "hold on;\n";
	}

	int j;

	// Original model
	for (j = 0; j < segments2.GetSize(); j++)
	{
		const LineSegment& seg = segments2[j];
		const char* c = "k";

		os << "x = " << seg.p0.x << ':' << seg.p1.x << ";\n";
		os << "y = " << seg.m << " * x + " << seg.b << ";\n";
		os << "plot(x,y,'" << c << "');\n";
	}

	// Model with new params
	for (j = 0; j < m_segments.GetSize(); j++)
	{
		const LineSegment& seg = m_segments[j];
		const char* c = (j % 2) ? "b":"r";

		os << "x = " << seg.p0.x << ':' << seg.p1.x << ";\n";
		os << "y = " << seg.m << " * x + " << seg.b << ";\n";
		os << "plot(x,y,'" << c << "', 'linewidth', 2);\n";
	}

	double minerror = INFINITY;

	if (m_segments.GetSize() > 0 && m_points.GetSize() > 0)
		minerror = m_minerrors[0][m_segments.GetSize()][0][m_points.GetSize() - 1].GetMinError();

	os << "title('Num of segments: " << m_segments.GetSize() << ", min lines len: " << m_totalMinLineLen;
	os << ", data len: " << m_totalDataLen	<< ", error: " << minerror << ",  min len coeff: " << m_minLenCoeff << "');\n";
	os << "hold off;\npause;\n\n";
	os << "% End fitting...\n";
}