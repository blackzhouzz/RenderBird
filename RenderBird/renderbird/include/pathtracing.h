#pragma once
#include "math/ray.h"
#include "math/spectrum.h"
using namespace Core;

namespace RenderBird
{
	struct Radiance
	{

	};

	struct SurfaceData
	{

	};

	struct PathTracingState
	{
		int m_bounceCount;
		RGB32 m_throughtPut;
	};

	class PathTracing
	{
	public:
		void Integrate(Radiance& Lo);
	private:
		void Pass0_Preprocess(int x, int y, Ray& ray) {}
		bool Pass1_SceneIntersect() {}
		void Pass2_ShaderSetup() {}
		void Pass3_SurfaceSetup() {}
		void Pass4_DirectLighting() {}
		void Pass5_SurfaceBounce() {}
		void Pass6_Output() {}
	};

}