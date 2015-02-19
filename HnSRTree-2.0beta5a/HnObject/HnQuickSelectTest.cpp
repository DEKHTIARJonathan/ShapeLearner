/*
 * HnQuickSelectTest.cc
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
 * Jun.21, 1999 Norio KATAYAMA
 * $Id: HnQuickSelectTest.cc,v 1.2 2000/06/10 11:18:31 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnQuickSelect.hh"

class IntegerSelect: HnQuickSelect {
private:
    int *array;
    int count;

private:
    int getNumElements(void) {
	return count;
    }
    int compareElementsAt(int i, int j) {
	return array[i] - array[j];
    }
    void swapElementsAt(int i, int j) {
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
    }

public:
    IntegerSelect(int *array, int count) {
	this->debug = HnTRUE;
	this->array = array;
	this->count = count;
    }
    static void select(int *array, int count, int index) {
	IntegerSelect selecter(array, count);
	selecter.selectElement(index);
    }
};

int
main(void)
{
    int count;
    int *array1, *array2;
    int i;

    count = 4096;
    array1 = new int[count];
    array2 = new int[count];

    for ( i=0; i<count; i++ ) {
	array1[i] = i;
    }

    /* shuffle */
    for ( i=0; i<count; i++ ) {
	int j = (int)(drand48() * count);
	int temp = array1[i];
	array1[i] = array1[j];
	array1[j] = temp;
    }

    for ( i=0; i<count; i++ ) {
	memcpy(array2, array1, sizeof(int) * count);

	/* select */
	IntegerSelect::select(array2, count, i);

	if ( array2[i] != i ) {
	    fprintf(stderr, "Error: selection is failed.\n");
	    return 1;
	}
    }

    printf("Integers are successfully selected!\n");

    return 0;
}
