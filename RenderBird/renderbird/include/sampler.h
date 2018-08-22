#pragma once
#include "renderbird_private.h"
#include "pathtracing_common.h"
namespace RenderBird
{
	class Sampler
	{
	public:
		Sampler(uint32 numSamplers);
		Float Random1D()const;
		Vector2f Random2D()const;
		CameraSample GetCameraSample(int pixelX, int pixelY)const;
	public:
		uint32 m_numSamplers;
	};
}