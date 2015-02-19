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
#include <algorithm>
#include "BoneGraph.h"
#include "BoneGraphConstructor.h"

//#include <LEDA/graphics/graphwin.h>

#define MAX_CORNER_BAR 0.8

using namespace dml;

DBG_ONLY(extern LogFile g_dagCreationLog)

typedef BoneGraphConstructor BGC;

//////////////////////////////////////////////////////////////////////////////////////////
// Public functions

/*!
	@brief Initializes all member variables
*/
BGC::BoneGraphConstructor(BoneGraph* pBoneGraph, const BoneGraphParams& bgparams)
{
	ASSERT(pBoneGraph->IsEmpty());

	m_pBoneGraph = pBoneGraph;      // will add node and edges into this graph
	m_params = bgparams;            // construction parameters
	m_nLastIndexUsed = 1;           // first non-root node index is 1 (root is 0)
	m_dTotalArea = -1;              // total area is not set yet
}

/*!
	@brief Creates a bone graph from a given skeleton graph. The given
	skeleton graph is modified in a non destructive way, and so it can
	be re-used to build other bone graphs or for other purposes. The
	changes to the given skeleton graph are only related to the initialization
	of certain variables. The structure and flux values of the skeleton graphs
	are not changed at any time.
*/
bool BGC::CreateBoneGraph(SkeletalGraph* pSkeletalGraph)
{
	return true;
}