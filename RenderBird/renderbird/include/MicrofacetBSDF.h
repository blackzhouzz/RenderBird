#pragma once
#include "RenderBirdPrivate.h"
#include "BSDF.h"
#include "MicrofacetDistribution.h"

namespace RenderBird
{
	class MicrofacetConductorReflection : public BSDF
	{
	public:
		MicrofacetConductorReflection(Float roughnessU, Float roughnessV);
		virtual bool Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum);
		bool EvalSpectrum(const Vector3f& localWi, const Vector3f& localWo, const Vector3f& wh, Float* pdf, LightSpectrum* lightSpectrum);
	private:
		Float m_roughnessU;
		Float m_roughnessV;
		std::unique_ptr<MicrofacetDistribution> m_distribution;
		Float m_alpha;
		RGB32 m_eta;
		RGB32 m_k;
	};
}