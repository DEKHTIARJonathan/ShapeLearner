/*
 * HnStringObj.cc
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
 * 11/20/95 katayama@rd.nacsis.ac.jp
 * $Id: HnStringObj.cc,v 1.5 2000/06/10 11:11:57 katayama Exp $
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "HnSRTree/HnString.hh"
#include "HnSRTree/HnStringObj.hh"

HnStringObj::HnStringObj(void)
{
    initialize();

    buffer = (char *)HnMalloc(1);
    len = 0;
    buffer[len] = 0;
}

HnStringObj::HnStringObj(const char *ptr, int n)
{
    initialize();

    buffer = (char *)HnMalloc(n + 1);
    memcpy(buffer, ptr, n);
    len = n;
    buffer[len] = 0;
}

HnStringObj::HnStringObj(const HnString &string)
{
    const char *ptr = string.chars();
    int n = string.length();

    initialize();

    buffer = (char *)HnMalloc(n + 1);
    memcpy(buffer, ptr, n);
    len = n;
    buffer[len] = 0;
}

HnStringObj::HnStringObj(const char *ptr1, int n1, const char *ptr2, int n2)
{
    initialize();

    buffer = (char *)HnMalloc(n1 + n2 + 1);
    memcpy(buffer, ptr1, n1);
    memcpy(buffer + n1, ptr2, n2);
    len = n1 + n2;
    buffer[len] = 0;
}

HnStringObj::~HnStringObj(void)
{
    dispose();
}

HnString
HnStringObj::concat(const char *ptr, int n) const
{
    if ( n == 0 ) {
	return this;
    }
    else {
	return new HnStringObj(chars(), length(), ptr, n);
    }
}

HnString
HnStringObj::concat(const char *ptr) const
{
    return concat(ptr, ((ptr == NULL) ? 0 : strlen(ptr)));
}

HnString
HnStringObj::concat(const HnString &string) const
{
    return concat(string.getObject()->chars(), string.getObject()->length());
}

HnBool
HnStringObj::equals(const HnString &string) const
{
    if ( len == string.length() && strcmp(buffer, string.chars()) == 0 ) {
	return HnTRUE;
    }
    else {
	return HnFALSE;
    }
}

int
HnStringObj::compareTo(const HnString &string) const
{
    return strcmp(buffer, string.chars());
}

HnBool
HnStringObj::startsWith(const char *prefix, int n) const
{
    if ( n > len ) {
	return HnFALSE;
    }
    else {
	return (strncmp(chars(), prefix, n) == 0);
    }
}

HnBool
HnStringObj::startsWith(const char *prefix) const
{
    return startsWith(prefix, strlen(prefix));
}

HnBool
HnStringObj::startsWith(const HnString &string) const
{
    return startsWith(string.getObject()->chars(),
		      string.getObject()->length());
}

HnBool
HnStringObj::endsWith(const char *suffix, int n) const
{
    if ( n > len ) {
	return HnFALSE;
    }
    else {
	return (strncmp(chars() + len - n, suffix, n) == 0);
    }
}

HnBool
HnStringObj::endsWith(const char *suffix) const
{
    return endsWith(suffix, strlen(suffix));
}

HnBool
HnStringObj::endsWith(const HnString &string) const
{
    return endsWith(string.getObject()->chars(),
		    string.getObject()->length());
}

int
HnStringObj::indexOf(int ch, int fromIndex) const
{
    for ( int i=fromIndex; i<len; i++ ) {
	if ( buffer[i] == ch ) {
	    return i;
	}
    }

    return -1;
}

int
HnStringObj::indexOf(int ch) const
{
    return indexOf(ch, 0);
}

int
HnStringObj::lastIndexOf(int ch, int fromIndex) const
{
    for ( int i=fromIndex; i>=0; i-- ) {
	if ( buffer[i] == ch ) {
	    return i;
	}
    }

    return -1;
}

int
HnStringObj::lastIndexOf(int ch) const
{
    return lastIndexOf(ch, length() - 1);
}

HnString
HnStringObj::substring(int beginIndex, int endIndex) const
{
    /*
     * NOTE:
     *	    `beginIndex' is inclusive, while `endIndex' is exclusive.
     */

    if ( beginIndex < 0 || endIndex > len ) {
	HnAbort("HnStringObj::substring: index is out of bounds.");
    }

    return new_HnString(buffer + beginIndex, endIndex - beginIndex);
}

HnString
HnStringObj::substring(int beginIndex) const
{
    return substring(beginIndex, length());
}

HnString
HnStringObj::trim(void) const
{
    int beginIndex, endIndex;

    beginIndex = 0;
    while ( beginIndex < len && isspace(buffer[beginIndex]) ) {
	beginIndex ++;
    }

    endIndex = len;
    while ( endIndex > 0 && isspace(buffer[endIndex - 1]) ) {
	endIndex --;
    }

    return substring(beginIndex, endIndex);
}

/*
 * HnString
 */

HnString::HnString(const char *ptr)
{
    if ( ptr == NULL ) {
	assign(HnString::null);
    }
    else {
	assign(new_HnString(ptr, strlen(ptr)));
    }
}

HnString &
HnString::operator += (const char *ptr)
{
    assign(concat(ptr));
    return *this;
}

HnString &
HnString::operator += (const HnString &value)
{
    assign(concat(value.chars(), value.length()));
    return *this;
}

HnString &
HnString::operator += (char c)
{
    assign(concat(&c, 1));
    return *this;
}

HnString &
HnString::operator += (int value)
{
    assign(concat(valueOf(value)));
    return *this;
}

HnString &
HnString::operator += (double value)
{
    assign(concat(valueOf(value)));
    return *this;
}

void
HnString::append(const char *ptr, int n)
{
    assign(concat(ptr, n));
}

void
HnString::append(const char *ptr)
{
    append(ptr, ((ptr == NULL) ? 0 : strlen(ptr)));
}

void
HnString::append(const HnString &string)
{
    append(string.chars(), string.length());
}

#define FORMAT_ARRAY_SIZE	4096
#define FORMAT_MAX_BUFFER_SIZE  65536

HnString
HnString::format(const char *format, ... )
{
    va_list ap;
    char array[FORMAT_ARRAY_SIZE];
    int code;
    HnString string;

    va_start(ap, format);
    code = vsnprintf(array, sizeof(array), format, ap);
    va_end(ap);

    if ( code < (int)sizeof(array) - 1 ) {
	string = new_HnString(array);
    }
    else {
	char *buffer;
	int bufferSize;

	bufferSize = strlen(format) * 2;
	if ( (buffer = (char *)malloc(bufferSize)) == NULL ) {
	    HnSysError("malloc");
	}

	for ( ;; ) {
	    printf("bufferSize=%d\n", bufferSize);

	    va_start(ap, format);
	    code = vsnprintf(buffer, bufferSize, format, ap);

	    if ( code < bufferSize - 1 ) {
		break;
	    }

	    bufferSize *= 2;

	    if ( bufferSize > FORMAT_MAX_BUFFER_SIZE ) {
		HnAbort("string is too long.");
	    }

	    if ( (buffer = (char *)realloc(buffer, bufferSize)) == NULL ) {
		HnSysError("realloc");
	    }
	}

	string = new_HnString(buffer);

	free(buffer);
    }

    return string;
}

/*
 * Class Methods
 */

HnString
HnStringObj::getString(FILE *fp)
{
    HnString string = new_HnString();
    char buffer[4096];
    int length;

    while ( fgets(buffer, sizeof(buffer), fp) != NULL ) {
	length = strlen(buffer);
	if ( buffer[length - 1] == '\n' ) {
	    buffer[length - 1] = 0;
	    string.append(buffer, length - 1);
	    return string;
	}
	else {
	    string.append(buffer, length);
	}
    }

    if ( feof(fp) && string.length() == 0 ) {
	return HnString::null;
    }
    else {
	return string;
    }
}

HnStringVector
HnStringObj::sort(const HnStringVector &strings)
{
    HnStringVector vector = new_HnStringVector(strings);
    int i, j, len = strings.size();

    for ( i=0; i<len; i++ ) {
	for ( j=i+1; j<len; j++ ) {
	    if ( strcmp(vector[i], vector[j]) > 0 ) {
		vector.swapElementsAt(i, j);
	    }
	}
    }

    return vector;
}

/*
 * valueOf
 */

HnString
HnStringObj::valueOf(double value)
{
    return valueOf(value, 0, 6);
}

HnString
HnStringObj::valueOf(double value, int ndigit, int trailing)
{
    char buffer[256];

    sprintf(buffer, "%*.*g", ndigit, trailing, value);

    return (HnString)buffer;
}

int
HnStringObj::parseInt(const HnString &string)
{
    const char *ptr = string.chars();
    int value;
    char *endptr;

    value = (int)strtol(ptr, &endptr, 10);

    if ( endptr - ptr != string.length() ) {
	HnAbort("invalid string for an integer `%s'.",
		(char *)string);
    }

    return value;
}

double
HnStringObj::parseDouble(const HnString &string)
{
    const char *ptr = string.chars();
    double value;
    char *endptr;

    value = strtod(ptr, &endptr);

    if ( endptr - ptr != string.length() ) {
	HnAbort("invalid string for a double-precision number `%s'.",
		(char *)string);
    }

    return value;
}

/*
 * pack and unpack
 */

/* 
 * NOTE:
 *	packString() converts an arbitrary character string into an
 *	escaped unique form. The conversion syntax is as follows:
 *
 *		invalid string	   -> 	%UNDEF
 *		empty string	   -> 	%EMPTY
 *
 *		`%' character	   ->	%%
 *		` ' character	   ->	+
 *		`{' character	   ->	%(
 *		`}' character	   ->	%(
 *
 *		control	characters ->	%[0-9A-F][0-9A-F]
 *		`+' character	   ->	%[0-9A-F][0-9A-F]
 *		`/', `=', `,' 	   ->	%[0-9A-F][0-9A-F]
 *		`[', `]', `~', `:' ->	%[0-9A-F][0-9A-F]
 */

static void
escapeString(const char *ptr, HnString *string_return)
{
#ifdef DEBUG
    struct timeval s, e;
    gettimeofday(&s, NULL);
#endif
    /*
     * NOTE:
     *	The variable `string_return' is only appended not cleared.
     */

    int maxLength;
    char *bp;
    static char *buffer = NULL;

    /*
     * special treatment for `%UNDEF' and '%EMPTY'
     */

    if ( ptr == NULL ) {
	string_return->append("%UNDEF");
	return;
    }

    if ( ptr[0] == 0 ) {
	string_return->append("%EMPTY");
	return;
    }

    /*
     * allocate buffer
     *
     * NOTE:
     *	    The maxium length of the result is the three time length of
     *	    the given string.
     */

    maxLength = strlen(ptr) * 3;

    if ( buffer == NULL ) {
	if ( (buffer = (char *)malloc(maxLength + 1)) == NULL )
	    HnSysError("malloc");
    }
    else {
	if ( (buffer = (char *)realloc(buffer, maxLength + 1)) == NULL )
	    HnSysError("realloc");
    }

    bp = buffer;

    while ( *ptr != 0 ) {
	if ( iscntrl((unsigned char)*ptr) ||
	     strchr("% {}+/=,[]~:", *ptr) != NULL ) {
	    switch ( *ptr ) {
	    case '%':
		*bp++ = '%';
		*bp++ = '%';
		break;
	    case ' ':
		*bp++ = '+';
		break;
	    case '{':
		*bp++ = '%';
		*bp++ = '(';
		break;
	    case '}':
		*bp++ = '%';
		*bp++ = ')';
		break;
	    default:
		sprintf(bp, "%%%02X", *ptr);
		bp += 3;
		break;
	    }
	    ptr ++;
	}
	else
	    *bp++ = *ptr++;
    }

    string_return->append(buffer, bp - buffer);
#ifdef DEBUG
    gettimeofday(&e, NULL);
    sumEscapeString +=
	e.tv_sec * 1000 + e.tv_usec / 1000
	- s.tv_sec * 1000 - s.tv_usec / 1000;
#endif
}

static void
unescapeString(const char *ptr, int n, HnString *string_return)
{
#ifdef DEBUG
    struct timeval s, e;
    gettimeofday(&s, NULL);
#endif
    HnString &string = *string_return;
    const char *start = ptr;
    const char *end = ptr + n;
    char *bp;
    static char *buffer = NULL;

    /*
     * special treatment for `%UNDEF' and `%EMPTY'
     */

    if ( strncmp(ptr, "%UNDEF", n) == 0 ) {
	string = HnString::null;
	return;
    }
    if ( strncmp(ptr, "%EMPTY", n) == 0 ) {
	string = "";
	return;
    }

    /*
     * allocate buffer
     */

    if ( buffer == NULL ) {
	if ( (buffer = (char *)malloc(n + 1)) == NULL )
	    HnSysError("malloc");
    }
    else {
	if ( (buffer = (char *)realloc(buffer, n + 1)) == NULL )
	    HnSysError("realloc");
    }

    bp = buffer;
	
    while ( ptr < end ) {
	switch ( *ptr ) {
	case '+':
	    *bp++ = ' ';
	    ptr ++;
	    break;
	case '%':
	    ptr ++;
	    if ( *ptr == '%' ) {
		*bp++ = '%';
		ptr ++;
	    }
	    else if ( *ptr == '(' ) {
		*bp++ = '{';
		ptr ++;
	    }
	    else if ( *ptr == ')' ) {
		*bp++ = '}';
		ptr ++;
	    }
	    else if ( isxdigit(ptr[0]) && isxdigit(ptr[1]) ) {
		int c;

		sscanf(ptr, "%2X", &c);
		*bp++ = c;
		ptr += 2;
	    }
	    else {
		HnString string = new_HnString(start, end - start);
		HnAbort("unescapeString: an unexpected "
			"character follows after `%%' "
			"at `%s'[%d].",
			(char *)string, ptr - start);
	    }
	    break;
	default:
	    *bp ++ = *ptr;
	    ptr ++;
	    break;
	}
    }

    string = new_HnString(buffer, bp - buffer);
#ifdef DEBUG
    gettimeofday(&e, NULL);
    sumUnescapeString +=
	e.tv_sec * 1000 + e.tv_usec / 1000
	- s.tv_sec * 1000 - s.tv_usec / 1000;
#endif
}

void
HnStringObj::escape(const char *ptr, HnString *string_return)
{
    HnString &string = *string_return;

    string = new_HnString();
    escapeString(ptr, &string);
}

void
HnStringObj::unescape(const char *ptr, HnString *string_return)
{
    unescapeString(ptr, (ptr == NULL ? 0 : strlen(ptr)), string_return);
}

/*
 * pack and unpack character strings
 *
 * NOTE:
 *	The partial match for truncated array should be permitted, because
 *	HnEntityFile, HnConnectionFile, HnLinkFile, HnPropertyFile, and 
 *	HnReferenceFile depend on that property.
 *
 *	The conversion syntax is as follows:
 *		invalid array	->	%UNDEF
 *		empty array	->	%EMPTY
 *		normal array	->	{array[0]}{array[1]}...{array[n-1]}
 */

void
HnStringObj::pack(const HnStringVector &strings, HnString *string_return)
{
    HnString &string = *string_return;
    int i;

    /*
     * special treatment for `%UNDEF' and '%EMPTY'
     */

    if ( strings.isInvalid() ) {
	string = "%UNDEF";
	return;
    }

    if ( strings.size() == 0 ) {
	string = "%EMPTY";
	return;
    }

    string = new_HnString();

    for ( i=0; i<strings.size(); i++ ) {
	string += '{';
	escapeString(strings[i], &string);
	string += '}';
    }
}

void
HnStringObj::unpack(const char *chars, HnStringVector *strings_return)
{
    HnStringVector &strings = *strings_return;
    const char *head, *ptr;
    HnString string;

    /*
     * special treatment for `%UNDEF' and `%EMPTY'
     */

    if ( strcmp(chars, "%UNDEF") == 0 ) {
	strings.invalidate();
	return;
    }
    if ( strcmp(chars, "%EMPTY") == 0 ) {
	strings = new_HnStringVector();
	strings.removeAllElements();
	return;
    }

    strings = new_HnStringVector();
    strings.removeAllElements();
    ptr = chars;

    while ( *ptr != 0 ) {
	if ( *ptr != '{' ) {
	    HnAbort("HnStringObj::unpack: "
		    "`{' is not found at `%s'[%d].",
		    chars, ptr - chars);
	}
	head = ptr + 1;

	if ( (ptr = strchr(head, '}')) == NULL )
	    HnAbort("HnStringObj::unpack: "
		    "`}' is not found at `%s'[%d].",
		    chars, head - chars);

	unescapeString(head, ptr - head, &string);
	strings.addElement(string);

	ptr++;
    }
}

/*
 * variants of pack() and unpack()
 */

void
HnStringObj::pack(const char *array[], int length, HnString *string_return)
{
    HnStringVector strings;
    int i;

    if ( array == NULL )
	strings.invalidate();
    else {
	strings = new_HnStringVector();
	strings.removeAllElements();
	for ( i=0; i<length; i++ ) {
	    strings.addElement(array[i]);
	}
    }

    HnStringObj::pack(strings, string_return);
}

void
HnStringObj::pack(const char *ptr1, HnString *string_return)
{
    const char *array[1];

    array[0] = ptr1;

    HnStringObj::pack(array, 1, string_return);
}

void
HnStringObj::unpack(const char *chars, HnString *string1_return)
{
    HnStringVector strings;

    HnStringObj::unpack(chars, &strings);

    if ( string1_return != NULL )
	*string1_return = strings[0];
}

void
HnStringObj::pack(const char *ptr1, const char *ptr2, HnString *string_return)
{
    const char *array[2];

    array[0] = ptr1;
    array[1] = ptr2;

    HnStringObj::pack(array, 2, string_return);
}

void
HnStringObj::unpack(const char *chars,
		    HnString *string1_return, HnString *string2_return)
{
    HnStringVector strings;

    HnStringObj::unpack(chars, &strings);

    if ( string1_return != NULL )
	*string1_return = strings[0];
    if ( string2_return != NULL )
	*string2_return = strings[1];
}

void
HnStringObj::pack(const char *ptr1, const char *ptr2, const char *ptr3,
		  HnString *string_return)
{
    const char *array[3];

    array[0] = ptr1;
    array[1] = ptr2;
    array[2] = ptr3;

    HnStringObj::pack(array, 3, string_return);
}

void
HnStringObj::unpack(const char *chars,
		    HnString *string1_return, HnString *string2_return,
		    HnString *string3_return)
{
    HnStringVector strings;

    HnStringObj::unpack(chars, &strings);

    if ( string1_return != NULL )
	*string1_return = strings[0];
    if ( string2_return != NULL )
	*string2_return = strings[1];
    if ( string3_return != NULL )
	*string3_return = strings[2];
}

void
HnStringObj::pack(const char *ptr1, const char *ptr2,
		  const char *ptr3, const char *ptr4, HnString *string_return)
{
    const char *array[4];

    array[0] = ptr1;
    array[1] = ptr2;
    array[2] = ptr3;
    array[3] = ptr4;

    HnStringObj::pack(array, 4, string_return);
}

void
HnStringObj::unpack(const char *chars,
		    HnString *string1_return, HnString *string2_return,
		    HnString *string3_return, HnString *string4_return)
{
    HnStringVector strings;

    HnStringObj::unpack(chars, &strings);

    if ( string1_return != NULL )
	*string1_return = strings[0];
    if ( string2_return != NULL )
	*string2_return = strings[1];
    if ( string3_return != NULL )
	*string3_return = strings[2];
    if ( string4_return != NULL )
	*string4_return = strings[3];
}
