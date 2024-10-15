#pragma once
#include "Filter.h"
class GrayscaleFilter : public Filter
{
public:
	GrayscaleFilter();
	virtual ~GrayscaleFilter();

public:

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel);
};

