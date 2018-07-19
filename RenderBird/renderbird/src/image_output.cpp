#include "image_output.h"

namespace RenderBird
{
	void ImageOutput::WriteBMP(std::string filePath, RGB32* data, size_t resX, size_t resY)
	{
		size_t size = resX * resY;
		RGB8* rgb8 = new RGB8[size];

		for (int i = 0; i < size; ++i)
		{
			rgb8[i] = RGB32toRGB8(data[i]);
		}
		const uint8* ptrs = (const uint8*)rgb8;
		CImg<uint8> img = CImg<uint8>(resX, resY, 1, 3);

		uint8 *pR = img.data(0, 0, 0, 0),
			*pG = img.data(0, 0, 0, 1),
			*pB = img.data(0, 0, 0, 2);

		for (int i = 0; i < size; ++i)
		{
			*(pR++) = (uint8)*(ptrs++);
			*(pG++) = (uint8)*(ptrs++);
			*(pB++) = (uint8)*(ptrs++);
		}

		img.save_bmp(filePath.c_str());
	}
}