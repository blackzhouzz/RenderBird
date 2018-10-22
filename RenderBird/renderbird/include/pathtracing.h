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
		bool SampleLight(State* state, Light* light, Float sampleLightPdf, SurfaceSample* ss, Radiance* L);
		bool SampleBSDF(State* state, Light* light, Float sampleLightPdf, SurfaceSample* ss, Radiance* L);
		bool BacksideCheck(const Vector3f& ng, const Vector3f& w);
		Light* GetSampleLight(State* state, Float& sampleLightPdf);
	private:
		Renderer* m_renderer;
	};

}