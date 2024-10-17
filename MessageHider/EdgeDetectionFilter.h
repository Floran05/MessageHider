#pragma once

#include "KernelBasedFilter.h"

class EdgeDetectionFilter : public KernelBasedFilter
{
public:

	EdgeDetectionFilter();
	virtual ~EdgeDetectionFilter();

protected:

	std::vector<std::vector<float>> GetKernel() override;

public:

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel) override;

};

