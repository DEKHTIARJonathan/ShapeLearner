/*
 * checkIndex.cc
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
 * 10/14/96 katayama@rd.nacsis.ac.jp
 * $Id: checkIndex.cc,v 1.3 2000/06/10 11:20:56 katayama Exp $
 */

#include <stdio.h>
#include "HnSRTree/HnSRTreeFile.hh"

int
main(int argc, char *argv[])
{
    HnSRTreeFile indexFile;

    if ( argc != 2 ) {
	fprintf(stderr,	"Usage: %s indexFile\n", argv[0]);
	return 1;
    }

    indexFile = new_HnSRTreeFile(argv[1], "r");

    if ( indexFile == HnSRTreeFile::null ) {
	perror(argv[1]);
	return 1;
    }

    indexFile.check();
    indexFile.close();

    return 0;
}
