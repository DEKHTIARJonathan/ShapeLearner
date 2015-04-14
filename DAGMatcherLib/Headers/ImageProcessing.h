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
 * Author: Diego Macrini
 *
 * $Log: ImageProcessing.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

#include <functional>
#include <tools/connected.h>
#include "field.h"
#include <CImg/CImg.h>
#include "sg.h"

namespace dml {
int RemoveNonMaxComponents(cimg_library::CImg<float>& img);
FIELD<float>* CopyImageToField(const cimg_library::CImg<float>& img);
void SmoothShape(cimg_library::CImg<float>& img, int nSmoothIter = 5);
} // namespace dml

#endif //_IMAGE_PROCESSING_H_
