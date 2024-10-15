#pragma once
class Filter
{

public:

	Filter();
	virtual ~Filter();

public:

	virtual void Apply(unsigned char* pixels, int width, int height, int bytesPerPixel) = 0;

};

