#pragma once
#include "RenderbirdPrivate.h"

namespace RenderBird
{
	class Material;
	class BSDF;
	class SceneObject;

	struct RayHitInfo
	{
		RayHitInfo()
			: m_pos(C_Zero_v3f)
			, m_n(C_Zero_v3f)
			, m_material(nullptr)
			, m_ng(C_Zero_v3f)
			, m_u(0)
			, m_v(0)
			, m_t(C_FLOAT_MAX)
			, m_flags(0)
			, m_obj(nullptr)
		{
		}
		void TransformBy(const Matrix4f& mat)
		{
			m_pos = mat * m_pos;
			m_ng = MathUtils::TransformDirection(mat, m_ng).Normalized();
			m_n = MathUtils::TransformDirection(mat, m_n).Normalized();
		}
		bool IsHit()const
		{
			return m_obj != nullptr;
		}
		Vector3f m_pos;
		Vector3f m_dpdu;
		Vector3f m_dpdv;
		Vector3f m_ns;
		Vector3f m_n;
		Vector3f m_ng;
		Material* m_material;
		Float m_u;
		Float m_v;
		Float m_t;
		uint32 m_flags;
		const SceneObject* m_obj;
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

	struct SurfaceSample
	{
		explicit SurfaceSample(const Ray& ray, const RayHitInfo& hitInfo);
		~SurfaceSample();
		Vector3f m_pos;
		Vector3f m_ng;
		Vector3f m_n;
		Vector3f m_wo;
		void TransformBy(const Matrix4f& mat)
		{
			m_pos = mat * m_pos;
			m_ng = MathUtils::TransformDirection(mat, m_ng).Normalized();
			m_n = MathUtils::TransformDirection(mat, m_n).Normalized();
		}
		BSDF* m_bsdf;
	};

	struct LightSample
	{
		Vector3f m_pos;
		Vector3f m_n;
		Vector3f m_wi;
		void TransformBy(const Matrix4f& mat)
		{
			m_pos = mat * m_pos;
			m_n = MathUtils::TransformDirection(mat, m_n).Normalized();
		}
		Float m_distance;
		RGB32 m_li;
	};

	struct PathTracingUtils
	{
		static bool IsSameHemisphere(const Vector3f& w1, const Vector3f& w2);
		static Float GammaCorrect(Float value);
	};

}
