#include "FileManager.h"

#include <algorithm>
#include <iostream>

#include "BitmapHandler.h"
#include "PNGHandler.h"
#include "BasicSteganoR.h"
#include "ComplexStegano.h"
#include "JournalManager.h"

#include "BlurFilter.h"

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
		JournalManager::Instance->LogWrite(L"Can't load image : this format is not supported (" + FileHandler::ConvertStringToWString(ext) + L")");
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
		JournalManager::Instance->LogError(L"Can't decrypt message : no algorithm selected");
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
		JournalManager::Instance->LogError(L"Can't encrypt the message : no algorithm selected");
		return;
	}

	BlurFilter* f = new BlurFilter();
	mFileHandler->AddFilter(f);
	mFileHandler->ApplyFilters();

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
