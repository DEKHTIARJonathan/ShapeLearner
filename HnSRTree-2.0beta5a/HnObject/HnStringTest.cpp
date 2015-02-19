/*
 * HnStringTest.cc
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
 * 06/15/96 katayama@rd.nacsis.ac.jp
 * $Id: HnStringTest.cc,v 1.4 2000/06/10 11:18:37 katayama Exp $
 */

#include <stdio.h>
#include "HnSRTree/HnString.hh"

int
main(void)
{
	HnString string, string1, string2, string3;

	/*
	 * HnString
	 */

	/* constructors */
	printf("+ string = new_HnString();\n");
	string = new_HnString();
	printf("+ printf(\"string: %%s\\n\", (char *)string);\n");
	printf("string: %s\n", (char *)string);
	printf("\n");

	printf("+ string = new_HnString(\"Hello world!\");\n");
	string = new_HnString("Hello world!");
	printf("+ printf(\"string: %%s\\n\", (char *)string);\n");
	printf("string: %s\n", (char *)string);
	printf("\n");

	printf("+ string = \"Hello world!\";\n");
	string = "Hello world!";
	printf("+ printf(\"string: %%s\\n\", (char *)string);\n");
	printf("string: %s\n", (char *)string);
	printf("\n");

	printf("+ string = HnString::null;\n");
	string = HnString::null;
	printf("+ printf(\"%%s\\n\", "
	       "((string == HnString::null) ? \"yes\" : \"no\"));\n");
	printf("%s\n", ((string == HnString::null) ? "yes" : "no"));
	printf("\n");

	/* concat */
	printf("+ string = (HnString)\"Hello\" + \" \" + \"world!\";\n");
	string = (HnString)"Hello" + " " + "world!";
	printf("+ printf(\"string: %%s\\n\", (char *)string);\n");
	printf("string: %s\n", (char *)string);
	printf("\n");

	printf("+ string1 = \"Hello \";\n");
	string1 = "Hello ";
	printf("+ string2 = \"world!\";\n");
	string2 = "world!";
	printf("+ string3 = string1.concat(string2);\n");
	string3 = string1.concat(string2);
	printf("+ printf(\"string1: %%s\\n\", (char *)string1);\n");
	printf("string1: %s\n", (char *)string1);
	printf("+ printf(\"string2: %%s\\n\", (char *)string2);\n");
	printf("string2: %s\n", (char *)string2);
	printf("+ printf(\"string3: %%s\\n\", (char *)string3);\n");
	printf("string3: %s\n", (char *)string3);
	printf("\n");

	printf("+ string1 = \"Hello \";\n");
	string1 = "Hello ";
	printf("+ string2 = \"world!\";\n");
	string2 = "world!";
	printf("+ string1.append(string2);\n");
	string1.append(string2);
	printf("+ printf(\"string1: %%s\\n\", (char *)string1);\n");
	printf("string1: %s\n", (char *)string1);
	printf("+ printf(\"string2: %%s\\n\", (char *)string2);\n");
	printf("string2: %s\n", (char *)string2);
	printf("\n");

	printf("+ string1 = \"Hello \";\n");
	string1 = "Hello ";
	printf("+ string2 = \"world!\";\n");
	string2 = "world!";
	printf("+ string1 += string2;\n");
	string1 += string2;
	printf("+ printf(\"string1: %%s\\n\", (char *)string1);\n");
	printf("string1: %s\n", (char *)string1);
	printf("+ printf(\"string2: %%s\\n\", (char *)string2);\n");
	printf("string2: %s\n", (char *)string2);
	printf("\n");

	/* equals */
	printf("+ string1 = \"Hello world!\";\n");
	string1 = "Hello world!";
	printf("+ string2 = \"Hello world!\";\n");
	string2 = "Hello world!";
	printf("+ printf(\"%%s\\n\", "
	       "((string1 == string2) ? \"yes\" : \"no\"));\n");
	printf("%s\n", ((string1 == string2) ? "yes" : "no"));
	printf("+ printf(\"%%s\\n\", "
	       "((string1.equals(string2)) ? \"yes\" : \"no\"));\n");
	printf("%s\n", ((string1.equals(string2)) ? "yes" : "no"));
	printf("\n");

	printf("+ string1 = \"Hi everyone!\";\n");
	string1 = "Hi everyone!";
	printf("+ string2 = \"Hello world!\";\n");
	string2 = "Hello world!";
	printf("+ printf(\"%%s\\n\", "
	       "((string1 == string2) ? \"yes\" : \"no\"));\n");
	printf("%s\n", ((string1 == string2) ? "yes" : "no"));
	printf("+ printf(\"%%s\\n\", "
	       "((string1.equals(string2)) ? \"yes\" : \"no\"));\n");
	printf("%s\n", ((string1.equals(string2)) ? "yes" : "no"));
	printf("\n");

	/* The following line should cause to abort */
	// string = new_HnString(NULL);

	return 0;
}
