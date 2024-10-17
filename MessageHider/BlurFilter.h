#pragma once

#include "KernelBasedFilter.h"

#include <vector>

#define M_PI 3.1415926

class BlurFilter : public KernelBasedFilter
{
public:
	
	BlurFilter();
	virtual ~BlurFilter();

protected:

	int mKernelSize;
	float mKernelSigma;

	std::vector<std::vector<float>> GetKernel() override;

public:

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel);

};

