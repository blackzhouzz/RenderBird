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
			bool m_useMis;
		};
		PathTracing(Renderer* renderer);
		void Render(int pixelX, int pixelY, TileRenderer* tile);
	private:
		void InitState(int pixelX, int pixelY, State* state);
		void Integrate(State* state, Radiance* L);
		bool SampleLight(State* state, SurfaceSample* ss, Radiance* L);
		void AccumRadiance(State* state, BsdfSpectrum* bs, Radiance* L);
		bool ProbabilityStop();
	private:
		Renderer* m_renderer;
	};

}