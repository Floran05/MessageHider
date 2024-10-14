#include "FileHandler.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <Windows.h>

FileHandler::FileHandler()
{
}

FileHandler::~FileHandler()
{
}

BYTE* FileHandler::ReadBitmap(const char* filename, BITMAPINFOHEADER& infoHeader)
{
	HBITMAP handleBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (handleBitmap == NULL)
	{
		std::cerr << "Impossible de charger l'image" << std::endl;
		return new BYTE[0];
	}

	BITMAP bitmap;
	if (!GetObject(handleBitmap, sizeof(BITMAP), &bitmap))
	{
		std::cerr << "Impossible de récupérer les données de l'image" << std::endl;
		return new BYTE[0];
	}

	std::cout << "Image Height: " << bitmap.bmHeight << std::endl;
	std::cout << "Image Width: " << bitmap.bmWidth << std::endl;
	std::cout << "Bits per pixel: " << bitmap.bmBitsPixel << std::endl;

	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, handleBitmap);

	infoHeader = { 0 };
	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biWidth = bitmap.bmWidth;
	infoHeader.biHeight = bitmap.bmHeight;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = bitmap.bmBitsPixel;
	infoHeader.biCompression = BI_RGB;
	infoHeader.biSizeImage = bitmap.bmWidth * bitmap.bmHeight * (bitmap.bmBitsPixel / 8);

	BYTE* pixels = new BYTE[infoHeader.biSizeImage];

	//data[i * 4] = 255; // B
	//data[i * 4 + 1] = 0; // G
	//data[i * 4 + 2] = 0; // R

	//data.resize(bitmap.bmWidth * bitmap.bmHeight * (bitmap.bmBitsPixel / 8));
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader = infoHeader;
	if (!GetDIBits(hdc, handleBitmap, 0, bitmap.bmHeight, pixels/*data.data()*/, &bitmapInfo, DIB_RGB_COLORS))
	{
		std::cerr << "Impossible de récupérer les données de l'image" << std::endl;
		DeleteDC(hdc);
		DeleteObject(handleBitmap);
		//data.clear();
		return new BYTE[0];
	}

	return pixels;
}

void FileHandler::WriteBitmap(const char* filename, BYTE* pixels, BITMAPINFOHEADER& infoHeader)
{
	HANDLE file = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		std::cerr << "La création du fichier a échoué" << std::endl;
		return;
	}

	BITMAPFILEHEADER header;
	header.bfType = 0x4d42;
	header.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER)) + infoHeader.biSize + infoHeader.biClrUsed * sizeof(RGBQUAD) + infoHeader.biSizeImage;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + infoHeader.biSize + infoHeader.biClrUsed * sizeof(RGBQUAD);

	DWORD dwordTemp;
	if (!WriteFile(file, (LPVOID)&header, sizeof(BITMAPFILEHEADER), &dwordTemp, NULL))
	{
		std::cerr << "Impossible d'écrire le header de fichier BITMAPFILEHEADER" << std::endl;
		CloseHandle(file);
		return;
	}

	std::cout << "CLR USED: " << infoHeader.biClrUsed << std::endl;
	if (!WriteFile(file, (LPVOID)&infoHeader, sizeof(BITMAPINFOHEADER) + infoHeader.biClrUsed * sizeof(RGBQUAD), &dwordTemp, NULL))
	{
		std::cerr << "Impossible d'écrire le header de fichier BITMAPINFOHEADER ainsi que le tableau RGBQUAD" << std::endl;
		CloseHandle(file);
		return;
	}

	DWORD totalBytes = infoHeader.biSizeImage;
	if (!WriteFile(file, pixels, (int)totalBytes, &dwordTemp, NULL))
	{
		std::cerr << "Impossible d'écrire les pixels dans le fichier" << std::endl;
		CloseHandle(file);
		return;
	}

	if (!CloseHandle(file))
	{
		std::cerr << "Une erreur est survenue lors de la fermeture du fichier" << std::endl;
		return;
	}
}
