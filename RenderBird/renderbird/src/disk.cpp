#include "disk.h"

namespace RenderBird
{
	Disk::Disk(DiskComponent* disk)
		: m_disk(disk)
	{
	}

	Float Disk::Pdf(const Matrix4f& localToWorld, const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)
	{
		Float pdf = 1.0f / GetArea();

		Vector3f vecLight = hitInfo.m_pos - ss->m_pos;
		pdf *= vecLight.LengthSquared() / std::abs(Vector3f::DotProduct(hitInfo.m_n, -wi));
		return pdf;
	}

	Float Disk::GetArea()
	{
		return m_disk->m_phiMax * 0.5f * (m_disk->m_radius * m_disk->m_radius - m_disk->m_innerRadius * m_disk->m_innerRadius);
	}

	bool Disk::Sample(const Vector2f& rand2d, LightSample* ls, Float* pdf)
	{
		Vector2f pd = SampleUtils::ToUnitDisk(rand2d);
		Vector3f objPos(pd.x * m_disk->m_radius, pd.y * m_disk->m_radius, 0);
		ls->m_pos = objPos;
		ls->m_n = C_AxisZ_v3f;
		*pdf = 1.0f / GetArea();
		return true;
	}

	bool Disk::Intersect(const Ray& ray, RayHitInfo* hitInfo)const
	{
		if (ray.m_direction.z == 0) return false;
		Float t = (-ray.m_origin.z) / ray.m_direction.z;
		if (t <= 0 || t >= ray.m_maxT)
			return false;

		Vector3f hitPoint = ray.GetPoint(t);
		Float dist2 = hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y;
		if (dist2 > m_disk->m_radius * m_disk->m_radius || dist2 < m_disk->m_innerRadius * m_disk->m_innerRadius)
			return false;

		Float phi = std::atan2(hitPoint.y, hitPoint.x);
		if (phi < 0) phi += 2 * C_PI;
		if (phi > m_disk->m_phiMax)
			return false;

		Float u = phi / m_disk->m_phiMax;
		Float sqrDist2 = std::sqrt(dist2);
		Float oneMinusV = ((sqrDist2 - m_disk->m_innerRadius) / (m_disk->m_radius - m_disk->m_innerRadius));
		Float v = 1 - oneMinusV;
		Vector3f dpdu(-m_disk->m_phiMax * hitPoint.y, m_disk->m_phiMax * hitPoint.x, 0);
		Vector3f dpdv = Vector3f(hitPoint.x, hitPoint.y, 0.) * (m_disk->m_radius - m_disk->m_innerRadius) / sqrDist2;
		//Vector3f dndu(0, 0, 0), dndv(0, 0, 0);
		hitPoint.z = 0;

		hitInfo->m_dpdu = dpdu;
		hitInfo->m_dpdv = dpdv;
		hitInfo->m_ns = Vector3f::CrossProduct(dpdu, dpdv).Normalized();
		hitInfo->m_u = u;
		hitInfo->m_v = v;
		hitInfo->m_t = t;
		hitInfo->m_pos = hitPoint;
		hitInfo->m_ng = hitInfo->m_n = C_AxisZ_v3f;
		return true;
	}

	//How to calc aabb from disk
	//http://www.iquilezles.org/www/articles/diskbbox/diskbbox.htm
	BoundingBox Disk::GetBoundingBox(const Matrix4f& localToWorld)const
	{
		Vector3f center = localToWorld * C_Zero_v3f;
		Vector3f forward = Vector3f(localToWorld(2, 0), localToWorld(2, 1), localToWorld(2, 2));
		Vector3f sqrForward = forward * forward;
		Vector3f e = m_disk->m_radius * Vector3f(std::sqrt(1.0 - sqrForward.x), std::sqrt(1.0 - sqrForward.y), std::sqrt(1.0 - sqrForward.z));
		return BoundingBox(center - e, center + e);
	}

	Vector2f Disk::GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal)
	{
		Vector3f center = localToWorld * C_Zero_v3f;
		Float d = Vector3f::DotProduct(normal, center);

		return Vector2f(d - m_disk->m_radius, d + m_disk->m_radius);
	}
}