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
 * $Log: Vector.h,v $
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 * Revision 1.1  2002/01/15 16:38:36  dmac
 * Helper class to compute the Hausdorff-like node distance function
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef __VECTOR_H__
#define __VECTOR_H__

namespace dml {

class Vector : public RowVector
{
public:
	typedef RowVector BaseClass;

	Vector(int nSize) : RowVector(nSize) {}
	Vector(const BaseClass& v) : BaseClass(v) {}

	//Vector CrossProduct(const BaseClass& a, const BaseClass& b);
	int Size() const { return Ncols();}

	static double DotProduct(const BaseClass& a, const BaseClass& b);
	static double CrossProductMag(const BaseClass& a, const BaseClass& b);

	static double Norm2(const BaseClass& a) { return sqrt(DotProduct(a, a)); }
	double Norm2() const { return Norm2(*this); }

	static double DistFromPtToLine(const BaseClass& x, const BaseClass& a, const BaseClass& b);

	double DistanceFromLine(const BaseClass& a, const BaseClass& b) const
	{
		return DistFromPtToLine(*this, a, b);
	}

	void operator=(const BaseClass& rhs) { BaseClass::operator=(rhs); }
};
} //namespace dml

#endif //__VECTOR_H__
