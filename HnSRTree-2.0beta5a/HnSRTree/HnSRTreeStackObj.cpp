/*
 * HnSRTreeStackObj.cc
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
 * $Id: HnSRTreeStackObj.cc,v 1.5 2000/06/10 11:20:07 katayama Exp $
 */

#include "HnSRTree/HnSRTreeStack.hh"
#include "HnSRTree/HnSRTreeStackObj.hh"

void
HnSRTreeStackObj::push(const HnSRTreeNode &node, int cursor)
{
    if ( this->leaf != HnSRTreeLeaf::null ) {
	HnAbort("cannot push a node onto a leaf.");
    }

    nodes.addElement(node);
    cursors.addElement(cursor);
}

void
HnSRTreeStackObj::push(const HnSRTreeLeaf &leaf, int cursor)
{
    if ( this->leaf != HnSRTreeLeaf::null ) {
	HnAbort("cannot push a leaf onto a leaf.");
    }

    this->leaf = leaf;
    cursors.addElement(cursor);
}

void
HnSRTreeStackObj::pop(void)
{
    if ( leaf != HnSRTreeLeaf::null ) {
	leaf = HnSRTreeLeaf::null;
	cursors.removeElementAt(cursors.size() - 1);
    }
    else {
	nodes.removeElementAt(nodes.size() - 1);
	cursors.removeElementAt(cursors.size() - 1);
    }	
}

void
HnSRTreeStackObj::advance(void)
{
    if ( leaf != HnSRTreeLeaf::null ) {
	if ( cursors[getDepth() - 1] == leaf.getCount() - 1 ) {
	    HnAbort("cursor is on the tail");
	}
	cursors[getDepth() - 1] ++;
    }
    else {
	HnSRTreeNode node = nodes[nodes.size() - 1];

	if ( cursors[getDepth() - 1] == node.getCount() - 1 ) {
	    HnAbort("cursor is on the tail");
	}
	cursors[getDepth() - 1] ++;
    }
}
			
int
HnSRTreeStackObj::getDepth(void) const
{
    if ( leaf != HnSRTreeLeaf::null ) {
	return nodes.size() + 1;
    }
    else {
	return nodes.size();
    }
}

int
HnSRTreeStackObj::getCursor(void) const
{
    return cursors[getDepth() - 1];
}

HnBool
HnSRTreeStackObj::hasMore(void) const
{
    if ( leaf != HnSRTreeLeaf::null ) {
	if ( cursors[getDepth() - 1] == leaf.getCount() - 1 ) {
	    return HnFALSE;
	}
	else {
	    return HnTRUE;
	}
    }
    else {
	HnSRTreeNode node = nodes[nodes.size() - 1];

	if ( cursors[getDepth() - 1] == node.getCount() - 1 ) {
	    return HnFALSE;
	}
	else {
	    return HnTRUE;
	}
    }
}
