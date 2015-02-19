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
 * $Log: Vector.cpp,v $
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#define WANT_STREAM                  // include.h will get stream fns
#define WANT_MATH                    // include.h will get math fns
                                     // newmatap.h will get include.h
#include <newmatap.h>                // need matrix applications
#include <newmatio.h>                // need matrix output routines

using namespace NEWMAT;              // access NEWMAT namespace

#include "Vector.h"
#include "BasicUtils.h"
#include "MathUtils.h"

using namespace dml;

double Vector::DotProduct(const BaseClass& a, const BaseClass& b)
{
	ASSERT(a.Ncols() == b.Ncols());

	int nCols = a.Ncols();
	double r = 0;
	
	for (int i = 1; i <= nCols; i++)
		r += a(i) * b(i);

	return r;
}

double Vector::CrossProductMag(const BaseClass& a, const BaseClass& b)
{
	double dAngleAB = 0;
	return DotProduct(a, b) * sin(dAngleAB);
}

/* static */
double Vector::DistFromPtToLine(const BaseClass& x, const BaseClass& a, const BaseClass& b)
{
	double m, t, n = 0, d = 0;
	int i, k = x.Ncols();

	ASSERT(k == a.Ncols() && k == b.Ncols());

	for (i = 1; i <= k; i++)
	{
		m = b(i) - a(i);
		n += m * (x(i) - a(i));
		d += m * m;
	}

	t = n / d;

	if (t >= 0 && t <= 1)
		return Norm2((a - x) + (b - a) * t);
	else
		return ((n = Norm2(a - x)) < (d = Norm2(b - x))) ? n:d;
}