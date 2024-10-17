#include "BlurFilter.h"
#include "JournalManager.h"

#include <cmath>

BlurFilter::BlurFilter()
{
}

BlurFilter::~BlurFilter()
{
}

std::vector<std::vector<float>> BlurFilter::GetKernel()
{
	std::vector<std::vector<float>> kernel(mKernelSize, std::vector<float>(mKernelSize));

	int half = mKernelSize / 2;
	float sum = 0.f;
	for (int x = -half; x <= half; ++x)
	{
		for (int y = -half; y <= half; ++y)
		{
			float expo = -(x * x + y * y) / (2 * mKernelSigma * mKernelSigma);
			kernel[x + half][y + half] = exp(expo) / (2 * M_PI * mKernelSigma * mKernelSigma);
			sum += kernel[x + half][y + half];
		}
	}

	for (int i = 0; i < mKernelSize; i++)
	{
		for (int j = 0; j < mKernelSize; j++)
		{
			kernel[i][j] /= sum;
		}
	}

	return kernel;
}

void BlurFilter::Apply(unsigned char* pixels, int width, int height, int bytesPerPixel)
{
	mKernelSize = 5;
	mKernelSigma = 2.f;

	JournalManager::Instance->LogWrite(L"Applying Gaussian Blur filter");

	KernelBasedFilter::Apply(pixels, width, height, bytesPerPixel);
}
