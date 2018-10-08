#include "PathTracingCommon.h"
#include "BSDF.h"
#include "Material.h"

namespace RenderBird
{
	SurfaceSample::SurfaceSample(const Ray& ray, const RayHitInfo& hitInfo)
	{
		m_pos = hitInfo.m_pos;
		m_n = hitInfo.m_n;
		m_ng = hitInfo.m_ng;
		if (m_n == C_Zero_v3f)
		{
			m_n = m_ng;
		}

		m_wo = -ray.m_direction;
		DiffuseBSDF* bsdf = new DiffuseBSDF(hitInfo.m_dpdu.Normalized(), hitInfo.m_dpdv.Normalized(), hitInfo.m_ns);
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