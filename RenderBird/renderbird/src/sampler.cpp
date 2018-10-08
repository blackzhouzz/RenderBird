#include "sampler.h"
#include "random/random.hpp"
#include "RenderStatistic.h"

namespace RenderBird
{
	std::random_device Sampler::m_rd;
	using Random = effolkronium::random_static;

	Sampler::Sampler(uint32 numSamplers)
		: m_numSamplers(numSamplers)

	{
		m_gen = new std::mt19937(m_rd());
		m_distribution = new std::uniform_real_distribution<Float>(0.0f, 1.0f);
	}

	Sampler::~Sampler()
	{
		delete m_gen;
		delete m_distribution;
	}

	Float Sampler::Random1D()const
	{
		//return 0;
		return (*m_distribution)(*m_gen);
		//return Random::get(0.0f, 1.0f);
	}

	Vector2f Sampler::Random2D()const
	{
		//return C_Zero_v2f;
		return Vector2f((*m_distribution)(*m_gen), (*m_distribution)(*m_gen));
		//return Vector2f(Random::get(0.0f, 1.0f), Random::get(0.0f, 1.0f));
	}

	CameraSample Sampler::GetCameraSample(int pixelX, int pixelY)const
	{
		CameraSample ret;
		Vector2f rand2d = Random2D();
		//rand2d = C_Zero_v2f;
		ret.m_pixel = Vector2f(pixelX + rand2d.x, pixelY + rand2d.y);

		return ret;
	}
}