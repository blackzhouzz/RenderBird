#pragma once
#include "RenderbirdPrivate.h"
#include "PathTracingCommon.h"
#include "Sampler.h"

namespace RenderBird
{
	class BSDF
	{
	public:
		BSDF();
		virtual void Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* lightSpectrum) = 0;
		virtual void Sample(SurfaceSample* ss, Sampler* sampler, Vector3f* wi, Float* pdf, LightSpectrum* lightSpectrum) = 0;
		virtual RGB32 Albedo() { return m_color; }
		void SetCordinate(const Vector3f& dpdu, const Vector3f& dpdv, const Vector3f& ns);
		Vector3f LocalToWorld(const Vector3f &v) const;
		Vector3f WorldToLocal(const Vector3f &v) const;
	public:
		RGB32 m_color;

		TangentFrame m_frame;
		bool m_doubleSide;
	};
}