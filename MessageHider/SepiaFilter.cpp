#include "SepiaFilter.h"
#include "JournalManager.h"

SepiaFilter::SepiaFilter()
{
}

SepiaFilter::~SepiaFilter()
{
}

void SepiaFilter::Apply(unsigned char* pixels, int width, int height, int bytesPerPixel)
{
	JournalManager::Instance->LogWrite(L"Applying Sepia filter");

	const int size = width * height;
	int targetPixel = 0;
	for (int i = 0; i < size; ++i)
	{
		targetPixel = i * bytesPerPixel;
		pixels[targetPixel]		= static_cast<unsigned char>(0.393f * pixels[targetPixel] + 0.769f * pixels[targetPixel + 1] + 0.189f * pixels[targetPixel + 2]);
		pixels[targetPixel + 1] = static_cast<unsigned char>(0.349f * pixels[targetPixel] + 0.686f * pixels[targetPixel + 1] + 0.168f * pixels[targetPixel + 2]);
		pixels[targetPixel + 2] = static_cast<unsigned char>(0.272f * pixels[targetPixel] + 0.534f * pixels[targetPixel + 1] + 0.131f * pixels[targetPixel + 2]);
	}
}
