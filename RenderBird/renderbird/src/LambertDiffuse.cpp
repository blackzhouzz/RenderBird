#include "LambertDiffuse.h"
#include "SampleUtils.h"

namespace RenderBird
{
	LambertDiffuse::LambertDiffuse()
	{
		m_flags = DiffuseReflection;
	}

	bool LambertDiffuse::Eval(SurfaceSample* ss, LightSpectrum* lightSpectrum)
	{
		auto localWi = ss->m_wi;
		if (ss->m_wo.z <= 0 || localWi.z <= 0)
		{
			ss->m_pdf = 0;
			return false;
		}
		Float cosPi = SampleUtils::CosHemispherePdf(localWi.z);
		ss->m_pdf = cosPi;

		lightSpectrum->m_diffuse = Albedo() * cosPi;
		return true;
	}

	bool LambertDiffuse::Sample(SurfaceSample* ss, Sampler* sampler, LightSpectrum* lightSpectrum)
	{
		if (ss->m_wo.z <= 0)
			return false;
		SampleUtils::CosHemisphere(sampler->Next2D(), &ss->m_wi, &ss->m_pdf);

		lightSpectrum->m_diffuse = Albedo() * ss->m_pdf;
		return true;
	}
}