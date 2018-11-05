#include "AreaLight.h"
#include "DiskComponent.h"

namespace RenderBird
{
	AreaLight::AreaLight()
		: m_shape(nullptr)
		, m_areaLight(nullptr)
	{
	}

	void AreaLight::OnCreate(EntityId id)
	{
		Light::OnCreate(id);
		m_areaLight = EntityManager::IntancePtr()->GetComponent<AreaLightComponent>(id);
	}

	bool AreaLight::IsSameHemisphere(const Vector3f& w1, const Vector3f& w2)const
	{
		return (!m_areaLight->m_isDoubleSide && !PathTracingUtils::IsSameHemisphere(w1, w2));
	}

	bool AreaLight::Sample(Sampler* sampler, SurfaceSample* ss, LightSample* ls, Float* pdf)
	{
		m_shape->Sample(sampler, ls, pdf);
		if (*pdf == 0.0f)
			return false;
		const Matrix4f localToWorld = TransformUtils::GetMatrix(m_transform);
		ls->TransformBy(localToWorld);

		Vector3f vecLight = ls->m_pos - ss->m_pos;
		ls->m_distance = vecLight.Length();
		if (ls->m_distance == 0 || IsSameHemisphere(-vecLight, ls->m_n))
		{
			*pdf = 0.0;
		}
		else
		{
			ss->m_wi = vecLight / ls->m_distance;
			// dw = dA * cos(theta) / (r*r)
			*pdf *= ls->m_distance * ls->m_distance / std::abs(Vector3f::DotProduct(ls->m_n, -ss->m_wi));
		}

		ls->m_li = m_lightProp->m_color;
		return true;
	}

	RGB32 AreaLight::Le(SurfaceSample* ss, const Vector3f& w)const
	{
		RGB32 le = m_lightProp->m_color;
		if (IsSameHemisphere(w, ss->m_n))
		{
			le = RGB32::BLACK;
		}
		return le;
	}

	Float AreaLight::Pdf(const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)const
	{
		const Matrix4f localToWorld = TransformUtils::GetMatrix(m_transform);
		return m_shape->Pdf(localToWorld, hitInfo, ss, wi);
	}

	bool AreaLight::Intersect(const Ray& worldRay, RayHitInfo* hitInfo)const
	{
		const Matrix4f localToWorld = TransformUtils::GetMatrix(m_transform);
		const Matrix4f worldToLocal = localToWorld.Inverse();
		Ray ray = Ray::TransformBy(worldRay, worldToLocal);

		if (m_shape->Intersect(ray, hitInfo))
		{
			hitInfo->TransformBy(localToWorld);
			hitInfo->m_obj = this;
			return true;
		}
		return false;
	}

	void AreaLight::UpdateBoundingBox()
	{
		m_boundingBox = m_shape->GetBoundingBox(TransformUtils::GetMatrix(m_transform));
	}

	Vector2f AreaLight::GetDistanceNearFar(const Vector3f& normal)
	{
		return m_shape->GetDistanceNearFar(TransformUtils::GetMatrix(m_transform), normal);
	}
}