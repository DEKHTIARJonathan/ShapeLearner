/*
 * HnStringObj.hh
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
 * $Id: HnStringObj.hh,v 1.5 2000/06/10 11:11:28 katayama Exp $
 */

#ifndef _HnStringObj_hh
#define _HnStringObj_hh

#include <stdlib.h>
#include "HnSRTree/HnObject.hh"

/*{
#include <string.h>
class HnStringVector;
class HnString;
}*/

class HnStringObj: public HnObject {
private:
    char *buffer;
    int len;

    void initialize(void) {
	buffer = NULL;
	len = 0;
    }
    void dispose(void) {
	if ( buffer != NULL ) {
	    HnFree(buffer, len + 1);
	}

	buffer = NULL;
    }

private:
    HnStringObj(const char *ptr1, int n1, const char *ptr2, int n2);

public:
/*{
    HnString(const HnString &string): HnPointer(string) {};
    HnString(const char *ptr);
    operator char *(void) const;
    HnString operator + (const char *ptr) const;
    HnString operator + (int value) const;
    HnString operator + (double value) const;
    HnString &operator += (const char *ptr);
    HnString &operator += (const HnString &value);
    HnString &operator += (char c);
    HnString &operator += (int value);
    HnString &operator += (double value);
    void append(const char *ptr, int n);
    void append(const char *ptr);
    void append(const HnString &string);
    HnBool equals(const char *ptr, int n) const;
    HnBool equals(const char *ptr) const;
    static HnString format(const char *format, ... )
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6)
__attribute__ ((format (printf, 1, 2)))
#endif
    ;
}*/
public:
    /* constructor and destructor */
    HnStringObj(void);
    HnStringObj(const char *ptr, int n);
    HnStringObj(const HnString &string);
    ~HnStringObj(void);

    /* chars and length */
    const char *chars(void) const { return buffer; };
    const char &get(int i) const { return buffer[i]; };
    const char &operator[](int i) const { return get(i); }
    int length(void) const { return len; };
    HnString toString(void) const { return this; };

    /* concat */
    /*
     * NOTE:
     *	The current implementation copies the entire string on concat.
     */
    HnString concat(const char *ptr, int n) const;
    HnString concat(const char *ptr) const;
    HnString concat(const HnString &string) const;

    /* utilities */
    HnBool equals(const HnString &string) const;
    int compareTo(const HnString &string) const;

    HnBool startsWith(const char *prefix, int n) const;
    HnBool startsWith(const char *prefix) const;
    HnBool startsWith(const HnString &string) const;
    HnBool endsWith(const char *suffix, int n) const;
    HnBool endsWith(const char *suffix) const;
    HnBool endsWith(const HnString &string) const;

    int indexOf(int ch, int fromIndex) const;
    int indexOf(int ch) const;
    int lastIndexOf(int ch, int fromIndex) const;
    int lastIndexOf(int ch) const;

    HnString substring(int beginIndex, int endIndex) const;
    HnString substring(int beginIndex) const;
    HnString trim(void) const;

public:
    static HnString getString(FILE *fp);
    static HnStringVector sort(const HnStringVector &strings);

    /*
     * valueOf
     */

    static HnString valueOf(double value);
    static HnString valueOf(double value, int ndigit, int trailing);

    static int parseInt(const HnString &string);
    static double parseDouble(const HnString &string);

    /*
     * pack and unpack
     */

    static void escape(const char *ptr, HnString *string_return);
    static void unescape(const char *ptr, HnString *string_return);
    static void pack(const HnStringVector &strings,
		     HnString *string_return);
    static void unpack(const char *ptr, HnStringVector *strings_return);

    /*
     * variants of pack() and unpack()
     */

    static void pack(const char *array[], int length,
		     HnString *string_return);

    static void pack(const char *ptr1, HnString *string_return);
    static void unpack(const char *ptr, HnString *string1_return);

    static void pack(const char *ptr1, const char *ptr2,
		     HnString *string_return);
    static void unpack(const char *ptr,
		       HnString *string1_return, HnString *string2_return);

    static void pack(const char *ptr1, const char *ptr2, const char *ptr3,
		     HnString *string_return);
    static void unpack(const char *ptr,
		       HnString *string1_return, HnString *string2_return,
		       HnString *string3_return);

    static void pack(const char *ptr1, const char *ptr2, const char *ptr3,
		     const char *ptr4, HnString *string_return);
    static void unpack(const char *ptr,
		       HnString *string1_return, HnString *string2_return,
		       HnString *string3_return, HnString *string4_return);
};

/*{
inline
HnString::operator char *(void) const {
    if ( isInvalid() ) {
	return NULL;
    }
    else {
	return (char *)chars();
    }
}

inline HnString
HnString::operator + (const char *ptr) const {
    return concat(ptr);
}

inline HnString
HnString::operator + (int value) const
{
    return concat(valueOf(value));
}

inline HnString
HnString::operator + (double value) const
{
    return concat(valueOf(value));
}

inline HnBool
HnString::equals(const char *ptr, int n) const
{
    if ( getObject() == NULL ) {
	if ( ptr == NULL ) {
	    return HnTRUE;
	}
	else {
	    return HnFALSE;
	}
    }
    else {
	if ( ptr == NULL ) {
	    return HnFALSE;
	}
	else {
	    return (length() == n && strncmp(chars(), ptr, n) == 0);
	}
    }
}

inline HnBool
HnString::equals(const char *ptr) const
{
    return equals(ptr, ((ptr == NULL) ? 0 : strlen(ptr)));
}
}*/

#endif /* _HnStringObj_hh */
