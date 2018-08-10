#pragma once
#include "renderbird_private.h"

namespace RenderBird
{
	struct Radiance
	{
		RGB32 m_directDiffuse;
		//RGB32 m_indirect;
	};

	struct SurfaceSample
	{
		Vector3f m_position;
		Vector3f m_geomNormal;
		Vector3f m_normal;
		Vector3f m_wo;
		//Material* m_material;
		bool m_isBackfacing;
		class BSDF* m_bsdf;
	};

	struct LightSample
	{
		LightSample()
			: m_pdf(0.0f)
			, m_li(RGB32::BLACK)
		{
		}
		Vector3f m_position;
		Vector3f m_normal;
		Vector3f m_wi;
		Float m_distance;
		Float m_pdf;
		RGB32 m_li;
	};

	struct BsdfSpectrum
	{
		BsdfSpectrum()
			: m_diffuse(RGB32::BLACK)
		{
		}
		RGB32 m_diffuse;
		void Mul(RGB32 value)
		{
			m_diffuse *= value;
		}
		void Add(RGB32 value)
		{
			m_diffuse += value;
		}
		bool IsZero()const
		{
			return m_diffuse.IsZero();
		}
	};

	struct PathTracingUtils
	{
		static void InitRadiance(Radiance* L);
		static RGB32 EvalEmissive(const Vector3f& geomNormal, const Vector3f& dir, const RGB32& lightColor);
		static bool SampleHemisphere(const Vector3f& w1, const Vector3f& w2);
	};
}
