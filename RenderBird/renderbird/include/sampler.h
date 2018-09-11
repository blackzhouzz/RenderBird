#pragma once
#include "renderbird_private.h"
#include "pathtracing_common.h"
#include <random>

namespace RenderBird
{
	class Sampler
	{
	public:
		Sampler(uint32 numSamplers);
		~Sampler();
		Float Random1D()const;
		Vector2f Random2D()const;
		CameraSample GetCameraSample(int pixelX, int pixelY)const;
	public:
		uint32 m_numSamplers;
		static std::random_device m_rd;
		std::mt19937* m_gen;
		std::uniform_real_distribution<Float>* m_distribution;
	};
}