#pragma once
#include "RenderBirdPrivate.h"
#include "BSDF.h"

namespace RenderBird
{
	class Fresnel
	{
	public:
		static RGB32 Conductor(const RGB32 &eta, const RGB32 &k, Float cosThetaI);
	};

	class Distribution
	{
	public:
		enum Type
		{
			GGX,
			Phong,
			Beckmann,
		};
		Distribution(Type type);
		Vector3f Sample(const Vector2f& rand2d, Float alpha);
		Float D(Float alpha, const Vector3f& wh);
		Float RoughnessToAlpha(Float roughness);
		Float G1(Float alpha, const Vector3f &v, const Vector3f &wh);
		Float G(Float alpha, const Vector3f& wi, const Vector3f& wo, const Vector3f &wh);
		Float Pdf(Float alpha, const Vector3f &wh);

		Type m_type;
	};

	class MicrofacetConductorReflection : public BSDF
	{
	public:
		MicrofacetConductorReflection(Float roughness);
		virtual bool Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum);
	private:
		Float m_roughness;
		std::unique_ptr<Distribution> m_distribution;
		Float m_alpha;
		RGB32 m_eta;
		RGB32 m_k;
	};
}