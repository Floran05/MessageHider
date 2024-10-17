#include "SharpeningFilter.h"
#include "JournalManager.h"

SharpeningFilter::SharpeningFilter()
{
}

SharpeningFilter::~SharpeningFilter()
{
}

std::vector<std::vector<float>> SharpeningFilter::GetKernel()
{
	return std::vector<std::vector<float>>({
		{ 0, -1, 0 },
		{ -1, 5, -1 },
		{ 0, -1, 0 }
	});
}

void SharpeningFilter::Apply(unsigned char* pixels, int width, int height, int bytesPerPixel)
{
	JournalManager::Instance->LogWrite(L"Applying Sharpening Filter");
}
