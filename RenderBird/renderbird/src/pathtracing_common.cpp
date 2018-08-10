#include "pathtracing_common.h"

namespace RenderBird
{
	void PathTracingUtils::InitRadiance(Radiance* L)
	{
		L->m_directDiffuse = RGB32::BLACK;
		//L->m_indirect = RGB32::BLACK;
	}

	RGB32 PathTracingUtils::EvalEmissive(const Vector3f& geomNormal, const Vector3f& dir, const RGB32& lightColor)
	{
		Float weight = SampleHemisphere(geomNormal, dir) ? 1.0f : 0.0f;
		return lightColor * weight;
	}

	bool PathTracingUtils::SampleHemisphere(const Vector3f& w1, const Vector3f& w2)
	{
		return Dot(w1, w2) > 0.0f;
	}
}