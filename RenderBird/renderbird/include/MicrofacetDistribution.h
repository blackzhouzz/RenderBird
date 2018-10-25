#pragma once
#include "RenderBirdPrivate.h"

namespace RenderBird
{
	class Fresnel
	{
	public:
		static RGB32 Conductor(const RGB32 &eta, const RGB32 &k, Float cosThetaI);
	};

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