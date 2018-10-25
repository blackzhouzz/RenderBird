#include "MicrofacetBSDF.h"

namespace RenderBird
{
	MicrofacetConductorReflection::MicrofacetConductorReflection(Float roughnessU, Float roughnessV)
		: m_roughnessU(roughnessU)
		, m_roughnessV(roughnessV)
		, m_distribution(new MicrofacetDistribution(MicrofacetDistribution::Type::GGX, roughnessU, roughnessV))
		, m_eta(0.200438f, 0.924033f, 1.10221f)
		, m_k(3.91295f, 2.45285f, 2.14219f)
	{
	}

	bool MicrofacetConductorReflection::EvalSpectrum(const Vector3f& localWi, const Vector3f& localWo, const Vector3f& wh, Float* pdf, LightSpectrum* lightSpectrum)
	{
		auto woDotwh = Vector3f::DotProduct(localWo, wh);

		if (woDotwh <= 0.0f || localWi.z <= 0.0f || localWo.z <= 0.0)
			return false;

		Float cosThetaO = localWo.z;
		auto F = Fresnel::Conductor(m_eta, m_k, Vector3f::DotProduct(localWi, wh));
		auto G = m_distribution->G(localWi, localWo, wh);
		auto D = m_distribution->D(wh);

		lightSpectrum->m_diffuse = Albedo() * 0.25 * D * G * F / cosThetaO;

		*pdf = m_distribution->Pdf(wh) * 0.25 / woDotwh;
		return true;
	}

	bool MicrofacetConductorReflection::Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum)
	{
		auto localWi = WorldToLocal(wi);
		auto localWo = ss->m_localWo;
		if (localWo.z <= 0 || localWi.z <= 0)
			return false;
		Vector3f wh = (localWi + localWo).Normalized();
		return EvalSpectrum(localWi, localWo, wh, pdf, lightSpectrum);
	}

	bool MicrofacetConductorReflection::Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum)
	{
		auto localWo = ss->m_localWo;
		if (localWo.z <= 0)
			return false;
		auto wh = m_distribution->Sample(sampler->Next2D());
		auto woDotwh = Vector3f::DotProduct(localWo, wh);
		auto localWi = (2.0 * woDotwh * wh - localWo).Normalized();

		*wi = localWi;

		return EvalSpectrum(localWi, localWo, wh, pdf, lightSpectrum);
	}
}