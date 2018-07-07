#include "BitmapImage.h"

#include <fstream>
#include <exception>

const unsigned int BitmapImage::BITMAP_IMAGE_FILE_HEADER_SIZE = 14;
const unsigned int BitmapImage::BITMAP_IMAGE_INFO_HEADER_SIZE = 40;

BitmapImage::BitmapImage(const std::string& filepath)
	: mData(nullptr), mWidth(0), mHeight(0), mSize(0), mBitsPerPixel(0)
{
	LoadImage(filepath);
}

BitmapImage::~BitmapImage()
{
	if (mData) delete [] mData;
	mData = nullptr;
}

unsigned int BitmapImage::Width() const
{
	return mWidth;
}

unsigned int BitmapImage::Height() const
{
	return mHeight;
}

unsigned short BitmapImage::BitsPerPixel() const
{
	return mBitsPerPixel;
}

unsigned int BitmapImage::Size() const
{
	return mSize;
}

unsigned char* BitmapImage::Data() const
{
	return mData;
}

template <typename T>
T BitmapImage::ExtractValue(const unsigned char* data, unsigned int pos)
{
	size_t index = sizeof(T);
    T result = 0;

    while (index--)
    {
        result = result << 8;
        result = result | data[index + pos];
    }
    return result;
}

void BitmapImage::LoadImage(const std::string& filepath)
{
	std::fstream hfile;
	hfile.open(filepath.c_str(), std::ios::in | std::ios::binary);

	if (!hfile.is_open())
		throw std::exception("Error: File not found");

	unsigned char fileHeader[BITMAP_IMAGE_FILE_HEADER_SIZE];
	unsigned char infoHeader[BITMAP_IMAGE_INFO_HEADER_SIZE];

	hfile.read(reinterpret_cast<char*>(fileHeader), BITMAP_IMAGE_FILE_HEADER_SIZE);
	hfile.read(reinterpret_cast<char*>(infoHeader), BITMAP_IMAGE_INFO_HEADER_SIZE);

	if (fileHeader[0] != 'B' && fileHeader[1] != 'M')
		throw std::exception("Error: File is not BitmapImage");
	
	hfile.seekg(0, std::ios::end);

	unsigned int fileLength = hfile.tellg();
	unsigned int fileSize   = ExtractValue<unsigned int>(fileHeader, 2);

	if (fileSize != fileLength)
		throw std::exception("Error: Invalid BitmapImage");

	mWidth        = ExtractValue<unsigned int>(infoHeader, 4);
	mHeight       = ExtractValue<unsigned int>(infoHeader, 8);
	mBitsPerPixel = ExtractValue<unsigned short>(infoHeader, 14);
	mSize         = ((mWidth * mBitsPerPixel + 31) / 32) * 4 * mHeight;
	mData         = new unsigned char[mSize];

	unsigned int pixelsOffset = ExtractValue<unsigned int>(fileHeader, 10);
	hfile.seekg(pixelsOffset, std::ios::beg);
    hfile.read(reinterpret_cast<char*>(mData), mSize);

    hfile.close();
}