/*
 * HnFTypeArrayTest.cc
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
 * $Id: HnFTypeArrayTest.cc,v 1.3 2000/06/10 11:18:23 katayama Exp $
 */

#include "HnSRTree/HnFTcharArray.hh"
#include "HnSRTree/HnFTintArray.hh"
#include "HnSRTree/HnFTlongArray.hh"
#include "HnSRTree/HnFTucharArray.hh"
#include "HnSRTree/HnFTuintArray.hh"
#include "HnSRTree/HnFTulongArray.hh"
#include "HnSRTree/HnFTfloatArray.hh"
#include "HnSRTree/HnFTdoubleArray.hh"

void
testChar() {
    HnFTcharArray array;
    int i;

    array = new_HnFTcharArray(20);

    printf("char: %s\n", (char *)array.toString());

    for ( i=0; i<array.length; i++ ) {
	array[i] = i;
    }

    printf("char: %s\n", (char *)array.toString());
}

void
testInt() {
    HnFTintArray array;
    int i;

    array = new_HnFTintArray(20);

    printf("int: %s\n", (char *)array.toString());

    for ( i=0; i<array.length; i++ ) {
	array[i] = i;
    }

    printf("int: %s\n", (char *)array.toString());
}

void
testLong() {
    HnFTlongArray array;
    int i;

    array = new_HnFTlongArray(20);

    printf("long: %s\n", (char *)array.toString());

    for ( i=0; i<array.length; i++ ) {
	array[i] = i;
    }

    printf("long: %s\n", (char *)array.toString());
}

void
testUnsignedChar() {
    HnFTucharArray array;
    int i;

    array = new_HnFTucharArray(20);

    printf("unsigned char: %s\n", (char *)array.toString());

    for ( i=0; i<array.length; i++ ) {
	array[i] = i;
    }

    printf("unsigned char: %s\n", (char *)array.toString());
}

void
testUnsignedInt() {
    HnFTuintArray array;
    int i;

    array = new_HnFTuintArray(20);

    printf("unsigned int: %s\n", (char *)array.toString());

    for ( i=0; i<array.length; i++ ) {
	array[i] = i;
    }

    printf("unsigned int: %s\n", (char *)array.toString());
}

void
testUnsignedLong() {
    HnFTulongArray array;
    int i;

    array = new_HnFTulongArray(20);

    printf("unsigned long: %s\n", (char *)array.toString());

    for ( i=0; i<array.length; i++ ) {
	array[i] = i;
    }

    printf("unsigned long: %s\n", (char *)array.toString());
}

void
testFloat() {
    HnFTfloatArray array;
    int i;

    array = new_HnFTfloatArray(20);

    printf("float: %s\n", (char *)array.toString());

    for ( i=0; i<array.length; i++ ) {
	array[i] = i;
    }

    printf("float: %s\n", (char *)array.toString());
}

void
testDouble() {
    HnFTdoubleArray array;
    int i;

    array = new_HnFTdoubleArray(20);

    printf("double: %s\n", (char *)array.toString());

    for ( i=0; i<array.length; i++ ) {
	array[i] = i;
    }

    printf("double: %s\n", (char *)array.toString());
}

void
testPointer()
{
    HnFTintArray array, ptr;

    printf("HnFTintArray array;\n");
    printf("    array.length: %d\n", array.length);
    if ( array.length != -1 ) {
	fprintf(stderr, "Error: -1 is expected.\n");
	exit(1);
    }

    array = new_HnFTintArray(100);
    printf("array = new_HnFTintArray(100);\n");
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
    testChar();
    testInt();
    testLong();
    testUnsignedChar();
    testUnsignedInt();
    testUnsignedLong();
    testFloat();
    testDouble();
    testPointer();
}
