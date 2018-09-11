#include "bsdf.h"
#include "sample_utils.h"

namespace RenderBird
{
	void DiffuseBSDF::Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, RGB32* eval)
	{
		auto localWi = WorldToLocal(wi);
		Float cos_pi = std::abs(localWi.z) * C_1_INV_PI;
		*pdf = cos_pi;
		*eval = m_color * cos_pi;
		if (!PathTracingUtils::IsSameHemisphere(ss->m_ng, wi))
		{
			*pdf = 0.0f;
		}
	}

	void DiffuseBSDF::Sample(SurfaceSample* ss, const Vector2f& rand2d, Vector3f* wi, Float* pdf, RGB32* bsdfWeight)
	{
		auto woLocal = WorldToLocal(ss->m_wo);
		SampleUtils::CosHemisphere(rand2d, wi, pdf);
		if (woLocal.z < 0) wi->z *= -1;

		*wi = LocalToWorld(*wi);
		*bsdfWeight = m_color * (*pdf);
		if (!PathTracingUtils::IsSameHemisphere(ss->m_ng, *wi))
		{
			*pdf = 0.0f;
		}
	}
}