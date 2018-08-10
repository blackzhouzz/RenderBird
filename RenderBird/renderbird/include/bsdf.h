#pragma once
#include "renderbird_private.h"
#include "pathtracing_common.h"

namespace RenderBird
{
	class BSDF
	{
	public:
		virtual void Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, BsdfSpectrum* bs) = 0;
		virtual void Sample(SurfaceSample* ss, const Vector2f rand2d, Float* pdf, Vector3f* wi, RGB32* weight) = 0;
	};

	class DiffuseBSDF : public BSDF
	{
	public:
		DiffuseBSDF()
			: m_color(RGB32::WHITE)
		{
		}
		virtual void Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, BsdfSpectrum* bs);
		virtual void Sample(SurfaceSample* ss, const Vector2f rand2d, Float* pdf, Vector3f* wi, RGB32* weight);
		RGB32 m_color;
	};
}