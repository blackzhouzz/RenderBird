#pragma once
#include "MathCore.h"
#include "BoundingBox.h"
#include "Sphere.h"

namespace Core
{
	enum class PlaneSide
	{
		Front = 0,
		In,
		Back,
		Cross,
	};

	class Plane
	{
	public:
		Plane(void) = default;
		Plane(const Vector3f& normal, Float d);
		Plane(Vector3f a, Vector3f b, Vector3f c);
		bool operator == (const Plane& plane)const;
		bool operator != (const Plane& plane)const;
		void Normalize();
		Float Distance(const Vector3f& point)const;
		PlaneSide GetPlaneSide(const Vector3f& point)const;
		PlaneSide GetPlaneSide(const BoundingBox& box)const;
		PlaneSide GetPlaneSide(const Sphere& sphere)const;
	public:
		Vector3f normal;
		Float distance;
		static const Plane ZERO;
	};
}