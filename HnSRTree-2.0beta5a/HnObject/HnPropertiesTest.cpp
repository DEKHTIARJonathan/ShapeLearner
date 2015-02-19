/*
 * HnPropertiesTest.cc
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
 * 01/29/99 katayama@rd.nacsis.ac.jp
 * $Id: HnPropertiesTest.cc,v 1.2 2000/06/10 11:18:30 katayama Exp $
 */

#include <stdio.h>
#include "HnSRTree/HnProperties.hh"

static void
testLeadingWhiteSpace()
{
    HnProperties properties = new_HnProperties();
    const char *src = "Key1   =    \\      Value1";
    const char *dst = "Key1=\\u20     Value1\n";
    HnString saved;

    printf("Test of leading whitespace: ");

    properties.load(src);
    saved = properties.save();

    if ( saved.equals(dst) ) {
	printf("ok!\n");
    }
    else {
	printf("bad!\n");
	exit(1);
    }
}

static void
testContinuedLine()
{
    HnProperties properties = new_HnProperties();
    const char *src = "Key1   =  Value1\\\n     ContinuedLine1\n";
    const char *dst = "Key1=Value1ContinuedLine1\n";
    HnString saved;

    printf("Test of continued line: ");

    properties.load(src);
    saved = properties.save();

    if ( saved.equals(dst) ) {
	printf("ok!\n");
    }
    else {
	printf("bad\n");
	exit(1);
    }
}

static void
testCarriageReturn()
{
    HnProperties properties = new_HnProperties();
    const char *src =
	"Key1   =  Value1\\\r\n     ContinuedLine1\n\r"
	"Key2   =  Value2\\\n\r     ContinuedLine2\r\n"
	"Key3   =  Value3\\\n\r     ContinuedLine3\n\r";
    const char *dst =
	"Key1=Value1ContinuedLine1\n"
	"Key2=Value2ContinuedLine2\n"
	"Key3=Value3ContinuedLine3\n";
    HnString saved;

    printf("Test of carriage return: ");

    properties.load(src);
    saved = properties.save();

    if ( saved.equals(dst) ) {
	printf("ok!\n");
    }
    else {
	printf("bad\n");
	exit(1);
    }
}

static void
testEscapeCharacter()
{
    HnProperties properties = new_HnProperties();
    const char *src =
	"Key   =  \\t\\n\\r\\u0\\u4\\u8\\uA\\uC\\u00\\u44\\u88\\uAA\\uCC\n";
    const char *dst =
	"Key=\\t\\n\\r\000\004\010\014\000\044\0110\0220\0440\n";
    HnString saved;

    printf("Test of escape character: ");

    properties.load(src);
    saved = properties.save();

    if ( saved.equals(dst) ) {
	printf("ok!\n");
    }
    else {
	printf("bad\n");
	exit(1);
    }
}

int
main(void)
{
    testLeadingWhiteSpace();
    testContinuedLine();
    testCarriageReturn();
    testEscapeCharacter();
}
