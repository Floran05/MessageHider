#include "BlurFilter.h"

BlurFilter::BlurFilter()
{
}

BlurFilter::~BlurFilter()
{
}

std::vector<float> BlurFilter::createKernel(float sigma)
{
	return std::vector<float>();
}

void BlurFilter::Apply(unsigned char* pixels, int width, int height, int bytesPerPixel)
{
}
