#include <cstdio>
#include <cstdlib>
#include "image.h"
#include "misc.h"
#include "pnmfile.h"
#include "segment-image.h"

#include "FHImageSegmentation.h"

image<rgb>* ConvertFromVXLImage(const RGBImg vxlImage)
{
	int width = vxlImage.ni();
	int height = vxlImage.nj();
	
	image<rgb>* im = new image<rgb>(width, height);
	
	/*std::cerr << "width " << im->width()
		<< " height " << im->height() << std::endl;*/
	
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) 
		{
			imRef(im, x, y).r = vxlImage(x, y).R();
			imRef(im, x, y).g = vxlImage(x, y).G();
			imRef(im, x, y).b = vxlImage(x, y).B();
		}
			
	return im;
}

void ConvertToVXLImage(image<rgb>* im, RGBImg& vxlImage)
{
	int width = im->width();
	int height = im->height();
	
	vxlImage.set_size(width, height);
	
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) 
		{
			vxlImage(x, y).r = imRef(im, x, y).r;
			vxlImage(x, y).g = imRef(im, x, y).g;
			vxlImage(x, y).b = imRef(im, x, y).b;
		}
}

FHImageSegmentation::FHImageSegmentation()
{
	sigma = 0.5;
	k = 500;
	min_size = 20;
	num_ccs = 0;
}

int FHImageSegmentation::Segment(const RGBImg inputImg) 
{
	image<rgb>* input = ConvertFromVXLImage(inputImg);
	
	//ShowStatus("Processing...");
	
	num_ccs = 0;
	image<rgb>* seg = segment_image(input, sigma, k, min_size, &num_ccs); 
	
	ConvertToVXLImage(seg, segImg);
	
	delete input;
	delete seg;
	
	//ShowStatus("Done!");
	
	return num_ccs;
}

