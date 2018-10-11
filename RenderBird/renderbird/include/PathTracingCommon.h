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

	struct LightSpectrum
	{
		LightSpectrum()
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
		RGB32 Resolve()const
		{
			return m_diffuse;
		}
	};

	struct Radiance
	{
		Radiance()
			: m_directDiffuse(RGB32::BLACK)
			, m_indirectDiffuse(RGB32::BLACK)
			, m_directEmission(RGB32::BLACK)
		{
		}
		void Accum(LightSpectrum* bs, bool firstBounce)
		{
			if (firstBounce)
			{
				m_directDiffuse += bs->m_diffuse;
			}
			else
			{
				m_indirectDiffuse += bs->m_diffuse;
			}
		}
		RGB32 Resolve()const
		{
			return m_indirectDiffuse + m_directDiffuse + m_directEmission;
		}
		RGB32 ResolveIndirect()const
		{
			return m_indirectDiffuse;
		}
		RGB32 ResolveDirect()const
		{
			return m_directDiffuse + m_directEmission;
		}

		RGB32 m_directDiffuse;
		RGB32 m_indirectDiffuse;
		RGB32 m_directEmission;
	};

	struct CameraSample
	{
		Vector2f m_pixel;
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
