#pragma once
#include "BSDF.h"

namespace RenderBird
{
	class LambertDiffuse : public BSDF
	{
	public:
		LambertDiffuse();
		virtual void Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum);
		virtual void Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum);
	};
}