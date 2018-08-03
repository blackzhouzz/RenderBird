#pragma once
#include "renderbird_private.h"

namespace RenderBird
{
	class Sampler
	{
	public:
		Sampler(uint32 numSamplers);
		Float Random1D();
		Vector2f Random2D();
	private:
		uint32 m_numSamplers;
	};
}