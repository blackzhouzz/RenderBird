#pragma once
#include "RenderbirdPrivate.h"
#include "OutputBuffer.h"

namespace RenderBird
{
	struct AuxiliaryBuffer
	{
		std::unique_ptr<OutputBufferVec3f> _colorBuffer;
		std::unique_ptr<OutputBufferF>     _depthBuffer;
		std::unique_ptr<OutputBufferVec3f> _normalBuffer;
		std::unique_ptr<OutputBufferVec3f> _albedoBuffer;
		std::unique_ptr<OutputBufferF> _visibilityBuffer;
	};
}