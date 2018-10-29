#pragma once
#include "RenderBirdPrivate.h"
#include "BSDF.h"

namespace RenderBird
{
	class DisneyDiffuse : public BSDF
	{
	public:
		DisneyDiffuse(Float roughness);
		virtual bool Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum);
		RGB32 EvalSpectrum(const Vector3f& localWi, const Vector3f& localWo);
	private:
		Float m_roughness;
	};
}