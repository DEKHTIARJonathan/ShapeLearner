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
 * $Log: ImageProcessing.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

#include "ImageProcessing.h"

using namespace dml;

struct MyComparator
{
	bool operator()(const float& a, const float& b) { return a == b; }
};

typedef std::vector<int> IntVector;

/*!
	@brief Removes non-maximum components

	Finds the largest white component and the largest black component. It removes the
	remaining components. Note: if there are "rings" in the image that must be removed, then
	this function must be called multiple times until it returns 2.

	It uses the connected components funtion from http://people.csail.mit.edu/rahimi/connected/

	@return the orginal number of components in the image. If greater than 2, it means that
	some components were removed.
*/
int dml::RemoveNonMaxComponents(cimg_library::CImg<float>& img)
{
	cimg_library::CImg<int> pixLbl(img.dimx(), img.dimy());
	ConnectedComponents cc(10); // this number is just an initial size that can grow if necessary

	// find components. this is a template function whose params' types get defined implicitly
/*#ifndef WIN32
	int n = cc.connected(img.ptr(), pixLbl.ptr(), img.dimx(), img.dimy(),
		 std::equal_to<float>(),
		 constant<bool,true>());
#else*/
	int n = cc.connected(img.ptr(), pixLbl.ptr(), img.dimx(), img.dimy(),
		 std::equal_to<float>(),
		 true);
//#endif

	//pixLbl.save_ascii("labels.raw");

	if (n > 2)
	{
		IntVector whiteLbls(n, 0), blackLbls(n, 0);
		int i, maxWhiteLbl = 0, maxBlackLbl = 0, maxWhiteSz = 0, maxBlackSz = 0;

		// count number of black and white labels
		cimg_mapXY(img,x,y)
		{
			if (img(x,y) == 0)
				blackLbls[pixLbl(x,y)]++;
			else
				whiteLbls[pixLbl(x,y)]++;
		}

		// find the largest white group and the largest black group
		for (i = 0; i < n; i++)
		{
			if (whiteLbls[i] > maxWhiteSz) {
				maxWhiteSz = whiteLbls[i];
				maxWhiteLbl = i;
			}

			if (blackLbls[i] > maxBlackSz) {
				maxBlackSz = blackLbls[i];
				maxBlackLbl = i;
			}
		}

		// remove non-maximum components
		cimg_mapXY(img,xx,yy)
		{
			i = pixLbl(xx,yy);

			if (i != maxWhiteLbl && i != maxBlackLbl)
				img(xx,yy) = (img(xx,yy) == 0) ? 255:0;
		}
	}

	return n;
}

/*!
	@brief Returns a pointer to a FIELD<float> object with a copy of img.

	The returned pointer must be deleted by the last user of the FIELD.
*/
FIELD<float>* dml::CopyImageToField(const cimg_library::CImg<float>& img)
{
	FIELD<float>* pField = new FIELD<float>(img.dimx(), img.dimy());
	float* const p = pField->data();

	for (int y = 0; y < img.dimy(); y++)
		for (int x = 0; x < img.dimx(); x++)
			*(p + y * img.dimx() + x) = img(x, y);

	return pField;
}

// From the internet (link in Wikipedia)
bool IsInsidePoly(int npol, double* xp, double* yp, const double& x, const double& y)
{
	int i, j;
	bool c = false;

	for (i = 0, j = npol-1; i < npol; j = i++) {
		if ((((yp[i]<=y) && (y<yp[j])) ||
			((yp[j]<=y) && (y<yp[i]))) &&
			(x < (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))

			c = !c;
	}

	return c;
}

/*!
	@brief Smooths a shape by averaging over boundary points. This function can also be
	used to upscale a shape while smoothing its boundary, which is useful for low
	resolution shapes.
*/
void dml::SmoothShape(cimg_library::CImg<float>& img, int nSmoothIter /*=5*/)
{
	const int nDimY = img.dimy(), nDimX = img.dimx();
	int x,y;
	sg::SimpleShapeMaker ssm(nDimX, nDimY, 0, false);

	for (y=0; y < nDimY; y++)
		for (x=0; x < nDimX; x++) {
			ssm(x,y) = img(x,y) == 0;
			//std::cout << pField->value(x,y)<<','<< std::flush;
		}

	// Make the "smooth" shape contour
	sg::SimpleShape* ss = (sg::SimpleShape*)ssm.getShape();

	// Create a LEDA polygone with the contour points
	sg::DiscreteSegCurve* c = dynamic_cast<sg::DiscreteSegCurve*>(ss->getCurves()->front());
	const int N = c->segments.size();

	double* xCoords = new double[N];
	double* yCoords = new double[N];

	for (int i = 0; i < N; i++)
	{
		const sg::Point& pt = static_cast<sg::LineSeg*>(c->segments[i])->startPt;
		xCoords[i] = pt.x;
		yCoords[i] = pt.y;
	}

	//DBG_PRINT5(N, xCoords[0], yCoords[0], xCoords[N-1], yCoords[N-1])

	delete ss; // done with the shape

	// Copy the smooth shape back to into the image
	for (y=0; y < nDimY; y++)
		for (x=0; x < nDimX; x++)
			img(x,y) = (IsInsidePoly(N, xCoords, yCoords, x, y)) ? 0:255;

	delete[] xCoords;
	delete[] yCoords;
}