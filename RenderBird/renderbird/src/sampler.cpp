#include "sampler.h"
#include "RenderStatistic.h"

namespace RenderBird
{
	Sampler::Sampler(uint32 numSamplers)
		: m_numSamplers(numSamplers)
		, m_sampleIndex(0)
		, m_pixelX(0)
		, m_pixelY(0)
		, m_currentDim(0)

	{
	}

	Sampler::~Sampler()
	{
	}

	void Sampler::Init(uint32 resX, uint32 resY)
	{
		m_resX = resX;
		m_resY = resY;
		m_currentDim = 0;
	}

	void Sampler::StartSample(uint32 pixelX, uint32 pixelY)
	{
		m_pixelX = pixelX;
		m_pixelY = pixelY;
		m_sampleIndex = 0;
	}

	void Sampler::NextSample()
	{
		m_sampleIndex++;
		m_currentDim = 0;
	}

	Vector2f Sampler::Next2D()
	{
		return Vector2f(Next1D(), Next1D());
	}

	CameraSample Sampler::GetCameraSample(int pixelX, int pixelY)
	{
		CameraSample ret;
		Vector2f rand2d = Next2D();
		ret.m_pixel = Vector2f(pixelX + rand2d.x, pixelY + rand2d.y);

		return ret;
	}

	std::random_device RandomSampler::m_rd;

	RandomSampler::RandomSampler(uint32 numSamplers)
		: Sampler(numSamplers)
	{
		m_gen = new std::mt19937(m_rd());
		m_distribution = new std::uniform_real_distribution<Float>(0.0f, 1.0f);
	}

	RandomSampler::~RandomSampler()
	{
		delete m_gen;
		delete m_distribution;
	}

	Float RandomSampler::Next1D()
	{
		return (*m_distribution)(*m_gen);
	}

	int random(int i) { return std::rand() % i; }
	Halton_sampler* HaltonSampler::sHalton_sampler = nullptr;
	Halton_enum* HaltonSampler::sHalton_enum = nullptr;
	bool HaltonSampler::m_useFaurePermutation = false;

	Halton_sampler* HaltonSampler::GetHalton_sampler(uint32 resX, uint32 resY)
	{
		if (sHalton_sampler == nullptr)
		{
			sHalton_sampler = new Halton_sampler();
			if (m_useFaurePermutation)
			{
				sHalton_sampler->init_faure();
			}
			else
			{
				sHalton_sampler->init_random(random);
			}
			sHalton_enum = new Halton_enum(resX, resY);
		}
		return sHalton_sampler;
	}

	HaltonSampler::HaltonSampler(uint32 numSamplers)
		: Sampler(numSamplers)
	{
	}

	void HaltonSampler::Init(uint32 resX, uint32 resY)
	{
		Sampler::Init(resX, resY);
		GetHalton_sampler(resX, resY);
	}

	Float HaltonSampler::Next1D()
	{
		int dim = m_currentDim++;
		unsigned index = sHalton_enum->get_index(m_sampleIndex, m_pixelX, m_pixelY);
		if (dim == 0)
			index /= sHalton_enum->get_scale_x();
		else if (dim == 1)
			index /= sHalton_enum->get_scale_y();
		return GetHalton_sampler(m_resX, m_resY)->sample(dim, index);
	}

}