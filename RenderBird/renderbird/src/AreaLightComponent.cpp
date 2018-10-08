#include "AreaLightComponent.h"
#include "LightComponent.h"

IMPLEMENT_TYPE(RenderBird, AreaLightComponent)

namespace RenderBird
{
	//bool AreaLightUtils::SampleDisk(EntityId id, const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls, Float* pdf)
	//{
	//	DiskComponent* comp = EntityManager::Instance().GetComponent<DiskComponent>(id);
	//	Transform* trans = EntityManager::Instance().GetComponent<Transform>(id);
	//	LightPropertyComponent* lightProp = EntityManager::Instance().GetComponent<LightPropertyComponent>(id);
	//	AreaLightComponent* areaLight = EntityManager::Instance().GetComponent<AreaLightComponent>(id);
	//	if (comp == nullptr || trans == nullptr || lightProp == nullptr || areaLight == nullptr)
	//	{
	//		return false;
	//	}
	//	const Matrix4f localToWorld = TransformUtils::GetMatrix(trans);

	//	Vector2f pd = SampleUtils::ToUnitDisk(rand2d);
	//	Vector3f objPos(pd.x * comp->m_radius, pd.y * comp->m_radius, 0);
	//	ls->m_n = MathUtils::TransformDirection(localToWorld, C_AxisZ_v3f).Normalized();
	//	ls->m_pos = localToWorld * (objPos);
	//	*pdf = 1.0f / DiskComponentUtils::GetArea(comp);
	//	Vector3f vecLight = ls->m_pos - ss->m_pos;
	//	ls->m_distance = vecLight.Length();
	//	if (ls->m_distance == 0 || (!areaLight->m_isDoubleSide && !PathTracingUtils::IsSameHemisphere(-vecLight, ls->m_n)))
	//	{
	//		*pdf = 0.0;
	//	}
	//	else
	//	{
	//		ls->m_wi = vecLight / ls->m_distance;
	//		// dw = dA * cos(theta) / (r*r)
	//		*pdf *= vecLight.LengthSquared() / std::abs(Vector3f::DotProduct(ls->m_n, -ls->m_wi));
	//	}

	//	ls->m_li = lightProp->m_color;
	//	return *pdf > 0.0f;
	//}

	//Float AreaLightUtils::PdfDisk(const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)
	//{
	//	EntityId id = hitInfo.m_id;
	//	DiskComponent* comp = EntityManager::Instance().GetComponent<DiskComponent>(id);
	//	Transform* trans = EntityManager::Instance().GetComponent<Transform>(id);
	//	if (comp == nullptr || trans == nullptr)
	//	{
	//		return false;
	//	}
	//	Float pdf = 1.0f / DiskComponentUtils::GetArea(comp);

	//	Vector3f vecLight = hitInfo.m_pos - ss->m_pos;
	//	pdf *= vecLight.LengthSquared() / std::abs(Vector3f::DotProduct(hitInfo.m_n, -wi));
	//	return pdf;
	//}

	//RGB32 AreaLightUtils::Le(EntityId id, SurfaceSample* ss, const Vector3f& w)
	//{
	//	AreaLightComponent* areaLight = EntityManager::Instance().GetComponent<AreaLightComponent>(id);
	//	LightPropertyComponent* lightProp = EntityManager::Instance().GetComponent<LightPropertyComponent>(id);
	//	RGB32 le = lightProp->m_color;
	//	if (!areaLight->m_isDoubleSide && !PathTracingUtils::IsSameHemisphere(w, ss->m_n))
	//	{
	//		le = RGB32::BLACK;
	//	}
	//	return le;
	//}
}

