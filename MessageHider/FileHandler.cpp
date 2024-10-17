#include "FileHandler.h"

#include <iostream>
#include <locale>
#include <codecvt>
#include <string>

#include "Filter.h"
#include "JournalManager.h"

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
	JournalManager::Instance->LogWrite(L"Reorder components to RGB");
	if (pixels == nullptr && mLastLoadedFilePixels == nullptr)
	{
		JournalManager::Instance->LogError(L"Pixels array is not set");
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
		JournalManager::Instance->LogError(L"Can't generate a GDI+ Bitmap : no image has been loaded");
		return nullptr;
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
	if (!lpwStr)
	{
		return "";
	}

	int len = lstrlenW(lpwStr);
	int multibyteLen = WideCharToMultiByte(CP_UTF8, 0, lpwStr, len, NULL, 0, NULL, NULL);

	std::string out(multibyteLen, 0);
	WideCharToMultiByte(CP_UTF8, 0, lpwStr, len, &out[0], multibyteLen, NULL, NULL);

	/*std::wstring wstr(lpwStr);
	std::string outStr(wstr.begin(), wstr.end());*/
	
	return out;
}

std::string FileHandler::ConvertWStringToString(std::wstring& wstr)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(wstr);
}

std::wstring FileHandler::ConvertStringToWString(std::string& string)
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(string);
}

void FileHandler::AddFilter(Filter* newFilter)
{
	filters.emplace_back(newFilter);
}

void FileHandler::ApplyFilters()
{
	JournalManager::Instance->LogWrite(L"Applying filters...");
	for (Filter* filter : filters)
	{
		filter->Apply(mLastLoadedFilePixels, mLastLoadedFileWidth, mLastLoadedFileHeight, mLastLoadedFileBitsPerPixel / 8);
	}
}

void FileHandler::ClearFilters()
{
	filters.clear();
}
