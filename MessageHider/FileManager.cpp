#include "FileManager.h"

#include <algorithm>
#include <iostream>

#include "BitmapHandler.h"
#include "PNGHandler.h"
#include "BasicSteganoR.h"
#include "ComplexStegano.h"

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

void FileManager::LoadImageFromFilename(const std::string& filename)
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

	mFileHandler->Read(filename.c_str());
}

void FileManager::SetPath(const std::wstring& filename)
{
	FileManager::path = filename;
}

std::string FileManager::Decrypt()
{
	std::string message;
	if (mSelectedAlgorithm == SteganoAlgorithm::None && mSteganoAlgorithm != nullptr)
	{
		std::cerr << "Impossible de déchiffrer le message de l'image : aucun algorithme n'a été sélectionné" << std::endl;
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
		std::cerr << "Impossible de chiffrer le message dans l'image : aucun algorithme n'a été sélectionné" << std::endl;
		return;
	}

	mSteganoAlgorithm->Encrypt(
		mFileHandler->GetPixels(),
		mFileHandler->GetLastLoadedFileWidth() * mFileHandler->GetLastLoadedFileHeight(),
		mFileHandler->GetLastLoadedFileBitsPerPixel() / 8,
		message);
	mFileHandler->Write(FileHandler::ConvertWStringToString(path).c_str());
}

int FileManager::GetMessageMaxLenght()
{
	return mSteganoAlgorithm->MessageMaxLenght(
		mFileHandler->GetLastLoadedFileWidth() * mFileHandler->GetLastLoadedFileHeight(), 
		mFileHandler->GetLastLoadedFileBitsPerPixel() / 8);
}

void FileManager::SelectAlgorithm(const SteganoAlgorithm& algo)
{
	mSelectedAlgorithm = algo;
	switch (algo)
	{
	case SteganoAlgorithm::BasicSteganoR:
		mSteganoAlgorithm = new BasicSteganoR();
		break;
	case SteganoAlgorithm::ComplexStegano:
		mSteganoAlgorithm = new ComplexStegano();
		break;
	}
}
