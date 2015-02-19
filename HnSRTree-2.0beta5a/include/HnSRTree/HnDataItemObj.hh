/*
 * HnDataItemObj.hh
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
 * 03/27/96 katayama@rd.nacsis.ac.jp
 * $Id: HnDataItemObj.hh,v 1.3 2000/06/10 14:04:48 katayama Exp $
 */

#ifndef _HnDataItemObj_hh
#define _HnDataItemObj_hh

#include <stdlib.h>
#include "HnSRTree/HnObject.hh"

/*{
#include "HnSRTree/HnBlockStream.hh"
}*/

class HnDataItemObj: public HnObject {
private:
    char *buffer;
    int len;

    void initialize(void) {
	buffer = NULL;
	len = 0;
    }
    void dispose(void) {
	if ( buffer != NULL ) {
	    HnFree(buffer, len);
	}

	buffer = NULL;
	len = 0;
    }

public:
    /* constructor and destructor */
    HnDataItemObj(void);
    HnDataItemObj(int n);
    HnDataItemObj(const void *ptr, int n);
    HnDataItemObj(const void *ptr1, int n1, const void *ptr2, int n2);
    HnDataItemObj(int n, HnBlockStream &blockStream);
    ~HnDataItemObj(void);

    /* HnBlockStream */
    void writeTo(HnBlockStream &blockStream) const {
	blockStream.writeCharArray(buffer, len);
    }
    static int getSizeInBlockStream(int n) {
	return n;
    }

    /* attributes */
    char *toCharArray(void) const { return buffer; };
    char &charAt(int i) const { return buffer[i]; };
    int length(void) const { return len; };

    /* utilities */
    HnBool equals(const HnDataItem &dataItem) const;
};

#endif /* _HnDataItemObj_hh */
