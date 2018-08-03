#pragma once
#include "renderbird_private.h"
#include "CImg\CImg.h"
using namespace cimg_library;

namespace RenderBird
{
	class ImageOutput
	{
	public:
		static void WriteBMP(std::string filePath, RGB32* data, size_t resX, size_t resY);
	};
}