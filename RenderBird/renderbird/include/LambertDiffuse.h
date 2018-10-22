#pragma once
#include "BSDF.h"

namespace RenderBird
{
	class LambertDiffuse : public BSDF
	{
	public:
		LambertDiffuse();
		virtual bool Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum);
	};
}