#include "sampler.h"
#include "random/random.hpp"

namespace RenderBird
{
	using Random = effolkronium::random_static;

	Sampler::Sampler(uint32 numSamplers)
		: m_numSamplers(numSamplers)
	{
	}

	Float Sampler::Random1D()const
	{
		return Random::get(0.0f, 1.0f);
	}

	Vector2f Sampler::Random2D()const
	{
		return Vector2f(Random::get(0.0f, 1.0f), Random::get(0.0f, 1.0f));
	}

	CameraSample Sampler::GetCameraSample(int pixelX, int pixelY)const
	{
		CameraSample ret;
		Vector2f rand2d = Random2D();
		ret.m_pixel = Vector2f(pixelX + rand2d.x, pixelY + rand2d.y);
		return ret;
	}
}