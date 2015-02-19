/*
 * HnTimesObj.cc
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
 * 02/02/99 Norio KATAYAMA
 * $Id: HnTimesObj.cc,v 1.6 2002/09/13 03:42:20 katayama Exp $
 */

#include <stdio.h>
#ifdef _MSC_VER
#include <sys/types.h>
#else
#include <unistd.h>
#endif
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

#include "HnSRTree/HnTimes.hh"
#include "HnSRTree/HnTimesObj.hh"

HnTimesObj::HnTimesObj(void)
{
    rtime.tv_sec = 0;
    rtime.tv_usec = 0;

    utime.tv_sec = 0;
    utime.tv_usec = 0;

    stime.tv_sec = 0;
    stime.tv_usec = 0;
}

void
HnTimesObj::setCurrentTimes(void)
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
	    rtime.tv_sec = 0;
	    rtime.tv_usec = 0;
	    utime.tv_sec = 0;
	    utime.tv_usec = 0;
	    stime.tv_sec = 0;
	    stime.tv_usec = 0;
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

    rtime.tv_sec = realTime / 10000000;
    rtime.tv_usec = (realTime % 10000000) / 10;

    utime.tv_sec = ((((LONGLONG)userTime.dwHighDateTime) << 32) +
		    userTime.dwLowDateTime) / 10000000;
    utime.tv_usec = (((((LONGLONG)userTime.dwHighDateTime) << 32) +
		      userTime.dwLowDateTime) % 10000000) / 10;

    stime.tv_sec = ((((LONGLONG)kernelTime.dwHighDateTime) << 32) +
		    kernelTime.dwLowDateTime) / 10000000;
    stime.tv_usec = (((((LONGLONG)kernelTime.dwHighDateTime) << 32) +
		      kernelTime.dwLowDateTime) % 10000000) / 10;
#else
    gettimeofday(&rtime, NULL);

#ifdef HAVE_GETRUSAGE
    {
	struct rusage rusage;

	if(getrusage(RUSAGE_SELF, &rusage) < 0) {
	    perror("getrusage");
	    exit(1);
	}

	utime = rusage.ru_utime;
	stime = rusage.ru_stime;
    }
#else
    {
	struct tms tms;

	if(times(&tms) == (clock_t)-1) {
	    perror("times");
	    exit(1);
	}

	utime.tv_sec = tms.tms_utime / CLK_TCK;
	utime.tv_usec = (tms.tms_utime % CLK_TCK) * 1000000 / CLK_TCK;
	stime.tv_sec = tms.tms_stime / CLK_TCK;
	stime.tv_usec = (tms.tms_stime % CLK_TCK) * 1000000 / CLK_TCK;
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

HnTimes
HnTimesObj::add(const HnTimes &times) const
{
    HnTimes newTimes = new_HnTimes();

    addTimeval(&newTimes.getObject()->rtime,
	       &rtime, &times.getObject()->rtime);
    addTimeval(&newTimes.getObject()->utime,
	       &utime, &times.getObject()->utime);
    addTimeval(&newTimes.getObject()->stime,
	       &stime, &times.getObject()->stime);

    return newTimes;
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

HnTimes
HnTimesObj::subtract(const HnTimes &times) const
{
    HnTimes newTimes = new_HnTimes();

    subtractTimeval(&newTimes.getObject()->rtime,
		    &rtime, &times.getObject()->rtime);
    subtractTimeval(&newTimes.getObject()->utime,
		    &utime, &times.getObject()->utime);
    subtractTimeval(&newTimes.getObject()->stime,
		    &stime, &times.getObject()->stime);

    return newTimes;
}

double
HnTimesObj::getRealTime(void) const
{
    return ((double)rtime.tv_sec + (double)rtime.tv_usec / 1000000);
}
    
double
HnTimesObj::getUserTime(void) const
{
    return ((double)utime.tv_sec + (double)utime.tv_usec / 1000000);
}

double
HnTimesObj::getSystemTime(void) const
{
    return ((double)stime.tv_sec + (double)stime.tv_usec / 1000000);
}

double
HnTimesObj::getCPUTime(void) const
{
    return (getUserTime() + getSystemTime());
}
