#pragma once
#include "RenderbirdPrivate.h"
#include "BSDF.h"

namespace RenderBird
{
	class OrenNayarDiffuse : public BSDF
	{
	public:
		OrenNayarDiffuse(Float roughness);
		virtual RGB32 Eval(SurfaceSample* ss);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, RGB32& weight);
		RGB32 EvalSpectrum(const Vector3f& localWi, const Vector3f& localWo);
	private:
		Float m_sigmaSq;
		Float m_roughness;
	};
}