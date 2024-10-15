#pragma once
#include "Filter.h"
class NegativeFilter : public Filter
{
public:
	NegativeFilter();
	virtual ~NegativeFilter();

public:

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel) override;
};

