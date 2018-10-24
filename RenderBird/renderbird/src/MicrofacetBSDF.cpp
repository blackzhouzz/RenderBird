#include "MicrofacetBSDF.h"

namespace RenderBird
{
	static Float ConductorReflectance(Float eta, Float k, Float cosThetaI)
	{
		Float cosThetaISq = cosThetaI * cosThetaI;
		Float sinThetaISq = Max(1.0 - cosThetaISq, 0.0);
		Float sinThetaIQu = sinThetaISq * sinThetaISq;

		Float innerTerm = eta * eta - k * k - sinThetaISq;
		Float aSqPlusBSq = std::sqrt(Max(innerTerm*innerTerm + 4.0*eta*eta*k*k, 0.0));
		Float a = std::sqrt(Max((aSqPlusBSq + innerTerm)*0.5, 0.0));

		Float Rs = ((aSqPlusBSq + cosThetaISq) - (2.0*a*cosThetaI)) /
			((aSqPlusBSq + cosThetaISq) + (2.0*a*cosThetaI));
		Float Rp = ((cosThetaISq*aSqPlusBSq + sinThetaIQu) - (2.0*a*cosThetaI*sinThetaISq)) /
			((cosThetaISq*aSqPlusBSq + sinThetaIQu) + (2.0*a*cosThetaI*sinThetaISq));

		return 0.5f*(Rs + Rs * Rp);
	}

	RGB32 Fresnel::Conductor(const RGB32 &eta, const RGB32 &k, Float cosThetaI)
	{
		return RGB32(
			ConductorReflectance(eta[0], k[0], std::abs(cosThetaI)),
			ConductorReflectance(eta[1], k[1], std::abs(cosThetaI)),
			ConductorReflectance(eta[2], k[2], std::abs(cosThetaI))
		);
	}

	Distribution::Distribution(Type type)
		: m_type(type)
	{
	}

	//https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/
	Vector3f Distribution::Sample(const Vector2f& rand2d, Float alpha)
	{
		Float phi = rand2d.y * C_2_PI;
		Float cosTheta = 0.0f;

		switch (m_type)
		{
		case Beckmann: {
			cosTheta = 1.0f / std::sqrt(1.0f - alpha * alpha * std::log(1.0f - rand2d.x));
			break;
		} 
		case Phong:
			cosTheta = Float(std::pow(double(rand2d.x), 1.0 / (double(alpha) + 2.0)));
			break;
		case GGX:
			cosTheta = std::sqrt((1 - rand2d.x) / (rand2d.x * (alpha * alpha - 1.0) + 1));
			break;
		}

		Float sinTheta = std::sqrt(Max(1.0 - cosTheta * cosTheta, 0.0));
		return Vector3f(std::cos(phi) * sinTheta, std::sin(phi) * sinTheta, cosTheta);
	}

	//<Microfacet Models for Refraction through Rough Surfaces>
	Float Distribution::D(Float alpha, const Vector3f& wh)
	{
		if (wh.z <= 0.0f)
			return 0.0f;

		switch (m_type)
		{
		case Beckmann: {
			Float alphaSq = alpha * alpha;
			Float cosThetaSq = wh.z * wh.z;
			Float tanThetaSq = Max(1.0 - cosThetaSq, 0.0) / cosThetaSq;
			Float cosThetaQu = cosThetaSq * cosThetaSq;
			return C_1_INV_PI * std::exp(-tanThetaSq / alphaSq) / (alphaSq*cosThetaQu);
		}
		case Phong:
			return (alpha + 2.0f) * 0.5 * C_1_INV_PI * Float(std::pow(double(wh.z), double(alpha)));
		case GGX: {
			Float alphaSq = alpha * alpha;
			Float cosThetaSq = wh.z * wh.z;
			Float tanThetaSq = Max(1.0 - cosThetaSq, 0.0) / cosThetaSq;
			Float cosThetaQu = cosThetaSq * cosThetaSq;
			return alphaSq * C_1_INV_PI / (cosThetaQu * Square(alphaSq + tanThetaSq));
		}
		}

		return 0.0f;
	}

	//<Microfacet Models for Refraction through Rough Surfaces>
	Float Distribution::G1(Float alpha, const Vector3f &v, const Vector3f &wh)
	{
		if (Vector3f::DotProduct(v, wh) * v.z <= 0.0f)
			return 0.0f;

		switch (m_type)
		{
		case Beckmann: {
			Float cosThetaSq = v.z * v.z;
			Float tanTheta = std::abs(std::sqrt(Max(1.0 - cosThetaSq, 0.0)) / v.z);
			Float a = 1.0f / (alpha*tanTheta);
			if (a < 1.6f)
				return (3.535f * a + 2.181f * a * a) / (1.0f + 2.276f*a + 2.577f*a*a);
			else
				return 1.0f;
		} case Phong: {
			Float cosThetaSq = v.z * v.z;
			Float tanTheta = std::abs(std::sqrt(Max(1.0 - cosThetaSq, 0.0)) / v.z);
			Float a = std::sqrt(0.5f*alpha + 1.0f) / tanTheta;
			if (a < 1.6f)
				return (3.535f*a + 2.181f*a*a) / (1.0f + 2.276f*a + 2.577f*a*a);
			else
				return 1.0f;
		}
		case GGX: {
			Float alphaSq = alpha * alpha;
			Float cosThetaSq = v.z * v.z;
			Float tanThetaSq = Max(1.0 - cosThetaSq, 0.0) / cosThetaSq;
			return 2.0f / (1.0f + std::sqrt(1.0f + alphaSq * tanThetaSq));
		}
		}

		return 0.0f;
	}

	Float Distribution::G(Float alpha, const Vector3f& wi, const Vector3f& wo, const Vector3f &wh)
	{
		return G1(alpha, wi, wh) * G1(alpha, wo, wh);
	}

	Float Distribution::RoughnessToAlpha(Float roughness)
	{
		const Float MinAlpha = 1e-3f;
		roughness = Max(roughness, MinAlpha);

		if (m_type == Type::Phong)
		{
			return 2.0f / (roughness*roughness) - 2.0f;
		}
		else
		{
			return roughness;
		}
	}

	Float Distribution::Pdf(Float alpha, const Vector3f &wh)
	{
		return D(alpha, wh) * wh.z;
	}

	MicrofacetConductorReflection::MicrofacetConductorReflection(Float roughness)
		: m_roughness(roughness)
		, m_distribution(new Distribution(Distribution::Type::GGX))
		, m_eta(0.200438f, 0.924033f, 1.10221f)
		, m_k(3.91295f, 2.45285f, 2.14219f)
	{
		m_alpha = m_distribution->RoughnessToAlpha(roughness);
	}

	bool MicrofacetConductorReflection::EvalSpectrum(const Vector3f& localWi, const Vector3f& localWo, const Vector3f& wh, Float* pdf, LightSpectrum* lightSpectrum)
	{
		auto woDotwh = Vector3f::DotProduct(localWo, wh);

		if (woDotwh <= 0.0f || localWi.z <= 0.0f || localWo.z <= 0.0)
			return false;

		Float cosThetaO = localWo.z;
		auto F = Fresnel::Conductor(m_eta, m_k, Vector3f::DotProduct(localWi, wh));
		auto G = m_distribution->G(m_alpha, localWi, localWo, wh);
		auto D = m_distribution->D(m_alpha, wh);

		lightSpectrum->m_diffuse = Albedo() * 0.25 * D * G * F / cosThetaO;

		*pdf = m_distribution->Pdf(m_alpha, wh) * 0.25 / woDotwh;
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
		auto wh = m_distribution->Sample(sampler->Next2D(), m_alpha);
		auto woDotwh = Vector3f::DotProduct(localWo, wh);
		auto localWi = (2.0 * woDotwh * wh - localWo).Normalized();

		*wi = localWi;

		return EvalSpectrum(localWi, localWo, wh, pdf, lightSpectrum);
	}
}