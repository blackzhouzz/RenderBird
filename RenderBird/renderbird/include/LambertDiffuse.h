#pragma once
#include "BSDF.h"

namespace RenderBird
{
	class LambertDiffuse : public BSDF
	{
	public:
		LambertDiffuse();
		virtual bool Eval(SurfaceSample* ss, LightSpectrum* lightSpectrum);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, LightSpectrum* lightSpectrum);
	};
}