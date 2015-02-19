/*
 * HnPropertiesObj.hh
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
 * $Id: HnPropertiesObj.hh,v 1.2 2000/06/10 11:11:26 katayama Exp $
 */

#ifndef _HnPropertiesObj_hh
#define _HnPropertiesObj_hh

#include "HnSRTree/HnObject.hh"

class HnPropertiesObj: public HnObject {
private:
    HnProperties defaults;
    HnStringVector keys;
    HnStringVector values;

private:
    void initialize(void) {
	defaults = HnProperties::null;
	keys = new_HnStringVector();
	values = new_HnStringVector();
    }

    const char *skipSpaces(const char *bp);
    const char *readOneChar(const char *bp, char *ch_return);
    const char *readOneProperty(const char *bp);

public:
    HnPropertiesObj(void) {
	initialize();
    }
    
    HnPropertiesObj(const HnProperties &defaults) {
	initialize();
	this->defaults = defaults;
    }

    HnString getProperty(const HnString &key) const;
    HnString getProperty(const HnString &key, const HnString &defaultValue);

    HnString setProperty(const HnString &key, const HnString &value);
    HnString removeProperty(const HnString &key);

    int size(void) const {
	return keys.size();
    }
    HnString getKeyAt(int index) const {
	return keys.elementAt(index);
    }
    HnString getValueAt(int index) const {
	return values.elementAt(index);
    }

    void load(const char *buffer);
    HnString save(void) const;
    void print(void) const;
};

#endif /* _HnPropertiesObj_hh */
