#include "FileHandler.h"

#include <cstdio>
#include <Windows.h>

FileHandler::FileHandler()
{
}

FileHandler::~FileHandler()
{
}

unsigned char* FileHandler::ReadBitmap(const WCHAR* filename)
{
	/*FILE* file = fopen(filename, "rb");

	const int header_size = 54;

	unsigned char header[header_size];
	fread(header, sizeof(unsigned char), header_size, file);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];

	int size = 3 * width * height;
	unsigned char* data = new unsigned char[size];

	fread(data, sizeof(unsigned char), size, file);
	fclose(file);*/

	HBITMAP hBMP = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	return nullptr;

}

void FileHandler::WriteBitmap(const unsigned char* data)
{
}
