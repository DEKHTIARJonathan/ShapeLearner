/*
 * HnPropertiesObj.c
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
 * $Id: HnPropertiesObj.cc,v 1.2 2000/06/10 11:11:56 katayama Exp $
 */

#include "HnSRTree/HnProperties.hh"
#include "HnSRTree/HnPropertiesObj.hh"

#include "HnSRTree/HnStringBuffer.hh"

HnString
HnPropertiesObj::getProperty(const HnString &key) const
{
    int index;

    if ( (index = keys.indexOf(key)) >= 0 ) {
	return values.elementAt(index);
    }

    if ( defaults == HnProperties::null ) {
	return HnString::null;
    }
    else {
	return defaults.getProperty(key);
    }
}

HnString
HnPropertiesObj::getProperty(const HnString &key, const HnString &defaultValue)
{
    HnString value;

    if ( (value = getProperty(key)) == HnString::null ) {
	return defaultValue;
    }
    else {
	return value;
    }
}

HnString
HnPropertiesObj::setProperty(const HnString &key, const HnString &value)
{
    int index;

    if ( key == HnString::null ) {
	HnAbort("null is invalid.");
    }
    if ( value == HnString::null ) {
	HnAbort("null is invalid.");
    }

    if ( (index = keys.indexOf(key)) >= 0 ) {
	HnString oldValue = values.elementAt(index);
	values.setElementAt(value, index);
	return oldValue;
    }
    else {
	keys.addElement(key);
	values.addElement(value);
	return HnString::null;
    }
}

HnString
HnPropertiesObj::removeProperty(const HnString &key)
{
    int index;
    HnString oldValue;

    if ( (index = keys.indexOf(key)) < 0 ) {
	HnAbort("no such key exists.");
    }

    oldValue = values.elementAt(index);

    keys.removeElementAt(index);
    values.removeElementAt(index);

    return oldValue;
}

const char *
HnPropertiesObj::skipSpaces(const char *bp)
{
    while ( *bp == ' ' || *bp == '\t' ) {
	bp ++;
    }

    return bp;
}

const char *
HnPropertiesObj::readOneChar(const char *bp, char *ch_return)
{
    int i, ch;

    ch = 0;
    for ( i=0; i<2; i++ ) {
	int digit;

	switch ( *bp ) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	    digit = *bp - '0';
	    bp ++;
	    break;
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
	    digit = *bp - 'A';
	    bp ++;
	    break;
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	    digit = *bp - 'a';
	    bp ++;
	    break;
	default:
	    digit = -1;
	    break;
	}
	if ( digit < 0 ) {
	    break;
	}
	else {
	    ch = (ch << 4) + digit;
	}
    }

    *ch_return = ch;
    return bp;
}

const char *
HnPropertiesObj::readOneProperty(const char *bp)
{
    HnStringBuffer key = new_HnStringBuffer();
    HnStringBuffer value = new_HnStringBuffer();
    char ch;

    while ( *bp != 0 && *bp != '=' && *bp != ':' &&
	    *bp != ' ' && *bp != '\t' && *bp != '\n' && *bp != '\r' ) {
	key.append(*bp);
	bp ++;
    }
    bp = skipSpaces(bp);
    if ( *bp == '=' || *bp == ':' ) {
	bp ++;
    }
    bp = skipSpaces(bp);
    while ( *bp != 0 && *bp != '\n' && *bp != '\r' ) {
	if ( *bp == '\\' ) {
	    bp ++;
	    switch (*bp) {
	    case '\r':
		bp ++;
		if ( *bp == '\n' ) {
		    bp ++;
		}
		bp = skipSpaces(bp);
		break;
	    case '\n':
		bp ++;
		if ( *bp == '\r' ) {
		    bp ++;
		}
		bp = skipSpaces(bp);
		break;
	    case 't':
		value.append('\t');
		bp ++;
		break;
	    case 'n':
		value.append('\n');
		bp ++;
		break;
	    case 'r':
		value.append('\r');
		bp ++;
		break;
	    case 'u':
		bp ++;
		bp = readOneChar(bp, &ch);
		value.append(ch);
		break;
	    default:
		value.append(*bp);
		bp ++;
		break;
	    }
	}
	else {
	    value.append(*bp);
	    bp ++;
	}
    }

    setProperty(key.toString(), value.toString());

    return bp;
}

void
HnPropertiesObj::load(const char *buffer)
{
    const char *bp = buffer;

    while ( *bp != 0 ) {
	if ( *bp == '#' || *bp == '!' ) {
	    bp ++;
	    while ( *bp != 0 && *bp != '\n' && *bp != '\r') {
		bp ++;
	    }
	}
	else if ( *bp == '\n' || *bp == '\r' ) {
	    bp ++;
	}
	else if ( *bp == ' ' || *bp == '\t' ) {
	    bp = skipSpaces(bp);
	}
	else {
	    bp = readOneProperty(bp);
	}
    }
}

HnString
HnPropertiesObj::save(void) const
{
    HnStringBuffer sb = new_HnStringBuffer();
    int i, numElements;

    numElements = keys.size();

    for ( i=0; i<numElements; i++ ) {
	HnBool isEmpty;
	const char *bp;

	sb += keys.elementAt(i);
	sb += '=';

	bp = values.elementAt(i).chars();
	isEmpty = HnTRUE;

	while ( *bp != 0 ) {
	    switch ( *bp ) {
	    case '\\':
		sb += "\\\\";
		bp ++;
		break;
	    case '\t':
		sb += "\\t";
		bp ++;
		break;
	    case '\n':
		sb += "\\n";
		bp ++;
		break;
	    case '\r':
		sb += "\\r";
		bp ++;
		break;
	    default:
		if ( *bp != ' ' || !isEmpty ) {
		    sb += *bp;
		    bp ++;
		}
		else {
		    static char hexDigit[] = { '0', '1', '2', '3', '4',
					       '5', '6', '7', '8', '9',
					       'A', 'B', 'C', 'D', 'E', 'F' };

		    sb += "\\u";
		    sb += hexDigit[(*bp >> 4) & 0x0F];
		    sb += hexDigit[(*bp     ) & 0x0F];

		    bp ++;
		}
		break;
	    }
	    isEmpty = HnFALSE;
	}

	sb += '\n';
    }

    return sb.toString();
}

void
HnPropertiesObj::print(void) const
{
    int i;

    printf("HnProperties[\n");
    for ( i=0; i<size(); i++ ) {
	printf("    %s=%s\n", (char *)getKeyAt(i), (char *)getValueAt(i));
    }
    printf("]\n");
}
