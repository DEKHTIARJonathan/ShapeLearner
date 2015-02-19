/*
 * HnTimesSt.c
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
 * 07/27/1999 Norio KATAYAMA
 * $Id: HnTimesSt.c,v 1.4 2002/09/13 03:42:45 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/config.h"

#ifdef _MSC_VER
#include <windows.h>
#else
#ifdef HAVE_GETRUSAGE
#include <sys/resource.h>
#else
#include <sys/times.h>
#include <limits.h>
#endif
#endif

#include "HnSRTree/HnTimesSt.h"

HnTimesSt *
HnTimesSt_allocate(void)
{
    HnTimesSt *tp;

    tp = (HnTimesSt *)HnMalloc(sizeof(HnTimesSt));

    tp->rtime.tv_sec = 0;
    tp->rtime.tv_usec = 0;

    tp->utime.tv_sec = 0;
    tp->utime.tv_usec = 0;

    tp->stime.tv_sec = 0;
    tp->stime.tv_usec = 0;

    return tp;
}

void
HnTimesSt_free(HnTimesSt *tp)
{
    HnFree(tp, sizeof(HnTimesSt));
}

double
HnTimesSt_getRealTime(const HnTimesSt *tp)
{
    return ((double)tp->rtime.tv_sec + (double)tp->rtime.tv_usec / 1000000);
}

double
HnTimesSt_getUserTime(const HnTimesSt *tp)
{
    return ((double)tp->utime.tv_sec + (double)tp->utime.tv_usec / 1000000);
}

double
HnTimesSt_getSystemTime(const HnTimesSt *tp)
{
    return ((double)tp->stime.tv_sec + (double)tp->stime.tv_usec / 1000000);
}

double
HnTimesSt_getCPUTime(const HnTimesSt *tp)
{
    return (HnTimesSt_getUserTime(tp) + HnTimesSt_getSystemTime(tp));
}

void
HnTimesSt_setCurrentTimes(HnTimesSt *tp)
{
#ifdef _MSC_VER
    FILETIME creationTime, exitTime, kernelTime, userTime;
    SYSTEMTIME currentSystemTime;
    FILETIME currentTime;
    LONGLONG realTime;

    if ( GetProcessTimes(GetCurrentProcess(),
			 &creationTime, &exitTime,
			 &kernelTime, &userTime) == 0 ) {
	LPVOID lpMsgBuf;

	if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED ) {
	    /* Windows 95/98 does not implement GetProcessTimes() */
	    tp->rtime.tv_sec = 0;
	    tp->rtime.tv_usec = 0;
	    tp->utime.tv_sec = 0;
	    tp->utime.tv_usec = 0;
	    tp->stime.tv_sec = 0;
	    tp->stime.tv_usec = 0;
	    return;
	}

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		      FORMAT_MESSAGE_FROM_SYSTEM |
		      FORMAT_MESSAGE_IGNORE_INSERTS,
		      NULL, GetLastError(),
		      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		      (LPTSTR)&lpMsgBuf, 0, NULL);
	fprintf(stderr, "Error: %s: %s\n", "GetProcessTimes", (LPSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	exit(1);
    }

    GetSystemTime(&currentSystemTime);
    
    if ( SystemTimeToFileTime(&currentSystemTime, &currentTime) == 0 ) {
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		      FORMAT_MESSAGE_FROM_SYSTEM |
		      FORMAT_MESSAGE_IGNORE_INSERTS,
		      NULL, GetLastError(),
		      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		      (LPTSTR)&lpMsgBuf, 0, NULL);
	fprintf(stderr, "Error: %s: %s\n",
		"SystemTimeToFileTime", (LPSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	exit(1);
    }

    realTime = (((((LONGLONG)currentTime.dwHighDateTime) << 32) +
		 currentTime.dwLowDateTime) -
		((((LONGLONG)creationTime.dwHighDateTime) << 32) +
		 creationTime.dwLowDateTime));

    tp->rtime.tv_sec = realTime / 10000000;
    tp->rtime.tv_usec = (realTime % 10000000) / 10;

    tp->utime.tv_sec = ((((LONGLONG)userTime.dwHighDateTime) << 32) +
			userTime.dwLowDateTime) / 10000000;
    tp->utime.tv_usec = (((((LONGLONG)userTime.dwHighDateTime) << 32) +
			  userTime.dwLowDateTime) % 10000000) / 10;

    tp->stime.tv_sec = ((((LONGLONG)kernelTime.dwHighDateTime) << 32) +
			kernelTime.dwLowDateTime) / 10000000;
    tp->stime.tv_usec = (((((LONGLONG)kernelTime.dwHighDateTime) << 32) +
			  kernelTime.dwLowDateTime) % 10000000) / 10;
#else
    gettimeofday(&tp->rtime, NULL);

#ifdef HAVE_GETRUSAGE
    {
	struct rusage rusage;

	if(getrusage(RUSAGE_SELF, &rusage) < 0) {
	    perror("getrusage");
	    exit(1);
	}

	tp->utime = rusage.ru_utime;
	tp->stime = rusage.ru_stime;
    }
#else
    {
	struct tms tms;

	if(times(&tms) == -1) {
	    perror("times");
	    exit(1);
	}

	tp->utime.tv_sec = tms.tms_utime / CLK_TCK;
	tp->utime.tv_usec = (tms.tms_utime % CLK_TCK) * 1000000 / CLK_TCK;
	tp->stime.tv_sec = tms.tms_stime / CLK_TCK;
	tp->stime.tv_usec = (tms.tms_stime % CLK_TCK) * 1000000 / CLK_TCK;
    }
#endif
#endif
}

static void
addTimeval(struct timeval *tp3,
	   const struct timeval *tp1, const struct timeval *tp2)
{
    tp3->tv_sec  = tp1->tv_sec + tp2->tv_sec;
    tp3->tv_usec = tp1->tv_usec + tp2->tv_usec;

    if (tp3->tv_usec >= 1000000) {
	tp3->tv_sec ++;
	tp3->tv_usec -= 1000000;
    }
}

void
HnTimesSt_add(HnTimesSt *tp3,
	      const HnTimesSt *tp1, const HnTimesSt *tp2)
{
    addTimeval(&tp3->rtime, &tp1->rtime, &tp2->rtime);
    addTimeval(&tp3->utime, &tp1->utime, &tp2->utime);
    addTimeval(&tp3->stime, &tp1->stime, &tp2->stime);
}

static void
subtractTimeval(struct timeval *tp3,
		const struct timeval *tp1, const struct timeval *tp2)
{
    tp3->tv_sec  = tp1->tv_sec - tp2->tv_sec;

    if (tp1->tv_usec >= tp2->tv_usec) {
	tp3->tv_usec = tp1->tv_usec - tp2->tv_usec;
    }
    else {
	tp3->tv_sec --;
	tp3->tv_usec = 1000000 + tp1->tv_usec - tp2->tv_usec;
    }
}

void
HnTimesSt_subtract(HnTimesSt *tp3,
		   const HnTimesSt *tp1, const HnTimesSt *tp2)
{
    subtractTimeval(&tp3->rtime, &tp1->rtime, &tp2->rtime);
    subtractTimeval(&tp3->utime, &tp1->utime, &tp2->utime);
    subtractTimeval(&tp3->stime, &tp1->stime, &tp2->stime);
}

void
HnTimesSt_print(const HnTimesSt *tp)
{
    printf("HnTimesSt[\n");
    printf("    realTime  : %g\n", HnTimesSt_getRealTime(tp));
    printf("    cpuTime   : %g\n", HnTimesSt_getCPUTime(tp));
    printf("    userTime  : %g\n", HnTimesSt_getUserTime(tp));
    printf("    systemTime: %g\n", HnTimesSt_getSystemTime(tp));
    printf("]\n");
}

#define HnClass HnTimes
#include "HnSRTree/HnClassArraySt.c"
