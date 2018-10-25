#include "MicrofacetDistribution.h"

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

	MicrofacetDistribution::MicrofacetDistribution(Type type, Float roughnessU, Float roughnessV)
		: m_type(type)
	{
		m_alphaU = RoughnessToAlpha(roughnessU);
		m_alphaV = RoughnessToAlpha(roughnessV);
	}

	Vector3f MicrofacetDistribution::Sample(const Vector2f& rand2d)
	{
		Float phi = rand2d.y * C_2_PI;
		Float cosTheta = 0.0f;
		Float cosPhi = 0;
		Float sinPhi = 0;
		SinCos(phi, sinPhi, cosPhi);

		switch (m_type)
		{
		case Beckmann: {
			if (IsAnisotropic())
			{
				phi = std::atan(m_alphaU / m_alphaV * std::tan(C_2_PI * rand2d.y)) + C_PI * std::floor(2 * rand2d.y + 0.5);
				SinCos(phi, sinPhi, cosPhi);
				cosTheta = std::cos(std::atan(std::log(rand2d.x) / (Square(cosPhi / m_alphaU) + Square(sinPhi / m_alphaV))));
			}
			else
			{
				cosTheta = 1.0f / std::sqrt(1.0f - m_alphaU * m_alphaU * std::log(1.0f - rand2d.x));
			}
			break;
		} 
		case Phong:
			if (IsAnisotropic())
			{
				phi = std::atan(std::sqrt((m_alphaU + 2.0) / (m_alphaV + 2.0)) * std::tan(C_2_PI * rand2d.y)) + C_PI * std::floor(2 * rand2d.y + 0.5);
				SinCos(phi, sinPhi, cosPhi);
				Float root = Square(cosPhi) * m_alphaU + Square(sinPhi) * m_alphaV + 2.0;
				cosTheta = std::pow(rand2d.x, 1.0 / root);
			}
			else
			{
				cosTheta = std::pow(double(rand2d.x), 1.0 / (double(m_alphaU) + 2.0));
			}
			break;
		case GGX:
			if (IsAnisotropic())
			{
				phi = std::atan(m_alphaU / m_alphaV * std::tan(C_2_PI * rand2d.y)) + C_PI * std::floor(2 * rand2d.y + 0.5);
				SinCos(phi, sinPhi, cosPhi);
				Float root = (1 - rand2d.x) * (Square(cosPhi / m_alphaU) + Square(sinPhi / m_alphaV));
				cosTheta = std::cos(std::atan(std::sqrt(rand2d.x / root)));
			}
			else
			{
				cosTheta = std::sqrt((1 - rand2d.x) / (rand2d.x * (m_alphaU * m_alphaU - 1.0) + 1));
			}
			break;
		}

		Float sinTheta = std::sqrt(Max(1.0 - cosTheta * cosTheta, 0.0));
		return Vector3f(cosPhi * sinTheta, sinPhi * sinTheta, cosTheta);
	}

	Float MicrofacetDistribution::D(const Vector3f& wh)
	{
		Float cosTheta = wh.z;
		if (cosTheta <= 0.0f)
			return 0.0f;

		Float cosTheta2 = cosTheta * cosTheta;
		Float sinTheta2 = 1 - cosTheta2;
		Float beckmannExponent = ((wh.x*wh.x) / (m_alphaU * m_alphaU) + (wh.y * wh.y) / (m_alphaV * m_alphaV)) / cosTheta2;

		switch (m_type)
		{
		case Beckmann: {
			return std::exp(-beckmannExponent) / (C_PI * m_alphaU * m_alphaV * cosTheta2 * cosTheta2);
		}
		case Phong: {
			Float exp;
			if (!IsAnisotropic())
			{
				exp = m_alphaU;
			}
			else
			{
				Float invSinTheta2 = 1 / sinTheta2;
				Float cosPhi2 = wh.x * wh.x * invSinTheta2;
				Float sinPhi2 = wh.y * wh.y * invSinTheta2;

				exp = m_alphaU * cosPhi2 + m_alphaV * sinPhi2;
			}
			return std::sqrt((m_alphaU + 2.0) * (m_alphaV + 2.0)) * 0.5 * C_1_INV_PI * Float(std::pow(double(cosTheta), double(exp)));
		}

		case GGX: {
			Float root = ((Float)1 + beckmannExponent) * cosTheta2;
			return (Float)1 / (C_PI * m_alphaU * m_alphaV * root * root);
		}
		}

		return 0.0f;
	}

	Float MicrofacetDistribution::ProjectRoughness(const Vector3f &v)const 
	{
		Float invSinTheta2 = 1 / (1.0f - v.z * v.z);

		if (!IsAnisotropic() || invSinTheta2 <= 0)
			return m_alphaU;

		Float cosPhi2 = v.x * v.x * invSinTheta2;
		Float sinPhi2 = v.y * v.y * invSinTheta2;

		return std::sqrt(cosPhi2 * m_alphaU * m_alphaU + sinPhi2 * m_alphaV * m_alphaV);
	}

	//<Microfacet Models for Refraction through Rough Surfaces>
	Float MicrofacetDistribution::G1(const Vector3f &v, const Vector3f &wh)
	{
		if (Vector3f::DotProduct(v, wh) * v.z <= 0.0f)
			return 0.0f;

		Float tanTheta = std::abs(TanTheta(v));
		if (tanTheta == 0.0f)
			return 1.0f;

		Float alpha = ProjectRoughness(v);

		switch (m_type)
		{
		case Beckmann: {
			Float a = 1.0f / (alpha * tanTheta);
			if (a < 1.6f)
				return (3.535f * a + 2.181f * a * a) / (1.0f + 2.276f*a + 2.577f*a*a);
			else
				return 1.0f;
		} case Phong: {
			Float a = std::sqrt(0.5f * alpha + 1.0f) / tanTheta;
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

	Float MicrofacetDistribution::G(const Vector3f& wi, const Vector3f& wo, const Vector3f &wh)
	{
		return G1(wi, wh) * G1(wo, wh);
	}

	Float MicrofacetDistribution::RoughnessToAlpha(Float roughness)
	{
		const Float MinAlpha = 1e-3f;
		roughness = Max(roughness, MinAlpha);

		if (m_type == Type::Phong)
		{
			return 2.0f / (roughness * roughness) - 2.0f;
		}
		else
		{
			return roughness;
		}
	}

	Float MicrofacetDistribution::Pdf(const Vector3f &wh)
	{
		return D(wh) * wh.z;
	}
}