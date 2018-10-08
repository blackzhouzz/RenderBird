#include "Sphere.h"
#include "SampleUtils.h"

namespace RenderBird
{
	Sphere::Sphere(SphereComponent* sphere)
		: m_sphere(sphere)
	{
	}

	Float Sphere::GetArea()
	{
		return 4.0 * C_PI * m_sphere->m_radius;
	}

	Float Sphere::Pdf(const Matrix4f& localToWorld, const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)
	{
		const Float radius2 = m_sphere->m_radius * m_sphere->m_radius;

		Vector3f center = localToWorld * C_Zero_v3f;
		// Return uniform PDF if point is inside sphere
		//Vector3f origin =
		//	OffsetRayOrigin(ref.p, ref.pError, ref.n, center - ref.p);
		//if (Vector3f::DistanceSquared(origin, center) <= radius2)
		//	return Shape::Pdf(ref, wi);

		Float sinThetaMax2 = radius2 / Vector3f::DistanceSquared(hitInfo.m_pos, center);
		Float cosThetaMax = std::sqrt(std::max((Float)0, 1 - sinThetaMax2));
		return 0.5f * C_1_INV_PI / (1.0f - cosThetaMax);
	}

	void Sphere::Sample(const Vector2f& rand2d, LightSample* ls, Float* pdf)
	{
		Vector3f wi;
		SampleUtils::UniformSphere(rand2d, &wi, pdf);
		Vector3f pos = m_sphere->m_radius * 
		ls->m_n = Vector3f(pos.x, pos.y, pos.z).Normalized();
		pos *= m_sphere->m_radius / Vector3f::Distance(pos, C_Zero_v3f);
		*pdf = 1.0 / GetArea();
	}

	bool Sphere::Intersect(const Ray& ray, RayHitInfo* hitInfo)const
	{
		const Float radius2 = m_sphere->m_radius * m_sphere->m_radius;
		const Float radius = m_sphere->m_radius;
		Vector3f L = ray.m_origin - C_Zero_v3f;
		Float a = Vector3f::DotProduct(ray.m_direction, ray.m_direction);
		Float b = 2 * Vector3f::DotProduct(ray.m_direction, L);
		Float c = Vector3f::DotProduct(L, L) - radius2;
		Float t0, t1;
		if (!SolveQuadratic(a, b, c, t0, t1)) 
			return false;
		if (t0 > ray.m_maxT || t1 > ray.m_maxT)
			return false;
		if (t0 > t1) std::swap(t0, t1);
		if (t0 < 0) 
		{
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0)  return false; // both t0 and t1 are negative 
		}
		hitInfo->m_t = t0;

		Vector3f hitPoint = ray.GetPoint(t0);
		hitPoint *= radius / Vector3f::Distance(hitPoint, C_Zero_v3f);
		if (hitPoint.x == 0 && hitPoint.y == 0)
			hitPoint.x = C_FLOAT_EPSILON * radius;

		Float phi = std::atan2(hitPoint.y, hitPoint.x);
		if (phi < 0) phi += 2 * C_PI;

		Float theta = std::acos(Clamp(hitPoint.z / radius, -1.0, 1.0));
		Float zRadius = std::sqrt(hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y);
		Float invZRadius = 1 / zRadius;
		Float cosPhi = hitPoint.x * invZRadius;
		Float sinPhi = hitPoint.y * invZRadius;

		Vector3f dpdu(-2 * C_PI * hitPoint.y, 2 * C_PI * hitPoint.x, 0);
		Vector3f dpdv =
			(-C_PI) *
			Vector3f(hitPoint.z * cosPhi, hitPoint.z * sinPhi, -radius * std::sin(theta));

		hitInfo->m_dpdu = dpdu;
		hitInfo->m_dpdv = dpdv;
		hitInfo->m_ns = Vector3f::CrossProduct(dpdu, dpdv).Normalized();
		hitInfo->m_u = phi / C_2_PI;
		hitInfo->m_v = (C_PI - theta) / C_PI;
		hitInfo->m_pos = hitPoint;
		hitInfo->m_ng = hitInfo->m_n = hitPoint.Normalized();
		return true;
	}

	Vector2f Sphere::GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal)
	{
		Vector3f center = localToWorld * C_Zero_v3f;
		Float d = Vector3f::DotProduct(normal, center);

		return Vector2f(d - m_sphere->m_radius, d + m_sphere->m_radius);
	}

	BoundingBox Sphere::GetBoundingBox(const Matrix4f& localToWorld)const
	{
		Vector3f center = localToWorld * C_Zero_v3f;

		return BoundingBox(center - m_sphere->m_radius, center + m_sphere->m_radius);
	}
}