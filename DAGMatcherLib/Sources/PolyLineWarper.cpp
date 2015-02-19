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
 * $Log: PolyLineWarper.cpp,v $
 *
 *
 *-----------------------------------------------------------------------*/
#include <stdlib.h>
#include <iostream>
#include "PolyLineWarper.h"
#include "HelperFunctions.h"
#include "BasicUtils.h"

using namespace dml;

double PolyLineWarper::Warp(const LineSegmentArray& src,
							const LineSegmentArray& tgt,
							const POINTS& tgtPts)
{
/*	// Determine global scale paramater
	m_scale = tgt.GetHead().Y0() / src.GetHead().Y0();

	// Copy the source polyline and apply the global scale to it
	m_src = src;
	m_src.Scale(m_scale);

	// Each line segment has an associated pair of parameters (dx, dy)
	// that defined the transformation of their endpoint 1.
	m_transParams.Resize(tgt.Size());

	// We beging by finding the maximum parameter values for the last
	// source segment, whose endpoint 1 must agree with the endpoint 1 of
	// the last target line segment. The scale parametes of this point's
	// transformation define the upper bound on all other points' scale params.
	Transformation& T = m_transParams.GetTail();

	POINT srcPt = src.GetTail().GetP1();
	POINT tgtPt = tgt.GetTail().GetP1();

	T.dx = tgtPt.x / srcPt.x;
	T.dy = tgtPt.y / srcPt.y;

	// Let the knots of the polyline be all its internal vertices
	// Initialize the memoization blocks associated with each knot
	// The left most knot, v0, can have a dx at most as big as that of
	// uniformiley distributing all dy's across verices. The next
	// left most vertex, v1, can have a maximum dx that is equal to that
	// of v0 plus an euqual distribution of the possibly unused dx0, and so on.
	// The case of the dy's is different, as there is not finite quantity to share.
	for (i = 0; i < N; i++)
	{
		m_cumErrors[i].Resize(f(max_dx), max_dy);
	}

	AsssignAllValidSegmentErrors();

	// Now, we have to find the set of valid parameters for each knot that
	// yields the smallest error
	FindBestParameters(N);
	*/
	return 0;
}
/*
PolyLineWarper::AsssignAllValidSegmentErrors()
{
	// Now, each element of these matrices can become and endpoint of
	// a valid line segment. Here, we initialize the error of these segments
	int x0, y0, x1, y1;
	int max_x0, max_y1, max_x1, max_y1;

	const int numSegs = m_src.Size();

	for (int i = 0; i < numSegs; i++)
	{
		max_x0 = m_cumErrors[i].NRows();
		max_y0 = m_cumErrors[i].NCols();

		max_x1 = m_cumErrors[i + 1].NRows();
		max_y1 = m_cumErrors[i + 1].NCols();

		m_simpleErrors[i].Resize(max_x0, max_y0);

		for (x0 = 0; x0 < max_x0; x0++)
		{
			for (y0 = 0; y0 < max_y0; y0++)
			{
				m_simpleErrors[i][x0][y0].Resize(max_x1, max_y1);

				for (x1 = 0; x1 < max_x1; x1++)
				{
					for (y1 = 0; y1 < max_y1; y1++)
					{
						m_simpleErrors[i][x0][y0][x1][y1] = ComputeSimpleError(i, x0, y0, x1, y1);
					}
				}
			}
		}
	}
}

PolyLineWarper::FindBestParameters(int i)
{
	int x1, y1;
	int best_x1 = 0, best_y1 = 0; // init in case best error is INFINITY
	double error, minError = INFINITY;

	max_x1 = m_minCumulativeErrors[i].NRows();
	max_y1 = m_minCumulativeErrors[i].NCols();

	for (x1 = 0; x1 < max_x1; x1++)
	{
		for (y1 = 0; y1 < max_y1; y1++)
		{
			error = GetMinCumulativeError(N - 1, x1, y1);

			if (error < minError)
			{
				minError = error;
				best_x1 = x1;
				best_y1 = y1;
			}
		}
	}
}

PolyLineWarper::GetMinCumulativeError(int i, int x1, int y1)
{
	MEMDATA& d = m_minCumulativeErrors[i][x1][y1];

	if (d.IsEmpty())
	{
		int x0, y0, max_x0, max_y0;
		int best_x0 = 0, best_y0 = 0; // init in case best error is INFINITY
		double error, minError = INFINITY;

		max_x0 = m_minCumulativeErrors[i].NRows();
		max_y0 = m_minCumulativeErrors[i].NCols();

		for (x0 = 0; x0 <= max_x0; x0++)
		{
			for (y0 = 0; y0 <= max_y0; y0++)
			{
				error = m_simpleErrors[i][x0][y0][x1][y1];

				if (i > 0)
					error += GetMinCumulativeError(i - 1, x0, y0);

				if (error < minError)
				{
					minError = error;
					best_x0 = x0;
					best_y0 = y0;
				}
			}
		}

		d.Set(minError, best_x0, best_y0);
	}

	return d;
}

PolyLineWarper::GetSimpleError(i, x0, y0, x1, y1)
{
}*/