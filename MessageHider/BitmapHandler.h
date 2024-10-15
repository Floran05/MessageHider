#pragma once

#include "FileHandler.h"

class BitmapHandler : public FileHandler
{
public:
	BitmapHandler();
	virtual ~BitmapHandler();

private:

	BITMAPINFOHEADER lastLoadedFileHeader;

private:

public:

	virtual BYTE* Read(const char* filename);
	virtual void Write(const char* filename, BYTE* pixels);

	BITMAPINFOHEADER& GetLastLoadedFileHeader() { return lastLoadedFileHeader; }

};

