#include "SphereObject.h"
#include "SampleUtils.h"

namespace RenderBird
{
	SphereObject::SphereObject(SphereComponent* sphere)
		: m_sphere(sphere)
	{
	}

	Float SphereObject::GetArea()
	{
		return 4.0 * C_PI * m_sphere->m_radius;
	}

	Float SphereObject::Pdf(const Matrix4f& localToWorld, const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)
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

	void SphereObject::Sample(Sampler* sampler, LightSample* ls)
	{
		Vector3f wi;
		SampleUtils::UniformSphere(sampler->Next2D(), &wi, &ls->m_pdf);
		Vector3f pos = m_sphere->m_radius * 
		ls->m_n = Vector3f(pos.x, pos.y, pos.z).Normalized();
		pos *= m_sphere->m_radius / Vector3f::Distance(pos, C_Zero_v3f);
		ls->m_pdf = 1.0 / GetArea();
	}

	bool SphereObject::Intersect(const Ray& ray, RayHitInfo* hitInfo)const
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

		Float theta = std::acos(Clamp(hitPoint.z / radius, (Float)-1.0, (Float)1.0));

		hitInfo->m_u = phi / C_2_PI;
		hitInfo->m_v = (C_PI - theta) / C_PI;
		hitInfo->m_pos = hitPoint;
		hitInfo->m_ng = hitInfo->m_n = hitPoint.Normalized();
		return true;
	}

	bool SphereObject::CalcTangentSpace(RayHitInfo* hitInfo, Vector3f& T, Vector3f& B)const
	{
		Vector3f hitPoint = hitInfo->m_pos;
		const Float radius = m_sphere->m_radius;

		hitPoint *= radius / Vector3f::Distance(hitPoint, C_Zero_v3f);
		if (hitPoint.x == 0 && hitPoint.y == 0)
			hitPoint.x = C_FLOAT_EPSILON * radius;

		Float theta = std::acos(Clamp(hitPoint.z / radius, (Float)-1.0, (Float)1.0));
		Float zRadius = std::sqrt(hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y);
		Float invZRadius = 1 / zRadius;
		Float cosPhi = hitPoint.x * invZRadius;
		Float sinPhi = hitPoint.y * invZRadius;

		T = Vector3f(-2 * C_PI * hitPoint.y, 2 * C_PI * hitPoint.x, 0);
		B = (-C_PI) * Vector3f(hitPoint.z * cosPhi, hitPoint.z * sinPhi, -radius * std::sin(theta));
		return true;
	}

	Vector2f SphereObject::GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal)
	{
		Vector3f center = localToWorld * C_Zero_v3f;
		Float d = Vector3f::DotProduct(normal, center);

		return Vector2f(d - m_sphere->m_radius, d + m_sphere->m_radius);
	}

	BoundingBox SphereObject::GetBoundingBox(const Matrix4f& localToWorld)const
	{
		Vector3f center = localToWorld * C_Zero_v3f;

		return BoundingBox(center - m_sphere->m_radius, center + m_sphere->m_radius);
	}
}