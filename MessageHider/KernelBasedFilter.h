#pragma once

#include "Filter.h"
#include <vector>

class KernelBasedFilter : public Filter
{
public:
	KernelBasedFilter();
	virtual ~KernelBasedFilter();

protected:

	virtual std::vector<std::vector<float>> GetKernel() = 0;

public:

	static unsigned char ClampByte(int value);

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel) override;
};

