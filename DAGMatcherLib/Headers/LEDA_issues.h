/**------------------------------------------------------------------------
 * @file
 *
 * Copyright 2001 by the University of Toronto (UT).
 *
 *                       All Rights Reserved
 *
 * Permission to copy and modify this software and its documentation
 * only for internal use in your organization is hereby granted,
 * provided that this notice is retained thereon and on all copies.
 * UT makes no representations as to the suitability and operability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * UT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
 * SHALL UT BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * No other rights, including for example, the right to redistribute
 * this software and its documentation or the right to prepare
 * derivative works, are granted unless specifically provided in a
 * separate license agreement.
 *
 * Author: Diego Macrini (dmac@cs.toronto.edu)
 *
 *-----------------------------------------------------------------------*/

#ifndef __LEDA_ISSUES_H__
#define __LEDA_ISSUES_H__

/*!
   There is a bug in LEDA 5.1 and earlier up to 4.4 (4.3 is ok). they promised to
   fix it in the next LEDA. By now, define this:
*/
//#define USE_TEMPLATE_BASED_MWBM_FUNCTION

/*!
  This file defines all the constants necessary to overcome the missing definitions in LEDA 4.3
  If LEDA_VERSION > 43 (eg 44), it means we are compiling and linking to a LEDA newer than 4.3
*/

#define LEDA_VERSION 44

#if LEDA_VERSION == 43

#define leda_list_item          list_item
#define leda_seq_item           seq_item
#define leda_pq_item            pq_item

#define LEDA_GRAPH              GRAPH

#define leda_GraphWin           GraphWin
#define leda_gw_node_shape      gw_node_shape
#define leda_roman_font         roman_font
#define leda_key_release_event  key_release_event
#define leda_destroy_event      destroy_event
#define leda_exposure_event     exposure_event

#define leda_ellipse_node       ellipse_node
#define leda_roundrect_node     roundrect_node
#define leda_rectangle_node     rectangle_node

#else

#define LEDA_GRAPH              LEDA::GRAPH

#define leda_GraphWin           LEDA::GraphWin
#define leda_gw_node_shape      LEDA::gw_node_shape
#define leda_roman_font         LEDA::roman_font
#define leda_key_release_event  LEDA::key_release_event
#define leda_destroy_event      LEDA::destroy_event
#define leda_exposure_event     LEDA::exposure_event

#define leda_ellipse_node       LEDA::ellipse_node
#define leda_roundrect_node     LEDA::roundrect_node
#define leda_rectangle_node     LEDA::rectangle_node

#endif

#endif //__LEDA_ISSUES_H__
