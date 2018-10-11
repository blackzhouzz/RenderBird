#pragma once
#include "RenderbirdPrivate.h"
#include "PathTracingCommon.h"
#include "CImg\CImg.h"
#include "Pixelmap.h"
using namespace cimg_library;

namespace RenderBird
{
	class ImageIO
	{
	public:
		static void WriteBMP(std::string filePath, Pixmap3f& data);
	};
}