#pragma once
#include "renderbird_private.h"

namespace RenderBird
{
	class Material;
	struct RayHitInfo
	{
		RayHitInfo()
			: m_position(C_Zero_v3f)
			, m_normal(C_Zero_v3f)
			, m_material(nullptr)
			, m_geomNormal(C_Zero_v3f)
			, m_u(0)
			, m_v(0)
			, m_t(0)
		{
		}
		void TransformBy(const Matrix4f& mat)
		{
			m_position = mat * m_position;
			m_geomNormal = MathUtils::TransformDirection(mat, m_geomNormal).Normalized();
			m_normal = MathUtils::TransformDirection(mat, m_normal).Normalized();
		}
		bool HasHit()const
		{
			return m_t != 0;
		}
		Vector3f m_position;
		Vector3f m_normal;
		Vector3f m_geomNormal;
		Material* m_material;
		Float m_u;
		Float m_v;
		Float m_t;
	};
	struct Radiance
	{
		Radiance()
			: m_directDiffuse(RGB32::BLACK)
		{
		}
		bool IsZero()const { return m_directDiffuse.IsZero(); }
		RGB32 m_directDiffuse;
		//RGB32 m_indirect;
	};

	struct SampleInfo
	{
		Vector3f m_position;
		Vector3f m_normal;
		Float m_pdf;
	};

	struct SurfaceSample
	{
		Vector3f m_position;
		Vector3f m_geomNormal;
		Vector3f m_normal;
		Vector3f m_wo;
		bool m_isBackfacing;
		class BSDF* m_bsdf;
	};

	struct LightSample
	{
		Vector3f m_position;
		Vector3f m_normal;
		Vector3f m_wi;
		Float m_distance;
		RGB32 m_li;
	};

	struct BsdfSpectrum
	{
		BsdfSpectrum()
			: m_diffuse(RGB32::BLACK)
		{
		}
		RGB32 m_diffuse;
		void Mul(Float value)
		{
			m_diffuse *= value;
		}
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

	struct CameraSample
	{
		Vector2f m_pixel;
	};

	struct PixelData
	{
		PixelData()
			: m_colorSum(RGB32::BLACK)
			, m_weight(0.0f)
		{
		}
		RGB32 m_colorSum;
		Float m_weight;
		RGB32 GetColor()const
		{
			if (m_weight == 0.0f)
			{
				return m_colorSum;
			}
			return m_colorSum / m_weight;
		}
	};

	struct PathTracingUtils
	{
		static RGB32 EvalEmissive(const Vector3f& geomNormal, const Vector3f& dir, const RGB32& lightColor);
		static bool SampleHemisphere(const Vector3f& w1, const Vector3f& w2);
		static Float GammaCorrect(Float value);
	};

}
