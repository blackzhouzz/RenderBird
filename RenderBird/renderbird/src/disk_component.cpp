#include "disk_component.h"

IMPLEMENT_TYPE(RenderBird, DiskComponent)

namespace RenderBird
{
	//Float DiskComponentUtils::GetArea(DiskComponent* comp)
	//{
	//	return comp->m_phiMax * 0.5f * (comp->m_radius * comp->m_radius - comp->m_innerRadius * comp->m_innerRadius);
	//}

	//bool DiskComponentUtils::Intersect(EntityId id, const Ray& worldRay, RayHitInfo* hitInfo)
	//{
	//	DiskComponent* comp = EntityManager::Instance().GetComponent<DiskComponent>(id);
	//	Transform* trans = EntityManager::Instance().GetComponent<Transform>(id);
	//	if (comp == nullptr || trans == nullptr)
	//		return false;

	//	const Matrix4f localToWorld = TransformUtils::GetMatrix(trans);
	//	const Matrix4f worldToLocal = localToWorld.Inverse();
	//	Ray ray = Ray::TransformBy(worldRay, worldToLocal);

	//	if (ray.m_direction.z == 0) return false;
	//	Float t = (-ray.m_origin.z) / ray.m_direction.z;
	//	if (t <= 0 || t >= ray.m_maxT)
	//		return false;

	//	Vector3f hitPoint = ray.GetPoint(t);
	//	Float dist2 = hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y;
	//	if (dist2 > comp->m_radius * comp->m_radius || dist2 < comp->m_innerRadius * comp->m_innerRadius)
	//		return false;

	//	Float phi = std::atan2(hitPoint.y, hitPoint.x);
	//	if (phi < 0) phi += 2 * C_PI;
	//	if (phi > comp->m_phiMax)
	//		return false;

	//	Float u = phi / comp->m_phiMax;
	//	Float sqrDist2 = std::sqrt(dist2);
	//	Float oneMinusV = ((sqrDist2 - comp->m_innerRadius) / (comp->m_radius - comp->m_innerRadius));
	//	Float v = 1 - oneMinusV;
	//	Vector3f dpdu(-comp->m_phiMax * hitPoint.y, comp->m_phiMax * hitPoint.x, 0);
	//	Vector3f dpdv = Vector3f(hitPoint.x, hitPoint.y, 0.) * (comp->m_radius - comp->m_innerRadius) / sqrDist2;
	//	//Vector3f dndu(0, 0, 0), dndv(0, 0, 0);
	//	hitPoint.z = 0;

	//	hitInfo->m_dpdu = dpdu;
	//	hitInfo->m_dpdv = dpdv;
	//	hitInfo->m_ns = Vector3f::CrossProduct(dpdu, dpdv).Normalized();
	//	hitInfo->m_u = u;
	//	hitInfo->m_v = v;
	//	hitInfo->m_t = t;
	//	hitInfo->m_pos = hitPoint;
	//	hitInfo->m_ng = hitInfo->m_n = C_AxisZ_v3f;
	//	hitInfo->TransformBy(localToWorld);
	//	hitInfo->m_id = id;
	//	return true;
	//}
}