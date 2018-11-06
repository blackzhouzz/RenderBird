#pragma once
#include "RenderbirdPrivate.h"
#include "sampler.h"
#include "PathTracingCommon.h"

namespace RenderBird
{
	class Renderer;
	class TileRenderer;
	class Light;
	class PathTracing
	{
	public:
		struct State
		{
			int m_pixelX;
			int m_pixelY;
			int m_currentBounce;
			RGB32 m_throughput;
			Sampler* m_sampler;
			CameraSample m_cameraSample;
			int m_curSamplerIndex;
			bool m_useMis;
		};
		PathTracing(Renderer* renderer);
		void Render(int pixelX, int pixelY, TileRenderer* tile);
	private:
		void Integrate(State* state, Radiance* L);
		RGB32 EvalDirect(State* state, SurfaceSample* ss);
		RGB32 SampleLight(State* state, Light* light, SurfaceSample* ss);
		RGB32 SampleBSDF(State* state, Light* light, SurfaceSample* ss);
		RGB32 EvalSample(State* state, Light* light, SurfaceSample* ss);
		RGB32 EvalLightAtten(State* state, Light* light, SurfaceSample* ss, const Ray& ray, RayHitInfo& hitInfo);
		Light* GetSampleLight(State* state, Float& sampleLightPdf);
	private:
		Renderer* m_renderer;
	};

}