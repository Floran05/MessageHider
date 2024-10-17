#include "EdgeDetectionFilter.h"
#include "JournalManager.h"

EdgeDetectionFilter::EdgeDetectionFilter()
{
}

EdgeDetectionFilter::~EdgeDetectionFilter()
{
}

std::vector<std::vector<float>> EdgeDetectionFilter::GetKernel()
{
	return std::vector<std::vector<float>>({
		{ -1, 0, 1 },
		{ -2, 0, 2 },
		{ -1, 0, 1 }
	});
}

void EdgeDetectionFilter::Apply(unsigned char* pixels, int width, int height, int bytesPerPixel)
{
	JournalManager::Instance->LogWrite(L"Applying Edge Detection filter");
	KernelBasedFilter::Apply(pixels, width, height, bytesPerPixel);
}
