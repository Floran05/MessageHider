#pragma once

#include <Windows.h>
#include <string>
#include <vector>

class Filter;

class FileHandler
{
public:

	FileHandler();
	virtual ~FileHandler();

protected:

	BYTE* mLastLoadedFilePixels;
	int mLastLoadedFileHeight;
	int mLastLoadedFileWidth;
	int mLastLoadedFileBitsPerPixel;

	virtual void OrderRGBComponents(BYTE* pixel = nullptr, const std::string& fromOrder = "BGR");

	std::vector<Filter*> filters;

public:

	int GetLastLoadedFileHeight() const { return mLastLoadedFileHeight; }
	int GetLastLoadedFileWidth() const { return mLastLoadedFileWidth; }
	int GetLastLoadedFileBitsPerPixel() const { return mLastLoadedFileBitsPerPixel; }

	virtual BYTE* Read(const char* filename) = 0;
	virtual void Write(const char* filename, BYTE* pixels = nullptr) = 0;

	static WCHAR* ConvertToWide(const char* charStr);

	void AddFilter(Filter* newFilter);
	void ApplyFilters();

};

//01000000 00000000 00010011