#pragma once
#include "Filter.h"
class EdgeDetectionFilter : public Filter
{
public:

	EdgeDetectionFilter();
	virtual ~EdgeDetectionFilter();

public:

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel);
};

