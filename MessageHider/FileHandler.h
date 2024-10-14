#pragma once

#include <Windows.h>
#include <vector>

class FileHandler
{
public:

	FileHandler();
	virtual ~FileHandler();

public:

	BYTE* ReadBitmap(const char* filename, BITMAPINFOHEADER& infoHeader);
	void WriteBitmap(const char* filename, BYTE* pixels, BITMAPINFOHEADER& infoHeader);

};

