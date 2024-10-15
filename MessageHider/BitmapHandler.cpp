#include "BitmapHandler.h"

#include <cstdio>
#include <fstream>
#include <iostream>

BitmapHandler::BitmapHandler()
{
}

BitmapHandler::~BitmapHandler()
{
}

BYTE* BitmapHandler::Read(const char* filename)
{
	HBITMAP handleBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (handleBitmap == NULL)
	{
		std::cerr << "Impossible de charger l'image" << std::endl;
		return new BYTE[0];
	}

	return Read(handleBitmap);
}

BYTE* BitmapHandler::Read(HBITMAP& handleBitmap)
{
	BITMAP bitmap;
	if (!GetObject(handleBitmap, sizeof(BITMAP), &bitmap))
	{
		std::cerr << "Impossible de r�cup�rer les donn�es de l'image" << std::endl;
		return new BYTE[0];
	}

	std::cout << "Image Height: " << bitmap.bmHeight << std::endl;
	std::cout << "Image Width: " << bitmap.bmWidth << std::endl;
	std::cout << "Bits per pixel: " << bitmap.bmBitsPixel << std::endl;

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
		std::cerr << "Impossible de r�cup�rer les donn�es de l'image" << std::endl;
		DeleteDC(hdc);
		DeleteObject(handleBitmap);
		return new BYTE[0];
	}

	OrderRGBComponents(pixels, "BGR");

	return pixels;
}

void BitmapHandler::Write(const char* filename, BYTE* pixels)
{
	if (pixels == nullptr && mLastLoadedFilePixels == nullptr)
	{
		std::cerr << "Vous n'avez pas renseign� le tableau de pixels � sauvegarder dans l'image" << std::endl;
		return;
	}

	if (pixels == nullptr) pixels = mLastLoadedFilePixels;

	HANDLE file = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		std::cerr << "La cr�ation du fichier a �chou�" << std::endl;
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
		std::cerr << "Impossible d'�crire le header de fichier BITMAPFILEHEADER" << std::endl;
		CloseHandle(file);
		return;
	}

	if (!WriteFile(file, (LPVOID)&lastLoadedFileHeader, sizeof(BITMAPINFOHEADER) + lastLoadedFileHeader.biClrUsed * sizeof(RGBQUAD), &dwordTemp, NULL))
	{
		std::cerr << "Impossible d'�crire le header de fichier BITMAPINFOHEADER ainsi que le tableau RGBQUAD" << std::endl;
		CloseHandle(file);
		return;
	}

	OrderRGBComponents(pixels, "BGR");
	DWORD totalBytes = lastLoadedFileHeader.biSizeImage;
	if (!WriteFile(file, pixels, (int)totalBytes, &dwordTemp, NULL))
	{
		std::cerr << "Impossible d'�crire les pixels dans le fichier" << std::endl;
		CloseHandle(file);
		return;
	}

	if (!CloseHandle(file))
	{
		std::cerr << "Une erreur est survenue lors de la fermeture du fichier" << std::endl;
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
