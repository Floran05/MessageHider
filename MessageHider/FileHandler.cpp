#include "FileHandler.h"

FileHandler::FileHandler()
	: mLastLoadedFileHeight(0)
	, mLastLoadedFileWidth(0)
	, mLastLoadedFileBitsPerPixel(0)
{
}

FileHandler::~FileHandler()
{
}

void FileHandler::OrderRGBComponents(BYTE* pixels, const std::string& fromOrder)
{
	int fileSize = mLastLoadedFileWidth * mLastLoadedFileHeight;
	int componentsCount = mLastLoadedFileBitsPerPixel / 8;
	struct{
		int R;
		int G;
		int B;
	} components;
	for (int i = 0; i < fileSize; i++)
	{
		for (unsigned char j = 0; j < 3; j++)
		{
			switch (fromOrder[j])
			{
			case 'R':
				components.R = pixels[i * componentsCount + j];
				break;
			case 'G':
				components.G = pixels[i * componentsCount + j];
				break;
			case 'B':
				components.B = pixels[i * componentsCount + j];
				break;
			}
		}

		pixels[i * componentsCount] = components.R;
		pixels[i * componentsCount + 1] = components.G;
		pixels[i * componentsCount + 2] = components.B;
	}
}
