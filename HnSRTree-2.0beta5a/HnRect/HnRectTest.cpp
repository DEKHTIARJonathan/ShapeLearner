/*
 * HnRectTest.cc
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
 * 08/27/96 katayama@rd.nacsis.ac.jp
 * $Id: HnRectTest.cc,v 1.6 2000/06/10 11:19:21 katayama Exp $
 */

#include "HnSRTree/HnString.hh"
#include "HnSRTree/HnRect.hh"

static HnRect
new_HnRect(double x1, double x2, double y1, double y2)
{
    HnRect rect = new_HnRect(2);

    rect.setRangeAt(x1, x2, 0);
    rect.setRangeAt(y1, y2, 1);

    return rect;
}

static void
intersect(const HnRect &rect1, const HnRect &rect2)
{
    HnRect boundingRect = rect1.getBoundingRect(rect2);
    HnRect overlappingRect = rect1.getOverlappingRect(rect2);

    printf("rect1: %s\n", (char *)rect1.toString());
    printf("rect2: %s\n", (char *)rect2.toString());
    printf("bounding rect   : %s\n", (char *)boundingRect.toString());
    printf("overlapping rect: %s\n", (char *)overlappingRect.toString());
    printf("\n");
}

static void
testRectangleIntersection()
{
    HnRect rect1414, rect0214, rect2314, rect2514, rect0114;
    HnRect rect0514, rect4514;
    HnRect rect0303;

    rect1414 = new_HnRect(1, 4, 1, 4);
    rect0214 = new_HnRect(0, 2, 1, 4);
    rect2314 = new_HnRect(2, 3, 1, 4);
    rect2514 = new_HnRect(2, 5, 1, 4);
    rect0114 = new_HnRect(0, 1, 1, 4);
    rect0514 = new_HnRect(0, 5, 1, 4);
    rect4514 = new_HnRect(4, 5, 1, 4);
    rect0303 = new_HnRect(0, 3, 0, 3);

    printf("intersect(rect1414, rect0214)\n");
    intersect(rect1414, rect0214);

    printf("intersect(rect1414, rect2314)\n");
    intersect(rect1414, rect2314);

    printf("intersect(rect1414, rect2514)\n");
    intersect(rect1414, rect2514);

    printf("intersect(rect1414, rect0114)\n");
    intersect(rect1414, rect0114);

    printf("intersect(rect1414, rect0514)\n");
    intersect(rect1414, rect0514);

    printf("intersect(rect1414, rect4514)\n");
    intersect(rect1414, rect4514);

    printf("intersect(rect1414, rect0303)\n");
    intersect(rect1414, rect0303);
}

int
main(void)
{
    testRectangleIntersection();

    return 0;
}

