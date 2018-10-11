#include "ImageIO.h"
#include "PathTracingCommon.h"

namespace RenderBird
{
	void ImageIO::WriteBMP(std::string filePath, Pixmap3f& data)
	{
		size_t size = data.GetWidth() * data.GetHeight();
		RGB8* rgb8 = new RGB8[size];

		for (int i = 0; i < size; ++i)
		{
			RGB32 color = RGB32(data[i][0], data[i][1], data[i][2]);
			color[0] = PathTracingUtils::GammaCorrect(color[0]);
			color[1] = PathTracingUtils::GammaCorrect(color[1]);
			color[2] = PathTracingUtils::GammaCorrect(color[2]);
			rgb8[i] = RGB32toRGB8(color);
		}
		const uint8* ptrs = (const uint8*)rgb8;
		CImg<uint8> img = CImg<uint8>(data.GetWidth(), data.GetHeight(), 1, 3);

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