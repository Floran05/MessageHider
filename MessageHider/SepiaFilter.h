#pragma once
#include "Filter.h"
class SepiaFilter : public Filter
{
public:
	SepiaFilter();
	virtual ~SepiaFilter();

public:

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel);

};

