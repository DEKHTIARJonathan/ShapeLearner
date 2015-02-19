/*
 * HnSRTreeStackObj.hh
 * Copyright (C) 1997 Norio Katayama
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA
 *
 * 03/21/96 katayama@rd.nacsis.ac.jp
 * $Id: HnSRTreeStackObj.hh,v 1.5 2000/06/10 11:20:08 katayama Exp $
 */

#ifndef _HnSRTreeStackObj_hh
#define _HnSRTreeStackObj_hh

#include "HnSRTree/HnObject.hh"
#include "HnSRTree/HnFTintArray.hh"

/*{
#include "HnSRTree/HnSRTreeNode.hh"
#include "HnSRTree/HnSRTreeLeaf.hh"
}*/

#include <stdlib.h>

class HnSRTreeStackObj: public HnObject {
private:
    HnSRTreeNodeVector nodes;
    HnSRTreeLeaf leaf;
    HnFTintVector cursors;

    void initialize(void) {
	nodes = new_HnSRTreeNodeVector();
	leaf = HnSRTreeLeaf::null;
	cursors = new_HnFTintVector();
    }

public:
    HnSRTreeStackObj(void) {
	initialize();
    }

    void push(const HnSRTreeNode &node, int cursor);
    void push(const HnSRTreeLeaf &leaf, int cursor);
    void push(const HnSRTreeNode &node) { push(node, 0); }
    void push(const HnSRTreeLeaf &leaf) { push(leaf, 0); }
    void pop(void);
    void advance(void);

    int getDepth(void) const;

    int getCursor(void) const;
    HnBool hasMore(void) const;

    HnBool isTopNode(void) const {
	return leaf == HnSRTreeLeaf::null;
    }
    HnBool isTopLeaf(void) const {
	return leaf != HnSRTreeLeaf::null;
    }


    const HnSRTreeNode &getTopNode(void) const {
#ifdef DEBUGGING
	if ( leaf != HnSRTreeLeaf::null ) {
	    HnAbort("the top element is not a node.");
	}
#endif
	return nodes[nodes.size() - 1];
    }
    const HnSRTreeLeaf &getTopLeaf(void) const {
#ifdef DEBUGGING
	if ( leaf == HnSRTreeLeaf::null ) {
	    HnAbort("the top element is not a leaf.");
	}
#endif
	return leaf;
    }
};

#endif /* _HnSRTreeStackObj_hh */
