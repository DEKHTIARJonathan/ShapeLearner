/*
 * HnClassArrayTest.cc
 * Copyright (C) 1998 Norio Katayama
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
 * 10/21/98 katayama@rd.nacsis.ac.jp
 * $Id: HnClassArrayTest.cc,v 1.1 2000/06/06 09:27:07 katayama Exp $
 */

#include "HnString.hh"

void
testArray()
{
    HnStringArray array1, array2;
    int i;

    printf("**** HnStringArray ****\n");

    printf("array1 = new_HnStringArray(10)\n");
    array1 = new_HnStringArray(10);

    printf("array1.toString()=%s\n", (char *)array1.toString());

    for ( i=0; i<array1.length; i++ ) {
	printf("array1[%d]=HnString::valueOf(%d)\n", i, i);
	array1[i] = HnString::valueOf(i);
    }

    printf("array1.toString()=%s\n", (char *)array1.toString());

    for ( i=0; i<array1.length; i++ ) {
	if ( array1[i].equals(HnString::valueOf(i)) ) {
	    printf("array1[%d].equals(HnString::valueOf(%d))\n", i, i);
	}
	else {
	    fprintf(stderr,
		    "array1[%d] is not equal to HnString::valueOf(%d)\n",
		    i, i);
	    exit(1);
	}
    }

    printf("array2 = new_HnStringArray(array1)\n");
    array2 = new_HnStringArray(array1);

    printf("array2.toString()=%s\n", (char *)array2.toString());

    if ( array1.equals(array2) ) {
	printf("array1.equals(array2)\n");
    }
    else {
	fprintf(stderr, "Error: array1 and array2 must be equal.\n");
	exit(1);
    }
}

void
testVector()
{
    HnStringVector vector1, vector2;
    int i;

    printf("**** HnStringVector ****\n");

    printf("vector1 = new_HnStringVector()\n");
    vector1 = new_HnStringVector();

    printf("vector1.toString()=%s\n", (char *)vector1.toString());

    printf("vector1.setSize(10)\n");
    vector1.setSize(10);

    printf("vector1.toString()=%s\n", (char *)vector1.toString());

    printf("vector1.removeAllElements()\n");
    vector1.removeAllElements();

    printf("vector1.toString()=%s\n", (char *)vector1.toString());

    for ( i=0; i<10; i++ ) {
	printf("vector1.addElement(HnString::valueOf(%d))\n", i);
	vector1.addElement(HnString::valueOf(i));
    }

    printf("vector1.toString()=%s\n", (char *)vector1.toString());

    for ( i=0; i<vector1.size(); i++ ) {
	if ( vector1.elementAt(i).equals(HnString::valueOf(i)) ) {
	    printf("vector1.elementAt(%d).equals(HnString::valueOf(%d))\n",
		   i, i);
	}
	else {
	    fprintf(stderr,
		    "vector1.elementAt(%d) is not equal to "
		    "HnString::valueOf(%d)\n", i, i);
	    exit(1);
	}
    }

    printf("vector2 = new_HnStringVector(vector1)\n");
    vector2 = new_HnStringVector(vector1);

    printf("vector2.toString()=%s\n", (char *)vector2.toString());

    if ( vector1.equals(vector2) ) {
	printf("vector1.equals(vector2)\n");
    }
    else {
	fprintf(stderr, "Error: vector1 and vector2 must be equal.\n");
	exit(1);
    }
}

void
testPointer()
{
    HnStringArray array, ptr;

    printf("HnStringArray array;\n");
    printf("    array.length: %d\n", array.length);
    if ( array.length != -1 ) {
	fprintf(stderr, "Error: -1 is expected.\n");
	exit(1);
    }

    array = new_HnStringArray(100);
    printf("array = new_HnStringArray(100);\n");
    printf("    array.length: %d\n", array.length);
    if ( array.length != 100 ) {
	fprintf(stderr, "Error: 100 is expected.\n");
	exit(1);
    }

    ptr = array;
    printf("ptr = array;\n");
    printf("    ptr.length: %d\n", ptr.length);
    if ( ptr.length != 100 ) {
	fprintf(stderr, "Error: 100 is expected.\n");
	exit(1);
    }
}

int
main(void) {
    testArray();
    testVector();
    testPointer();
}
