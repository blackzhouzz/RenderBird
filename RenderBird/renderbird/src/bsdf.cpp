#include "bsdf.h"
#include "sample_utils.h"

namespace RenderBird
{
	void DiffuseBSDF::Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, RGB32* eval)
	{
		Float cos_pi = std::max(Vector3f::DotProduct(ss->m_n, wi), 0.0) * C_1_INV_PI;
		*pdf = cos_pi;
		*eval = m_color * cos_pi;
	}

	void DiffuseBSDF::Sample(SurfaceSample* ss, const Vector2f& rand2d, Vector3f* wi, Float* pdf, RGB32* sampleWeight)
	{
		auto woLocal = WorldToLocal(ss->m_wo);
		SampleUtils::CosHemisphere(rand2d, wi, pdf);
		if (woLocal.z < 0) wi->z *= -1;

		*wi = LocalToWorld(*wi);
		if (Vector3f::DotProduct(ss->m_ng, *wi) > 0.0f)
		{
			*sampleWeight = m_color * (*pdf);
		}
		else
		{
			*pdf = 0.0f;
		}
	}
}