#include "sampler.h"
#include "random/random.hpp"

namespace RenderBird
{
	using Random = effolkronium::random_static;

	Sampler::Sampler(uint32 numSamplers)
		: m_numSamplers(numSamplers)
	{
	}

	Float Sampler::Random1D()
	{
		return Random::get(0.0, 1.0);
	}

	Vector2f Sampler::Random2D()
	{
		return Vector2f(Random::get(0.0, 1.0), Random::get(0.0, 1.0));
	}
}