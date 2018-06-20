#pragma once
#include "vector.h"
#include "boundingbox.h"
#include "sphere.h"

namespace MathLib
{
	enum EPlaneSide
	{
		PlaneSide_Front = 0,
		PlaneSide_In,
		PlaneSide_Back,
		PlaneSide_Cross,
	};

	class Plane
	{
	public:
		Plane(void) {}
		Plane(const Vector3f& normal, Float d);
		Plane(const Plane& plane);
		Plane(Vector3f a, Vector3f b, Vector3f c);
		Plane& operator = (const Plane& plane);
		bool operator == (const Plane& plane)const;
		bool operator != (const Plane& plane)const;
		void Normalize();
		Float Distance(const Vector3f& point)const;
		EPlaneSide GetPlaneSide(const Vector3f& point)const;
		EPlaneSide GetPlaneSide(const BoundingBox& box)const;
		EPlaneSide GetPlaneSide(const Sphere& sphere)const;
	public:
		Vector3f normal;
		Float distance;
		static const Plane ZERO;
	};
}