#include "NegativeFilter.h"
#include "JournalManager.h"

NegativeFilter::NegativeFilter()
{
}

NegativeFilter::~NegativeFilter()
{
}

void NegativeFilter::Apply(unsigned char* pixels, int width, int height, int bytesPerPixel)
{
	JournalManager::Instance->LogWrite(L"Applying Negative filter");

	const int size = width * height;
	int targetPixel = 0;
	for (int i = 0; i < size; ++i)
	{
		targetPixel = i * bytesPerPixel;
		pixels[targetPixel] = 255 - pixels[targetPixel];
		pixels[targetPixel + 1] = 255 - pixels[targetPixel + 1];
		pixels[targetPixel + 2] = 255 - pixels[targetPixel + 2];
	}
}
