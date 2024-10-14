#pragma once

class FileHandler
{
public:

	FileHandler();
	virtual ~FileHandler();

public:

	unsigned char* ReadBitmap(const WCHAR* filename);
	void WriteBitmap(const unsigned char* data);

};

