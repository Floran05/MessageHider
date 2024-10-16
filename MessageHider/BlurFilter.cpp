#include "BlurFilter.h"

#include <cmath>

BlurFilter::BlurFilter()
{
}

BlurFilter::~BlurFilter()
{
}

std::vector<std::vector<float>> BlurFilter::CreateKernel(int size, float sigma)
{
	std::vector<std::vector<float>> kernel(size, std::vector<float>(size));

	int half = size / 2;
	float sum = 0.f;
	for (int x = -half; x <= half; ++x)
	{
		for (int y = -half; y <= half; ++y)
		{
			float expo = -(x * x + y * y) / (2 * sigma * sigma);
			kernel[x + half][y + half] = exp(expo) / (2 * M_PI * sigma * sigma);
			sum += kernel[x + half][y + half];
		}
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			kernel[i][j] /= sum;
		}
	}

	return kernel;
}

unsigned char BlurFilter::ClampByte(int value)
{
	return std::max(0, std::min(value, 255));
}

void BlurFilter::Apply(unsigned char* pixels, int width, int height, int bytesPerPixel)
{
	const int kernelSize = 5;
	const float sigma = 3.f;
	std::vector<std::vector<float>> kernel = CreateKernel(kernelSize, sigma);
	int bytesCount = width * height * bytesPerPixel;

	unsigned char* blurredPixels = new unsigned char[bytesCount];

	int halfKernelSize = kernelSize / 2;
	for (int x = halfKernelSize; x < width - halfKernelSize; ++x)
	{
		for (int y = halfKernelSize; y < height - halfKernelSize; ++y)
		{
			float sumR = 0.f, sumG = 0.f, sumB = 0.f;
			for (int i = -halfKernelSize; i <= halfKernelSize; i++)
			{
				for (int j = -halfKernelSize; j <= halfKernelSize; j++)
				{
					int targetPixel = (x + i) * bytesPerPixel + (y + j) * width * bytesPerPixel;
					sumR += pixels[targetPixel] * kernel[i + halfKernelSize][j + halfKernelSize];
					sumG += pixels[targetPixel + 1] * kernel[i + halfKernelSize][j + halfKernelSize];
					sumB += pixels[targetPixel + 2] * kernel[i + halfKernelSize][j + halfKernelSize];
				}
			}

			int target = x * bytesPerPixel + y * width * bytesPerPixel;
			blurredPixels[target] = ClampByte(sumR);
			blurredPixels[target + 1] = ClampByte(sumG);
			blurredPixels[target + 2] = ClampByte(sumB);
			blurredPixels[target + 3] = pixels[target + 3];
		}
	}

	std::memcpy(pixels, blurredPixels, bytesCount);

	delete[] blurredPixels;
}
