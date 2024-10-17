#pragma once

#include <string>
#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
#include <Filter.h>

class FileHandler;
class AlgoStegano;

enum class ESteganoAlgorithm
{
	BasicSteganoR,
	ComplexStegano,
	None
};

enum class EFilterType
{
	Blur,
	Negative,
	Sepia,
	Grayscale,
	Sharpening,
	EdgeDetection
};

class FileManager
{
public:

	FileManager();
	virtual ~FileManager();

protected:

	FileHandler* mFileHandler;
	Gdiplus::Bitmap* mImage;
	ESteganoAlgorithm mSelectedAlgorithm;

	AlgoStegano* mSteganoAlgorithm;
	std::wstring path;

public:

	bool LoadImageFromFilename(const std::string& filename);
	void SetPath(const std::wstring& filename);
	std::string Decrypt();
	void Encrypt(const std::string& message);
	int GetMessageMaxLenght();

	void SelectAlgorithm(const ESteganoAlgorithm& algo);
	void AddFilter(const EFilterType& type);
	void ApplyFilters();
	void ClearFilters();

};

