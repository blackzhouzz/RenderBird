#include "pathtracing_common.h"

namespace RenderBird
{
	RGB32 PathTracingUtils::EvalEmissive(const Vector3f& geomNormal, const Vector3f& dir, const RGB32& lightColor)
	{
		Float weight = SampleHemisphere(geomNormal, dir) ? 1.0f : 0.0f;
		return lightColor * weight;
	}

	bool PathTracingUtils::SampleHemisphere(const Vector3f& w1, const Vector3f& w2)
	{
		return Vector3f::DotProduct(w1, w2) > 0.0f;
	}

	Float PathTracingUtils::GammaCorrect(Float value)
	{
		if (value <= 0.0031308f) 
			return 12.92f * value;
		return 1.055f * std::pow(value, (Float)(1.f / 2.4f)) - 0.055f;
	}
}