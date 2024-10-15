#pragma once

#include "FileHandler.h"

class PNGHandler : public FileHandler
{
public:

	PNGHandler();
	virtual ~PNGHandler();

public:

	virtual BYTE* Read(const char* filename) override;
	virtual void Write(const char* filename, BYTE* pixels) override;

};

