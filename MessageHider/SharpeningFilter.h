#pragma once

#include "KernelBasedFilter.h"

class SharpeningFilter : public KernelBasedFilter
{
public:
	SharpeningFilter();
	virtual ~SharpeningFilter();

protected:

	std::vector<std::vector<float>> GetKernel() override;

public:

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel) override;

};

