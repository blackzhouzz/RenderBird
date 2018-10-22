#include "LambertDiffuse.h"
#include "SampleUtils.h"

namespace RenderBird
{
	LambertDiffuse::LambertDiffuse()
	{
	}

	void LambertDiffuse::Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum)
	{
		auto localWi = WorldToLocal(wi);
		Float cosPi = SampleUtils::CosHemispherePdf(localWi.z);
		*pdf = cosPi;
		//if (ss->m_localWo.z < 0)
		//{
		//	*pdf = 0;
		//	return;
		//}
		lightSpectrum->m_diffuse = m_color * cosPi;
	}

	void LambertDiffuse::Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum)
	{
		SampleUtils::CosHemisphere(sampler->Next2D(), wi, pdf);
		//if (ss->m_localWo.z < 0)
		//	wi->z *= -1;
		//if (ss->m_localWo.z < 0)
		//{
		//	*pdf = 0;
		//	return;
		//}
		lightSpectrum->m_diffuse = m_color * (*pdf);
	}
}