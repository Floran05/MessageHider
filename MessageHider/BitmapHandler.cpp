#include "BitmapHandler.h"

#include <cstdio>
#include <fstream>
#include <iostream>

#include "JournalManager.h"

BitmapHandler::BitmapHandler()
	: lastLoadedFileHeader({0})
{
}

BitmapHandler::~BitmapHandler()
{
}

BYTE* BitmapHandler::Read(const char* filename)
{
	JournalManager::Instance->LogWrite(L"Reading BMP file...");
	HBITMAP handleBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (handleBitmap == NULL)
	{
		JournalManager::Instance->LogError(L"Can't load image");
		return new BYTE[0];
	}

	return Read(handleBitmap);
}

BYTE* BitmapHandler::Read(HBITMAP& handleBitmap)
{
	BITMAP bitmap;
	if (!GetObject(handleBitmap, sizeof(BITMAP), &bitmap))
	{
		JournalManager::Instance->LogError(L"Can't get image data");
		return new BYTE[0];
	}

	JournalManager::Instance->LogWrite(L"Image heigh: " + std::to_wstring(bitmap.bmHeight));
	JournalManager::Instance->LogWrite(L"Image width: " + std::to_wstring(bitmap.bmWidth));
	JournalManager::Instance->LogWrite(L"Bits per pixel: " + std::to_wstring(bitmap.bmBitsPixel));

	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, handleBitmap);

	lastLoadedFileHeader = { 0 };
	lastLoadedFileHeader.biSize = sizeof(BITMAPINFOHEADER);
	lastLoadedFileHeader.biWidth = bitmap.bmWidth;
	lastLoadedFileHeader.biHeight = bitmap.bmHeight;
	lastLoadedFileHeader.biPlanes = 1;
	lastLoadedFileHeader.biBitCount = bitmap.bmBitsPixel;
	lastLoadedFileHeader.biCompression = BI_RGB;
	lastLoadedFileHeader.biSizeImage = bitmap.bmWidth * bitmap.bmHeight * (bitmap.bmBitsPixel / 8);

	BYTE* pixels = new BYTE[lastLoadedFileHeader.biSizeImage];

	mLastLoadedFileWidth = bitmap.bmWidth;
	mLastLoadedFileHeight = bitmap.bmHeight;
	mLastLoadedFileBitsPerPixel = bitmap.bmBitsPixel;
	mLastLoadedFilePixels = pixels;

	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader = lastLoadedFileHeader;
	if (!GetDIBits(hdc, handleBitmap, 0, bitmap.bmHeight, pixels, &bitmapInfo, DIB_RGB_COLORS))
	{
		JournalManager::Instance->LogError(L"Can't get image data");
		DeleteDC(hdc);
		DeleteObject(handleBitmap);
		return new BYTE[0];
	}

	OrderRGBComponents(pixels, "BGR");

	return pixels;
}

void BitmapHandler::Write(const char* filename, BYTE* pixels)
{
	JournalManager::Instance->LogWrite(L"Writing BMP file...");
	if (pixels == nullptr && mLastLoadedFilePixels == nullptr)
	{
		JournalManager::Instance->LogError(L"Can't save image : pixels array not set");
		return;
	}

	if (pixels == nullptr) pixels = mLastLoadedFilePixels;

	HANDLE file = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		JournalManager::Instance->LogError(L"File creation failed");
		return;
	}

	BITMAPFILEHEADER header;
	header.bfType = 0x4d42;
	header.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER)) + lastLoadedFileHeader.biSize + lastLoadedFileHeader.biClrUsed * sizeof(RGBQUAD) + lastLoadedFileHeader.biSizeImage;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lastLoadedFileHeader.biSize + lastLoadedFileHeader.biClrUsed * sizeof(RGBQUAD);

	DWORD dwordTemp;
	if (!WriteFile(file, (LPVOID)&header, sizeof(BITMAPFILEHEADER), &dwordTemp, NULL))
	{
		JournalManager::Instance->LogWrite(L"Can't write BITMAPFILEHEADER header");
		CloseHandle(file);
		return;
	}

	if (!WriteFile(file, (LPVOID)&lastLoadedFileHeader, sizeof(BITMAPINFOHEADER) + lastLoadedFileHeader.biClrUsed * sizeof(RGBQUAD), &dwordTemp, NULL))
	{
		JournalManager::Instance->LogWrite(L"Can't write BITMAPINFOHEADER header and RGBQUAD array");
		CloseHandle(file);
		return;
	}

	OrderRGBComponents(pixels, "BGR");
	DWORD totalBytes = lastLoadedFileHeader.biSizeImage;
	if (!WriteFile(file, pixels, (int)totalBytes, &dwordTemp, NULL))
	{
		JournalManager::Instance->LogWrite(L"Can't write pixels in file");
		CloseHandle(file);
		return;
	}

	if (!CloseHandle(file))
	{
		JournalManager::Instance->LogWrite(L"An error occured while trying to close the file");
		return;
	}
}

BYTE* BitmapHandler::InvertImage(const BYTE* pixels, int width, int height, int bytesPerPixel)
{
	BYTE* inverted = new BYTE[width * height * bytesPerPixel];

	int lineSize = width * bytesPerPixel;

	for (int i = 0; i < height; ++i)
	{
		const BYTE* source = pixels + i * lineSize;
		BYTE* target = inverted + (height - 1 - i) * lineSize;
		std::copy(source, source + lineSize, target);
	}

	return inverted;
}
