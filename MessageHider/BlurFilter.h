#pragma once

#include "Filter.h"

#include <vector>

class BlurFilter : public Filter
{
public:
	BlurFilter();
	virtual ~BlurFilter();

public:

	std::vector<float> createKernel(float sigma);

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel);

};

