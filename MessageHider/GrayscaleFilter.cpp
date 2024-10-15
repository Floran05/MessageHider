#include "GrayscaleFilter.h"

GrayscaleFilter::GrayscaleFilter()
{
}

GrayscaleFilter::~GrayscaleFilter()
{
}

void GrayscaleFilter::Apply(unsigned char* pixels, int width, int height, int bytesPerPixel)
{
	const int size = width * height;
	int targetPixel = 0;
	for (int i = 0; i < size; ++i)
	{
		targetPixel = i * bytesPerPixel;
		unsigned int grey = 0.299f * pixels[targetPixel] + 0.587f * pixels[targetPixel + 1] * 0.114f * pixels[targetPixel + 2];
		pixels[targetPixel] = grey;
		pixels[targetPixel + 1] = grey;
		pixels[targetPixel + 2] = grey;
	}
}
