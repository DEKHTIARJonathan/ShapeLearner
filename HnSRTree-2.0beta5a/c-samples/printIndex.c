/*
 * printIndex.c
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
 * $Id: printIndex.c,v 1.5 2002/09/13 05:14:32 katayama Exp $
 */

#include <stdio.h>
#ifdef _MSC_VER
#include "HnSRTree/HnGetOpt.h"
#else
#include <unistd.h>
#endif
#include "HnSRTree/HnSRTreeFileSt.h"

int
main(int argc, char *argv[])
{
    char *indexFileName;
    HnSRTreeFileSt *indexFile;
    HnBool verbose;
    int c, errflag;
    extern int optind;

    verbose = HnFALSE;

    errflag = 0;
    while ( (c = getopt(argc, argv, "v")) != EOF ) {
	switch ( c ) {
	case 'v':
	    verbose = HnTRUE;
	    break;
	case '?':
	    errflag = 1;
	    break;
	}
    }
    if ( errflag || optind != argc - 1 ) {
	fprintf(stderr,	"Usage: %s [-v] indexFile\n", argv[0]);
	return 1;
    }
    indexFileName = argv[optind];

    indexFile = HnSRTreeFileSt_open(indexFileName, "r");

    if ( indexFile == NULL ) {
	perror(indexFileName);
	return 1;
    }

    HnSRTreeFileSt_print(indexFile, verbose);
    HnSRTreeFileSt_close(indexFile);

    return 0;
}
