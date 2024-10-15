#include "PNGHandler.h"

#include <vector>
#include <iostream>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

#include "BitmapHandler.h"

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
	if (pixels == nullptr && mLastLoadedFilePixels == nullptr)
	{
		std::cerr << "Vous n'avez pas renseigné le tableau de pixels à sauvegarder dans l'image" << std::endl;
		return;
	}

	if (pixels == nullptr) pixels = mLastLoadedFilePixels;

	OrderRGBComponents(pixels, "BGR");
	BYTE* reversedImage = BitmapHandler::InvertImage(pixels, mLastLoadedFileWidth, mLastLoadedFileHeight, mLastLoadedFileBitsPerPixel / 8);
	Gdiplus::Bitmap bitmap(mLastLoadedFileWidth, mLastLoadedFileHeight, mLastLoadedFileWidth * 4, PixelFormat32bppARGB, reversedImage);

	CLSID clsidPng;
	if (!GetEncoderClsid("image/png", &clsidPng))
	{
		std::cerr << "Impossible de récupérer le CLSID correspondant au format de fichier" << std::endl;
		return;
	}
	bitmap.Save(FileHandler::ConvertToWide(filename), &clsidPng, NULL);

}
