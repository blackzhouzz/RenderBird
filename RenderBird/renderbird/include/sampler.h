#pragma once
#include "RenderbirdPrivate.h"
#include "PathTracingCommon.h"
#include <random>
#include "halton_sampler.h"
#include "halton_enum.h"

namespace RenderBird
{
	enum SamplerDimension
	{
		CameraU = 0,
		CameraV = 1,
		CameraLenU = 2,
		CameraLenV = 3,
		RussianRoulette = 4,
		LightIndex = 5,
		LightSampleU = 6,
		LightSampleV = 7,
		BSDFSampleU = 8,
		BSDFSampleV = 9
	};

	class Sampler
	{
	public:
		Sampler(uint32 numSamplers);
		~Sampler();
		virtual void Init(uint32 resX, uint32 resY);
		virtual void StartSample(uint32 pixelX, uint32 pixelY);
		virtual void NextSample();
		virtual Float Get1D(int dim);
		virtual Vector2f Get2D(int dimU, int dimV);
		Vector2f Get2DRandom(int dimU, int dimV)const;
		CameraSample GetCameraSample(int pixelX, int pixelY);
		uint32 GetNumSamplers()const { return m_numSamplers; }
		uint32 GetSampleIndex()const { return m_sampleIndex; }
		virtual Sampler* Clone() { return new Sampler(m_numSamplers); }
	protected:
		uint32 m_sampleIndex;
		uint32 m_numSamplers;
		static std::random_device m_rd;
		std::mt19937* m_gen;
		std::uniform_real_distribution<Float>* m_distribution;
		uint32 m_pixelX;
		uint32 m_pixelY;
		uint32 m_resX;
		uint32 m_resY;
		uint32 m_currentDim;
	};

	class HaltonSampler : public Sampler
	{
	public:
		HaltonSampler(uint32 numSamplers);
		virtual void Init(uint32 resX, uint32 resY);
		virtual Float Get1D(int dim);
		virtual Vector2f Get2D(int dimU, int dimV);
		virtual Sampler* Clone() { return new HaltonSampler(m_numSamplers); }
	private:
		static Halton_sampler* sHalton_sampler;
		static Halton_enum* sHalton_enum;
		static Halton_sampler* GetHalton_sampler(uint32 resX, uint32 resY);
	private:
		static bool m_useFaurePermutation;
	};
}