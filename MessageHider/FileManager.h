#pragma once

#include <string>
#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
#include <Filter.h>

class FileHandler;
class AlgoStegano;

enum class SteganoAlgorithm
{
	BasicSteganoR,
	ComplexStegano,
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
	std::wstring path;

public:

	void LoadImageFromFilename(const std::string& filename);
	void SetPath(const std::wstring& filename);
	std::string Decrypt();
	void Encrypt(const std::string& message);
	int GetMessageMaxLenght();

	void SelectAlgorithm(const SteganoAlgorithm& algo);

};

