#include "FileManager.h"

#include <algorithm>
#include <iostream>

#include "BitmapHandler.h"
#include "PNGHandler.h"
#include "BasicSteganoR.h"
#include "ComplexStegano.h"
#include "JournalManager.h"

#include "BlurFilter.h"
#include "GrayscaleFilter.h"
#include "NegativeFilter.h"
#include "SepiaFilter.h"
#include "SharpeningFilter.h"
#include "EdgeDetectionFilter.h"

FileManager::FileManager()
	: mFileHandler(nullptr)
	, mImage(nullptr)
	, mSelectedAlgorithm(ESteganoAlgorithm::None)
	, mSteganoAlgorithm(nullptr)
{

}

FileManager::~FileManager()
{
}

bool FileManager::LoadImageFromFilename(const std::string& filename)
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
		return false;
	}

	mFileHandler->Read(filename.c_str());
	return true;
}

void FileManager::SetPath(const std::wstring& filename)
{
	FileManager::path = filename;
}


std::string FileManager::Decrypt()
{
	std::string message;
	if (mSelectedAlgorithm == ESteganoAlgorithm::None || mSteganoAlgorithm == nullptr)
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
	if (mSelectedAlgorithm == ESteganoAlgorithm::None || mSteganoAlgorithm == nullptr)
	{
		JournalManager::Instance->LogError(L"Can't encrypt the message : no algorithm selected");
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

void FileManager::SelectAlgorithm(const ESteganoAlgorithm& algo)
{
	mSelectedAlgorithm = algo;
	switch (algo)
	{
	case ESteganoAlgorithm::BasicSteganoR:
		mSteganoAlgorithm = new BasicSteganoR();
		break;
	case ESteganoAlgorithm::ComplexStegano:
		mSteganoAlgorithm = new ComplexStegano();
		break;
	}
}

void FileManager::AddFilter(const EFilterType& type)
{
	Filter* f = nullptr;
	switch (type)
	{
	case EFilterType::Blur:
		f = new BlurFilter();
		break;
	case EFilterType::Negative:
		f = new NegativeFilter();
		break;
	case EFilterType::Sepia:
		f = new SepiaFilter();
		break;
	case EFilterType::Grayscale:
		f = new GrayscaleFilter();
		break;
	case EFilterType::Sharpening:
		f = new SharpeningFilter();
		break;
	case EFilterType::EdgeDetection:
		f = new EdgeDetectionFilter();
		break;
	}
	if (f == nullptr) return;

	mFileHandler->AddFilter(f);
}

void FileManager::ApplyFilters()
{
	mFileHandler->ApplyFilters();
}

void FileManager::ClearFilters()
{
	mFileHandler->ClearFilters();
}
