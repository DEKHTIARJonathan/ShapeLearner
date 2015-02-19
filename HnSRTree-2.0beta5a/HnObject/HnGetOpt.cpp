/*
 * HnGetOpt.c
 * Copyright (C) 2002 Norio Katayama
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
 * 09/10/2002 Norio KATAYAMA
 * $Id: HnGetOpt.c,v 1.2 2002/09/13 18:38:42 katayama Exp $
 */

#include <stdio.h>
#include <string.h>
#include "HnSRTree/HnGetOpt.h"

int HnGetOpt_optind = 1;
char *HnGetOpt_optarg = NULL;

int
HnGetOpt(int argc, char *argv[], char *optstring)
{
    if ( argv[HnGetOpt_optind] == NULL ||
	 argv[HnGetOpt_optind][0] != '-' ) {
	return EOF;
    }
    else {
	char optchar = argv[HnGetOpt_optind][1];

	if ( optchar == '-' ) {
	    HnGetOpt_optind ++;
	    return EOF;
	}
	else {
	    char *ptr;

	    if ( (ptr = strchr(optstring, optchar)) == NULL ) {
		fprintf(stderr, "%s: illegal option -- %c\n",
			argv[0], optchar);
		HnGetOpt_optind ++;
		return '?';
	    }
	    else {
		if ( ptr[1] == ':' ) {
		    HnGetOpt_optarg = argv[HnGetOpt_optind + 1];
		    HnGetOpt_optind += 2;
		}
		else {
		    HnGetOpt_optarg = NULL;
		    HnGetOpt_optind ++;
		}
		return optchar;
	    }
	}
    }
}
