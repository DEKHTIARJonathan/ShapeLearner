/*
 * HnAbort.h
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
 * 11/25/95 katayama@rd.nacsis.ac.jp
 * $Id: HnAbort.h,v 1.2 2002/09/10 11:37:33 katayama Exp $
 */

#ifndef _HnAbort_h
#define _HnAbort_h

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

/*
 * Utility function getting the system error message string
 */

_HNSRTIMP extern char *HnGetSystemErrorMessage(void);

/*
 * Error message
 */

_HNSRTIMP extern void HnSetErrorMessage(const char *format, ... )
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6)
__attribute__ ((format (printf, 1, 2)))
#endif
;

_HNSRTIMP extern char *HnGetErrorMessage(void);
_HNSRTIMP extern void HnClearErrorMessage(void);

/*
 * Logging
 */

_HNSRTIMP extern void HnLog(const char *format, ... )
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6)
__attribute__ ((format (printf, 1, 2)))
#endif
;

_HNSRTIMP extern void HnSetLogFile(const char *path);

/*
 * Termination by fatal errors
 */

#ifdef _MSC_VER
_declspec(noreturn)
#endif
_HNSRTIMP extern void HnAbort(const char *format, ... )
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6)
__attribute__ ((format (printf, 1, 2)))
__attribute__ ((noreturn))
#endif
;
#ifdef _MSC_VER
_declspec(noreturn)
#endif
_HNSRTIMP extern void HnSysError(const char *format, ... )
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6)
__attribute__ ((format (printf, 1, 2)))
__attribute__ ((noreturn))
#endif
;

typedef void HnAbortProc(const char *errmsg);
_HNSRTIMP extern void HnSetAbortProc(HnAbortProc *proc);

#ifdef __cplusplus
};
#endif

#endif /* _HnAbort_h */
