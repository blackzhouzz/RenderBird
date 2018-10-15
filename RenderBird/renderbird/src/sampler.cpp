#include "sampler.h"
#include "random/random.hpp"
#include "RenderStatistic.h"

namespace RenderBird
{
	std::random_device Sampler::m_rd;
	using Random = effolkronium::random_static;

	Sampler::Sampler(uint32 numSamplers)
		: m_numSamplers(numSamplers)
		, m_sampleIndex(0)
		, m_pixelX(0)
		, m_pixelY(0)
		, m_currentDim(0)

	{
		m_gen = new std::mt19937(m_rd());
		m_distribution = new std::uniform_real_distribution<Float>(0.0f, 1.0f);
	}

	Sampler::~Sampler()
	{
		delete m_gen;
		delete m_distribution;
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

	Float Sampler::Get1D(int dim)
	{
		//return 0;
		return (*m_distribution)(*m_gen);
		//return Random::get(0.0f, 1.0f);
	}

	Vector2f Sampler::Get2D(int dimU, int dimV)
	{
		//return C_Zero_v2f;
		return Vector2f((*m_distribution)(*m_gen), (*m_distribution)(*m_gen));
		//return Vector2f(Random::get(0.0f, 1.0f), Random::get(0.0f, 1.0f));
	}

	Vector2f Sampler::Get2DRandom(int dimU, int dimV)const
	{
		return Vector2f((*m_distribution)(*m_gen), (*m_distribution)(*m_gen));
	}

	CameraSample Sampler::GetCameraSample(int pixelX, int pixelY)
	{
		CameraSample ret;
		Vector2f rand2d = Get2D(SamplerDimension::CameraU, SamplerDimension::CameraV);
		ret.m_pixel = Vector2f(pixelX + rand2d.x, pixelY + rand2d.y);

		return ret;
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

	Float HaltonSampler::Get1D(int dim)
	{
		dim = m_currentDim++;
		unsigned index = sHalton_enum->get_index(m_sampleIndex, m_pixelX, m_pixelY);
		if (dim == SamplerDimension::CameraU)
			index /= sHalton_enum->get_scale_x();
		else if (dim == SamplerDimension::CameraV)
			index /= sHalton_enum->get_scale_y();
		return GetHalton_sampler(m_resX, m_resY)->sample(dim, index);
	}

	Vector2f HaltonSampler::Get2D(int dimU, int dimV)
	{
		Vector2f ret;
		//auto val0 = Get1D(0);
		//auto val1 = Get1D(1);
		//auto val2 = Get1D(2);
		//auto val3 = Get1D(3);
		//auto val4 = Get1D(4);
		//auto val5 = Get1D(5);
		//auto val6 = Get1D(6);
		//auto val7 = Get1D(7);
		//auto val8 = Get1D(8);
		//auto val9 = Get1D(9);
		ret.x = Get1D(dimU);
		ret.y = Get1D(dimV);
		return ret;
	}
}