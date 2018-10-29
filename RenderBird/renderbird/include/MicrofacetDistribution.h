#pragma once
#include "RenderBirdPrivate.h"

namespace RenderBird
{
	class Fresnel
	{
	public:
		static RGB32 Conductor(const RGB32 &eta, const RGB32 &k, Float cosThetaI);
		static RGB32 Dielectric(const RGB32 &eta, Float cosThetaI, Float& cosThetaT);
		static RGB32 ThinFilmInterference(const RGB32 & etaEx, const RGB32 & etaTh, const RGB32 & etaIn, const RGB32 & thickness, const RGB32& lambda, Float cosThetaO, Float &cosThetaT);
		static Float Schlick(Float cosTheta);
	};

	inline void ConvertAnisotropyToRoughness(Float roughness, Float anisotropy, Float& roughnessU, Float& roughnessV)
	{
		// (0 <= anisotropy <= 1), therefore (0 <= anisoAspect <= 1)
		// The 0.9 factor limits the aspect ratio to 10:1.
		anisotropy = Clamp(anisotropy, 0.0, 1.0);
		Float anisoAspect = std::sqrt(1.0 - 0.9 * anisotropy);

		roughnessU = roughness / anisoAspect; // Distort along tangent (rougher)
		roughnessV = roughness * anisoAspect; // Straighten along bitangent (smoother)
	}

	//reference:
	//https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/
	//<Microfacet Models for Refraction through Rough Surfaces>

	class MicrofacetDistribution
	{
	public:
		enum Type
		{
			GGX,
			Phong,
			Beckmann,
		};
		MicrofacetDistribution(Type type, Float roughnessU, Float roughnessV);
		Vector3f Sample(const Vector2f& rand2d);
		Float D(const Vector3f& wh);
		Float RoughnessToAlpha(Float roughness);
		Float G1(const Vector3f &v, const Vector3f &wh);
		Float G(const Vector3f& wi, const Vector3f& wo, const Vector3f &wh);
		Float Pdf(const Vector3f &wh);
		bool IsAnisotropic()const { return m_alphaU != m_alphaV; }
		Float ProjectRoughness(const Vector3f &v)const;
	private:
		Type m_type;
		Float m_alphaU;
		Float m_alphaV;
	};
}