#include "plane.h"

namespace Core
{
	const Plane Plane::ZERO = Plane(Vector3f(0, 0, 0), 0);

	Plane::Plane(const Vector3f& normal, Float d)
		: normal(normal)
		, distance(-d)
	{
	}

	Plane::Plane(Vector3f a, Vector3f b, Vector3f c)
	{
		normal = Vector3f::CrossProduct((b - a), (c - a)).Normalized();
		distance = Vector3f::DotProduct(a, normal);
	}

	bool Plane::operator == (const Plane& plane)const
	{
		return normal == plane.normal && distance == plane.distance;
	}

	bool Plane::operator != (const Plane& plane)const
	{
		return normal != plane.normal || distance != plane.distance;
	}

	void Plane::Normalize()
	{
		Float len = normal.Length();
		if (len > C_FLOAT_EPSILON)
		{
			Float invLen = 1.0f / len;
			normal *= invLen;
			distance *= invLen;
		}
	}

	Float Plane::Distance(const Vector3f& point)const
	{
		return Vector3f::DotProduct(normal, point) + distance;
	}

	PlaneSide Plane::GetPlaneSide(const Vector3f& point)const
	{
		Float dis = Distance(point);
		if (fabs(dis) < C_FLOAT_EPSILON)
		{
			return PlaneSide::In;
		}
		else if (dis > C_FLOAT_EPSILON)
		{
			return PlaneSide::Front;
		}
		else
		{
			return PlaneSide::Back;
		}
	}

	PlaneSide Plane::GetPlaneSide(const BoundingBox& box)const
	{
		Float minD, maxD;

		if (normal.x > 0)
		{
			minD = normal.x * box.m_min.x;
			maxD = normal.x * box.m_max.x;
		}
		else
		{
			minD = normal.x * box.m_max.x;
			maxD = normal.x * box.m_min.x;
		}

		if (normal.y > 0)
		{
			minD += normal.y * box.m_min.y;
			maxD += normal.y * box.m_max.y;
		}
		else
		{
			minD += normal.y * box.m_max.y;
			maxD += normal.y * box.m_min.y;
		}

		if (normal.z > 0)
		{
			minD += normal.z * box.m_min.z;
			maxD += normal.z * box.m_max.z;
		}
		else
		{
			minD += normal.z * box.m_max.z;
			maxD += normal.z * box.m_min.z;
		}

		//box at front side of plane
		if (minD > -distance)
		{
			return PlaneSide::Front;
		}

		//box at back side of plane
		if (maxD < -distance)
		{
			return PlaneSide::Back;
		}

		//box cross plane
		return PlaneSide::Cross;
	}

	PlaneSide Plane::GetPlaneSide(const Sphere& sphere)const
	{
		Float d = Distance(sphere.center);
		if (d > sphere.radius)
		{
			return PlaneSide::Front;
		}
		if (d < -sphere.radius)
		{
			return PlaneSide::Back;
		}
		return PlaneSide::Cross;
	}
}