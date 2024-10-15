#pragma once
#include "Filter.h"
class BlurFilter : public Filter
{
public:
	BlurFilter();
	virtual ~BlurFilter();

public:

	void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel);

};

