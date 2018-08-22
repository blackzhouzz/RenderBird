#pragma once
#include "renderbird_private.h"
#include "sampler.h"
#include "pathtracing_common.h"

namespace RenderBird
{
	class Renderer;
	class TileRenderer;

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
			CameraSample m_cameraSample;
			int m_curSamplerIndex;
		};
		PathTracing(Renderer* renderer);
		void Render(int pixelX, int pixelY, TileRenderer* tile);
	private:
		void InitState(int pixelX, int pixelY, State* state);
		void InitSurfaceData(State* state, SurfaceSample* ss, const Ray& ray, const RayHitInfo& hitInfo);
		bool SurfaceBounce(State* state, SurfaceSample* ss, Ray& ray);
		void Integrate(State* state, Radiance* L);
		bool SampleLight(State* state, SurfaceSample* ss, Radiance* L);
		void EvalBSDF(State* state, SurfaceSample* ss, const Vector3f& wi, Float* pdf, BsdfSpectrum* bs);
		void AccumRadianceSpectrum(State* state, BsdfSpectrum* bs, Radiance* L);
		void AccumBSDFSpectrum(SurfaceSample* ss, BsdfSpectrum* bs, const RGB32& value);
		bool ProbabilityStop();
	private:
		Renderer* m_renderer;
	};

}