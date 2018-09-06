#include "sphere_component.h"
#include "sample_utils.h"

IMPLEMENT_TYPE(RenderBird, SphereComponent)

namespace RenderBird
{
	//Float SphereComponentUtils::GetArea(SphereComponent* comp)
	//{
	//	return comp->m_radius * comp->m_radius * C_PI * 4.0;
	//}

	//bool SphereComponentUtils::Intersect(EntityId id, const Ray& worldRay, RayHitInfo* hitInfo)
	//{
	//	SphereComponent* comp = EntityManager::Instance().GetComponent<SphereComponent>(id);
	//	Transform* trans = EntityManager::Instance().GetComponent<Transform>(id);
	//	if (comp == nullptr || trans == nullptr)
	//		return false;

	//	const Matrix4f localToWorld = TransformUtils::GetMatrix(trans);
	//	const Matrix4f worldToLocal = localToWorld.Inverse();
	//	Ray ray = Ray::TransformBy(worldRay, worldToLocal);

	//	Float radius2 = comp->m_radius * comp->m_radius;
	//	Float radius = comp->m_radius;
	//	Vector3f L = ray.m_origin - C_Zero_v3f;
	//	Float a = Vector3f::DotProduct(ray.m_direction, ray.m_direction);
	//	Float b = 2 * Vector3f::DotProduct(ray.m_direction, L);
	//	Float c = Vector3f::DotProduct(L, L) - radius2;
	//	Float t0, t1;
	//	if (!SolveQuadratic(a, b, c, t0, t1)) 
	//		return false;
	//	if (t0 > ray.m_maxT || t1 > ray.m_maxT)
	//		return false;
	//	if (t0 > t1) std::swap(t0, t1);
	//	if (t0 < 0) 
	//	{
	//		t0 = t1; // if t0 is negative, let's use t1 instead 
	//		if (t0 < 0)  return false; // both t0 and t1 are negative 
	//	}
	//	hitInfo->m_t = t0;

	//	Vector3f hitPoint = ray.GetPoint(t0);
	//	hitPoint *= radius / Vector3f::Distance(hitPoint, C_Zero_v3f);
	//	if (hitPoint.x == 0 && hitPoint.y == 0)
	//		hitPoint.x = C_FLOAT_EPSILON * radius;

	//	Float phi = std::atan2(hitPoint.y, hitPoint.x);
	//	if (phi < 0) phi += 2 * C_PI;

	//	Float theta = std::acos(Clamp(hitPoint.z / radius, -1.0, 1.0));
	//	Float zRadius = std::sqrt(hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y);
	//	Float invZRadius = 1 / zRadius;
	//	Float cosPhi = hitPoint.x * invZRadius;
	//	Float sinPhi = hitPoint.y * invZRadius;

	//	Vector3f dpdu(-2 * C_PI * hitPoint.y, 2 * C_PI * hitPoint.x, 0);
	//	Vector3f dpdv =
	//		(-C_PI) *
	//		Vector3f(hitPoint.z * cosPhi, hitPoint.z * sinPhi, -radius * std::sin(theta));

	//	hitInfo->m_dpdu = dpdu;
	//	hitInfo->m_dpdv = dpdv;
	//	hitInfo->m_ns = Vector3f::CrossProduct(dpdu, dpdv).Normalized();
	//	hitInfo->m_u = phi / C_2_PI;
	//	hitInfo->m_v = (C_PI - theta) / C_PI;
	//	hitInfo->m_pos = hitPoint;
	//	hitInfo->m_ng = hitInfo->m_n = hitPoint.Normalized();
	//	hitInfo->m_id = id;
	//	hitInfo->TransformBy(localToWorld);
	//	return true;
	//}
}