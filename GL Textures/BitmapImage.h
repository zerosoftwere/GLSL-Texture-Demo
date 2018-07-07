#pragma once
#include <string>
#include "GLUtil.h"

class BitmapImage
{
public:
	BitmapImage(const std::string& filepath);
	~BitmapImage();

	unsigned int   Width() const;
	unsigned int   Height() const;
	unsigned int   Size() const;
	unsigned short BitsPerPixel() const;

	unsigned char* Data() const;

protected:
	void LoadImage(const std::string& filepath);

	template <typename T>
	T ExtractValue(const unsigned char* data, unsigned int pos);

	unsigned int   mWidth;
	unsigned int   mHeight;
	unsigned int   mSize;
	unsigned short mBitsPerPixel;
	unsigned char* mData;

	const static unsigned int BITMAP_IMAGE_FILE_HEADER_SIZE;
	const static unsigned int BITMAP_IMAGE_INFO_HEADER_SIZE;

private:
	BitmapImage(const BitmapImage& rhs);
	BitmapImage& operator=(const BitmapImage &rhs);
};