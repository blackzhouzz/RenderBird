#include "DisneyBSDF.h"
#include "MicrofacetDistribution.h"
#include "SampleUtils.h"

namespace RenderBird
{
	DisneyDiffuse::DisneyDiffuse(Float roughness)
		: m_roughness(roughness)
	{
	}

	RGB32 DisneyDiffuse::EvalSpectrum(const Vector3f& localWi, const Vector3f& localWo)
	{
		auto wh = (localWo + localWi).Normalized();
		auto woDotwh = Vector3f::DotProduct(localWo, wh);
		Float fd90 = 0.5 + 2 * m_roughness * woDotwh * woDotwh;
		Float fl = Fresnel::Schlick(CosTheta(localWo));
		Float fv = Fresnel::Schlick(CosTheta(localWi));
		Float t = (1 + (fd90 - 1) * fl) * (1 + (fd90 - 1) * fv);
		auto albedo = Albedo();
		return albedo * t * CosTheta(localWi) * C_1_INV_PI;
	}

	bool DisneyDiffuse::Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum)
	{
		auto localWi = WorldToLocal(wi);
		auto localWo = ss->m_localWo;

		if (localWo.z <= 0 || localWi.z <= 0)
		{
			return false;
		}

		Float cosPi = SampleUtils::CosHemispherePdf(localWi.z);
		*pdf = cosPi;

		lightSpectrum->m_diffuse = EvalSpectrum(localWi, localWo);
		return true;
	}

	bool DisneyDiffuse::Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum)
	{
		auto localWo = ss->m_localWo;

		if (localWo.z <= 0)
			return false;

		SampleUtils::CosHemisphere(sampler->Next2D(), wi, pdf);

		lightSpectrum->m_diffuse = EvalSpectrum(*wi, localWo);

		return true;
	}
}