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
 * $Log: TSV.h,v $
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef __TSV_H__
#define __TSV_H__

#include "stdafx.h"

namespace dml {
int CompareTSVElement(const void* elem1, const void* elem2);

//! The Topological Signature Value ADT.
class TSV : public SmartArray<double>
{
public:

	TSV(int size = 0) : SmartArray<double>(size) { }
	TSV(const TSV& x) : SmartArray<double>(x) { }

	double Norm2() const;
	TSV operator-(const TSV& rhs) const;

	friend double Norm2(const TSV& tsv) { return tsv.Norm2(); }
	friend int CompareTSVElement(const void* elem1, const void* elem2);

	void Sort() { SmartArray<double>::Sort(CompareTSVElement); }
};
} //namespace dml

#endif //__TSV_H__
