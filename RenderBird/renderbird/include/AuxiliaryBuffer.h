#pragma once
#include "PixelMap.h"

namespace RenderBird
{
	class AuxiliaryBuffer
	{
	public:
		AuxiliaryBuffer(uint32 width, uint32 height);
		~AuxiliaryBuffer();
		void Resize(uint32 width, uint32 height);

		Pixmap<Float>* m_depthBuffer;
		Pixmap<Float>* m_visibilityBuffer;
		Pixmap<RGB32>* m_albedoBuffer;
		Pixmap<Vector3f>* m_normalBuffer;
	};
}