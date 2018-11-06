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
		virtual ~BSDF(){}
		virtual RGB32 Eval(SurfaceSample* ss) = 0;
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, RGB32& weight) = 0;
		virtual RGB32 Albedo() { return m_color; }
		Vector3f LocalToWorld(const Vector3f &v) const;
		Vector3f WorldToLocal(const Vector3f &v) const;
	public:
		RGB32 m_color;
		TangentFrame m_frame;
		bool m_doubleSide;
		unsigned int m_flags;
	};
}