/*
 * HnAbort.c
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
 * $Id: HnAbort.c,v 1.3 2002/09/10 11:35:08 katayama Exp $
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "HnSRTree/config.h"
#include "HnSRTree/HnAbort.h"

#define BufferSize	4096

/************************************************************************
 *
 * Utility function getting the system error message string
 *
 ************************************************************************/

#ifndef WIN32
#ifndef HAVE_STRERROR
#define strerror        mystrerror

static const char *
strerror(int errnum) {
    extern int sys_nerr;
//#ifdef _MSC_VER
//    extern char * sys_errlist[];
//#else
    extern const char * const sys_errlist[];
//#endif

    if ( errnum >= 0 && errnum < sys_nerr ) {
		return sys_errlist[errnum];
    }
    else {
		return NULL;
    }
}
#endif
#endif //no WIN32
/*
 * HnGetSystemErrorMessage()
 */

char *
HnGetSystemErrorMessage(void)
{
    char *errmsg;
    static char buffer[BufferSize];

    if ( (errmsg = (char*)strerror(errno)) == NULL ) {
	sprintf(buffer, "errno=%d", errno);
	return buffer;
    }

    return errmsg;
}

/************************************************************************
 *
 * Error message
 *
 ************************************************************************/

static char *HnErrorMessage = NULL;

void
HnSetErrorMessage(const char *format, ... )
{
    va_list ap;

    if ( HnErrorMessage == NULL ) {
	if ( (HnErrorMessage = (char *)malloc(BufferSize)) == NULL ) {
	    HnSysError("malloc");
	}
    }

    va_start(ap, format);
    vsprintf(HnErrorMessage, format, ap);
    va_end(ap);
}

char *
HnGetErrorMessage(void)
{
    return HnErrorMessage;
}

void
HnClearErrorMessage(void)
{
    if ( HnErrorMessage != NULL ) {
	free(HnErrorMessage);
	HnErrorMessage = NULL;
    }
}

/************************************************************************
 *
 * Logging
 *
 ************************************************************************/

static FILE *logfp = NULL;

void
HnSetLogFile(const char *path)
{
    FILE *fp;

    if ( path == NULL ) {
	if ( logfp != NULL ) {
	    fclose(logfp);
	}

	logfp = NULL;
    }
    else {
	if ( (fp = fopen(path, "a")) == NULL ) {
	    HnSysError("fopen(\"%s\", \"a\")", path);
	}

	if ( logfp != NULL ) {
	    fclose(logfp);
	}

	logfp = fp;
	setvbuf(logfp, NULL, _IOLBF, 0);
    }
}
		
void
HnLog(const char *format, ... )
{
    va_list ap;
    time_t t;
    static char when[256], buffer[1024];

    t = time(NULL);
    strftime(when, sizeof(when), "%D %H:%M:%S", localtime(&t));

    va_start(ap, format);
    vsprintf(buffer, format, ap);
    va_end(ap);

    if ( logfp != NULL ) {
	fprintf(logfp, "(%s) %s\n", when, buffer);
    }
    else {
	fprintf(stderr, "(%s) %s\n", when, buffer);
    }
}

/************************************************************************
 *
 * Termination by fatal errors
 *
 ************************************************************************/

static HnAbortProc *abortProc = NULL;

void
HnSetAbortProc(HnAbortProc *proc)
{
    abortProc = proc;
}

static void callAbortProc(const char *errmsg)
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6)
__attribute__ ((noreturn))
#endif
;

static void
callAbortProc(const char *errmsg)
{
    if ( logfp != NULL ) {
	fprintf(logfp, "%s\n", errmsg);
    }

    if ( abortProc != NULL ) {
	abortProc(errmsg);

	fprintf(stderr, "Error: assigned ``AbortProc'' did not terminated.\n");
	abort();
    }
    else {
	fprintf(stderr, "%s\n", errmsg);
	abort();
    }
}

/*
 * HnAbort()
 */

void
HnAbort(const char *format, ... )
{
    va_list ap;
    char buffer[BufferSize], *bp;

    bp = buffer;
    sprintf(bp, "Error: ");
    bp += strlen(bp);

    va_start(ap, format);
    vsprintf(bp, format, ap);
    va_end(ap);
    bp += strlen(bp);

    callAbortProc(buffer);
}

void
HnSysError(const char *format, ... )
{
    va_list ap;
    char buffer[BufferSize], *bp;

    bp = buffer;
    sprintf(bp, "Error: ");
    bp += strlen(bp);

    va_start(ap, format);
    vsprintf(bp, format, ap);
    va_end(ap);
    bp += strlen(bp);

    sprintf(bp, ": %s.", HnGetSystemErrorMessage());

    callAbortProc(buffer);
}

