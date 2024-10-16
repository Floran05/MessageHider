#include "FileManager.h"

#include <string>
#include <algorithm>
#include <iostream>

#include "BitmapHandler.h"
#include "PNGHandler.h"
#include "BasicSteganoR.h"

FileManager::FileManager()
	: mFileHandler(nullptr)
	, mImage(nullptr)
	, mSelectedAlgorithm(SteganoAlgorithm::None)
	, mSteganoAlgorithm(nullptr)
{

}

FileManager::~FileManager()
{
}

void FileManager::LoadImageFromFilename(const char* filename)
{
	std::string fn(filename);
	std::string ext = fn.substr(fn.find_last_of(".") + 1);
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });

	if (ext == "bmp")
	{
		mFileHandler = new BitmapHandler();
	}
	else if (ext == "png")
	{
		mFileHandler = new PNGHandler();
	}
	else
	{
		std::cerr << "Impossible de charger l'image : ce format n'est pas pris en charge (" << ext << ")" << std::endl;
		return;
	}

	mFileHandler->Read(filename);
}

std::string& FileManager::Decrypt()
{
	std::string message;
	if (mSelectedAlgorithm == SteganoAlgorithm::None && mSteganoAlgorithm != nullptr)
	{
		std::cerr << "Impossible de d�chiffrer le message de l'image : aucun algorithme n'a �t� s�lectionn�" << std::endl;
		return message;
	}

	message = mSteganoAlgorithm->Decrypt(
		mFileHandler->GetPixels(),
		mFileHandler->GetLastLoadedFileWidth() * mFileHandler->GetLastLoadedFileHeight(),
		mFileHandler->GetLastLoadedFileBitsPerPixel() / 8
	);
	return message;
}

void FileManager::Encrypt(const std::string& message)
{
	if (mSelectedAlgorithm == SteganoAlgorithm::None && mSteganoAlgorithm != nullptr)
	{
		std::cerr << "Impossible de chiffrer le message dans l'image : aucun algorithme n'a �t� s�lectionn�" << std::endl;
		return;
	}

	mSteganoAlgorithm->Encrypt(
		mFileHandler->GetPixels(),
		mFileHandler->GetLastLoadedFileWidth() * mFileHandler->GetLastLoadedFileHeight(),
		mFileHandler->GetLastLoadedFileBitsPerPixel() / 8,
		message);
}

int FileManager::GetMessageMaxLenght()
{
	//return mSteganoAlgorithm-
	return 0;
}

void FileManager::SelectAlgorithm(const SteganoAlgorithm& algo)
{
	mSelectedAlgorithm = algo;
	switch (algo)
	{
	case SteganoAlgorithm::BasicSteganoR:
		mSteganoAlgorithm = new BasicSteganoR();
		break;
	}
}
