#pragma once
#include "BSDF.h"

namespace RenderBird
{
	class LambertDiffuse : public BSDF
	{
	public:
		LambertDiffuse();
		virtual RGB32 Eval(SurfaceSample* ss);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, RGB32& weight);
	};
}