/*
 * HnBinarySearchTest.cc
 * Copyright (C) 1999 Norio Katayama
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
 * 02/12/99 katayama@rd.nacsis.ac.jp
 * $Id: HnBinarySearchTest.cc,v 1.2 2000/06/10 11:18:18 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnFTintArray.hh"
#include "HnSRTree/HnBinarySearch.hh"

class IntegerSearch: HnBinarySearch {
private:
    HnFTintVector vector;
    int key;

private:
    int getNumElements(void) {
	return vector.size();
    }
    int compareToElementAt(int i) {
	return key - vector.elementAt(i);
    }

public:
    IntegerSearch(HnFTintVector vector, int key) {
	this->vector = vector;
	this->key = key;
    }
    static void search(HnFTintVector vector, int key,
		       HnBool *found_return, int *index_return) {
	IntegerSearch searcher(vector, key);
	searcher.searchElements(found_return, index_return);
    }
};

int
main(void)
{
    HnFTintVector vector;
    int i, number;
    int maxNumber = 4096;

    vector = new_HnFTintVector();

    for ( number=2; number<maxNumber; number+=2 ) {
	for ( i=0; i<number; i++ ) {
	    vector.addElement(number);
	}
    }

    for ( number=2; number<maxNumber; number++ ) {
	HnBool found;
	int index;

	IntegerSearch::search(vector, number, &found, &index);

	if ( number % 2 == 0 ) {
	    if ( !found ) {
		fprintf(stderr,
			"the even number ``%d'' is not found.\n", number);
		return 1;
	    }
	    if ( index != (((number - 2) / 2) + 1) * ((number - 2) / 2) ) {
		fprintf(stderr, "index ``%d'' is incorrect.\n", index);
		return 1;
	    }
	}
	else {
	    if ( found ) {
		fprintf(stderr, "the odd number ``%d'' is found.\n", number);
		return 1;
	    }
	    if ( index != (((number - 1) / 2) + 1) * ((number - 1) / 2) ) {
		fprintf(stderr, "index ``%d'' is incorrect.\n", index);
		return 1;
	    }
	}
    }

    printf("Integers are successfully searched!\n");

    return 0;
}
