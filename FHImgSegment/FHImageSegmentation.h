#ifndef _FH_IMAGE_SEGMENTATION_H_
#define _FH_IMAGE_SEGMENTATION_H_

#include <Tools/ImageUtils.h>

/*!
	@brief Wrapper for a generic image segmentation algorithm
*/
class ImageSegmentation
{
protected:
	RGBImg segImg;
	int num_ccs;
	
public:	
	virtual ~ImageSegmentation() { }
	virtual int Segment(const RGBImg inputImg) = 0;
	virtual void SetScaleParameter(const double& sig) = 0;
	virtual double GetScaleParameter() const = 0;
	
	RGBImg Regions() { return segImg; }
	int NumRegions() { return num_ccs; }
};

/*!
	@brief Wrapper for the Felzenszwalb-Huttenlocher image segmentation algorithm.

	The algorith is described in the paper "Effcient Graph-Based Image Segmentation"
	by Pedro F. Felzenszwalb and Daniel P. Huttenlocher.
*/
class FHImageSegmentation : public ImageSegmentation
{
	float sigma;  // Used to smooth the input image before segmenting it
	float k;      // Value for the threshold function
	int min_size; // Minimum component size enforced by post-processing
	
	
public:
	FHImageSegmentation();
	int Segment(const RGBImg inputImg);
	void SetScaleParameter(const double& sig) { ASSERT(sig > 0); sigma = (float) sig; }
	double GetScaleParameter() const { return sigma; }
};

#endif //_FH_IMAGE_SEGMENTATION_H_
