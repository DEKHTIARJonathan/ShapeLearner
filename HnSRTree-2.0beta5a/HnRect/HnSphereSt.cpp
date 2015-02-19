/*
 * HnSphereSt.c
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
 * 09/12/2002 Norio KATAYAMA
 * $Id: HnSphereSt.c,v 1.2 2002/09/13 18:34:14 katayama Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "HnSRTree/HnMalloc.h"
#include "HnSRTree/HnSphereSt.h"

HnSphereSt *
HnSphereSt_allocate(int dimension)
{
    HnSphereSt *sphere;

    sphere = (HnSphereSt *)HnMalloc(sizeof(HnSphereSt));

    sphere->center = HnPointSt_allocate(dimension);
    sphere->radius = 0;

    return sphere;
}

void
HnSphereSt_free(HnSphereSt *sphere)
{
    HnPointSt_free(sphere->center);

    sphere->center = NULL;
    sphere->radius = 0;

    HnFree(sphere, sizeof(HnSphereSt));
}
    
#define HnClass	HnSphere
#include "HnSRTree/HnClassArraySt.c"
