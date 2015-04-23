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
 * $Log: TSV.cpp,v $
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

using namespace dml;

/*!
	\brief Helper funtion that compares a pair of TSV elements by their absolute value.

	It can be passed as an argument to the SmartArray::Sort() to obtain a
	descending order of a TSV.

	\precondition It is assumed that the values ar positive since the represent
	the sume of the k larges absolute eigenvalues of a submatrix.
*/

int dml::CompareTSVElement(const void* elem1, const void* elem2)
{
	double& e1 = *(double*)elem1;
	double& e2 = *(double*)elem2;

	ASSERT(e1 >= 0 && e2 >= 0);

	if (e1 < e2)
		return 1;
	else if (e1 > e2)
		return -1;
	else
		return 0;
}

//! Computes the L2 norm of the TSV
double TSV::Norm2() const
{
	const double* pData = GetData();
	double d = 0.0;

	for(int i = 0; i < GetSize(); i++)
		d += pData[i] * pData[i];

	return sqrt(d);
}

/*!
	\brief Computes the difference between two TSVs
*/
TSV TSV::operator-(const TSV& rhs) const
{
	bool bRhsIsBigger = rhs.GetSize() > GetSize();
	int i, nMinSize, nMaxSize;

	if (bRhsIsBigger) {
		nMinSize = GetSize();
		nMaxSize = rhs.GetSize();
	}
	else {
		nMinSize = rhs.GetSize();
		nMaxSize = GetSize();
	}

	TSV r(nMaxSize);

	for(i = 0; i < nMinSize; i++)
		r[i] = operator[](i) - rhs[i];

	if (bRhsIsBigger)
		for(; i < nMaxSize; i++)
			r[i] = -rhs[i];
	else
		for(; i < nMaxSize; i++)
			r[i] = operator[](i);

/*	int size = GetSize();
	TSV r(size);

	ASSERT(size == rhs.GetSize());

	for(int i = 0; i < size; i++)
		r[i] = operator[](i) - rhs[i];*/

	return r;
}