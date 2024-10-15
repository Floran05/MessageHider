#pragma once

#include "FileHandler.h"
#include <gdiplus.h>

#pragma comment (lib, "Gdiplus.lib")

class PNGHandler : public FileHandler
{
public:

	PNGHandler();
	virtual ~PNGHandler();

protected:

	bool GetEncoderClsid(const char* format, CLSID* clsid);

public:

	virtual BYTE* Read(const char* filename) override;
	virtual void Write(const char* filename, BYTE* pixels = nullptr) override;

};

