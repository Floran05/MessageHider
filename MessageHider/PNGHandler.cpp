#include "PNGHandler.h"

#include <vector>
#include <iostream>

#include "BitmapHandler.h"
#include "JournalManager.h"

PNGHandler::PNGHandler()
{
}

PNGHandler::~PNGHandler()
{
}

bool PNGHandler::GetEncoderClsid(const char* format, CLSID* cslid)
{
	UINT num = 0;
	UINT size = 0;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return false;
	}

	Gdiplus::ImageCodecInfo* imageCodecInfos = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (imageCodecInfos == NULL)
	{
		return false;
	}
	Gdiplus::GetImageEncoders(num, size, imageCodecInfos);

	for (UINT i = 0; i < num; i++)
	{
		if (wcscmp(imageCodecInfos[i].MimeType, FileHandler::ConvertToWide(format)) == 0)
		{
			*cslid = imageCodecInfos[i].Clsid;
			free(imageCodecInfos);
			return true;
		}
	}
	
	free(imageCodecInfos);
	return false;
}

BYTE* PNGHandler::Read(const char* filename)
{
	JournalManager::Instance->LogWrite(L"Reading PNG file...");
	HBITMAP result = NULL;
	Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(FileHandler::ConvertToWide(filename), false);
	if (bitmap)
	{
		bitmap->GetHBITMAP(Gdiplus::Color(255, 255, 255), &result);
		delete bitmap;
	}

	BitmapHandler* BMPHandler = new BitmapHandler();
	BYTE* out = BMPHandler->Read(result);
	mLastLoadedFilePixels = out;
	mLastLoadedFileHeight = BMPHandler->GetLastLoadedFileHeight();
	mLastLoadedFileWidth = BMPHandler->GetLastLoadedFileWidth();
	mLastLoadedFileBitsPerPixel = BMPHandler->GetLastLoadedFileBitsPerPixel();

	return out;
}

void PNGHandler::Write(const char* filename, BYTE* pixels)
{
	JournalManager::Instance->LogWrite(L"Writing PNG file...");
	if (pixels == nullptr && mLastLoadedFilePixels == nullptr)
	{
		JournalManager::Instance->LogError(L"Pixel array not set");
		return;
	}

	if (pixels == nullptr) pixels = mLastLoadedFilePixels;

	OrderRGBComponents(pixels, "BGR");
	BYTE* reversedImage = BitmapHandler::InvertImage(pixels, mLastLoadedFileWidth, mLastLoadedFileHeight, mLastLoadedFileBitsPerPixel / 8);
	Gdiplus::Bitmap bitmap(
		mLastLoadedFileWidth, 
		mLastLoadedFileHeight, 
		mLastLoadedFileWidth * mLastLoadedFileBitsPerPixel / 8, 
		mLastLoadedFileBitsPerPixel / 8 > 3 ? PixelFormat32bppARGB : PixelFormat24bppRGB,
		reversedImage);

	CLSID clsidPng;
	if (!GetEncoderClsid("image/png", &clsidPng))
	{
		JournalManager::Instance->LogError(L"Can't get CLSID corresponding to this format");
		return;
	}
	bitmap.Save(FileHandler::ConvertToWide(filename), &clsidPng, NULL);

}
