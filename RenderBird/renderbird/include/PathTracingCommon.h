#pragma once
#include "RenderbirdPrivate.h"

namespace RenderBird
{
	class Material;
	class BSDF;
	class SceneObject;
	class Shape;

	struct TangentFrame
	{
		Vector3f normal, tangent, bitangent;

		TangentFrame() = default;

		TangentFrame(const Vector3f &n, const Vector3f &t, const Vector3f &b)
			: normal(n), tangent(t), bitangent(b)
		{
		}

		TangentFrame(const Vector3f &n)
		{
			// [Duff et al. 17] Building An Orthonormal Basis, Revisited. JCGT. 2017.
			normal = n.Normalized();
			float sign = copysignf(1.0f, (float)normal.z);
			const Float a = -1.0f / (sign + normal.z);
			const Float b = normal.x * normal.y*a;
			tangent = Vector3f(1.0f + sign * normal.x*normal.x*a, sign*b, -sign * normal.x).Normalized();
			bitangent = Vector3f(b, sign + normal.y*normal.y*a, -normal.y).Normalized();
		}

		Vector3f toLocal(const Vector3f &p) const
		{
			return Vector3f(
				Vector3f::DotProduct(tangent, p),
				Vector3f::DotProduct(bitangent, p),
				Vector3f::DotProduct(normal, p)
			);
		}

		Vector3f toGlobal(const Vector3f &p) const
		{
			return tangent * p.x + bitangent * p.y + normal * p.z;
		}
	};

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
			, m_primId(-1)
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
		Vector3f m_n;
		Vector3f m_ng;
		Material* m_material;
		Float m_u;
		Float m_v;
		Float m_t;
		uint32 m_flags;
		const SceneObject* m_obj;
		int m_primId;
	};

	struct Radiance
	{
		Radiance()
			: m_directDiffuse(RGB32::BLACK)
			, m_indirectDiffuse(RGB32::BLACK)
			, m_directEmission(RGB32::BLACK)
		{
		}
		void Accum(const RGB32& ls, bool firstBounce)
		{
			if (firstBounce)
			{
				m_directDiffuse += ls;
			}
			else
			{
				m_indirectDiffuse += ls;
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
		explicit SurfaceSample(const Ray& ray, RayHitInfo& hitInfo);
		~SurfaceSample();
		Vector3f m_pos;
		Vector3f m_ng;
		Vector3f m_n;
		Vector3f m_wo;
		Vector3f m_wi;
		Float m_pdf;
		RGB32 m_weight;
		std::unique_ptr<BSDF> m_bsdf;
	};

	struct LightSample
	{
		LightSample()
			: m_pdf(0)
		{
		}
		Vector3f m_pos;
		Vector3f m_n;
		Float m_pdf;
		void TransformBy(const Matrix4f& mat)
		{
			m_pos = mat * m_pos;
			m_n = MathUtils::TransformDirection(mat, m_n).Normalized();
		}
		Float m_distance;
	};

	struct PathTracingUtils
	{
		static bool IsSameHemisphere(const Vector3f& w1, const Vector3f& w2);
		static Float GammaCorrect(Float value);
	};

	enum BSDFLobeType
	{
		DiffuseReflection		= 1 << 0,
		DiffuseTransmission		= 1 << 1,
		SpecularReflection		= 1 << 2,
		SpecularTransmission	= 1 << 3,
		GlossyReflection		= 1 << 4,
		GlossyTransmission		= 1 << 5,
		DiffuseLobe = DiffuseReflection | DiffuseTransmission,
		SpecularLobe = SpecularReflection | SpecularTransmission,
		GlossyLobe = GlossyReflection | GlossyTransmission,
		AllLobe					= 0xffff
	};
}
