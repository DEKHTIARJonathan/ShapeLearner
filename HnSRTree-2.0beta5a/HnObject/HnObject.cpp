/*
 * HnObject.cc
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
 * 11/23/95 katayama@rd.nacsis.ac.jp
 * $Id: HnObject.cc,v 1.6 2000/06/10 11:18:26 katayama Exp $
 */

#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include "HnSRTree/config.h"
#include "HnSRTree/HnObject.hh"
#include "HnSRTree/HnString.hh"

HnObject *HnObject::busyObject = NULL;
int HnObject::numBusyObjects = 0;

HnObject *HnObject::freeObject = NULL;
int HnObject::numFreeObjects = 0;

int HnObject::MAGIC = 0x05121010;

HnObject::HnObject(void)
{
    /* reference */
    magic = MAGIC;
    refCount = 0;

    setFree();

    /* constructor failure flag */
    constructorFailureFlag = HnFALSE;
}

HnObject::~HnObject(void)
{
    /* reference */
    if ( refCount != 0 ) {
	HnAbort("destructor is invoked for a busy object.");
    }

    unsetFree();

    magic = -1;
    refCount = -1;

    /* constructor failure flag */
    constructorFailureFlag = HnFALSE;
}

HnString
HnObject::toString(void) const
{
    char buffer[64];

    sprintf(buffer, "HnObject@%p", this);

    return buffer;
}
