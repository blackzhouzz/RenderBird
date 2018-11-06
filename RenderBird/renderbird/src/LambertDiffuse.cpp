#include "LambertDiffuse.h"
#include "SampleUtils.h"

namespace RenderBird
{
	LambertDiffuse::LambertDiffuse()
	{
		m_flags = DiffuseReflection;
	}

	RGB32 LambertDiffuse::Eval(SurfaceSample* ss)
	{
		auto localWi = ss->m_wi;
		if (ss->m_wo.z <= 0 || localWi.z <= 0)
		{
			ss->m_pdf = 0;
			return RGB32::BLACK;
		}
		Float cosPi = SampleUtils::CosHemispherePdf(localWi.z);
		ss->m_pdf = cosPi;

		return Albedo() * cosPi;
	}

	bool LambertDiffuse::Sample(SurfaceSample* ss, Sampler* sampler, RGB32& weight)
	{
		if (ss->m_wo.z <= 0)
			return false;
		SampleUtils::CosHemisphere(sampler->Next2D(), &ss->m_wi, &ss->m_pdf);

		weight = Albedo() * ss->m_pdf;
		return true;
	}
}