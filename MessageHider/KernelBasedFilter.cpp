#include "KernelBasedFilter.h"

KernelBasedFilter::KernelBasedFilter()
{
}

KernelBasedFilter::~KernelBasedFilter()
{
}

std::vector<std::vector<float>> KernelBasedFilter::GetKernel()
{
	return std::vector<std::vector<float>>();
}

unsigned char KernelBasedFilter::ClampByte(int value)
{
	return std::max(0, std::min(value, 255));
}

void KernelBasedFilter::Apply(unsigned char* pixels, int width, int height, int bytesPerPixel)
{
	std::vector<std::vector<float>> kernel = GetKernel();
	int bytesCount = width * height * bytesPerPixel;

	unsigned char* blurredPixels = new unsigned char[bytesCount];

	int halfKernelSize = kernel.size() / 2;
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
