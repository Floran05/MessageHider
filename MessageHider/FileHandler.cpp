#include "FileHandler.h"

#include <iostream>
#include <locale>
#include <codecvt>
#include <string>

#include "Filter.h"

FileHandler::FileHandler()
	: mLastLoadedFilePixels(nullptr)
	, mLastLoadedFileHeight(0)
	, mLastLoadedFileWidth(0)
	, mLastLoadedFileBitsPerPixel(0)
{
}

FileHandler::~FileHandler()
{
}

void FileHandler::OrderRGBComponents(BYTE* pixels, const std::string& fromOrder)
{
	if (pixels == nullptr && mLastLoadedFilePixels == nullptr)
	{
		std::cerr << "Vous n'avez pas renseigné le tableau de pixels à modifier" << std::endl;
		return;
	}
	
	if (pixels == nullptr) pixels = mLastLoadedFilePixels;

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

Gdiplus::Bitmap* FileHandler::GetGDIPlusBitmap()
{
	if (mLastLoadedFileWidth < 1 || mLastLoadedFileHeight < 1)
	{
		std::cerr << "Impossible de récupérer un Bitmap GDI+ : Aucune image n'a été chargée";
	}

	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(
		mLastLoadedFileWidth,
		mLastLoadedFileHeight,
		mLastLoadedFileWidth * mLastLoadedFileBitsPerPixel / 8,
		mLastLoadedFileBitsPerPixel / 8 > 3 ? PixelFormat32bppARGB : PixelFormat24bppRGB,
		mLastLoadedFilePixels);
	return bitmap;
}

WCHAR* FileHandler::ConvertToWide(const char* charStr)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, charStr, -1, NULL, 0);
	WCHAR* wideStr = new WCHAR[size];
	MultiByteToWideChar(CP_UTF8, 0, charStr, -1, wideStr, size);
	return wideStr;
}

std::string FileHandler::ConvertLPWSTRToString(LPWSTR& lpwStr)
{
	std::wstring wstr(lpwStr);
	std::string outStr(wstr.begin(), wstr.end());
	return outStr;
}

std::string FileHandler::ConvertWStringToString(std::wstring& wstr)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(wstr);
}

void FileHandler::AddFilter(Filter* newFilter)
{
	filters.emplace_back(newFilter);
}

void FileHandler::ApplyFilters()
{
	for (Filter* filter : filters)
	{
		filter->Apply(mLastLoadedFilePixels, mLastLoadedFileWidth, mLastLoadedFileHeight, mLastLoadedFileBitsPerPixel / 8);
	}
}
