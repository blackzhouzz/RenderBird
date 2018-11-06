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
		virtual RGB32 Eval(SurfaceSample* ss);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, RGB32& weight);
		RGB32 EvalSpectrum(const Vector3f& localWi, const Vector3f& localWo, const Vector3f& wh, Float* pdf);
	private:
		Float m_roughnessU;
		Float m_roughnessV;
		std::unique_ptr<MicrofacetDistribution> m_distribution;
		Float m_alpha;
		RGB32 m_eta;
		RGB32 m_k;
	};
}