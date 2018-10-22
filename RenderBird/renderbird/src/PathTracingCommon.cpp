#include "PathTracingCommon.h"
#include "LambertDiffuse.h"
#include "Material.h"
#include "OrenNayarDiffuse.h"
#include "SceneObject.h"

namespace RenderBird
{
	SurfaceSample::SurfaceSample(const Ray& ray, RayHitInfo& hitInfo)
	{
		m_pos = hitInfo.m_pos;
		m_n = hitInfo.m_n;
		m_ng = hitInfo.m_ng;
		if (m_n == C_Zero_v3f)
		{
			m_n = m_ng;
		}

		hitInfo.m_dpdu = hitInfo.m_dpdu.Normalized();
		hitInfo.m_dpdv = hitInfo.m_dpdv.Normalized();

		hitInfo.m_ns = hitInfo.m_ns.Normalized();
		hitInfo.m_n = hitInfo.m_n.Normalized();

		LambertDiffuse* bsdf = new LambertDiffuse();
		//OrenNayarDiffuse* bsdf = new OrenNayarDiffuse(0.5f);
		Vector3f T;
		Vector3f B;
		Vector3f N = hitInfo.m_n.Normalized();
		if (hitInfo.m_obj->CalcTangentSpace(&hitInfo, T, B))
		{
			T = T.Normalized();
			B = B.Normalized();
			N = Vector3f::CrossProduct(T, B).Normalized();
			bsdf->m_frame = TangentFrame(N, T, B);
		}
		else
		{
			bsdf->m_frame = TangentFrame(N.Normalized());
		}
		if (std::abs(hitInfo.m_ns.Length() - hitInfo.m_n.Length()) > 0.001)
			T = T;
		bool hitBackside = Vector3f::DotProduct(N, ray.m_direction) > 0.0f;
		bool flipNormal = hitBackside && bsdf->m_doubleSide;
		if (flipNormal)
		{
			bsdf->m_frame.normal = -bsdf->m_frame.normal;
			bsdf->m_frame.tangent = -bsdf->m_frame.tangent;
		}

		m_localWo = bsdf->WorldToLocal(-ray.m_direction);
		if (hitInfo.m_material != nullptr)
		{
			bsdf->m_color = RGB32(hitInfo.m_material->m_diffuseColor[0], hitInfo.m_material->m_diffuseColor[1], hitInfo.m_material->m_diffuseColor[2]);
		}
		m_bsdf = bsdf;
	}

	SurfaceSample::~SurfaceSample()
	{
		delete m_bsdf;
	}

	bool PathTracingUtils::IsSameHemisphere(const Vector3f& w1, const Vector3f& w2)
	{
		return Vector3f::DotProduct(w1, w2) > 0.0f;
	}

	Float PathTracingUtils::GammaCorrect(Float value)
	{
		if (value <= 0.0031308f) 
			return 12.92f * value;
		return 1.055f * std::pow(value, (Float)(1.f / 2.4f)) - 0.055f;
	}
}