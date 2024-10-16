#pragma once

#include <string>
#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

class FileHandler;
class AlgoStegano;

enum class SteganoAlgorithm
{
	BasicSteganoR,
	None
};

class FileManager
{
public:

	FileManager();
	virtual ~FileManager();

protected:

	FileHandler* mFileHandler;
	Gdiplus::Bitmap* mImage;
	SteganoAlgorithm mSelectedAlgorithm;

	AlgoStegano* mSteganoAlgorithm;

public:

	void LoadImageFromFilename(const std::string& filename);
	std::string Decrypt();
	void Encrypt(const std::string& message);
	int GetMessageMaxLenght();

	void SelectAlgorithm(const SteganoAlgorithm& algo);

};

