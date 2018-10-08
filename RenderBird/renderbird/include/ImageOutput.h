#pragma once
#include "RenderbirdPrivate.h"
#include "PathTracingCommon.h"
#include "CImg\CImg.h"
using namespace cimg_library;

namespace RenderBird
{
	class ImageOutput
	{
	public:
		static void WriteBMP(std::string filePath, PixelData* data, size_t resX, size_t resY);
	};
}