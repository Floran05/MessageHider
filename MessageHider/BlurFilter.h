#pragma once

#include "Filter.h"

#include <vector>

#define M_PI 3.1415926

class BlurFilter : public Filter
{
public:
	BlurFilter();
	virtual ~BlurFilter();

public:

	std::vector<std::vector<float>> CreateKernel(const int size, const float sigma);
	unsigned char ClampByte(int value);

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel);

};

