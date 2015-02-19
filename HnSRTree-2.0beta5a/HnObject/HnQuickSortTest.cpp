/*
 * HnQuickSortTest.cc
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
 * 02/02/99 katayama@rd.nacsis.ac.jp
 * $Id: HnQuickSortTest.cc,v 1.2 2000/06/10 11:18:33 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnQuickSort.hh"

class IntegerSort: HnQuickSort {
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
    IntegerSort(int *array, int count) {
	this->debug = HnTRUE;
	this->array = array;
	this->count = count;
    }
    static void sort(int *array, int count) {
	IntegerSort sorter(array, count);
	sorter.sortElements();
    }
};

int
main(void)
{
    int count;
    int *array;
    int i;

    count = 4096;
    array = new int[count];

    for ( i=0; i<count; i++ ) {
	array[i] = i;
    }

    /* shuffle */
    for ( i=0; i<count; i++ ) {
	int j = (int)(drand48() * count);
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
    }

    /* sort */
    IntegerSort::sort(array, count);

    for ( i=0; i<count; i++ ) {
	if ( array[i] != i ) {
	    fprintf(stderr, "Error: sort is failed.\n");
	    return 1;
	}
    }

    printf("Integers are successfully sorted!\n");

    return 0;
}
