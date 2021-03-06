#include "OrenNayarDiffuse.h"
#include "SampleUtils.h"

namespace RenderBird
{
	OrenNayarDiffuse::OrenNayarDiffuse(Float roughness)
		: m_roughness(roughness)
	{
		const Float sigma = 1.0f / std::sqrt(2.0f) * m_roughness;
		m_sigmaSq = sigma * sigma;
		m_flags = DiffuseReflection;
	}

	RGB32 OrenNayarDiffuse::EvalSpectrum(const Vector3f& localWi, const Vector3f& localWo)
	{
		Float thetaR = std::acos(localWi.z);
		Float thetaI = std::acos(localWo.z);
		Float alpha = Max(thetaR, thetaI);
		Float beta = Min(thetaR, thetaI);
		Float sinAlpha = std::sin(alpha);
		Float denom = (localWo.x * localWo.x + localWo.y * localWo.y) * (localWi.x * localWi.x + localWi.y * localWi.y);
		Float cosDeltaPhi;
		if (denom == 0.0f)
			cosDeltaPhi = 1.0f;
		else
			cosDeltaPhi = (localWo.x * localWi.x + localWo.y * localWi.y) / std::sqrt(denom);

		Float C1 = 1.0f - 0.5f * m_sigmaSq / (m_sigmaSq + 0.33f);
		Float C2 = 0.45f * m_sigmaSq / (m_sigmaSq + 0.09f);
		if (cosDeltaPhi >= 0.0f)
			C2 *= sinAlpha;
		else
			C2 *= sinAlpha - Cube((2.0f * C_1_INV_PI) * beta);
		Float C3 = 0.125f * (m_sigmaSq / (m_sigmaSq + 0.09f)) * Square((4.0f * C_1_INV_PI * C_1_INV_PI) * alpha * beta);

		Float fr1 = (C1 + cosDeltaPhi * C2 * std::tan(beta) + (1.0f - std::abs(cosDeltaPhi)) * C3 * std::tan(0.5f * (alpha + beta)));
		Float fr2 = 0.17f * m_sigmaSq / (m_sigmaSq + 0.13f) * (1.0f - cosDeltaPhi * Square((2.0f * C_1_INV_PI) * beta));

		auto albedo = Albedo();
		return (albedo * fr1 + albedo * albedo * fr2) * CosTheta(localWi) * C_1_INV_PI;
	}

	RGB32 OrenNayarDiffuse::Eval(SurfaceSample* ss)
	{
		auto localWi = ss->m_wi;
		auto localWo = ss->m_wo;

		if (localWo.z <= 0 || localWi.z <= 0)
		{
			return RGB32::BLACK;
		}

		Float ratio = Clamp(m_roughness, 0.01, 1.0);

		ss->m_pdf = Lerp(SampleUtils::CosHemispherePdf(localWi.z), SampleUtils::UniformHemispherePdf(), ratio);

		return EvalSpectrum(localWi, localWo);
	}

	bool OrenNayarDiffuse::Sample(SurfaceSample* ss, Sampler* sampler, RGB32& weight)
	{
		auto localWo = ss->m_wo;

		if (localWo.z <= 0)
			return false;
		Float ratio = Clamp(m_roughness, 0.01, 1.0);
		if (sampler->Next1D() < ratio)
		{
			SampleUtils::UniformHemisphere(sampler->Next2D(), &ss->m_wi, &ss->m_pdf);
		}
		else
		{
			SampleUtils::CosHemisphere(sampler->Next2D(), &ss->m_wi, &ss->m_pdf);
		}

		ss->m_pdf = Lerp(SampleUtils::CosHemispherePdf(ss->m_wi.z), SampleUtils::UniformHemispherePdf(), ratio);

		weight = EvalSpectrum(ss->m_wi, localWo);

		return true;
	}
}