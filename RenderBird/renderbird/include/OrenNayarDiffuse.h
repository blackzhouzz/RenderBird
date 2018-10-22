#pragma once
#include "RenderbirdPrivate.h"
#include "BSDF.h"

namespace RenderBird
{
	class OrenNayarDiffuse : public BSDF
	{
	public:
		OrenNayarDiffuse(Float roughness);
		virtual bool Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum);
		RGB32 EvalSpectrum(const Vector3f& localWo, const Vector3f& localWi);
	private:
		Float m_sigmaSq;
		Float m_roughness;
	};
}