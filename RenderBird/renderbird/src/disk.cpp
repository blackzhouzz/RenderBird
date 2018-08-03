#include "disk.h"

IMPLEMENT_TYPE(RenderBird, Disk)

namespace RenderBird
{
	Float DiskUtils::GetArea(Disk* disk)
	{
		return disk->m_phiMax * 0.5f * (disk->m_radius * disk->m_radius - disk->m_innerRadius * disk->m_innerRadius);
	}

	bool DiskUtils::Intersect(Disk* disk, const Matrix4f& objToWorld, const Ray& worldRay, RayHitInfo* hitInfo)
	{
		Matrix4f worldToObj = objToWorld.InverseSafe();
		Ray objRay = Ray::TransformRay(worldRay, worldToObj);

		if (objRay.m_direction.z == 0) return false;
		Float t = (-objRay.m_origin.z) / objRay.m_direction.z;
		if (t <= 0 || t >= objRay.m_maxT)
			return false;

		Vector3f hitPoint = objRay.GetPoint(t);
		Float dist2 = hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y;
		if (dist2 > disk->m_radius * disk->m_radius || dist2 < disk->m_innerRadius * disk->m_innerRadius)
			return false;

		Float phi = std::atan2(hitPoint.y, hitPoint.x);
		if (phi < 0) phi += 2 * C_PI;
		if (phi > disk->m_phiMax) 
			return false;

		Float u = phi / disk->m_phiMax;
		Float sqrDist2 = std::sqrt(dist2);
		Float oneMinusV = ((sqrDist2 - disk->m_innerRadius) / (disk->m_radius - disk->m_innerRadius));
		Float v = 1 - oneMinusV;
		//Vector3f dpdu(-disk->m_phiMax * hitPoint.y, disk->m_phiMax * hitPoint.x, 0);
		//Vector3f dpdv = Vector3f(hitPoint.x, hitPoint.y, 0.) * (disk->m_radius - disk->m_innerRadius) / sqrDist2;
		//Vector3f dndu(0, 0, 0), dndv(0, 0, 0);
		hitPoint.z = 0;

		hitInfo->m_u = u;
		hitInfo->m_v = v;
		hitInfo->m_t = t;
		hitInfo->m_position = objToWorld.TransformPoint(hitPoint);
		hitInfo->m_normal = objToWorld.TransformDirection(Vector3f::UNIT_Z).GetNormalized();
		return true;
	}
}