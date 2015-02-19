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
*-----------------------------------------------------------------------*/

#ifndef __SIMILARITY_MEASURER_T_H__
#define __SIMILARITY_MEASURER_T_H__

#include "SimilarityMeasurer.h"

namespace dml {
/*!
	Abstract and template-based node distance measurer for a
	DAG of type T and nodes of type V.

	This class implements the pure virtual function GetDistance(...),
	but leaves the implementation of the pure virtual function
	CompDistance() to its derived classes.
*/
template <class G> class SimilarityMeasurerT : public SimilarityMeasurer
{
 protected:
	 const G* m_pG1;
	 const G* m_pG2;

 public:
	 SimilarityMeasurerT()
	 {
		 m_pG1 = NULL;
		 m_pG2 = NULL;
	 }

	 virtual void Init(const DAG* pDag1, const DAG* pDag2)
	 {
		m_pG1 = dynamic_cast<const G*>(pDag1);
		m_pG2 = dynamic_cast<const G*>(pDag2);

		ASSERT(m_pG1 && m_pG2);
	 }
};
} //namespace dml

#endif //__SIMILARITY_MEASURER_T_H__
