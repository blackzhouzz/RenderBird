#pragma once
#include "renderbird_private.h"
#include "math/ray.h"
#include "math/spectrum.h"
#include "rayhitinfo.h"
#include "sampler.h"
#include "pathtracing_common.h"

namespace RenderBird
{
	class Renderer;

	class PathTracing
	{
	public:
		struct State
		{
			int m_pixelX;
			int m_pixelY;
			int m_currentBounce;
			RGB32 m_throughtput;
			Sampler* m_sampler;
			int m_curSamplerIndex;
		};
		struct Setting
		{
			int m_maxBounce;
			int m_numSamples;
		};
		PathTracing(Renderer* renderer);
		void Render(int pixelX, int pixelY);
	private:
		void InitState(int pixelX, int pixelY, State* state);
		void InitSurfaceData(State* state, SurfaceSample* ss, const Ray& ray, const RayHitInfo& hitInfo);
		bool SurfaceBounce(State* state, SurfaceSample* ss, Ray& ray);
		void Integrate(State* state, Radiance* L);
		void EvalLight(State* state, SurfaceSample* ss, Radiance* L);
		void EvalBSDF(State* state, SurfaceSample* ss, const Vector3f& wi, Float* pdf, BsdfSpectrum* bs);
		void AccumRadianceSpectrum(State* state, BsdfSpectrum* bs, Radiance* L);
		void AccumBSDFSpectrum(SurfaceSample* ss, BsdfSpectrum* bs, const RGB32& value);
		bool ProbabilityStop();
		//void WritePixelData();
	private:
		Renderer* m_renderer;
		Setting m_setting;
	};

}