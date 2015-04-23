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
 * $Log: HelperFunctions.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __HELPER_FUNCTIONS_H__
#define __HELPER_FUNCTIONS_H__

#include "BasicUtils.h"
#include "BasicTypes.h"

#include <fstream>

namespace dml {
double SIMILARITY(double x, double y, const double& minVal = .1);

/*!
	The LogFile class specializes a fstream object for logging debug information
	It is meant to be used with the macro DBG_LOG(LogFile, a << b << c ...)

	The class also adds the Print() function with variable number of parameters.

	Note: the log file is creared automatically the first time that Print() or
	the macro DBG_LOG() are called as long as the log status variable is true.
	Do not call operator<<() before the file is created.
*/
class LogFile : public std::fstream
{
	const char* m_szFileName;
	bool* m_pLogStatus;

public:
	LogFile(bool* pLogStatus = NULL)
	{
		m_pLogStatus = pLogStatus;
		m_szFileName = NULL;
	}

	LogFile(const char* szFileName, bool* pLogStatus = NULL)
	{
		m_pLogStatus = pLogStatus;
		m_szFileName = szFileName;

		if (m_pLogStatus == NULL)
			OpenFile(); // open it right away. szFileName may be invalid later
	}

	void SetFileName(const char* szFileName)
	{
		m_szFileName = szFileName;
	}

	bool IsActive() const
	{
		return (m_pLogStatus == NULL || *m_pLogStatus);
	}

	bool IsOpen() const
	{
		return is_open();
	}

	void OpenFile()
	{
		if (!IsOpen())
			open(m_szFileName, std::ios::out | std::ios::trunc);
	}

	void Print(const char* szFormat, ...);
};

//! Computes the area of the triangle with side lengths a, b and c
inline double TriangleArea(const double& a, const double& b, const double& c)
{
	double s = (a + b + c) / 2; //semiperimeter

	return sqrt(s * (s - a) * (s - b) * (s - c));
}

double SignedVectorAngle(const double& a_x, const double& a_y,
						 const double& b_x, const double& b_y,
						 const double& a_norm, const double& b_norm);

//! Computes the signed angle between two vectors
inline double SignedVectorAngle(const double& a_x, const double& a_y,
								const double& b_x, const double& b_y)
{
	double a_norm = sqrt(a_x * a_x + a_y * a_y);
	double b_norm = sqrt(b_x * b_x + b_y * b_y);

	return SignedVectorAngle(a_x, a_y, b_x, b_y, a_norm, b_norm);
}

int FindLineSegmentIntersection(const POINT& S1_P0, const POINT& S1_P1,
							const POINT& S2_P0, const POINT& S2_P1,
							POINT* I0, POINT* I1 = NULL);

bool IsInSegment(const POINT& P, const POINT& S_P0, const POINT& S_P1);

double LinearTotalLeastSquares(const POINT* vertices, int n, double& ax, double& by, double& c);

POINT GetClosestPointOnLine(const double& a, const double& b, const double& c, const POINT& p);

bool DoSegmentCircleIntersect(const POINT& p1, const POINT& p2, const POINT& sc, const double& r);

bool FindLineCircleIntersection(const POINT& p1, const POINT& p2, const POINT& sc,
								const double& r, double* mu1, double* mu2);
} // namespace dml

#endif //__HELPER_FUNCTIONS_H__
