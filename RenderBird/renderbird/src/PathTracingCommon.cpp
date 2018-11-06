#include "PathTracingCommon.h"
#include "LambertDiffuse.h"
#include "Material.h"
#include "OrenNayarDiffuse.h"
#include "SceneObject.h"
#include "MicrofacetBSDF.h"
#include "DisneyBSDF.h"

namespace RenderBird
{
	SurfaceSample::SurfaceSample(const Ray& ray, RayHitInfo& hitInfo)
	{
		m_weight = RGB32::BLACK;
		m_pdf = 0;
		m_pos = hitInfo.m_pos;
		m_n = hitInfo.m_n.Normalized();
		m_ng = hitInfo.m_ng.Normalized();
		if (m_n == C_Zero_v3f)
		{
			m_n = m_ng;
		}

		LambertDiffuse* bsdf = new LambertDiffuse();
		//OrenNayarDiffuse* bsdf = new OrenNayarDiffuse(0.5f);
		//MicrofacetConductorReflection* bsdf = new MicrofacetConductorReflection(0.1, 0.1);
		//DisneyBSDF* bsdf = new DisneyBSDF();
		bsdf->m_color = RGB32(0.82, 0.67, 0.15);
		Vector3f T;
		Vector3f B;
		Vector3f N = m_n;
		if (hitInfo.m_obj->CalcTangentSpace(&hitInfo, T, B))
		{
			T = (T - N * Vector3f::DotProduct(N, T));
			T = T.Normalized();
			B = Vector3f::CrossProduct(N, T);
			bsdf->m_frame = TangentFrame(N, T, B);
		}
		else
		{
			bsdf->m_frame = TangentFrame(N.Normalized());
		}
		bool hitBackside = Vector3f::DotProduct(N, ray.m_direction) > 0.0f;
		bool flipNormal = hitBackside && bsdf->m_doubleSide;
		if (flipNormal)
		{
			bsdf->m_frame.normal = -bsdf->m_frame.normal;
			bsdf->m_frame.tangent = -bsdf->m_frame.tangent;
		}

		m_wo = bsdf->WorldToLocal(-ray.m_direction);
		if (hitInfo.m_material != nullptr)
		{
			bsdf->m_color = RGB32(hitInfo.m_material->m_diffuseColor[0], hitInfo.m_material->m_diffuseColor[1], hitInfo.m_material->m_diffuseColor[2]);
		}
		m_bsdf.reset(bsdf);
	}

	SurfaceSample::~SurfaceSample()
	{
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