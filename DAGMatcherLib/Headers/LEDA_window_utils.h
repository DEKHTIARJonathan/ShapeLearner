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
 * $Log: LEDA_window_utils.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __LEDA_WINDOW_UTILS_H__
#define __LEDA_WINDOW_UTILS_H__

#include "stdafx.h"

//#include <LEDA/graphics/window.h>

//! Array of RGB colors
struct ColorArray
{
	struct RGBColor { int red, green, blue; };
	std::vector<RGBColor> colors;

	void Add(int r, int g, int b)
	{
		RGBColor col; col.red = r; col.green = g; col.blue = b;
		colors.push_back(col);
	}

	unsigned int Size() const { return colors.size(); }

	leda::color operator[](unsigned int i) const
	{
		return leda::color(colors[i].red, colors[i].green, colors[i].blue);
	}

	void operator=(const ColorArray& rhs)
	{
		colors = rhs.colors;
	}
};

#endif //__LEDA_WINDOW_UTILS_H__
