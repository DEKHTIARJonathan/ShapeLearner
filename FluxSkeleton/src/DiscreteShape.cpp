/************************************************************************

   File:		DiscreteShape.cpp

   Author(s):		Pavel Dimitrov

   Created:		19 Jun 2002

   Last Revision:	$Date: 2002/06/23 05:42:08 $

   Description:	

   $Revision: 1.1.1.1 $

   $Log: DiscreteShape.cpp,v $
   Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
   Initial import


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 ***********************************************************************/

#define DISCRETESHAPE_CPP

/**********************
     Include Files
 **********************/

/**********************
    Private Defines
 **********************/

/**********************
     Private Types
 **********************/

/**********************
    Private Variables
 **********************/

/**********************
    Private Functions
 **********************/

#include "DiscreteShape.h"

using namespace sg;


/**** This next function was taken from the graphics library Allegro *******/

/* do_line:
 *  Calculates all the points along a line between x1, y1 and x2, y2.
 */
   /* worker macro */
#define DO_LINE(pri_sign, pri_c, pri_cond, sec_sign, sec_c, sec_cond)     \
   {                                                                         \
      if (d##pri_c == 0) {                                              \
	 POINT_AT(x1, y1) = v;                                               \
	 return;                                                             \
      }                                                                      \
									     \
      i1 = 2 * d##sec_c;                                                     \
      dd = i1 - (sec_sign (pri_sign d##pri_c));                              \
      i2 = dd - (sec_sign (pri_sign d##pri_c));                              \
									     \
      x = x1;                                                                \
      y = y1;                                                                \
									     \
      while (pri_c pri_cond pri_c##2) {                                      \
	 POINT_AT(x, y) = v;                                                 \
									     \
	 if (dd sec_cond 0) {                                                \
	    sec_c sec_sign##= 1;                                             \
	    dd += i2;                                                        \
	 }                                                                   \
	 else                                                                \
	    dd += i1;                                                        \
									     \
	 pri_c pri_sign##= 1;                                                \
      }                                                                      \
   }

void DiscreteShape::do_line(int x1, int y1, int x2, int y2, char v)
{
   int dx = x2-x1;
   int dy = y2-y1;
   int i1, i2;
   int x, y;
   int dd;


   if (dx >= 0) {
      if (dy >= 0) {
	 if (dx >= dy) {
	    /* (x1 <= x2) && (y1 <= y2) && (dx >= dy) */
	    DO_LINE(+, x, <=, +, y, >=);
	 }
	 else {
	    /* (x1 <= x2) && (y1 <= y2) && (dx < dy) */
	    DO_LINE(+, y, <=, +, x, >=);
	 }
      }
      else {
	 if (dx >= -dy) {
	    /* (x1 <= x2) && (y1 > y2) && (dx >= dy) */
	    DO_LINE(+, x, <=, -, y, <=);
	 }
	 else {
	    /* (x1 <= x2) && (y1 > y2) && (dx < dy) */
	    DO_LINE(-, y, >=, +, x, >=);
	 }
      }
   }
   else {
      if (dy >= 0) {
	 if (-dx >= dy) {
	    /* (x1 > x2) && (y1 <= y2) && (dx >= dy) */
	    DO_LINE(-, x, >=, +, y, >=);
	 }
	 else {
	    /* (x1 > x2) && (y1 <= y2) && (dx < dy) */
	    DO_LINE(+, y, <=, -, x, <=);
	 }
      }
      else {
	 if (-dx >= -dy) {
	    /* (x1 > x2) && (y1 > y2) && (dx >= dy) */
	    DO_LINE(-, x, >=, -, y, <=);
	 }
	 else {
	    /* (x1 > x2) && (y1 > y2) && (dx < dy) */
	    DO_LINE(-, y, >=, -, x, <=);
	 }
      }
   }
}


void DiscreteShape::flood_fill(int x, int y){ // starts at 0,0 in the array coords
  

  if (x<0 || y<0 || x >=xSize || y>= ySize)
    return;

      
  if (POINT_AT(x,y) == INSIDE_COL)
    POINT_AT(x,y) = BACK_COL;


  if (POINT_AT(x+1,y) != CONTOUR_COL &&
      POINT_AT(x+1,y) != BACK_COL)
    flood_fill(x+1,y);
  if (POINT_AT(x-1,y) != CONTOUR_COL &&
      POINT_AT(x-1,y) != BACK_COL)
    flood_fill(x-1,y);

  if (POINT_AT(x,y+1) != CONTOUR_COL &&
      POINT_AT(x,y+1) != BACK_COL)
    flood_fill(x,y+1);
  if (POINT_AT(x,y-1) != CONTOUR_COL &&
      POINT_AT(x,y-1) != BACK_COL)
    flood_fill(x,y-1);
}
