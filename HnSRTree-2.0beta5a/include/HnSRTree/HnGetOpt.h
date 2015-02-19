/*
 * HnGetOpt.h
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
 * $Id: HnGetOpt.h,v 1.2 2002/09/13 18:37:21 katayama Exp $
 */

#ifndef _HnGetOpt_h
#define _HnGetOpt_h

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

_HNSRTIMP extern int HnGetOpt_optind;
_HNSRTIMP extern char *HnGetOpt_optarg;

_HNSRTIMP int HnGetOpt(int argc, char *argv[], char *optstring);

#ifdef __cplusplus
};
#endif

#define optind		HnGetOpt_optind
#define optarg		HnGetOpt_optarg
#define getopt(c, v, s)	HnGetOpt(c, v, s)

#endif /* _HnGetOpt_h */
