#include "ImageOutput.h"
#include "PathTracingCommon.h"

namespace RenderBird
{
	static void HorizontalFlip(Core::RGB32* data, int resX, int resY)
	{
		for (int y = 0; y < resY; ++y)
		{
			for (int x = 0; x < resX / 2; ++x)
			{
				int index = y * resX + x;
				int index2 = y * resX + resX - x - 1;
				std::swap(data[index], data[index2]);
			}
		}
	}

	void ImageOutput::WriteBMP(std::string filePath, PixelData* data, size_t resX, size_t resY)
	{
		//HorizontalFlip(data, resX, resY);
		size_t size = resX * resY;
		RGB8* rgb8 = new RGB8[size];

		for (int i = 0; i < size; ++i)
		{
			RGB32 color = data[i].GetColor();
			color[0] = PathTracingUtils::GammaCorrect(color[0]);
			color[1] = PathTracingUtils::GammaCorrect(color[1]);
			color[2] = PathTracingUtils::GammaCorrect(color[2]);
			rgb8[i] = RGB32toRGB8(color);
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