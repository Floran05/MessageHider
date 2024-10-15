#pragma once

#include <Windows.h>
#include <string>

class FileHandler
{
public:

	FileHandler();
	virtual ~FileHandler();

protected:

	int mLastLoadedFileHeight;
	int mLastLoadedFileWidth;
	int mLastLoadedFileBitsPerPixel;

	virtual void OrderRGBComponents(BYTE* pixels, const std::string& fromOrder = "BGR");

public:

	int GetLastLoadedFileHeight() const { return mLastLoadedFileHeight; }
	int GetLastLoadedFileWidth() const { return mLastLoadedFileWidth; }
	int GetLastLoadedFileBitsPerPixel() const { return mLastLoadedFileBitsPerPixel; }

	virtual BYTE* Read(const char* filename) = 0;
	virtual void Write(const char* filename, BYTE* pixels) = 0;

};
