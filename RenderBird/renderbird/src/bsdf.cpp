#include "bsdf.h"
#include "sample_utils.h"

namespace RenderBird
{
	void DiffuseBSDF::Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, BsdfSpectrum* bs)
	{
		float cos_pi = fmaxf(Dot(ss->m_normal, wi), 0.0f) * C_1_INV_PI;
		*pdf = cos_pi;
		bs->Add(m_color * cos_pi);
	}

	void DiffuseBSDF::Sample(SurfaceSample* ss, const Vector2f rand2d, Float* pdf, Vector3f* wi, RGB32* weight)
	{
		SampleUtils::CosHemisphere(ss->m_normal, rand2d, wi, pdf);
		//auto v = ReflectVector(*wi, ss->m_normal);
		if (Dot(ss->m_geomNormal, *wi) > 0.0f)
		{
			*weight = RGB32(*pdf, *pdf, *pdf);
		}
		else
		{
			*pdf = 0.0f;
		}
	}
}