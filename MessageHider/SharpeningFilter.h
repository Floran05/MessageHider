#pragma once

#include "Filter.h"

class SharpeningFilter : public Filter
{
public:
	SharpeningFilter();
	virtual ~SharpeningFilter();

public:

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel) override;
};

