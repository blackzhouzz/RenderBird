#include "MicrofacetDistribution.h"

namespace RenderBird
{
	Float ConductorReflectance(const Float &etaI, const Float &etaT, const Float &k, Float cosThetaI)
	{
		cosThetaI = Clamp(cosThetaI, -1.0, 1.0);
		Float eta = etaT / etaI;
		Float etak = k / etaI;

		Float cosThetaI2 = cosThetaI * cosThetaI;
		Float sinThetaI2 = 1. - cosThetaI2;
		Float eta2 = eta * eta;
		Float etak2 = etak * etak;

		Float t0 = eta2 - etak2 - sinThetaI2;
		Float a2plusb2 = std::sqrt(t0 * t0 + 4 * eta2 * etak2);
		Float t1 = a2plusb2 + cosThetaI2;
		Float a = std::sqrt(0.5f * (a2plusb2 + t0));
		Float t2 = (Float)2 * cosThetaI * a;
		Float Rs = (t1 - t2) / (t1 + t2);

		Float t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
		Float t4 = t2 * sinThetaI2;
		Float Rp = Rs * (t3 - t4) / (t3 + t4);

		return 0.5 * (Rp + Rs);
	}

	static Float DielectricReflectance(Float etaI, Float etaT, Float cosThetaI, Float& cosThetaT)
	{
		cosThetaI = Clamp(cosThetaI, -1.0, 1.0);
		// Potentially swap indices of refraction
		bool entering = cosThetaI > 0.f;
		if (!entering) {
			std::swap(etaI, etaT);
			cosThetaI = -cosThetaI;
		}

		// Compute _cosThetaT_ using Snell's law
		Float sinThetaI = std::sqrt(std::max((Float)0, 1 - cosThetaI * cosThetaI));
		Float sinThetaT = etaI / etaT * sinThetaI;

		// Handle total internal reflection
		if (sinThetaT >= 1) return 1;
		cosThetaT = std::sqrt(std::max((Float)0, 1 - sinThetaT * sinThetaT));
		Float t1 = etaT * cosThetaI;
		Float t2 = etaI * cosThetaT;
		Float t3 = etaI * cosThetaI;
		Float t4 = etaT * cosThetaT;
		Float Rs = (t1 - t2) / (t1 + t2);
		Float Rp = (t3 - t4) / (t3 + t4);
		return (Rs * Rs + Rp * Rp) * 0.5;
	}
	/* Amplitude reflection coefficient (s-polarized) */
	Float Rs(Float n1, Float n2, Float cosI, Float cosT)
	{
		return (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
	}

	/* Amplitude reflection coefficient (p-polarized) */
	Float Rp(Float n1, Float n2, Float cosI, Float cosT)
	{
		return (n2 * cosI - n1 * cosT) / (n1 * cosT + n2 * cosI);
	}

	/* Amplitude transmission coefficient (s-polarized) */
	Float Ts(Float n1, Float n2, Float cosI, Float cosT)
	{
		return 2 * n1 * cosI / (n1 * cosI + n2 * cosT);
	}

	/* Amplitude transmission coefficient (p-polarized) */
	Float Tp(Float n1, Float n2, Float cosI, Float cosT)
	{
		return 2 * n1 * cosI / (n1 * cosT + n2 * cosI);
	}

	// See http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/thin-film-interference-for-computer-graphics-r2962
	static Float ThinFilmInterferenceReflectance(Float etaEx, Float etaTh, Float etaIn, Float thickness, Float lambda, Float cosThetaO, Float &cosThetaT)
	{
		//const Vector3f invLambdas = Vector3f(1.0 / 650.0, 1.0 / 510.0, 1.0 / 475.0);
		Float delta10 = (etaTh < etaEx) ? C_PI : 0.0f;
		Float delta12 = (etaTh < etaIn) ? C_PI : 0.0f;
		Float delta = delta10 + delta12;

		// now, compute cos1, the cosine of the reflected angle
		Float sin1 = Square(etaEx / etaTh) * (1 - Square(cosThetaO));
		if (sin1 > 1) return 1.0f; // total internal reflection
		Float cos1 = std::sqrt(1 - sin1);

		// compute cos2, the cosine of the final transmitted angle, i.e. cos(theta_2)
		// we need this angle for the Fresnel terms at the bottom interface
		Float sin2 = Square(etaEx / etaIn) * (1 - Square(cosThetaO));
		if (sin2 > 1) return 1.0f; // total internal reflection
		Float cos2 = std::sqrt(1 - sin2);
		cosThetaT = cos2;

		// get the reflection transmission amplitude Fresnel coefficients
		Float alpha_s = Rs(etaTh, etaEx, cos1, cosThetaO) * Rs(etaTh, etaIn, cos1, cos2); // rho_10 * rho_12 (s-polarized)
		Float alpha_p = Rp(etaTh, etaEx, cos1, cosThetaO) * Rp(etaTh, etaIn, cos1, cos2); // rho_10 * rho_12 (p-polarized)

		Float beta_s = Ts(etaEx, etaTh, cosThetaO, cos1) * Ts(etaTh, etaIn, cos1, cos2); // tau_01 * tau_12 (s-polarized)
		Float beta_p = Tp(etaEx, etaTh, cosThetaO, cos1) * Tp(etaTh, etaIn, cos1, cos2); // tau_01 * tau_12 (p-polarized)

		// compute the phase term (phi)
		Float phi = (2 * C_PI / lambda) * (2 * etaTh * thickness * cos1) + delta;

		// finally, evaluate the transmitted intensity for the two possible polarizations
		Float ts = Square(beta_s) / (Square(alpha_s) - 2 * alpha_s * cos(phi) + 1);
		Float tp = Square(beta_p) / (Square(alpha_p) - 2 * alpha_p * cos(phi) + 1);

		// we need to take into account conservation of energy for transmission
		Float beamRatio = (etaIn * cos2) / (etaEx * cosThetaO);

		// calculate the average transmitted intensity (if you know the polarization distribution of your
		// light source, you should specify it here. if you don't, a 50%/50% average is generally used)
		Float t = beamRatio * (ts + tp) / 2;

		// and finally, derive the reflected intensity
		return 1 - t;
	}

	RGB32 Fresnel::Conductor(const RGB32 &eta, const RGB32 &k, Float cosThetaI)
	{
		return RGB32(
			ConductorReflectance(1.0, eta[0], k[0], std::abs(cosThetaI)),
			ConductorReflectance(1.0, eta[1], k[1], std::abs(cosThetaI)),
			ConductorReflectance(1.0, eta[2], k[2], std::abs(cosThetaI))
		);
	}

	RGB32 Fresnel::Dielectric(const RGB32 &etaI, const RGB32 &etaT, Float cosThetaI, Float& cosThetaT)
	{
		cosThetaT = 0;
		return RGB32(
			DielectricReflectance(etaI[0], etaT[0], cosThetaI, cosThetaT),
			DielectricReflectance(etaI[1], etaT[1], cosThetaI, cosThetaT),
			DielectricReflectance(etaI[2], etaT[2], cosThetaI, cosThetaT)
		);
	}

	Float Fresnel::Dielectric(Float etaI, Float etaT, Float cosThetaI, Float& cosThetaT)
	{
		cosThetaT = 0;
		return DielectricReflectance(etaI, etaT, cosThetaI, cosThetaT);
	}

	//	float thickness 0 3000 250	  # Thin film thickness(in nm)
	//	float externalIOR 0.2 3 1     # External(air) refractive index
	//	float thinfilmIOR 0.2 3 1.5   # Layer(thin film) refractive index
	//	float internalIOR 0.2 3 1.25  # Internal(object) refractive index
	//	float lambda			      # incident light wavelength 650, 510, 475

	RGB32 Fresnel::ThinFilmInterference(const RGB32 & etaEx, const RGB32 & etaTh, const RGB32 & etaIn, const RGB32 & thickness, const RGB32& lambda, Float cosThetaO, Float &cosThetaT)
	{
		cosThetaT = 0;
		return RGB32(
			ThinFilmInterferenceReflectance(etaEx[0], etaTh[0], etaIn[0], thickness[0], lambda[0], cosThetaO, cosThetaT),
			ThinFilmInterferenceReflectance(etaEx[1], etaTh[1], etaIn[1], thickness[1], lambda[1], cosThetaO, cosThetaT),
			ThinFilmInterferenceReflectance(etaEx[2], etaTh[2], etaIn[2], thickness[2], lambda[2], cosThetaO, cosThetaT)
		);
	}

	Float Fresnel::SchlickWeight(Float cosTheta)
	{
		Float m = Clamp(1 - cosTheta, 0.0, 1.0);
		Float m2 = m * m;
		return m2 * m2 * m; 
	}

	Float Fresnel::Schlick(Float f0, Float cosTheta)
	{
		return f0 + (1 - f0) * SchlickWeight(cosTheta);
	}

	RGB32 Fresnel::Schlick(const RGB32& f0, Float cosTheta)
	{
		return f0 + (RGB32::WHITE - f0) * SchlickWeight(cosTheta);
	}

	Float Fresnel::SchlickR0FromRelativeIOR(Float relativeIOR)
	{
		return Square((1.0 - relativeIOR) / (1 + relativeIOR));
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
		Float beckmannExponent = ((wh.x * wh.x) / (m_alphaU * m_alphaU) + (wh.y * wh.y) / (m_alphaV * m_alphaV)) / cosTheta2;

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
			Float a2 = a * a;
			if (a < 1.6f)
				return (3.535f * a + 2.181f * a2) / (1.0f + 2.276f * a + 2.577f * a2);
			else
				return 1.0f;
		} case Phong: {
			Float a = std::sqrt(0.5f * alpha + 1.0f) / tanTheta;
			Float a2 = a * a;
			if (a < 1.6f)
				return (3.535f * a + 2.181f * a2) / (1.0f + 2.276f * a + 2.577f * a2);
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