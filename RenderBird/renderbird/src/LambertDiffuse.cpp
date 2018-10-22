#include "LambertDiffuse.h"
#include "SampleUtils.h"

namespace RenderBird
{
	LambertDiffuse::LambertDiffuse()
	{
	}

	bool LambertDiffuse::Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum)
	{
		auto localWi = WorldToLocal(wi);
		if (ss->m_localWo.z <= 0 || localWi.z <= 0)
		{
			*pdf = 0;
			return false;
		}
		Float cosPi = SampleUtils::CosHemispherePdf(localWi.z);
		*pdf = cosPi;

		lightSpectrum->m_diffuse = m_color * cosPi;
		return true;
	}

	bool LambertDiffuse::Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum)
	{
		SampleUtils::CosHemisphere(sampler->Next2D(), wi, pdf);
		if (ss->m_localWo.z <= 0)
			return false;

		lightSpectrum->m_diffuse = m_color * (*pdf);
		return true;
	}
}