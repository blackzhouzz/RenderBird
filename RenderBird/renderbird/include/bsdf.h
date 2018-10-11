#pragma once
#include "RenderbirdPrivate.h"
#include "PathTracingCommon.h"

namespace RenderBird
{
	class BSDF
	{
	public:
		BSDF(const Vector3f& dpdu, const Vector3f& dpdv, const Vector3f& ns)
			: m_ns(ns)
			, m_ss(dpdu)
			, m_ts(dpdv)
		{
		}
		virtual void Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* eval) = 0;
		virtual void Sample(SurfaceSample* ss, const Vector2f& rand2d, Vector3f* wi, Float* pdf, LightSpectrum* bsdfWeight) = 0;
		virtual RGB32 Albedo() { return RGB32::BLACK; }
		Vector3f WorldToLocal(const Vector3f &v) const 
		{
			return Vector3f(Vector3f::DotProduct(v, m_ss), Vector3f::DotProduct(v, m_ts), Vector3f::DotProduct(v, m_ns));
		}
		Vector3f LocalToWorld(const Vector3f &v) const 
		{
			return Vector3f(m_ss.x * v.x + m_ts.x * v.y + m_ns.x * v.z,
				m_ss.y * v.x + m_ts.y * v.y + m_ns.y * v.z,
				m_ss.z * v.x + m_ts.z * v.y + m_ns.z * v.z);
		}
	public:
		Vector3f m_ns;
		Vector3f m_ts;
		Vector3f m_ss;
	};

	class DiffuseBSDF : public BSDF
	{
	public:
		DiffuseBSDF(const Vector3f& dpdu, const Vector3f& dpdv, const Vector3f& ns)
			: BSDF(dpdu, dpdv, ns),
			m_color(RGB32::WHITE * 0.5)
		{
		}
		virtual void Eval(SurfaceSample* ss, const Vector3f& wi, Float* pdf, LightSpectrum* eval);
		virtual void Sample(SurfaceSample* ss, const Vector2f& rand2d, Vector3f* wi, Float* pdf, LightSpectrum* eval);
		virtual RGB32 Albedo() { return m_color; }
		RGB32 m_color;
	};
}