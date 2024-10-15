#include "PNGHandler.h"

#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

#include "BitmapHandler.h"

PNGHandler::PNGHandler()
{
}

PNGHandler::~PNGHandler()
{
}

BYTE* PNGHandler::Read(const char* filename)
{
	HBITMAP result = NULL;
	Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(FileHandler::ConvertToWide(filename), false);
	if (bitmap)
	{
		bitmap->GetHBITMAP(Gdiplus::Color(255, 255, 255), &result);
		delete bitmap;
	}

	BitmapHandler* BMPHandler = new BitmapHandler();
	BYTE* out = BMPHandler->Read(result);
	mLastLoadedFileHeight = BMPHandler->GetLastLoadedFileHeight();
	mLastLoadedFileWidth = BMPHandler->GetLastLoadedFileWidth();
	mLastLoadedFileBitsPerPixel = BMPHandler->GetLastLoadedFileBitsPerPixel();

	return out;
}

void PNGHandler::Write(const char* filename, BYTE* pixels)
{
	Gdiplus::Bitmap bitmap(mLastLoadedFileWidth, mLastLoadedFileHeight, mLastLoadedFileWidth * 4, PixelFormat32bppARGB, pixels);

	CLSID clsidPng;
	CLSIDFromString(L"{557CF400-1A04-11D3-9A29-0000F81EF32E}", &clsidPng);
	bitmap.Save(FileHandler::ConvertToWide(filename), &clsidPng, NULL);
}
