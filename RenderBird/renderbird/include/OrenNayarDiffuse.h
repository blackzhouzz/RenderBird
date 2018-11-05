#pragma once
#include "RenderbirdPrivate.h"
#include "BSDF.h"

namespace RenderBird
{
	class OrenNayarDiffuse : public BSDF
	{
	public:
		OrenNayarDiffuse(Float roughness);
		virtual bool Eval(SurfaceSample* ss, LightSpectrum* lightSpectrum);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, LightSpectrum* lightSpectrum);
		RGB32 EvalSpectrum(const Vector3f& localWi, const Vector3f& localWo);
	private:
		Float m_sigmaSq;
		Float m_roughness;
	};
}