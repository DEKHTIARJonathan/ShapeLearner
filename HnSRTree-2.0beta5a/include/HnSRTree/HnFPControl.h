/*
 * HnFPControl.h
 * Copyright (C) 2000 Norio Katayama
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
 * $Id: HnFPControl.h,v 1.3 2002/09/10 11:51:24 katayama Exp $
 */

#ifndef _HnFPControl_h
#define _HnFPControl_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
/*
 * Windows
 */

#include <float.h>

#define HnFPControl_ROUND_NEAREST	_RC_NEAR
#define HnFPControl_ROUND_ZERO		_RC_CHOP
#define HnFPControl_ROUND_PLUS		_RC_UP
#define HnFPControl_ROUND_MINUS		_RC_DOWN

inline int HnFPControl_setRound(int flag) {
    unsigned int cw_old;

    cw_old = (_controlfp(0, 0) & _MCW_RC);

    _controlfp(flag, _MCW_RC);

    return cw_old;
}
inline int HnFPControl_getRound(void) {
    return (_controlfp(0, 0) & _MCW_RC);
}

#else
#ifdef HAVE_FPU_CONTROL_H

/*
 * Linux for x86
 *
 * NOTE:
 *
 *    Dec.13,2000 Norio KATAYAMA
 *
 *    The module using HnFPControl_setRound() should be compiled with
 *    the option `-ffloat-store'.  Otherwise, the optimizer might keep
 *    a computation result in a register of the FPU.  Then, the result
 *    will not be rounded until it is written back into memory.
 */

#include <fpu_control.h>

#define HnFPControl_ROUND_NEAREST	_FPU_RC_NEAREST
#define HnFPControl_ROUND_ZERO		_FPU_RC_ZERO
#define HnFPControl_ROUND_PLUS		_FPU_RC_UP
#define HnFPControl_ROUND_MINUS		_FPU_RC_DOWN

#define HnFPControl_ROUND_MASK	\
(_FPU_RC_NEAREST | _FPU_RC_ZERO | _FPU_RC_UP | _FPU_RC_DOWN)

inline int HnFPControl_setRound(int flag) {
    fpu_control_t cw_old, cw_new;

    _FPU_GETCW(cw_old);

    cw_new = (cw_old & ~HnFPControl_ROUND_MASK) | flag;

    _FPU_SETCW(cw_new);

    return (cw_old & HnFPControl_ROUND_MASK);
}
inline int HnFPControl_getRound(void) {
    fpu_control_t cw;

    _FPU_GETCW(cw);

    return (cw & HnFPControl_ROUND_MASK);
}

#else
/*
 * Solaris, IRIX, FreeBSD, etc.
 */

#ifdef __FreeBSD__
#define fp_rnd_t fp_rnd
#endif

#include <ieeefp.h>

#define HnFPControl_ROUND_NEAREST	FP_RN
#define HnFPControl_ROUND_ZERO		FP_RZ
#define HnFPControl_ROUND_PLUS		FP_RP
#define HnFPControl_ROUND_MINUS		FP_RM

inline int HnFPControl_setRound(int flag) {
    return fpsetround((fp_rnd)flag);
}
inline int HnFPControl_getRound(void) {
    return fpgetround();
}
#endif
#endif

#ifdef __cplusplus
};
#endif

#endif /* _HnFPControl_h */
