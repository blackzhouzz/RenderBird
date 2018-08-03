#pragma once
#include "renderbird_private.h"
#include "math/ray.h"
#include "math/spectrum.h"
#include "rayhitinfo.h"
#include "sampler.h"

namespace RenderBird
{
	struct Radiance
	{
		RGBA32 m_diffuse;
		RGBA32 m_specular;
	};

	struct SurfaceData
	{
		Vector3f m_position;
		Vector3f m_geomNormal;
		Vector3f m_normal;
		Vector3f m_wo;
		Material* m_material;
		bool m_isBackfacing;
	};

	class Renderer;

	class PathTracing
	{
	public:
		struct State
		{
			int m_pixelX;
			int m_pixelY;
			int m_bounceCount;
			Ray m_cameraRay;
			RGB32 m_throughtPut;
		};
		struct Setting
		{
			int m_maxBounce;
			int m_numSamples;
		};
		PathTracing(Renderer* renderer);
		void Render(int pixelX, int pixelY, Radiance& Lo);
	private:
		void InitState(int pixelX, int pixelY, State& state);
		void InitSurfaceData(SurfaceData& surfData, const Ray& ray, const RayHitInfo& hitInfo);
		bool SurfaceBounce(State& state, Ray& ray);
		void Integrate(State& state, Radiance& Lo);
		void SurfaceLighting(State& state);
		bool ProbabilityTerminate(State& state);
	private:
		Renderer* m_renderer;
		Setting m_setting;
		Sampler* m_sampler;
	};

}